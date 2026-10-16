// CTCCreatureGroup::RemoveMember(CThing* member) -- erase a member from the group map; an emptied shutdown-on-empty group kills its owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813100, length 122 bytes (vtable 0x0124326C slot 29).
// Callees: map::find @ 0x00812790, map::erase(const key&) @ 0x00813640, CThing::Kill(bool) @ 0x004C9B80.
// Shape: the UID is a named local kept in ebx/ebp and handed to two `__forceinline` by-value helpers, so retail rewrites the one
// key slot before find and before erase (const-ref calls on the local merge into one copy); the emptiness test reads
// the map's node count at +0x1C.
// Layout: Ego_r.pdb CTCCreatureGroup (0x28): MemberCount +0xC, ActiveMemberCount +0x10, ShutdownOnEmpty +0x14,
// Members +0x18 = STLport map<unsigned __int64, CGroupMemberData> {_M_header, _M_node_count}. A node holds the key at +0x10
// and CGroupMemberData at +0x18: PMember +0x18 (8 bytes), IsActive +0x20, JoinedFrame +0x24, DeactivatedFrame +0x28.
// Retail CThing: UniqueID +0x18, Dead bit +0x91. Vtable 0x0124326C: slot 29 RemoveMember, 30 DeactivateMember,
// 31 ActivateMember. The local polymorphic model is used because engine/CTCBase.h flattens the vptr.
struct CThing {
    unsigned char _pad00[0x18];
    unsigned long long UniqueID;                // +0x18
    unsigned char _pad20[0x91 - 0x20];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long long GetUID() const { return UniqueID; }
    void Kill(bool immediately);                // 0x004C9B80
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870 (static: reads the frame global)
};
struct CIntelligentPointerThing { void* vptr; void* item; };
struct CGroupMemberData {
    CIntelligentPointerThing PMember;           // +0x00
    bool IsActive;                              // +0x08
    unsigned long JoinedFrame;                  // +0x0c
    unsigned long DeactivatedFrame;             // +0x10
};
struct CMemberNode {
    CMemberNode* left; CMemberNode* parent; CMemberNode* right; long colour;
    unsigned long long first;                   // +0x10
    CGroupMemberData second;                    // +0x18
};
struct CMemberMap {                             // map<unsigned __int64, CGroupMemberData>
    CMemberNode* Header;                        // +0x00
    unsigned long NodeCount;                    // +0x04
    CMemberNode* find(const unsigned long long& key);       // 0x00812790 (iterator = node pointer in eax)
    unsigned long erase(const unsigned long long& key);     // 0x00813640
    unsigned long size() const { return NodeCount; }
};
class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CWorld* GetWorld() const;                   // 0x00686A80
};
class CTCCreatureGroup : public CTCBase {
public:
    virtual bool RemoveMember(CThing* member);      // slot 29
    virtual bool DeactivateMember(CThing* member);  // slot 30
    virtual bool ActivateMember(CThing* member);    // slot 31
    unsigned long MemberCount;                  // +0x0c
    unsigned long ActiveMemberCount;            // +0x10
    bool ShutdownOnEmpty;                       // +0x14
    CMemberMap Members;                         // +0x18
};

static __forceinline CMemberNode* FindMember(CMemberMap& map, unsigned long long uid) { return map.find(uid); }
static __forceinline void EraseMember(CMemberMap& map, unsigned long long uid) { map.erase(uid); }

bool CTCCreatureGroup::RemoveMember(CThing* member)
{
    if (!member)
        return false;
    unsigned long long uid = member->GetUID();
    if (FindMember(Members, uid) != Members.Header) {
        EraseMember(Members, uid);
        --MemberCount;
        if (ShutdownOnEmpty && Members.size() == 0)
            ParentThing->Kill(false);
    }
    return true;
}