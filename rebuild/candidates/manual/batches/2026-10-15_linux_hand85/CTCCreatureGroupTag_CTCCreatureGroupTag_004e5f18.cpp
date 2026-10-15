// CTCCreatureGroupTag::CTCCreatureGroupTag(CThing* owner) -- base group constructor, zero the tagger UID and frame, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5F18, length 35 bytes (size-optimised def/factory module 0x004Dxxxx-
// 0x004Exxxx: unaligned entry, `push dword ptr [esp+8]` pass-through -> #pragma optimize("s", on)).
// Callee: CTCCreatureGroup::CTCCreatureGroup(CThing*) @ 0x004E5C2F (manifest label "CScrollingViewport" is propagated).
// Vtable 0x012432F4 (CTCCreatureGroupTag). Built by Create_CTCCreatureGroupTag 0x004E762D (`new` of 0x38 bytes).
// Shape: initialiser-list scalars are scheduled before the derived vtable store: TaggerUID (+0x28, unsigned __int64:
// two dwords from the shared zero register) then TaggedFrame (+0x30). sizeof 0x38 = 0x34 rounded to the __int64
// alignment.
#pragma optimize("s", on)

struct CThing;

struct CTCCreatureGroup {
    CTCCreatureGroup(CThing* owner);        // 0x004E5C2F
    virtual void v00();
    unsigned char _body[0x28 - 4];          // CTCBase, MemberCount, ActiveMemberCount, ShutdownOnEmpty, Members map
};

struct CTCCreatureGroupTag : CTCCreatureGroup {
    unsigned __int64 TaggerUID;             // +0x28
    unsigned long TaggedFrame;              // +0x30
    CTCCreatureGroupTag(CThing* owner);
    virtual void v00();
};

CTCCreatureGroupTag::CTCCreatureGroupTag(CThing* owner)
    : CTCCreatureGroup(owner), TaggerUID(0), TaggedFrame(0)
{
}
