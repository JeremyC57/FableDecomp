// CKeyedCountedList::Add(const long&, const CCountedRef&) -- push the (key, counted object) pair onto the entry vector and mark the list changed
// Leaf: CKeyedCountedList
// Fable retry (2026-10-15): a named `SKeyedEntry entry(key, item)` local instead of the temporary grades DIFFER(134v114); the in-place copy
// still reloads both words from the frame after the add-ref store. The two-object idea (register-only counted copy + bitwise frame copy) is untried.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3620, length 114 bytes (+ int3 padding).
// Label correction: the manifest row "CCreatureAction_InterruptableMidAttackAutoTurn::CCreatureAction_InterruptableMidAttackAutoTurn" is
// propagated (no vtable store, `ret 8`, no direct callers or pointer references in the image); the class is named by its effect.
// Callees: the vector's _M_insert_overflow @ 0x005E29D0 (manifest "std::vector::_Reallocate").
// Shape: an 8-byte pair<long, counted pointer> local built from the two reference arguments (the pointer's copy constructor bumps
// the intrusive count at +4), the STLport push_back inlined (in-place null-tested placement copy, `add [edi+4],8`, else
// _M_insert_overflow(finish, value, tag, 1, true) with the empty tag homed in the dead argument slot), the local's destructor
// (`dec [esi+4] ; jne ; call [vtable+4]`), then the changed flag at +0xd.
#include <new>

struct SScalarTag { SScalarTag() { } };       // __false_type

struct CCounted {                           // intrusively counted object: vptr + count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
};
struct CCountedRef {                        // a counted pointer (one word)
    CCounted* m_p;
    CCountedRef(const CCountedRef& other)
    {
        CCounted* p = other.m_p;            // local pointer copies: the count updates go through the register
        m_p = p;
        if (p)
            ++p->m_refCount;
    }
    ~CCountedRef()
    {
        CCounted* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
        }
    }
};
struct SKeyedEntry {                        // pair<long, CCountedRef>
    long        first;
    CCountedRef second;
    SKeyedEntry(const long& key, const CCountedRef& item) : first(key), second(item) { }
};

struct CEntryVector {                       // STLport vector<SKeyedEntry>
    SKeyedEntry* m_start;                   // +0x00
    SKeyedEntry* m_finish;                  // +0x04
    SKeyedEntry* m_endOfStorage;            // +0x08
    void InsertOverflow(SKeyedEntry* position, const SKeyedEntry& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x005E29D0
    void push_back(const SKeyedEntry& value)
    {
        if (m_finish != m_endOfStorage) {
            new (m_finish) SKeyedEntry(value);
            ++m_finish;
        }
        else
            InsertOverflow(m_finish, value, SScalarTag(), 1, true);
    }
};
void* FindRef_CEntryVector_InsertOverflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CEntryVector::InsertOverflow(SKeyedEntry* position, const SKeyedEntry& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    void* p = FindRef_CEntryVector_InsertOverflow_0();
    if (!p)
        return;
    *(void**)position = p;
    if (*(void**)&value != p)
        return;
}
#pragma auto_inline(on)

struct CKeyedCountedList {
    CEntryVector  m_entries;                // +0x00
    unsigned char m_0c;                     // +0x0c
    bool          m_changed;                // +0x0d
    void Add(const long& key, const CCountedRef& item);
};

void CKeyedCountedList::Add(const long& key, const CCountedRef& item)
{
    m_entries.push_back(SKeyedEntry(key, item));
    m_changed = true;
}