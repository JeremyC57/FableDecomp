// CCoverListOwner_00786580::RemoveThingFromCoverList(CThing*) -- erase every entry for the thing, mark the list dirty
// Leaf: RemoveThingFromCoverList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786580, length 88 bytes (+ int3 padding).
// Callees: the VC6-era Dinkumware copy helper @ 0x00786EC0 (`_Copy_impl(first, last, dest,
// const Tag&, 0)`, __fastcall: first in ecx, last in edx; dest, the empty iterator-tag temporary
// (by const reference, homed in the now-dead `thing` argument slot: `lea eax,[esp+18h]`) and a
// literal 0 on the stack), the entry's CCharString destructor @ 0x0099EAE0 on the vacated last
// element (`--last` first, then the string at last+4).
// The list at +0x10 is a 12-byte retail vector {first, last, end} of 12-byte entries
// {CThing* thing, CCharString name, long value}; `erase(it)` is inlined: shift the tail down
// when there is one, pop the last element (destroy its string, `last -= 1`), and do not advance.
// The flag at +0xc is set once at the end. `it + 1` is a strength-reduced induction variable
// (ebx). The manifest label (CreatureUseCoverBase::RemoveThingFromCoverList) is propagated --
// that class keeps its list at +0x28 -- so the owner is named by shape.
struct CThing;

struct CCharString {
    char* m_pData;
    ~CCharString();                         // 0x0099EAE0
};

struct SCoverEntry {
    CThing*     thing;                      // +0
    CCharString name;                       // +4
    long        value;                      // +8
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };   // by const reference: homed in a dead slot

SCoverEntry* __fastcall CopyEntries(SCoverEntry* first, SCoverEntry* last, SCoverEntry* dest, const SRandomAccessIteratorTag& iterTag, long flags);   // 0x00786EC0

struct CCoverEntryVector {
    SCoverEntry* first;                     // +0
    SCoverEntry* last;                      // +4
    SCoverEntry* end;                       // +8
    void Erase(SCoverEntry* where)
    {
        if (where + 1 != last)
            CopyEntries(where + 1, last, where, SRandomAccessIteratorTag(), 0);
        --last;
        last->name.~CCharString();
    }
};

struct CCoverListOwner_00786580 {
    unsigned char _pad[0xc];
    bool m_coverListChanged;                // +0xc
    CCoverEntryVector m_coverList;          // +0x10
    void RemoveThingFromCoverList(CThing* thing);
};

void CCoverListOwner_00786580::RemoveThingFromCoverList(CThing* thing)
{
    for (SCoverEntry* it = m_coverList.first; it != m_coverList.last; ) {
        if (it->thing == thing)
            m_coverList.Erase(it);
        else
            ++it;
    }
    m_coverListChanged = true;
}