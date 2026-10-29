#pragma optimize("s",on)

class CRegion;
class CCharString;
class CMiniMapDisplay { public:
    void InitialiseOnRegionLoad(const CRegion& region);      // 0x0082BA00
    void InitialiseOnRegionLoad(const CCharString& name);    // 0x0082A180
};
namespace NPlayerGui {
class CDrawMiniMap { public:
    char pad0[0xc];
    char* World;                 // +0x0c
    char pad1[0x28 - 0x10];
    CMiniMapDisplay disp;      // +0x28
    void SetRegion(const CRegion& r);
protected:
    void UpdateRegionExitNames(const CRegion& r);   // 0x0064DD9A (Ego_r.pdb, protected)
};
}
extern unsigned char g_frontendActive;         // ds:0x13b8780
void NPlayerGui::CDrawMiniMap::SetRegion(const CRegion& r) {
    this->disp.InitialiseOnRegionLoad(r);
    if (g_frontendActive)
        this->disp.InitialiseOnRegionLoad(*(const CCharString*)(this->World + 0x84));
    this->UpdateRegionExitNames(r);
}