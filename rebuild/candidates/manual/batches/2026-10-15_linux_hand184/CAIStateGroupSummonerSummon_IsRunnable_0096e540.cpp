// CAIStateGroup_SummonerSummon::IsRunnable() -- runnable when the creature's interface-8 component names a live
// target and its summon component (interface 0xFA) still allows creatures; remember the target in PTarget.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096E540, length 208 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 / 0x004365B0 (inlined HasTC + entry-returning FindEntry, types 8
// and 0xFA; two byte-identical instantiations), the interface-8 component's virtual slot 29 (+0x74),
// CTCAISummon::GetMaxCreatureCount @ 0x007FDF60, CIntelligentPointer<const CThing>::CIntelligentPointer(CThing*)
// @ 0x004A96F0 (temporary), GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90 (on PTarget +0x14), dtor @ 0x00A01C10.
// Shape: as CAIStateGroup_MinionReactToEnemy::IsRunnable (0x00937F70) with a second HasTC/FindEntry and a
// converting temporary built by the out-of-line pointer ctor. The engine header for this class is all padding
// (only PTarget +0x14 named), so the shared local AI state-group model is used.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointerCtor {
    CIntelligentPointerConstThing(CThing* item);    // 0x004A96F0
};

struct CTCTargetSource {                        // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

struct CTCAISummon {                            // interface 0xFA
    long GetMaxCreatureCount();                 // 0x007FDF60
};

struct CAIStateGroup_SummonerSummon : CAIStateGroupBase {
    CBaseIntelligentPointerCtor PTarget;        // +0x14 CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_SummonerSummon::IsRunnable()
{
    if (PBrain->PCreature->HasTC(8)) {
        CThing* target = ((CTCTargetSource*)PBrain->PCreature->FindEntry(8)->component)->GetEnemy();
        if (target != 0 && !(target->m_flags91 & 1) && PBrain->PCreature->HasTC(0xFA)
            && ((CTCAISummon*)PBrain->PCreature->FindEntry(0xFA)->component)->GetMaxCreatureCount() != 0) {
            CIntelligentPointerConstThing converted(target);
            PTarget.SetPItem(converted.GetPItem());
            return true;
        }
    }
    return false;
}
