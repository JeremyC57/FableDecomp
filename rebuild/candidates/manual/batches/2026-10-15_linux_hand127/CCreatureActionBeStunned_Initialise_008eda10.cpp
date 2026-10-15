// CCreatureAction_BeStunned::Initialise() -- on the creature's mode manager (interface type 49): query the current mode's type (result dropped), remove modes 31 and 29, add mode 29, then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EDA10, length 120 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface
// lookup for type 49 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest
// "CPixelShader::CountReferences" is propagated), the mode's virtual slot 1 (its type; the value is not used),
// CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 (twice), CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10,
// CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, last).
// Shape: `this` rides in edi (the PCreature address is formed in place, `lea ecx,[edi+8]`); the GetInterface result is IGNORED, so the
// manager is read back from the frame slot on the not-present path and taken from the register on the found path (`jmp` joins them).
// Retail CCreatureActionBase is 0xa8 bytes; no engine header for this class, so `this` is modelled locally.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CTCPhysicsBase;
struct CTCGraphicAppearance;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x14];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }                           // inline accessors: the member is
    CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; }   //   loaded straight into ecx
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {            // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CMode {
    virtual void v00();
    virtual long GetModeType();             // slot 1
};
struct CTCCreatureModeManager {             // the interface-49 component
    CMode* GetCurrentMode();                // 0x0073A6E0
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
    bool AddMode(unsigned long mode);       // 0x0073AE10
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};

struct CCreatureAction_BeStunned : CCreatureActionBase {
    virtual void Initialise();
};

void CCreatureAction_BeStunned::Initialise()
{
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 49);   // result unchecked: the manager is assumed present
    pModes->GetCurrentMode()->GetModeType();           // queried and dropped (a leftover of the dazed-mode test, see 0x008ED850)
    pModes->RemoveMode(31);
    pModes->RemoveMode(29);
    pModes->AddMode(29);
    CCreatureActionBase::Initialise();
}
