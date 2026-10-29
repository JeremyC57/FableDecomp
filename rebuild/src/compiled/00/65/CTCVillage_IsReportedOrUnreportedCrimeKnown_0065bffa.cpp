#pragma optimize("s", on)
// CTCVillage::IsReportedOrUnreportedCrimeKnown() -- true when the village has recorded crimes, or when any live villager's opinion-of-hero component (interface 79) holds a crime deed that makes it run for a guard
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Leaf: IsReportedOrUnreportedCrimeKnown
// Retail: Fable.exe @ 0x0065BFFA, 144 bytes; size TU (unaligned entry): `optimize("s",on)`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE<CBaseObject*>(CBaseObject* const&) 0x0040DCE0
//          (__fastcall); std::bitset<>::test(size_t) const 0x00410DE0 (interface 79); CVectorMap<ETCInterfaceType,CTCBase*>::
//          LowerBound 0x004365B0 (this TU's own instantiation; inlined GetTC); CTCCreatureOpinionOfHero::FindMostRecentDeed<
//          COpinionDeedFilter_IsCrimeToRunAndFindAGuard>(const Filter&, long*, bool) 0x006610C7.
// Layout: retail CTCVillage VillagePeople +0x2c (bare list head pointer); PlayersCrimes (set<long>, 12 bytes) +0x64 with
//         its node count at +0x68 (PDB +0xdc/+0xe0).
// Shape: the non-empty early return precedes the frame's register pushes; `this` spilled to [ebp-0xc]; the villager binds
//        IS_THING_ALIVE through a derived-to-base temporary at [ebp-8] (then the GetTC key); the empty filter object is
//        the byte at [ebp-1]; the found arm's `mov al,1` jumps back to the shared epilogue.
class CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CBaseObject {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
};
class CThing : public CBaseObject {
public:
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return ((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0

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

class COpinionDeed;
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public:};
class CTCCreatureOpinionOfHero { public:
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* index,
                                     bool includeReported);   // 0x006610C7
};

struct SVillagerNode {                          // STLport list node
    SVillagerNode* _M_next;
    SVillagerNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;
};

class CTCVillage { public:
    unsigned char  _pad_0x00[0x2c];
    SVillagerNode* VillagePeople;               // +0x2c list head node
    unsigned char  _pad_0x30[0x34];
    void*          PlayersCrimesHeader;         // +0x64 set<long> header node
    unsigned long  PlayersCrimesCount;          // +0x68 node count
    bool IsReportedOrUnreportedCrimeKnown();
};

bool CTCVillage::IsReportedOrUnreportedCrimeKnown()
{
    if (PlayersCrimesCount != 0)
        return true;
    for (SVillagerNode* node = VillagePeople->_M_next; node != VillagePeople; node = node->_M_next) {
        CThing* villager = node->_M_data.GetPItem();
        if (IS_THING_ALIVE(villager) && villager->HasTC(79)) {
            CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)villager->GetTC(79);
            COpinionDeedFilter_IsCrimeToRunAndFindAGuard filter;
            if (opinion->FindMostRecentDeed(filter, 0, true))
                return true;
        }
    }
    return false;
}