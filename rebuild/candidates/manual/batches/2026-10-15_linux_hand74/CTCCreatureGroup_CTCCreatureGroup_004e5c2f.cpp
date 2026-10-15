// CTCCreatureGroup::CTCCreatureGroup(CThing* thing) -- CTCBase(thing), own vtable, MemberCount = ActiveMemberCount = 0, ShutdownOnEmpty = true, then the empty Members map
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5C2F, 44 bytes. Manifest label "CScrollingViewport::CScrollingViewport"
// is propagated and rejected: the vtable stored is 0x0124326C = CTCCreatureGroup (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (landed), the Members map constructor @ 0x004E4082
// (out of line; manifest "CDisplayManager::CopyBackBufferToTexture" is a propagated name).
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`): the pass-through `push dword ptr [esp+8]`
// and the `and dword ptr [..],0` zero stores; the init-list scalars are scheduled around the vtable store
// and the map member's constructor is called last.
// Layout = Ego_r.pdb CTCCreatureGroup (PDB 40 bytes, retail 0x24: the map is 12 bytes): CTCBase 0xC,
// MemberCount +0xC, ActiveMemberCount +0x10, ShutdownOnEmpty +0x14, Members
// (map<unsigned __int64, CGroupMemberData>) +0x18. Base modelled as in the landed CTCRockTrollShield 0x007AEE10.
// Leaf: ??0CTCCreatureGroup

#pragma optimize("s", on)

struct CThing;

struct CTCBase_Calls {                           // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    bool    ToBeRemoved;                         // +0x08
    bool    IsInGlobalUpdateTCs;                 // +0x09
    CTCBase_Calls(CThing* thing);                // 0x00686800
};

struct CGroupMemberMap {                         // std::map<unsigned __int64, CGroupMemberData> (retail 12 bytes)
    void* _Myhead;
    void* _Reserved;
    unsigned long _Mysize;
    CGroupMemberMap();                           // 0x004E4082
};

struct CTCCreatureGroup : CTCBase_Calls {        // vtable 0x0124326C
    unsigned long   MemberCount;                 // +0x0C
    unsigned long   ActiveMemberCount;           // +0x10
    bool            ShutdownOnEmpty;             // +0x14
    CGroupMemberMap Members;                     // +0x18
    virtual void v00();
    CTCCreatureGroup(CThing* thing);
};

CTCCreatureGroup::CTCCreatureGroup(CThing* thing)
    : CTCBase_Calls(thing)
    , MemberCount(0)
    , ActiveMemberCount(0)
    , ShutdownOnEmpty(true)
{
}
