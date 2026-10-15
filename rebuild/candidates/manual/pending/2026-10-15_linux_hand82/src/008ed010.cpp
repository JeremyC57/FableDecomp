// CTCBuyableHouse::MovedInNotification(CThing* thing) -- catalog label; the body reports whether a thing is held in the component's 8-byte entry list (+0x0c/+0x10)
// Leaf: ContainsThing
//
// Retail 0x008ED010, 33 bytes (`ret 4`). Callee: the SGI-style find over 8-byte entries @ 0x008ECF60
// (`__fastcall`: first in ecx, last in edx, the key BY VALUE and the iterator-category tag by const
// reference on the stack, `ret 8`; it compares each entry's first dword with the key, unrolled x4).
// Called only from CGameScriptInterface::IsObjectInThingsPossession (0x00897220) on the owner's
// interface-60 component, so the "CTCBuyableHouse::MovedInNotification" label is propagated over an
// unrelated body; the class is named here by what it does (CTCPossessionList_008ed010).
// Shape: `last` is a named local (cached in esi, compared after the call); the empty tag temporary
// has a user constructor and is homed in the key's dead argument slot (`lea eax,[esp+8]`) -- the key
// was read into edx before `push esi`; `find(...) != last ? true : false` gives the bare `setne al`.

struct CThing;

struct SRandomAccessTag { SRandomAccessTag() {} };  // random_access_iterator_tag

struct SPossessionEntry                             // 8-byte entry, keyed by its first dword
{
    CThing* m_pThing;
    long    m_count;
};

SPossessionEntry* __fastcall FindPossessionEntry(SPossessionEntry* first, SPossessionEntry* last,
                                                 CThing* key, const SRandomAccessTag& tag);   // 0x008ECF60

struct CTCPossessionList_008ed010
{
    unsigned char     _pad_0x00[0xc];               // +0x00 CTCBase (vptr, parent thing, flags)
    SPossessionEntry* m_first;                      // +0x0c vector<SPossessionEntry> {first, last, end}
    SPossessionEntry* m_last;                       // +0x10
    SPossessionEntry* m_end;                        // +0x14

    bool ContainsThing(CThing* thing) const;
};

bool CTCPossessionList_008ed010::ContainsThing(CThing* thing) const
{
    SPossessionEntry* last = m_last;
    return FindPossessionEntry(m_first, last, thing, SRandomAccessTag()) != last ? true : false;
}
