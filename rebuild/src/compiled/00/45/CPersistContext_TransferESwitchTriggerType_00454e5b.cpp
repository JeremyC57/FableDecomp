// CPersistContext::Transfer<ESwitchTriggerType>(ESwitchTriggerType&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass OWN12: one name for 0x00454E5B -- callers CThingMarkerDef::Transfer +0x80 / CThingSwitchDef::Transfer +0x70 = Ego_r TriggerType (ESwitchTriggerType); Ego_r has ??$Transfer@W4ESwitchTriggerType@@; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" was propagated.
// Leaf: ??$Transfer@W4ESwitchTriggerType@@@CPersistContext@@QAEXAAW4ESwitchTriggerType@@@Z
class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct Local {
    int x;
    void Fill(int a, int* b);
};

enum ESwitchTriggerType { };

class CPersistContext {
public:
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x08];
    int obj24;     // +0x24
    CDataOutputStream* sink28;  // +0x28
    template <class T> void Transfer(T& value);
};

extern void __stdcall Prep(unsigned int tag);   // push 0x122d70e; call

#pragma optimize("y", off)
#pragma optimize("s", on)
template <> void CPersistContext::Transfer<ESwitchTriggerType>(ESwitchTriggerType& value_)
{
    int* p = (int*)&value_;
    Local h;
    Prep(0x122d70eU);
    h.x = 0;
    switch (this->mode) {
    case 2:
        h.Fill(this->obj24, p);
        break;
    case 3:
        this->sink28->WriteSLONG(*p);
        break;
    }
}