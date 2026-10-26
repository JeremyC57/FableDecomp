// CPersistContext::Transfer<EPSwitchTriggerType>(EPSwitchTriggerType&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass OWN12: one name for 0x00455008 -- caller CThingPhysicalSwitchDef::Transfer +0x70 = Ego_r TriggerType (EPSwitchTriggerType); Ego_r has ??$Transfer@W4EPSwitchTriggerType@@; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" was propagated.
// Leaf: ??$Transfer@W4EPSwitchTriggerType@@@CPersistContext@@QAEXAAW4EPSwitchTriggerType@@@Z
#pragma optimize("s",on)

struct Local4 { int v; void Fill_00455049(void* obj24, void* arg); };
class CDataOutputStream { public: public:
    void WriteSLONG(long);   // 0x00993EB0
};

enum EPSwitchTriggerType { };

class CPersistContext { public:
    char pad_18[0x18];
    int mode;               // +0x18  scene/shadow mode selector
    char pad_1c[0x24-0x1c];
    void* obj24;            // +0x24
    CDataOutputStream* obj28;           // +0x28
    template <class T> void Transfer(T& value);
};

extern "C" void __stdcall ProfileScope(unsigned int hash);

template <> void CPersistContext::Transfer<EPSwitchTriggerType>(EPSwitchTriggerType& value_)
{
    CPersistContext* self = this;
    void* arg = &value_;
    Local4 local;
    ProfileScope(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Fill_00455049(self->obj24, arg);
        break;
    case 3:
        self->obj28->WriteSLONG(*(int*)arg);
        break;
    }
}