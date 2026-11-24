// Leaf: ?MarriageCommentIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable() -- a hero comment is due and one of the hero's living wives from this creature's village has a marriage state 1..5.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00986510, length 421 bytes.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface: 0x23 villager = bit 3 of +0x24, 4 hero stats =
// bit 4 of +0x20), GFGetPVillageInterface @ 0x007EE540 (__fastcall; manifest "IS_THING_ALIVE<...>" is propagated),
// CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable @ 0x009863E0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// std::list<const CThingAICreature*>(const allocator&) @ 0x006860C0 / _List_base::~_List_base @ 0x00685BC0 (manifest labels propagated),
// CTCHeroStats::GetWivesInMarriageState @ 0x0057AE53, CTCHeroStats::PeekMarriageInfo(unsigned __int64, CHeroMarriageInfo&)
// @ 0x0057AC89.
// Layout: engine/CAIStateGroup_OpinionReactionRoot.h is pack(1) with ThingReactingTo (CIntelligentPointer<CThing>) at
// +0x6b, modelled locally the same way (packed, the brain link at +0x04 of the flattened base); CTCHeroStats' wives
// list is embedded at +0x1d8 (`cmp [node],node` empty test). STLport list = one node pointer (shares the key slot).
// Shape: one nested `if` chain with a single `return false` (retail pushes all four registers up front and shares one
// false epilogue, keeping 0 in ebx); the loop reads the node's value afresh for each use; the UID goes through an inline
// by-value getter (loads before the &info address); the state test is a named bool set in an `||` chain (1,3,2,4,5).
#include "fable_compiler.h"
// NF246 (2026-11-22, the 0x00685BC0 family, nf243 NOTES section 2): the row-local global-namespace allocator / list_node / list models
//   (which spelled ??0?$list@PBVCThingAICreature@@@@.., ??1?$list@PBVCThingAICreature@@@@QAE@XZ and the long-state
//   GetWivesInMarriageState(list&, long)) replaced by the shared STLport list (stlport/fable_containers.h, with
//   FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE: ~_List_base out of line, as retail calls it); the local CTCHeroStats model's
//   member and GetWivesInMarriageState are respelled in place onto the PDB signature (Ego_r / FableWin, as engine/CTCHeroStats.h
//   declares it: std::list<const CThingAICreature*>&, EMarriageState). The three relocations now bind the spellings landed
//   0x00682810 binds at the same addresses: list(const allocator&) 0x006860C0, ~_List_base 0x00685BC0, GetWivesInMarriageState 0x0057AE53.
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#include "stlport/fable_containers.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned long m_presentBits[9];             // +0x20
    unsigned __int64 GetUID() const { return UID; }
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingAICreature;
class CTCVillage;
struct CTCVillager;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)
enum EMarriageState;                            // MS forward enum (as engine/CTCHeroStats.h)
class CHeroMarriageInfo { public:
    long a, b, c, d;
    long State;                                 // +0x10 EMarriageState
    bool Flag;                                  // +0x14
    CHeroMarriageInfo() : a(0), b(0), c(0), d(0), State(0), Flag(false) { }
};
class CTCHeroStats { public:
    unsigned char _pad[0x1d8];
    std::list<const CThingAICreature*> Wives;   // +0x1d8
    void GetWivesInMarriageState(std::list<const CThingAICreature*>& wives, EMarriageState state) const;   // 0x0057AE53
    // NF113 (PRIORITY q4715): respelled in place to Ego_r.pdb / FableWin.pdb ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z
    //   (the record by reference; was CHeroMarriageInfo*). Same code: the reference is passed as the address.
    bool PeekMarriageInfo(unsigned __int64 uid, CHeroMarriageInfo& info) const;            // 0x0057AC89
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
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot { public:
    void* __vftable;
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x6b - 0x08];
    unsigned char ThingReactingTo[9];           // +0x6b CIntelligentPointer<CThing>
private:
    bool CommentAboutHeroIsRunnable();          // 0x009863E0 (Ego_r.pdb + FableWin.pdb: private, AAE)
public:
    private:
    bool MarriageCommentIsRunnable();
    public:
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
        CTCVillage* village = ((CTCVillageMember*)villager)->GetPVillage();
        if (village != 0 && CommentAboutHeroIsRunnable()) {
            CTCHeroStats* stats;
            if (((CIntelligentPointer<CThing>*)ThingReactingTo)->GetPItem()->GetInterface(&stats, 4) && !stats->Wives.empty()) {
                std::list<const CThingAICreature*> wives;
                stats->GetWivesInMarriageState(wives, (EMarriageState)0);
                std::_List_node<const CThingAICreature*>* end = wives._M_node._M_data;
                for (std::_List_node<const CThingAICreature*>* node = (std::_List_node<const CThingAICreature*>*)end->_M_next; node != end;
                     node = (std::_List_node<const CThingAICreature*>*)node->_M_next) {
                    CTCVillager* wifeVillager;
                    if (node->_M_data != 0 && !(((const CThing*)node->_M_data)->m_flags91 & 1)
                        && ((const CThing*)node->_M_data)->GetInterface(&wifeVillager, 0x23)
                        && ((CTCVillageMember*)wifeVillager)->GetPVillage() == village) {
                        CHeroMarriageInfo info;
                        if (stats->PeekMarriageInfo(((const CThing*)node->_M_data)->GetUID(), info)) {
                            bool married = false;
                            if (info.State == 1 || info.State == 3 || info.State == 2 || info.State == 4 || info.State == 5)
                                married = true;
                            return married;
                        }
                    }
                }
            }
        }
    }
    return false;
}