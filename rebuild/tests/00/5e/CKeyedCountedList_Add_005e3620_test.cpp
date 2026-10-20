// Behaviour test for 005e3620: body copied from src/005e3620.cpp (CKeyedCountedList::Add), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SScalarTag { SScalarTag() { } };
static int g_destroyed = 0;
struct CCounted { virtual void Destroy() { ++g_destroyed; } long m_refCount; };
struct CCountedRef {
    CCounted* m_p;
    CCountedRef(CCounted* p) : m_p(p) { if (p) ++p->m_refCount; }
    CCountedRef(const CCountedRef& other) { CCounted* p = other.m_p; m_p = p; if (p) ++p->m_refCount; }
    ~CCountedRef() { CCounted* p = m_p; if (p) { if (--p->m_refCount == 0) p->Destroy(); } }
};
struct SKeyedEntry { long first; CCountedRef second; SKeyedEntry(const long& key, const CCountedRef& item) : first(key), second(item) { } };
static int g_overflow = 0;
struct CEntryVector {
    SKeyedEntry* m_start; SKeyedEntry* m_finish; SKeyedEntry* m_endOfStorage;
    void InsertOverflow(SKeyedEntry* position, const SKeyedEntry& value, const SScalarTag&, unsigned long count, bool atEnd)
    { ++g_overflow; CHECK(position == m_finish && count == 1 && atEnd); new (m_finish) SKeyedEntry(value); ++m_finish; ++m_endOfStorage; }
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
struct CKeyedCountedList {
    CEntryVector m_entries; unsigned char m_0c; bool m_changed;
    void Add(const long& key, const CCountedRef& item);
};
void CKeyedCountedList::Add(const long& key, const CCountedRef& item)
{
    m_entries.push_back(SKeyedEntry(key, item));
    m_changed = true;
}
int main()
{
    CCounted obj; obj.m_refCount = 1;
    unsigned char storage[sizeof(SKeyedEntry) * 4]; memset(storage, 0, sizeof storage);
    CKeyedCountedList list; list.m_entries.m_start = (SKeyedEntry*)storage; list.m_entries.m_finish = list.m_entries.m_start; list.m_entries.m_endOfStorage = list.m_entries.m_start + 1;
    list.m_changed = false;
    { CCountedRef ref(&obj); list.Add(5, ref); }
    CHECK(list.m_changed && list.m_entries.m_finish - list.m_entries.m_start == 1 && g_overflow == 0);
    CHECK(list.m_entries.m_start[0].first == 5 && list.m_entries.m_start[0].second.m_p == &obj && obj.m_refCount == 2 && g_destroyed == 0);
    list.m_changed = false;
    { CCountedRef ref(&obj); list.Add(9, ref); }                     // vector full: overflow path
    CHECK(list.m_changed && g_overflow == 1 && list.m_entries.m_start[1].first == 9 && obj.m_refCount == 3);
    { CCountedRef none(0); list.m_changed = false; list.Add(1, none); }
    CHECK(list.m_changed && list.m_entries.m_start[2].second.m_p == 0 && obj.m_refCount == 3 && g_destroyed == 0);
    if (fails == 0) printf("005e3620_TEST PASS\n"); else printf("005e3620_TEST FAIL (%d)\n", fails);
    return fails;
}