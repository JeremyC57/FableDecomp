// CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (0x00404500, the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass: one name for 0x00451E11 -- its callers call CPersistContext::Transfer<T>; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" / Transfer*_xxx was propagated or descriptive.
// source_cpp
// Leaf: ??$Transfer@VCEngineGraphic@@@CPersistContext@@QAEXAAVCEngineGraphic@@@Z
struct LocalObj {
    char pad[16];
    LocalObj();
    void apply24(void* fld, int arg);
    void apply28(void* fld, int arg);
};

extern void __stdcall ProfileEnter(int tag);

class CEngineGraphic;
class CPersistContext { public:
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* p24;     // +0x24
    void* p28;     // +0x28
    template <class T> void Transfer(T& value);
};

#pragma optimize("s", on)
#pragma optimize("y", off)
template <> void CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic& value_)
{
    CPersistContext* self = this;
    int arg = (int)&value_;
    ProfileEnter(0x122d70e);
    LocalObj local;
    switch (self->mode) {
    case 2:
        local.apply24(self->p24, arg);
        break;
    case 3:
        local.apply28(self->p28, arg);
        break;
    }
}