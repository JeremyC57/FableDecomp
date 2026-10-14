#include <cstdio>
struct CThing {};
static int g_destroyed;
struct CCharString { char* m_pData; ~CCharString() { ++g_destroyed; } };
struct SCoverEntry { CThing* thing; CCharString name; long value; };
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
static SCoverEntry* CopyEntries(SCoverEntry* first, SCoverEntry* last, SCoverEntry* dest, const SRandomAccessIteratorTag&, long)
{ for (; first != last; ++first, ++dest) { dest->thing = first->thing; dest->name.m_pData = first->name.m_pData; dest->value = first->value; } return dest; }
struct CCoverEntryVector {
    SCoverEntry* first; SCoverEntry* last; SCoverEntry* end;
    void Erase(SCoverEntry* where) { if (where + 1 != last) CopyEntries(where + 1, last, where, SRandomAccessIteratorTag(), 0); --last; last->name.~CCharString(); }
};
struct CCoverListOwner_00786580 {
    unsigned char _pad[0xc]; bool m_coverListChanged; CCoverEntryVector m_coverList;
    void RemoveThingFromCoverList(CThing* thing) { for (SCoverEntry* it = m_coverList.first; it != m_coverList.last; ) { if (it->thing == thing) m_coverList.Erase(it); else ++it; } m_coverListChanged = true; }
};
int main()
{
    CThing a, b, c; SCoverEntry store[5];
    CThing* order[5] = { &a, &b, &a, &c, &a };
    for (int i = 0; i < 5; ++i) { store[i].thing = order[i]; store[i].name.m_pData = 0; store[i].value = i; }
    CCoverListOwner_00786580 o; o.m_coverListChanged = false; o.m_coverList.first = store; o.m_coverList.last = store + 5; o.m_coverList.end = store + 5;
    o.RemoveThingFromCoverList(&a);
    if (!o.m_coverListChanged || o.m_coverList.last != store + 2 || store[0].thing != &b || store[0].value != 1 || store[1].thing != &c || store[1].value != 3 || g_destroyed != 3) { std::printf("00786580_TEST FAIL\n"); return 1; }
    o.RemoveThingFromCoverList(&a);
    if (o.m_coverList.last != store + 2 || g_destroyed != 3) { std::printf("00786580_TEST FAIL\n"); return 1; }
    std::printf("00786580_TEST PASS\n"); return 0;
}
