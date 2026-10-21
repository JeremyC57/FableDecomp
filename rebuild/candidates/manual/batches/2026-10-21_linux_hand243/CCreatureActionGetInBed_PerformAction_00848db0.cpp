// CCreatureAction_GetInBed::PerformAction() -- push a new NCreatureMode::CModeSleepBed(creature, AnimGroup, SleepAnimName) onto the creature's mode manager, then when the bed is alive and has a CTCBed component start its get-in animation (BedGetInAnim, BedSleepAnim) or, with no animation, just mark the bed as occupied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848DB0, length 343 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, bit 17 of +0x24, untested; 0x35 = CTCBed, bit 21 of +0x24, tested), operator new @ 0x00BFEA1A (0x34-byte
// mode, 12-byte count block), CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CAnimFilterOfGroup(const CCharString&) @ 0x0073E8F0,
// CCharString copy ctor @ 0x0099EC30, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, CTCBed::
// StartGetInBedAnimation(long, long) @ 0x007469C0; count-block destroy fn 0x007180B0; CModeSleepBed vtable 0x01275760.
// Layout: retail CCreatureActionBase is 0xa8 bytes (the PDB header is 12 larger): PBed +0xa8, AnimGroup +0xb0, SleepAnimName +0xb4,
// BedGetInAnim +0xb8, BedSleepAnim +0xbc; CTCBed +0x0c Empty (the landed 0x007469C0 naming).
// Shape: GetTC written as the end/hit ternary leaves the bed component in ecx for the call; the mode ctor is inline (base ctor, vtable, filter, name copy, two zeroed longs); the counted pointer is built in the
// argument area with its null-checked count block (the mode was never dereferenced); IS_THING_ALIVE on PBed then HasTC/GetTC.
void __fastcall Destroy_007180b0(void* object);   // 0x007180B0
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};
struct CAnimFilterOfGroup {                     // 12 bytes
    unsigned char _body[0xc];
    CAnimFilterOfGroup(const CCharString& group);   // 0x0073E8F0
};
struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_007180b0), m_pObject(object) { }
};
template <class T> struct CCountedPointer {
    T*           m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};
struct CModeBase {                              // retail sizeof 0x1c
    virtual void v00();
    unsigned char _pad04[0x1c - 4];
    CModeBase(CThing* thing);                   // 0x0073EAC0
};
struct CModeSleepBed : CModeBase {              // retail sizeof 0x34, vtable 0x01275760
    CAnimFilterOfGroup AnimFilter;              // +0x1c
    CCharString SleepAnimName;                  // +0x28
    long GetInAnim;                             // +0x2c
    long SleepAnim;                             // +0x30
    CModeSleepBed(CThing* creature, const CCharString& group, const CCharString& animName)
        : CModeBase(creature), AnimFilter(group), SleepAnimName(animName), GetInAnim(0), SleepAnim(0) {}
};
struct CTCCreatureModeManager {
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};
struct CTCBed {
    unsigned char _pad[0xc];
    bool Empty;                                 // +0x0c
    void StartGetInBedAnimation(long getInAnim, long sleepAnim);   // 0x007469C0
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
struct CCreatureAction_GetInBed : CCreatureActionBase {
    CBaseIntelligentPointer PBed;               // +0xa8
    CCharString AnimGroup;                      // +0xb0
    CCharString SleepAnimName;                  // +0xb4
    long BedGetInAnim;                          // +0xb8
    long BedSleepAnim;                          // +0xbc
    void PerformAction();
};

void CCreatureAction_GetInBed::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
    CModeSleepBed* mode = new CModeSleepBed(PCreature.GetPItem(), AnimGroup, SleepAnimName);
    modeManager->AddMode(CCountedPointer<CModeBase>(mode));
    if (IS_THING_ALIVE(PBed)) {
        CThing* bedThing = PBed.GetPItem();
        if (bedThing->HasTC(0x35)) {
            CTCBed* bed = bedThing->GetTC<CTCBed>(0x35);
            if (BedGetInAnim)
                bed->StartGetInBedAnimation(BedGetInAnim, BedSleepAnim);
            else
                bed->Empty = false;
        }
    }
}
