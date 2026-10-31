// CTCCreatureGroup::AddNewMember(const CThingAICreature* member) -- add a live thing that is not yet in the group: insert (UID, {member, inactive, joined this frame}) into the member map and count it; false only for a null or dead thing
// Grade: DIFFER(238v238) -- names now PDB-exact (virtual AddNewMember(const CThingAICreature*), CGroupMemberData(member, false, frame, 0), PeekWorld, static __fastcall GetFrame); residue: frame 0x64 v 0x5c -- retail shares the 8-byte insert result slot with the uid/find-key slot, ours gives it its own. Tried: named uid local (before and after PeekWorld), insert result with user constructors; earlier: separate return statements, data temporary in make_pair.
// Leaf: ?AddNewMember@CTCCreatureGroup@@UAE_NPBVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813010, 238 bytes (`ret 4`, 0x5c-byte frame).
// Callees: map<unsigned __int64, CGroupMemberData>::find @ 0x00812790 (node pointer in eax, as the landed RemoveMember
// 0x00813100), CTCBase::PeekWorld() const @ 0x00686A80, the static __fastcall CWorld::GetFrame() @ 0x0049D870, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (CIntelligentPointer<const CThingAICreature> vtable 0x01264BAC),
// make_pair<unsigned __int64, CGroupMemberData> @ 0x00813280 (`__cdecl`, result by hidden pointer), pair<const unsigned
// __int64, CGroupMemberData>'s converting ctor @ 0x00813220-family 0x008132D0, map::insert(const value_type&) @ 0x008136A0
// (pair<iterator, bool> through the hidden pointer). The three helpers carry propagated "_Cons_val"/"BinaryTree_InsertNode"
// labels.
// Layout: Ego_r.pdb CTCCreatureGroup: MemberCount +0x0c, Members +0x18 (STLport map, header first); CGroupMemberData: PMember +0,
// IsActive +8, JoinedFrame +0xc, DeactivatedFrame +0x10; retail CThing UniqueID +0x18, dead bit +0x91.
// Shape: `Members.insert(make_pair(uid, data))` with the UID read after PeekWorld() and before the static frame
// call; the data is a named local built from (member, frame) whose scalar stores sink below make_pair's pushes; the
// converted value_type temporary dies first, then the make_pair result, then the data.
class CThing { public:
    unsigned char      _pad00[0x18];
    unsigned __int64   UniqueID;                        // +0x18
    unsigned char      _pad20[0x91 - 0x20];
    unsigned char      Flags91;                         // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned __int64 GetUID() const { return UniqueID; }
};
class CThingAICreature : public CThing { };
class CWorld { public:
    static long __fastcall GetFrame();                  // 0x0049D870
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(const CThing* item);                  // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<const CThingAICreature>, vtable 0x01264BAC
    virtual void v00();
    __forceinline CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CGroupMemberData {
    CIntelligentPointer PMember;                        // +0x00
    bool                IsActive;                       // +0x08
    unsigned long       JoinedFrame;                    // +0x0c
    unsigned long       DeactivatedFrame;               // +0x10
    __forceinline CGroupMemberData(const CThingAICreature* member, bool isActive, unsigned long joinedFrame, unsigned long deactivatedFrame)
        : PMember(member), IsActive(isActive), JoinedFrame(joinedFrame), DeactivatedFrame(deactivatedFrame) { }
};
struct SMemberPair {                                    // pair<unsigned __int64, CGroupMemberData>
    unsigned __int64 first;
    CGroupMemberData second;
};
struct SMemberValue {                                   // pair<const unsigned __int64, CGroupMemberData>
    const unsigned __int64 first;
    CGroupMemberData       second;
    SMemberValue(const SMemberPair& other);             // 0x008132D0
};
SMemberPair make_pair(const unsigned __int64& first, const CGroupMemberData& second);   // 0x00813280
struct CMemberNode;
struct SInsertResult { CMemberNode* node; bool inserted; };
struct CMemberMap {                                     // map<unsigned __int64, CGroupMemberData>
    CMemberNode*  Header;                               // +0x00
    unsigned long NodeCount;                            // +0x04
    CMemberNode* find(const unsigned __int64& key);     // 0x00812790
    SInsertResult insert(const SMemberValue& value);    // 0x008136A0
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
};
struct CTCCreatureGroup : CTCBase {
    virtual bool AddNewMember(const CThingAICreature* member);
    unsigned long MemberCount;                          // +0x0c
    unsigned long ActiveMemberCount;                    // +0x10
    bool          ShutdownOnEmpty;                      // +0x14
    CMemberMap    Members;                              // +0x18
};

bool CTCCreatureGroup::AddNewMember(const CThingAICreature* member)
{
    if (!member)
        return false;
    if (member->IsDead())
        return false;
    if (Members.find(member->GetUID()) == Members.Header) {
        Members.insert(make_pair(member->GetUID(), CGroupMemberData(member, false, PeekWorld().GetFrame(), 0)));
        ++MemberCount;
    }
    return true;
}
