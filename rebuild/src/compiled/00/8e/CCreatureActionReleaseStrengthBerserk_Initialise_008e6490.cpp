// CCreatureAction_ReleaseStrengthBerserk::Initialise() -- base Initialise, then (if the creature has the berserk component, type 0x84) start the ability and set its animation time from this action's virtual duration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6490, length 99 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_ReleaseStrengthBerserk.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x84 = bit 4 of the presence word at +0x30, `test byte ptr [eax+30h],10h`),
// CTCBerserk::InitAbility @ 0x0078C900, this action's virtual slot 44 (`call [edx+0B0h]`, a float result), CTCBerserk::SetAnimationTime(float) @ 0x0078BE60.
// Shape: the GetInterface result guards both calls; the float goes `push ecx ; fstp [esp]` after the virtual call (the argument is
// evaluated after InitAbility because they are separate statements); the berserk component rides in esi.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCBerserk {                         // interface type 0x84
    void InitAbility();                     // 0x0078C900
    void SetAnimationTime(float seconds);   // 0x0078BE60
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00857520
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43();
    virtual float GetAnimationTime();       // slot 44 (+0xb0)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_ReleaseStrengthBerserk : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_ReleaseStrengthBerserk::Initialise()
{
    CCombatActionBase::Initialise();
    CTCBerserk* berserk;
    if (PCreature.GetPItem()->GetInterface(&berserk, 0x84)) {
        berserk->InitAbility();
        berserk->SetAnimationTime(GetAnimationTime());
    }
}