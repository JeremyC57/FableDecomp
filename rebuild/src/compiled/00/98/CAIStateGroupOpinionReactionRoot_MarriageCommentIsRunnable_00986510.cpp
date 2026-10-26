// CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable() -- a hero comment is due and one of the hero's living wives from this creature's village has a marriage state 1..5.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00986510, length 421 bytes.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface: 0x23 villager = bit 3 of +0x24, 4 hero stats =
// bit 4 of +0x20), GFGetPVillageInterface @ 0x007EE540 (__fastcall; manifest "IS_THING_ALIVE<...>" is propagated),
// CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable @ 0x009863E0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// std::list<const CThingAICreature*>(const allocator&) @ 0x006860C0 / ~list @ 0x00685BC0 (manifest labels propagated),
// CTCHeroStats::GetWivesInMarriageState @ 0x0057AE53, CTCHeroStats::PeekMarriageInfo(unsigned __int64, CHeroMarriageInfo*)
// @ 0x0057AC89.
// Layout: engine/CAIStateGroup_OpinionReactionRoot.h is pack(1) with ThingReactingTo (CIntelligentPointer<CThing>) at
// +0x6b, modelled locally the same way (packed, the brain link at +0x04 of the flattened base); CTCHeroStats' wives
// list is embedded at +0x1d8 (`cmp [node],node` empty test). STLport list = one node pointer (shares the key slot).
// Shape: one nested `if` chain with a single `return false` (retail pushes all four registers up front and shares one
// false epilogue, keeping 0 in ebx); the loop reads the node's value afresh for each use; the UID goes through an inline
// by-value getter (loads before the &info address); the state test is a named bool set in an `||` chain (1,3,2,4,5).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (const copy)
};
class CThing { public:
    unsigned char _pad0[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned long m_presentBits[9];             // +0x20
    unsigned __int64 GetUID() const { return UID; }
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingAICreature;
class CTCVillage;
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
template<class T> struct allocator { allocator() { } };
template<class T> struct list_node { list_node* _M_next; list_node* _M_prev; T _M_data; };
template<class T> struct list {
    list_node<T>* _M_node;
    list(const allocator<T>& a);                // 0x006860C0
    ~list();                                    // 0x00685BC0
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CHeroMarriageInfo { public:
    long a, b, c, d;
    long State;                                 // +0x10 EMarriageState
    bool Flag;                                  // +0x14
    CHeroMarriageInfo() : a(0), b(0), c(0), d(0), State(0), Flag(false) { }
};
class CTCHeroStats { public:
    unsigned char _pad[0x1d8];
    list<const CThingAICreature*> Wives;        // +0x1d8
    void GetWivesInMarriageState(list<const CThingAICreature*>& wives, long state) const;   // 0x0057AE53
    bool PeekMarriageInfo(unsigned __int64 uid, CHeroMarriageInfo* info) const;            // 0x0057AC89
};
struct CThingPointerView {
    CThing* GetPItem() const;                   // 0x00A01B50 CBaseIntelligentPointer::GetPItem
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
    bool CommentAboutHeroIsRunnable();          // 0x009863E0
    bool MarriageCommentIsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
        CTCVillage* village = GFGetPVillageInterface(villager);
        if (village != 0 && CommentAboutHeroIsRunnable()) {
            CTCHeroStats* stats;
            if (((CThingPointerView*)ThingReactingTo)->GetPItem()->GetInterface(&stats, 4) && !stats->Wives.empty()) {
                list<const CThingAICreature*> wives = list<const CThingAICreature*>(allocator<const CThingAICreature*>());
                stats->GetWivesInMarriageState(wives, 0);
                list_node<const CThingAICreature*>* end = wives._M_node;
                for (list_node<const CThingAICreature*>* node = end->_M_next; node != end; node = node->_M_next) {
                    CTCVillager* wifeVillager;
                    if (node->_M_data != 0 && !(((const CThing*)node->_M_data)->m_flags91 & 1)
                        && ((const CThing*)node->_M_data)->GetInterface(&wifeVillager, 0x23)
                        && GFGetPVillageInterface(wifeVillager) == village) {
                        CHeroMarriageInfo info;
                        if (stats->PeekMarriageInfo(((const CThing*)node->_M_data)->GetUID(), &info)) {
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