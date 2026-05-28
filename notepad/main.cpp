#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <commdlg.h>
#include <wchar.h>

#pragma comment(lib, "comdlg32.lib")

#define IDC_MAIN_EDIT   101
#define IDM_FILE_NEW    201
#define IDM_FILE_OPEN   202
#define IDM_FILE_SAVE   203
#define IDM_FILE_SAVEAS 204
#define IDM_FILE_EXIT   205
#define IDM_EDIT_CUT    301
#define IDM_EDIT_COPY   302
#define IDM_EDIT_PASTE  303
#define IDM_EDIT_SELALL 304

HWND  hEditEx;
WCHAR szCurrentFile[MAX_PATH] = L"";
BOOL  bModified = FALSE;

void UpdateTitle(HWND hwnd) {
    WCHAR title[MAX_PATH + 32];
    const WCHAR* name = (szCurrentFile[0] != L'\0')
        ? (wcsrchr(szCurrentFile, L'\\') + 1)
        : L"Untitled";
    swprintf(title, MAX_PATH + 32, L"%s%s — Acepad", bModified ? L"● " : L"", name);
    SetWindowText(hwnd, title);
}

BOOL AskSaveIfDirty(HWND hwnd) {
    if (!bModified) return TRUE;
    int r = MessageBox(hwnd,
        L"You have unsaved changes. Save before continuing?",
        L"Acepad", MB_YESNOCANCEL | MB_ICONWARNING);
    if (r == IDCANCEL) return FALSE;
    if (r == IDYES)    SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0);
    return TRUE;
}

BOOL SaveFile(HWND hwnd, const WCHAR* path) {
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(hwnd, L"Could not save the file.", L"Acepad", MB_ICONERROR);
        return FALSE;
    }
    int len = GetWindowTextLength(hEditEx);
    WCHAR* buf = (WCHAR*)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(WCHAR));
    GetWindowText(hEditEx, buf, len + 1);

    DWORD written;
    WORD bom = 0xFEFF;
    WriteFile(hFile, &bom, sizeof(bom), &written, NULL);
    WriteFile(hFile, buf, len * sizeof(WCHAR), &written, NULL);

    HeapFree(GetProcessHeap(), 0, buf);
    CloseHandle(hFile);

    wcscpy_s(szCurrentFile, MAX_PATH, path);
    bModified = FALSE;
    UpdateTitle(hwnd);
    return TRUE;
}

BOOL OpenFile(HWND hwnd, const WCHAR* path) {
    HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(hwnd, L"Could not open the file.", L"Acepad", MB_ICONERROR);
        return FALSE;
    }
    DWORD size = GetFileSize(hFile, NULL);
    char* raw = (char*)HeapAlloc(GetProcessHeap(), 0, size + 2);
    DWORD read;
    ReadFile(hFile, raw, size, &read, NULL);
    raw[size] = raw[size + 1] = 0;
    CloseHandle(hFile);

    WCHAR* text = NULL;
    // Detect UTF-16 LE BOM
    if (size >= 2 && (BYTE)raw[0] == 0xFF && (BYTE)raw[1] == 0xFE) {
        text = (WCHAR*)(raw + 2);
    } else {
        // Treat as UTF-8 / ANSI → convert
        int wlen = MultiByteToWideChar(CP_UTF8, 0, raw, -1, NULL, 0);
        text = (WCHAR*)HeapAlloc(GetProcessHeap(), 0, wlen * sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, raw, -1, text, wlen);
        SetWindowText(hEditEx, text);
        HeapFree(GetProcessHeap(), 0, text);
        HeapFree(GetProcessHeap(), 0, raw);
        wcscpy_s(szCurrentFile, MAX_PATH, path);
        bModified = FALSE;
        UpdateTitle(hwnd);
        return TRUE;
    }
    SetWindowText(hEditEx, text);
    HeapFree(GetProcessHeap(), 0, raw);
    wcscpy_s(szCurrentFile, MAX_PATH, path);
    bModified = FALSE;
    UpdateTitle(hwnd);
    return TRUE;
}

void ShowSaveDialog(HWND hwnd) {
    OPENFILENAME ofn = {};
    WCHAR szFile[MAX_PATH] = L"";
    if (szCurrentFile[0]) wcscpy_s(szFile, MAX_PATH, szCurrentFile);

    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = hwnd;
    ofn.lpstrFilter     = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrDefExt     = L"txt";
    ofn.Flags           = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

    if (GetSaveFileName(&ofn))
        SaveFile(hwnd, szFile);
}

void ShowOpenDialog(HWND hwnd) {
    OPENFILENAME ofn = {};
    WCHAR szFile[MAX_PATH] = L"";

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile   = szFile;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn))
        OpenFile(hwnd, szFile);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_CREATE: {
        // ── menu bar ──
        HMENU hMenu     = CreateMenu();
        HMENU hFileMenu = CreatePopupMenu();
        HMENU hEditMenu = CreatePopupMenu();

        AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW,    L"New\t\tCtrl+N");
        AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN,   L"Open…\t\tCtrl+O");
        AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE,   L"Save\t\tCtrl+S");
        AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVEAS, L"Save As…\tCtrl+Shift+S");
        AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT,   L"Exit\t\tAlt+F4");

        AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_CUT,    L"Cut\t\tCtrl+X");
        AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_COPY,   L"Copy\t\tCtrl+C");
        AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_PASTE,  L"Paste\t\tCtrl+V");
        AppendMenu(hEditMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hEditMenu, MF_STRING, IDM_EDIT_SELALL, L"Select All\tCtrl+A");

        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
        SetMenu(hwnd, hMenu);

        hEditEx = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
            ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NOHIDESEL,
            0, 0, 0, 0,
            hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL
        );

        HFONT hFont = CreateFont(
            18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas"
        );
        SendMessage(hEditEx, WM_SETFONT, (WPARAM)hFont, TRUE);

        UpdateTitle(hwnd);
        return 0;
    }

    case WM_SIZE: {
        int w = LOWORD(lParam), h = HIWORD(lParam);
        // Small padding so the edit box doesn't hug the window edges
        MoveWindow(hEditEx, 4, 4, w - 8, h - 8, TRUE);
        return 0;
    }

    case WM_KEYDOWN: {
        BOOL ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        BOOL shift = (GetKeyState(VK_SHIFT)   & 0x8000) != 0;
        if (ctrl) {
            if (wParam == 'N') { SendMessage(hwnd, WM_COMMAND, IDM_FILE_NEW,    0); return 0; }
            if (wParam == 'O') { SendMessage(hwnd, WM_COMMAND, IDM_FILE_OPEN,   0); return 0; }
            if (wParam == 'S' && shift) { SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVEAS, 0); return 0; }
            if (wParam == 'S') { SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE,   0); return 0; }
        }
        break;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_MAIN_EDIT:
            if (HIWORD(wParam) == EN_CHANGE) {
                if (!bModified) { bModified = TRUE; UpdateTitle(hwnd); }
            }
            break;

        // ── File menu ──
        case IDM_FILE_NEW:
            if (!AskSaveIfDirty(hwnd)) break;
            SetWindowText(hEditEx, L"");
            szCurrentFile[0] = L'\0';
            bModified = FALSE;
            UpdateTitle(hwnd);
            break;

        case IDM_FILE_OPEN:
            if (!AskSaveIfDirty(hwnd)) break;
            ShowOpenDialog(hwnd);
            break;

        case IDM_FILE_SAVE:
            if (szCurrentFile[0] == L'\0') ShowSaveDialog(hwnd);
            else SaveFile(hwnd, szCurrentFile);
            break;

        case IDM_FILE_SAVEAS:
            ShowSaveDialog(hwnd);
            break;

        case IDM_FILE_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        // ── Edit menu ──
        case IDM_EDIT_CUT:    SendMessage(hEditEx, WM_CUT,   0, 0); break;
        case IDM_EDIT_COPY:   SendMessage(hEditEx, WM_COPY,  0, 0); break;
        case IDM_EDIT_PASTE:  SendMessage(hEditEx, WM_PASTE, 0, 0); break;
        case IDM_EDIT_SELALL:
            SendMessage(hEditEx, EM_SETSEL, 0, -1);
            break;
        }
        return 0;
    }

    case WM_CLOSE:
        if (!AskSaveIfDirty(hwnd)) return 0;
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"AcepadWindowClass";

    WNDCLASSEX wc    = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Acepad",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 650,
        NULL, NULL, hInstance, NULL
    );
    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
