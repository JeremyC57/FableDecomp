// CTCBuyableHouse::MovedInNotification(CThing* thing) -- catalog label; the body reports whether a thing is in the component's 16-byte entry list (+0x38/+0x3c)
// Leaf: ContainsThing
//
// Retail 0x00747450, 40 bytes (`ret 4`). Callee: the SGI-style find over 16-byte entries @ 0x00748690
// (`__fastcall`: first in ecx, last in edx, the key BY VALUE and the iterator-category tag by const
// reference on the stack; it compares GetPItem() of each entry's leading intelligent pointer with
// the key, unrolled x4). Called from CombatDialogueManager::IsSpeechAllowed (0x008AF5F0) on the
// thing's interface-54 component, so the "CTCBuyableHouse::MovedInNotification" label is propagated
// over an unrelated body; named here by what it does (CTCThingList_00747450).
// Shape: the call goes through an inline find front end that takes the key by CONST REFERENCE and
// builds the empty tag temporary (user constructor) as the last argument: the tag is homed in the
// key's own argument slot (`lea eax,[esp+8] ; push eax`) and only then is the key read through the
// reference (`mov eax,[esp+0Ch]`). Calling the out-of-line find directly with the key by value
// reads the key first and schedules last/first differently (40v40). `m_last` is re-read after
// the call; `return find(...) != m_last;` keeps the full-register bool (`xor ecx,ecx ; setne cl ;
// mov al,cl`).

struct CThing;

struct SRandomAccessTag { SRandomAccessTag() {} };  // random_access_iterator_tag

struct SThingEntry                                  // 16 bytes: CIntelligentPointer<CThing> + 8 bytes
{
    unsigned char m_pThing[0x8];                    // +0x00 CIntelligentPointer<CThing>
    unsigned char _pad_0x08[0x8];
};

SThingEntry* __fastcall FindThingEntry(SThingEntry* first, SThingEntry* last,
                                       CThing* key, const SRandomAccessTag& tag);   // 0x00748690

inline SThingEntry* FindThing(SThingEntry* first, SThingEntry* last, CThing* const& key)   // the inline find front end
{
    return FindThingEntry(first, last, key, SRandomAccessTag());
}

struct CTCThingList_00747450
{
    unsigned char _pad_0x00[0x38];
    SThingEntry*  m_first;                          // +0x38 vector<SThingEntry> {first, last, end}
    SThingEntry*  m_last;                           // +0x3c
    SThingEntry*  m_end;                            // +0x40

    bool ContainsThing(CThing* thing) const;
};

bool CTCThingList_00747450::ContainsThing(CThing* thing) const
{
    return FindThing(m_first, m_last, thing) != m_last;
}
