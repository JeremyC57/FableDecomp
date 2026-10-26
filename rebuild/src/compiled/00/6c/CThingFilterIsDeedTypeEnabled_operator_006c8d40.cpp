// CThingFilter_IsDeedTypeEnabled::operator()(const CThing& thing) const -- whether the thing's opinion-of-hero component has the filter's deed type enabled; deed type 1 also needs a village member with a village
// Leaf: ??RCThingFilter_IsDeedTypeEnabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C8D40, length 156 bytes (+ int3 padding), thiscall `ret 4`.
// Single caller: CThingFilter_IsEligibleWitness::operator() (0x006C8E90) calls it on `lea ecx,[edi+0Ch]` =
// DeedTypeEnabled (Ego_r.pdb CThingFilter_IsEligibleWitness +0x0C, CThingFilter_IsDeedTypeEnabled {EOpinionDeedType
// DeedType}), right after HasOpinions (+8, CThingFilter_HasTC 0x4F). The manifest label "CTCInterface_GetProperty" is
// descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0; CTCVillageMember::GetPVillage
// @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" is propagated; landed callers 0x007B2CF0 / 0x00833710 name it).
// Layout: retail CThing interface bitset +0x20 (0x23 = CTCVillageMember: byte +0x24 bit 3; 0x4F =
// CTCCreatureOpinionOfHero: word +0x28 bit 15, `test ah,ah ; jns`), interface map +0x44. CTCCreatureOpinionOfHero:
// retail reads `DeedMask[DeedType]` as a BYTE through the pointer at +0x3C, i.e. its three vector<bool> masks are
// 12-byte byte vectors in retail (AttitudeMask +0x24, ReactionMask +0x30, DeedMask +0x3C) where the PDB's 0x14-byte
// vector<bool> puts DeedMask at +0x4C -- so the engine header's layout does not apply and a local model is used.
// Shape: the landed out-parameter GetInterface template (const thing), `__forceinline` (a plain inline template is
// called out of line from this two-lookup body): presence bit, key homed in the dead `thing`
// argument slot, LowerBound, the `it == last || type < it->type` select. The opinion pointer is NOT initialised:
// when the 0x4F bit is clear retail returns through the stale argument slot (`mov eax,[esp+10h]`).
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCVillage;
class CTCVillageMember { public:                                   // TC 0x23
    CTCVillage* GetPVillage() const;                        // 0x007EE540
};

struct CRetailBoolVector {                                  // retail vector<bool>: {start, finish, end_of_storage} of bytes
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool operator[](unsigned int n) const { return _M_start[n]; }
};

class CTCCreatureOpinionOfHero { public:                           // TC 0x4F (retail layout, see above)
    unsigned char     _pad00[0x24];
    CRetailBoolVector AttitudeMask;                         // +0x24
    CRetailBoolVector ReactionMask;                         // +0x30
    CRetailBoolVector DeedMask;                             // +0x3C
};

struct CThingFilter_IsDeedTypeEnabled {
    long DeedType;                                          // +0x00 EOpinionDeedType
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsDeedTypeEnabled::operator()(const CThing& thing) const
{
    if (DeedType == 1) {
        CTCVillageMember* member;
        if (!thing.GetInterface(&member, 0x23) || member->GetPVillage() == 0)
            return false;
    }
    CTCCreatureOpinionOfHero* opinion;
    thing.GetInterface(&opinion, 0x4f);
    return opinion->DeedMask[DeedType];
}