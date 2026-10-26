struct Dims { int w; int h; };
struct Device { void GetDims(Dims* out); };
class CSystemManager { public: char pad[0x60]; Device* dev; };
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
extern float g_dimW;
extern float g_dimH;

void __fastcall NUISystem_CUIScreenUnitConverter_ResetDimensions(void)
{
    Dims d;
    CSystemManager* p = GFGetSystemManager();
    p->dev->GetDims(&d);
    g_dimW = (float)d.w;
    g_dimH = (float)d.h;
}