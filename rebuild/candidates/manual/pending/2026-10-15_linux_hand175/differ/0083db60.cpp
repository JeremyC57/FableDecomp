// CThingPhysicalSwitch::GetCurrentGraphic() const -- the def's triggered or untriggered CEngineGraphic for the switch's current state
// Grade: DIFFER(162v125) -- the CDefPointer dtor re-reads the escaped local after `dec [eax+4]` (retail keeps the def in esi: `test esi ; dec [esi+4] ; jne`) and the by-value helper adds a copy; known lane gap (CDefPointer release without re-read). Tried: one shared def, per-arm scoped defs (tails merge, 104v125), dtor through a local, by-value GetSwitchDef helper (inline / __forceinline).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083DB60, length 128 bytes (plain `ret`, result in eax).
// Callees: the global definition-manager getter @ 0x004C7990 (`mov eax,[0x013B8A1C] ; mov eax,[eax+20h]`,
// manifest label CViewBase::GetDisplayEngine is a propagated twin), CDefinitionManager::GetDef
// <CThingPhysicalSwitchDef>(long, CDefPointer<const CThingPhysicalSwitchDef>*) @ 0x0083E0D0 (`ret 8`).
// Layout (Ego_r.pdb vs retail): CThing::DefGlobalIndex PDB +0x9c -> retail +0x8c; CThingPhysicalSwitch::
// Triggered PDB +0xe8 -> retail +0xd0 (the 0x18 CThing/CThingPhysical shrink, see the landed 0x0083D940);
// CThingPhysicalSwitchDef UntriggeredGraphic PDB +0x78 -> retail +0x74, TriggeredGraphic PDB +0x86 -> +0x82.
// Shape: one CDefPointer declared before the test (its zero store is hoisted above the branch), each arm
// fetches the def (arguments pushed before the manager getter is called), then the pointer's dtor releases
// the def (null test, `dec [esi+4]`, virtual slot 1) and the arm returns the member address of the def.
struct CEngineGraphic {
    unsigned char _data[0x0e];                         // 14-byte graphic handle (PDB size)
};

struct CThingPhysicalSwitchDef {
    virtual void VSlot0();
    virtual void Destroy() const;                      // slot 1: deleting release
    mutable long RefCount;                             // +0x04
    unsigned char _pad_0x08[0x74 - 0x08];
    CEngineGraphic UntriggeredGraphic;                 // +0x74 [PDB +0x78]
    CEngineGraphic TriggeredGraphic;                   // +0x82 [PDB +0x86]
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

struct CDefinitionManager {
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0083E0D0 for the switch def
};
CDefinitionManager* GetDefinitionManager();          // 0x004C7990

struct CThingPhysicalSwitch {
    unsigned char _pad_0x00[0x8c];
    unsigned short DefGlobalIndex;                     // +0x8c CThing [PDB +0x9c]
    unsigned char _pad_0x8e[0xd0 - 0x8e];
    bool Triggered;                                    // +0xd0 [PDB +0xe8]
    __forceinline CDefPointer<const CThingPhysicalSwitchDef> GetSwitchDef() const
    {
        CDefPointer<const CThingPhysicalSwitchDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def;
    }
    const CEngineGraphic* GetCurrentGraphic() const;
};

const CEngineGraphic* CThingPhysicalSwitch::GetCurrentGraphic() const
{
    if (Triggered)
        return &GetSwitchDef()->TriggeredGraphic;
    return &GetSwitchDef()->UntriggeredGraphic;
}
