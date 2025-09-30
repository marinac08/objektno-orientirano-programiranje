#include <fstream>
#include <cmath>
#include <vector>
#define endl "\n"

using namespace std;

const int img_wh = 1023;
const int img_wh2 = img_wh / 2;
const double Dist = sqrt((img_wh2*img_wh2) + (img_wh2*img_wh2));

struct Pixel {
    int r, g, b;
};

struct RGBImage {
    vector<vector<Pixel>> Image = vector<vector<Pixel>>(img_wh, vector<Pixel>(img_wh));
};

// --- Convert HSV to RGB ---
Pixel HSVtoRGB(double h, double s, double v) {
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double r, g, b;
    if (h < 60) { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    Pixel p;
    p.r = (int)((r + m) * 255);
    p.g = (int)((g + m) * 255);
    p.b = (int)((b + m) * 255);
    return p;
}

int main() {
    RGBImage img;
    ofstream out("Image.ppm");

    for (int i = 0; i < img_wh; i++) {
        for (int j = 0; j < img_wh; j++) {
            double dist = sqrt((i - img_wh2) * (i - img_wh2) + (j - img_wh2) * (j - img_wh2));
            double norm = dist / Dist; // 0 → 1
            double hue = 360.0 * norm; // map to rainbow
            double sat = 1.0;          // full saturation
            double val = 1.0;          // full brightness

            img.Image[i][j] = HSVtoRGB(hue, sat, val);
        }
    }

    out << "P3\n" << img_wh << " " << img_wh << "\n255\n";
    for (int i = 0; i < img_wh; i++) {
        for (int j = 0; j < img_wh; j++) {
            out << img.Image[i][j].r << " "
                << img.Image[i][j].g << " "
                << img.Image[i][j].b << "\n";
        }
    }
    return 0;
}
v