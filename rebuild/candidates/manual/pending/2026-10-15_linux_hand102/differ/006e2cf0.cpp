// CTCTalk::SetAsUsingNarrator(const CCharString& narrator) -- a pure-AI speaker stops being one when the narrator is not in the game's narrator name list
// Grade: DIFFER(66v64) -- scheduling only: retail reads the narrator reference into edx BEFORE loading last into esi and loads first straight into ecx; the lane loads last first and first through eax (`mov ecx,eax`). Tried: last local, no local (re-read), result local, begin()/end() accessors + template front end.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2CF0, length 64 bytes (ret 4).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall singleton accessor, as landed at 0x007C3040),
// the SGI find over CCharString elements @ 0x004B8FF0 (`__fastcall`: first in ecx, last in edx, the value by const
// reference and the iterator-category tag by const reference on the stack, unrolled x4; manifest
// "std::vector_find_pair_by_string").
// Ego_r.pdb CTCTalk: IsPureAISpeaker +0x34. The name list is the {first, last} pair at +0x100/+0x104 of the record
// the definition manager holds at +0xd0 (descriptively named here).
// Shape: the inline find front end takes the value by const reference and builds the empty tag temporary last, so the
// tag is homed in the narrator's own argument slot (`lea ecx,[esp+0Ch]`) after the reference was read into edx (the
// CTCBuyableHouse 0x00747450 shape); `last` is a named local kept in esi for the compare after the call.
struct CCharString { void* m_pData; };

struct SRandomAccessTag { SRandomAccessTag() {} };   // random_access_iterator_tag

CCharString* __fastcall FindString(CCharString* first, CCharString* last,
                                   const CCharString& value, const SRandomAccessTag& tag);   // 0x004B8FF0

inline CCharString* Find(CCharString* first, CCharString* last, const CCharString& value)
{
    return FindString(first, last, value, SRandomAccessTag());
}

struct SNarratorRecord {
    unsigned char _pad_0x00[0x100];
    CCharString*  m_pNamesFirst;             // +0x100 vector<CCharString> first
    CCharString*  m_pNamesLast;              // +0x104 vector<CCharString> last
};

struct CGameDefinitionManager {
    unsigned char    _pad_0x00[0xd0];
    SNarratorRecord* m_pNarrators;           // +0xd0
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

struct CTCTalk {
    unsigned char _base[0x34];               // CTCBase + SpeechSample .. LastNarrator
    bool          IsPureAISpeaker;           // +0x34
    void SetAsUsingNarrator(const CCharString& narrator);
};

void CTCTalk::SetAsUsingNarrator(const CCharString& narrator)
{
    if (IsPureAISpeaker) {
        SNarratorRecord* narrators = CGameDefinitionManager::Get().m_pNarrators;
        CCharString* last = narrators->m_pNamesLast;
        if (Find(narrators->m_pNamesFirst, last, narrator) == last)
            IsPureAISpeaker = false;
    }
}
