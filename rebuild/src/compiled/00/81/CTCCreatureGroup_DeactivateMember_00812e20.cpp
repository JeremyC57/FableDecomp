// CTCCreatureGroup::DeactivateMember(CThing* member) -- mark a group member inactive (a dead member is removed instead), stamping the deactivation frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812E20, length 144 bytes (vtable 0x0124326C slot 30).
// Callees: map::find @ 0x00812790, CTCBase::GetWorld @ 0x00686A80 (thiscall, ignores this), CWorld::GetWorldFrame @ 0x0049D870
// (static: `GetWorld()->GetWorldFrame()` still emits the GetWorld call), RemoveMember via vtable slot 29 (+0x74).
// Shape: the key is an inline by-value GetUID() temporary copied into the frame; `it != Members.Header` compares against
// the header in memory; IsActive is re-read after the frame call (the store could alias it).
// Layout: Ego_r.pdb CTCCreatureGroup (0x28): MemberCount +0xC, ActiveMemberCount +0x10, ShutdownOnEmpty +0x14,
// Members +0x18 = STLport map<unsigned __int64, CGroupMemberData> {_M_header, _M_node_count}. A node holds the key at +0x10
// and CGroupMemberData at +0x18: PMember +0x18 (8 bytes), IsActive +0x20, JoinedFrame +0x24, DeactivatedFrame +0x28.
// Retail CThing: UniqueID +0x18, Dead bit +0x91. Vtable 0x0124326C: slot 29 RemoveMember, 30 DeactivateMember,
// 31 ActivateMember. The local polymorphic model is used because engine/CTCBase.h flattens the vptr.
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned long long UniqueID;                // +0x18
    unsigned char _pad20[0x91 - 0x20];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    unsigned long long GetUID() const { return UniqueID; }
    void Kill(bool immediately);                // 0x004C9B80
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870 (static: reads the frame global)
};
struct CIntelligentPointerThing { void* vptr; void* item; };
class CGroupMemberData { public:
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

bool CTCCreatureGroup::DeactivateMember(CThing* member)
{
    if (!member)
        return false;
    if (member->IsDead()) {
        RemoveMember(member);
        return true;
    }
    CMemberNode* it = Members.find(member->GetUID());
    if (it != Members.Header) {
        if (it->second.IsActive) {
            it->second.DeactivatedFrame = GetWorld()->GetWorldFrame();
            if (it->second.IsActive)
                --ActiveMemberCount;
        }
        it->second.IsActive = false;
        return true;
    }
    return false;
}