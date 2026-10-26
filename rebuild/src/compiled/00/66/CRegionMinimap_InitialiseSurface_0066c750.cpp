// CRegionMinimap::InitialiseSurface  0x0066c750  __fastcall void(CRegionMinimap*)
class C2DExtentsI { public: int x; int y; };

class CPixelFormat { public:
    int a;
    void Initialise(long r, long g, long b2, long a2, bool signd);
};
class CSurface { public:
    char pad[8];
    bool InitialiseAsImage(C2DExtentsI* ext, CPixelFormat* pf, bool bb);
};
struct CRegionMinimap {
    int handle;
    char pad[0x20];
    CSurface surf;   // +0x24
};

void __fastcall InitialiseSurface(CRegionMinimap* minimap)
{
    CPixelFormat pf;
    int dim = minimap->handle;
    *(int*)&pf = -1;
    pf.Initialise(8, 8, 8, 0, true);
    C2DExtentsI ext;
    ext.x = dim;
    ext.y = dim;
    minimap->surf.InitialiseAsImage(&ext, &pf, false);
}