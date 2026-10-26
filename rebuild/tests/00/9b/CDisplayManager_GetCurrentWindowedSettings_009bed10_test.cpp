#include <cstdio>
// Behaviour test for 009bed10.

// CDisplayManager::GetCurrentWindowedSettings: window size from the rect, colour depth from the adapter's mode format.
struct RECT_ { long left, top, right, bottom; };
struct MODE_ { unsigned int Width, Height, RefreshRate, Format; };
static void GetWindowRect_(void* w, RECT_* r) { (void)w; r->left = 10; r->top = 20; r->right = 650; r->bottom = 500; }
struct CD3D { unsigned int adapter; void GetAdapterDisplayMode(unsigned int a, MODE_* m) { adapter = a; m->Format = 22; } };
class CPixelFormat { public: unsigned int f; void Initialise(unsigned int format) { f = format; } unsigned int GetColourDepth() { return f == 22 ? 32 : 16; } };
class C2DExtentsI { public: long X, Y; };
class CDisplayManager { public:
    CD3D* D3D; unsigned int AdapterOrdinal;
    void GetCurrentWindowedSettings(void* window, C2DExtentsI& size, unsigned int& depth)
    {
        MODE_ mode; D3D->GetAdapterDisplayMode(AdapterOrdinal, &mode);
        RECT_ rect; GetWindowRect_(window, &rect);
        size.X = rect.right - rect.left; size.Y = rect.bottom - rect.top;
        CPixelFormat format; format.Initialise(mode.Format);
        depth = format.GetColourDepth();
    }
};
int main()
{
    CD3D d3d; CDisplayManager m; m.D3D = &d3d; m.AdapterOrdinal = 1;
    C2DExtentsI size; unsigned int depth = 0;
    m.GetCurrentWindowedSettings(0, size, depth);
    int ok = size.X == 640 && size.Y == 480 && depth == 32 && d3d.adapter == 1;
    if (!ok) { std::printf("009bed10_TEST FAIL\n"); return 1; }
    std::printf("009bed10_TEST PASS\n");
    return 0;
}