// CPersistContext::Transfer<EOpinionDeedType>(EOpinionDeedType&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (0x00404500, the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass: one name for 0x00456B7D -- its callers call CPersistContext::Transfer<T>; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" / Transfer*_xxx was propagated or descriptive.
#pragma optimize("s",on)
// Leaf: ??$Transfer@W4EOpinionDeedType@@@CPersistContext@@QAEXAAW4EOpinionDeedType@@@Z
struct CTarget2 { void Draw(void* a, void* b); };
struct CTarget3 { void Draw(void* a); };

enum EOpinionDeedType { };
class CPersistContext { public:
    char pad[0x18];
    int mode;        // +0x18
    char pad2[0x24 - 0x1c];
    void* obj24;     // +0x24
    void* obj28;     // +0x28
    void Enter(void* d);   // thiscall member, void
    template <class T> void Transfer(T& value);
};

extern int g_descObj;

template <> void CPersistContext::Transfer<EOpinionDeedType>(EOpinionDeedType& value_)
{
    CPersistContext* self = this;
    void** param = (void**)&value_;
    int st;
    self->Enter(&g_descObj);
    st = 0;
    switch (self->mode) {
    case 2:
        ((CTarget2*)&st)->Draw(self->obj24, param);
        break;
    case 3:
        ((CTarget3*)self->obj28)->Draw(*param);
        break;
    }
}