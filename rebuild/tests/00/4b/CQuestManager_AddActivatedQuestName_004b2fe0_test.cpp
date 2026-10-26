// Behaviour test for 004b2fe0: src/004b2fe0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CQuestManager::AddActivatedQuestName
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::AddActivatedQuestName(const CCharString& questName) -- record a quest as between its start and end screens (once), counting it; false when it was already listed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B2FE0, length 114 bytes (`ret 4`, bool).
// Layout (retail CQuestManager: STLport lists are one word, vectors three): BetweenStartAndEndScreenQuests
// (vector<CCharString>) +0x4c (PDB +0x6c), NumberOfQuestsBetweenStartAndFinishScreens +0x98 (PDB +0xf4).
// Callees: STLport __find(first, last, const CCharString&, const random_access_iterator_tag&) @ 0x004B8E40 (`__fastcall`),
// Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, called from the inlined CCharString operator==),
// CCharString::CCharString(const CCharString&) @ 0x0099EC30 (push_back's in-place copy),
// vector<CCharString>::_M_insert_overflow(pos, const T&, __false_type, n, atend) @ 0x00433530.
// Shape: find() == end() on the member vector directly (no reference local), end() loaded once (ebx); push_back inlined
// with the null-tested placement copy. The STLport __find instantiation is defined in this TU (noinline, /Gr, real
// four-way unrolled body + remainder switch over the inlined CCharString operator==, value and tag by const reference)
// as retail compiled it in the caller's TU; with the body visible VC7.1 schedules `lea esi,[edi+4Ch]` between the tag
// and key pushes as retail. Compare 0x00411570 is the landed body.
// Leaf: AddActivatedQuestName
#include <stddef.h>
#include <new>

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

class CCharStringData { public:                            // the shared string block
    const char*   m_pData;                          // +0x00
    unsigned long m_length;                         // +0x04
};

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

#pragma auto_inline(off)
// 0x004B8E40: the STLport random-access __find instantiation (compiled in this TU, /Gr): four-way unrolled loop plus
// the remainder switch, value and iterator tag by const reference.
__declspec(noinline) CCharString* __fastcall FindString(CCharString* first, CCharString* last, const CCharString& value,
                                                        const random_access_iterator_tag&)
{
    long tripCount = (last - first) >> 2;
    for (; tripCount > 0; --tripCount) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)

struct CStringVector {                              // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
    void _M_insert_overflow(CCharString* pos, const CCharString& x, const __false_type&, size_t n, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

inline CCharString* find(CCharString* first, CCharString* last, const CCharString& value)
{
    return FindString(first, last, value, random_access_iterator_tag());
}

class CQuestManager { public:
    unsigned char _pad00[0x4c];
    CStringVector BetweenStartAndEndScreenQuests;               // +0x4c
    unsigned char _pad58[0x40];
    long          NumberOfQuestsBetweenStartAndFinishScreens;   // +0x98
    bool AddActivatedQuestName(const CCharString& questName);
};

bool CQuestManager::AddActivatedQuestName(const CCharString& questName)
{
    if (find(BetweenStartAndEndScreenQuests.begin(), BetweenStartAndEndScreenQuests.end(), questName) == BetweenStartAndEndScreenQuests.end()) {
        ++NumberOfQuestsBetweenStartAndFinishScreens;
        BetweenStartAndEndScreenQuests.push_back(questName);
        return true;
    }
    return false;
}

// ---- stubs ----
static int g_copies;
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_copies; }
static int g_over; static CCharString* g_overPos; static const CCharString* g_overX; static size_t g_overN; static bool g_overEnd;
static CCharStringData* g_raw2[16]; static CCharString* const g_storage2 = (CCharString*)g_raw2;
void CStringVector::_M_insert_overflow(CCharString* pos, const CCharString& x, const __false_type&, size_t n, bool atend)
{
    ++g_over; g_overPos = pos; g_overX = &x; g_overN = n; g_overEnd = atend;
    long count = _M_finish - _M_start;
    for (long i = 0; i < count; ++i) g_storage2[i].PStringData = _M_start[i].PStringData;
    g_storage2[count].PStringData = x.PStringData;
    _M_start = g_storage2; _M_finish = g_storage2 + count + 1; _M_end_of_storage = g_storage2 + 16;
}
static CCharStringData MakeData(const char* s) { CCharStringData d; d.m_pData = s; d.m_length = (unsigned long)strlen(s); return d; }
int main()
{
    static unsigned char qmBuf[sizeof(CQuestManager)]; CQuestManager* qm = (CQuestManager*)qmBuf;
    static CCharStringData* rawStore[8]; CCharString* storage = (CCharString*)rawStore;
    CCharStringData d[7] = { MakeData("Q0"), MakeData("Q1"), MakeData("Q2"), MakeData("Q3"), MakeData("Q4"), MakeData("Q5"), MakeData("QX") };
    for (int i = 0; i < 6; ++i) storage[i].PStringData = &d[i];
    qm->BetweenStartAndEndScreenQuests._M_start = storage; qm->BetweenStartAndEndScreenQuests._M_finish = storage + 6;
    qm->BetweenStartAndEndScreenQuests._M_end_of_storage = storage + 7;
    qm->NumberOfQuestsBetweenStartAndFinishScreens = 10;
    // already listed: same block, and an equal string in another block (unrolled part and remainder)
    CCharStringData* sameRaw = &d[1]; CCharString& same = *(CCharString*)&sameRaw;
    CHECK(!qm->AddActivatedQuestName(same) && qm->NumberOfQuestsBetweenStartAndFinishScreens == 10 && g_copies == 0);
    CCharStringData q5 = MakeData("Q5"); CCharStringData* eqRaw = &q5; CCharString& eq = *(CCharString*)&eqRaw;
    CHECK(!qm->AddActivatedQuestName(eq) && qm->NumberOfQuestsBetweenStartAndFinishScreens == 10);
    CCharStringData q2 = MakeData("Q2"); eq.PStringData = &q2;
    CHECK(!qm->AddActivatedQuestName(eq));
    // new name, room left: copied in place
    CCharStringData* nxRaw = &d[6]; CCharString& nx = *(CCharString*)&nxRaw;
    CHECK(qm->AddActivatedQuestName(nx) && qm->NumberOfQuestsBetweenStartAndFinishScreens == 11 && g_copies == 1 && g_over == 0);
    CHECK(qm->BetweenStartAndEndScreenQuests._M_finish == storage + 7 && storage[6].PStringData == &d[6]);
    // same length, different text: still new; storage full -> overflow path
    CCharStringData qy = MakeData("QY"); CCharStringData* nyRaw = &qy; CCharString& ny = *(CCharString*)&nyRaw;
    CHECK(qm->AddActivatedQuestName(ny) && qm->NumberOfQuestsBetweenStartAndFinishScreens == 12 && g_over == 1);
    CHECK(g_overPos == storage + 7 && g_overX == &ny && g_overN == 1 && g_overEnd && g_copies == 1);
    CHECK(qm->BetweenStartAndEndScreenQuests._M_finish - qm->BetweenStartAndEndScreenQuests._M_start == 8);
    // null block entry versus a non-null key: not equal; null key equals the null entry
    static CCharStringData* oneRaw[2]; CCharString* one = (CCharString*)oneRaw; one[0].PStringData = 0;
    qm->BetweenStartAndEndScreenQuests._M_start = one; qm->BetweenStartAndEndScreenQuests._M_finish = one + 1;
    qm->BetweenStartAndEndScreenQuests._M_end_of_storage = one + 2;
    CCharStringData* nulRaw = 0; CCharString& nul = *(CCharString*)&nulRaw;
    CHECK(!qm->AddActivatedQuestName(nul));
    CHECK(qm->AddActivatedQuestName(nx) && one[1].PStringData == &d[6]);
    // empty vector
    qm->BetweenStartAndEndScreenQuests._M_start = qm->BetweenStartAndEndScreenQuests._M_finish = one;
    CHECK(qm->AddActivatedQuestName(nx));
    if (fails == 0) printf("004b2fe0_TEST PASS\n"); else printf("004b2fe0_TEST FAIL\n");
    return fails;
}