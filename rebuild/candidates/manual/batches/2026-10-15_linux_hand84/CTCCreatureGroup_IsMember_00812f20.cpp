// CTCCreatureGroup::IsMember(const CThing*) const -- false for a null or dead thing; otherwise whether its unique id is a key of the Members map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812F20, length 76 bytes (+ int3 padding).
// Callees: map<unsigned __int64, CGroupMemberData> node lookup @ 0x00812790 (STLport _M_find: the node or
// the header; manifest "Faction_FindMemberIndex" is propagated).
// Layout = Ego_r.pdb CTCCreatureGroup: MemberCount +0xc, ActiveMemberCount +0x10, ShutdownOnEmpty +0x14,
// Members +0x18 (STLport map: the header node pointer first). CThing (retail, 0x10 below the PDB):
// UniqueID +0x18, the Dead bit = bit 0 of the flag byte +0x91 (PDB +0xa1).
// Shape: two SEQUENTIAL early `return false` tests (null, then Dead) put the shared `xor al,al` exit right
// after the second test (the `||` form or a nested success block places it at the end); the 64-bit id is
// copied into the 8-byte frame (low word first) and passed by const reference; `find(id) != end()` reads
// the header after the call and becomes the full-register `xor edx,edx ; setne dl ; mov al,dl` bool.
struct CThing {
    unsigned char    _pad0[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned char    _pad20[0x71];                          // +0x20
    unsigned char    Dead                        : 1;       // +0x91 bit 0
    unsigned char    CreatedByOwnerThingCreation : 1;
    unsigned char    IncludeInThingScans         : 1;
    unsigned char    Activated                   : 1;
    unsigned char    GloballyLoaded              : 1;
    unsigned char    InLimbo                     : 1;
    unsigned char    KillLocked                  : 1;
    unsigned char    CreatedBySerialisation      : 1;
};

struct SMemberNode;                                         // STLport _Rb_tree_node<pair<const u64, CGroupMemberData> >

struct CGroupMemberMap {                                    // STLport map<unsigned __int64, CGroupMemberData>
    SMemberNode*  Header;                                   // +0x00
    unsigned long Count;                                    // +0x04
    unsigned char Compare[4];                               // +0x08
    SMemberNode* FindNode(const unsigned __int64& key) const;   // 0x00812790
};

struct CTCCreatureGroup {
    void*           __vftable;                              // +0x00
    unsigned char   _pad04[8];                              // +0x04 CTCBase
    unsigned long   MemberCount;                            // +0x0c
    unsigned long   ActiveMemberCount;                      // +0x10
    bool            ShutdownOnEmpty;                        // +0x14
    CGroupMemberMap Members;                                // +0x18
    bool IsMember(const CThing* thing) const;
};

bool CTCCreatureGroup::IsMember(const CThing* thing) const
{
    if (thing == 0)
        return false;
    if (thing->Dead)
        return false;
    unsigned __int64 id = thing->UniqueID;
    return Members.FindNode(id) != Members.Header;
}
