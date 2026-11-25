// CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (0x00404500, the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass: one name for 0x00451E11 -- its callers call CPersistContext::Transfer<T>; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" / Transfer*_xxx was propagated or descriptive.
// source_cpp
// Leaf: ??$Transfer@VCEngineGraphic@@@CPersistContext@@QAEXAAVCEngineGraphic@@@Z
// Literal (NF415, 2026-11-25): was the bare integer 0x122d70e (the retail VA of the pooled ""); now the literal is spelled, cast to the
//   stand-in's existing int parameter so no callee or table line changes (the callee port is T3). Retail 0x0122D70E: 00 (retail's one pooled "", NF388 / NF413 b2 / NF414 b1).
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
    ProfileEnter((int)"");
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