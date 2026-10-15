// CThingPlayerCreature::UpdatePlayerControl() -- updates the thing's scripted-control component (interface 0x1f) when it is flagged or its script task is not running, then performs the controlled move
// Retail: Fable.exe 0x006AD970, 88 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inlined interface lookup;
// type 0x1f = bit 31 of the bitset dword at +0x20, `test eax,eax ; jns`), the component's script-task
// probe 0x007122C0 (labelled CScriptTask_IsRunning), CTCScriptedControl::Update 0x00713910,
// CThingPlayerCreature::ControlledMove 0x006AD700.
// Shape: the landed out-parameter GetInterface template (0x008386E0 model, the key temporary in the
// `push ecx` slot); the component flag byte at +0x18 short-circuits the probe; the controlled move
// always runs (`this` kept in edi).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCScriptedControl {                              // interface type 0x1f component
    unsigned char _pad00[0x18];
    bool          m_forceUpdate;                         // +0x18
    bool IsScriptTaskRunning();                          // 0x007122C0
    void Update();                                       // 0x00713910
};

struct CThingPlayerCreature : CThing {
    void ControlledMove();                               // 0x006AD700
    void UpdatePlayerControl();
};

void CThingPlayerCreature::UpdatePlayerControl()
{
    CTCScriptedControl* control;
    if (GetInterface(&control, 0x1f))
    {
        if (control->m_forceUpdate || !control->IsScriptTaskRunning())
            control->Update();
    }
    ControlledMove();
}
