// CCreatureAction_BalverineCorneredJumpIntoSky::Initialise() -- names the animation "ATTACK_THEN_JUMP_INTO_SKY", runs the base Initialise, clears the byte at +0x10 of the creature's interface-9 component, calls the physics component's virtual slot 84 with 0 and marks the creature as not willable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CC9C0, length 151 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 9 = bit 9 of
// the bitset dword at +0x20, `test ch,2`), the physics component's virtual slot 84 (+0x150) with one argument, and
// SetThingAsNotBloodyWillableAtAll(CThing*, bool) @ 0x007E4040 (`__fastcall`: ecx thing, dl flag).
// Shape: the animation name is an implicit conversion temporary (re-addressed with `lea`) that dies before the base call;
// the PCreature address rides in edi; one `push ecx` frame slot for the lookup key. Retail CCombatActionBase is 0x114 bytes
// (engine/CCreatureAction_BalverineCorneredJumpIntoSky.h is the PDB's 0x12c-byte layout), so `this` is modelled locally.
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CTCPhysicsBase;
struct CTCGraphicAppearance;
class CThing { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void SetFlying(bool flying);    // slot 84 (+0x150)
};
struct CTCCreatureFlags9 {                  // the interface-9 component: one byte at +0x10 toggled by this action
    unsigned char _pad[0x10];
    unsigned char m_canBeTargeted;          // +0x10
};
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_BalverineCorneredJumpIntoSky : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BalverineCorneredJumpIntoSky::Initialise()
{
    SetCombatAnimation("ATTACK_THEN_JUMP_INTO_SKY");
    CCombatActionBase::Initialise();
    CTCCreatureFlags9* pFlags;
    if (PCreature.GetPItem()->GetInterface(&pFlags, 9))
        pFlags->m_canBeTargeted = 0;
    PCreature.GetPItem()->GetPhysics()->SetFlying(false);
    SetThingAsNotBloodyWillableAtAll(PCreature.GetPItem(), true);
}