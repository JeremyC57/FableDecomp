// CAIStateGroup_MinionReactToCrime::IsStillValid() -- still valid while the villager's most recent "run and find a
// guard" crime deed exists, a guard is on the level, and the village has not yet dealt with that crime.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958E20, length 216 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (two inlined lookups: type 0x23 = CTCVillager, bit 3 of the dword at
// +0x24; type 0x4F = CTCCreatureOpinionOfHero, bit 15 of the dword at +0x28 -> `test ch,ch ; jns`),
// GFGetPVillageInterface @ 0x007EE540, CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_
// IsCrimeToRunAndFindAGuard> @ 0x006610C7 (empty filter, out handle preset to -1, true), IsGuardOnLevel @ 0x00958C00,
// the village's crime query @ 0x0065914B (manifest "CBankFileAsyncData::StartRead" is propagated and wrong).
// Shape: the deed's handle is `deed->type == 0x35 ? deed->+0x24 : handle` (the out handle shares the key word).
// Engine header names for the state group; the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
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

class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public: };
class COpinionDeed { public:
    long Type;                                  // +0x00
    unsigned char _pad[0x20];
    long CrimeHandle;                           // +0x24
};
class CTCCreatureOpinionOfHero { public:
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* handle, bool newest);   // 0x006610C7
};

struct CTCVillager;
class CTCVillage { public:
    bool IsCrimeHandled(long crimeHandle);      // 0x0065914B
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIStateGroup_MinionReactToCrime : CAIStateGroupBase {
    long OpinionDeedHandle;                     // +0x14
    bool GuardTold;                             // +0x18
    bool AllowDuplicateReactToCrime;            // +0x19
    bool DeedMaskApplied;                       // +0x1a
    bool IsGuardOnLevel();                      // 0x00958C00
    virtual bool IsStillValid();
};

bool CAIStateGroup_MinionReactToCrime::IsStillValid()
{
    if (PBrain->PCreature->HasTC(0x23)) {
        CTCVillage* village = GFGetPVillageInterface((CTCVillager*)PBrain->PCreature->FindEntry(0x23)->component);
        CTCCreatureOpinionOfHero* opinion;
        if (village != 0 && PBrain->PCreature->GetInterface(&opinion, 0x4f)) {
            COpinionDeedFilter_IsCrimeToRunAndFindAGuard filter;
            long handle = -1;
            COpinionDeed* deed = opinion->FindMostRecentDeed(filter, &handle, true);
            if (deed != 0) {
                long crime;
                if (deed->Type == 0x35)
                    crime = deed->CrimeHandle;
                else
                    crime = handle;
                if (IsGuardOnLevel() && !village->IsCrimeHandled(crime))
                    return true;
            }
        }
    }
    return false;
}