// Behaviour test for 0061a8a0: src/0061a8a0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCInventoryQuests::GetTestQuestsToDisplay(vector<CTestQuest>* quests)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryQuests::GetTestQuestsToDisplay(vector<CTestQuest>* quests) -- copies the world's test quest list sorted, and unless "show all" is set keeps only the quests the quest manager reports active
// Retail 0x0061A8A0, 304 bytes (+ int3), `ret 4`, esp frame (speed TU).
// Callees: CTCBase::GetWorld() const 0x00686A80, vector<CTestQuest>::operator= 0x00624A30 (manifest
// "vector_assign_questpair"), the std::sort instantiation 0x006257C0 (__fastcall first/last + the empty comparator by
// value; manifest "introsort_and_heapify"), CQuestManager::IsQuestActive(const CCharString&) 0x004AF610 (global
// 0x013B89FC), CTestQuest(const CTestQuest&) 0x004A89D0, vector<CTestQuest>::_M_insert_overflow(pos, const T&,
// const __false_type&, n, atend) 0x004ADB50, the CTestQuest range destructor 0x004ABD90, free 0x00BFEA14.
// Shape: the filtered list is a zeroed local filled by the inline STLport push_back (placement copy when there is room,
// else the out-of-line overflow insert); size() is the signed /28 division re-evaluated every pass; the result is
// assigned back and the local dies through the range destructor and free.
// The vector's _M_insert_overflow and operator= are instantiated in retail's TU, so VC7.1 sees that the local vector does
// not escape: its finish stays cached in ebx across IsQuestActive (reloaded only after the overflow insert), the
// strength-reduced element offset is homed in the dead `quests` slot, and the final _Destroy reuses ebx. The same-TU
// stand-ins below pass the bounds to the externs through their own locals to reproduce that.
// Layout (retail): CWorld test quest list +0xC4, TestQuestShowAll +0x160 (Ego_r.pdb, 0x20 below the PDB); CTestQuest
// 28 bytes, QuestName +0x00.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();
};
struct CTestQuest {                                // 28 bytes (Ego_r.pdb)
    CCharString QuestName;                         // +0x00
    unsigned char _pad_0x04[0x1c - 0x04];
    CTestQuest(const CTestQuest& other);           // 0x004A89D0
};
struct __false_type { __false_type() {} };
void __fastcall _Destroy(CTestQuest* first, CTestQuest* last, const __false_type& tag);   // 0x004ABD90
struct CTestQuestLess { CTestQuestLess() {} };
void __fastcall SortTestQuests(CTestQuest* first, CTestQuest* last, CTestQuestLess compare);   // 0x006257C0

struct CTestQuestVector {                          // vector<CTestQuest> (12 bytes)
    CTestQuest* _M_start;
    CTestQuest* _M_finish;
    CTestQuest* _M_end_of_storage;
    CTestQuestVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CTestQuestVector()
    {
        _Destroy(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    CTestQuestVector& operator=(const CTestQuestVector& other);   // 0x00624A30
    CTestQuest* begin() { return _M_start; }
    CTestQuest* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    CTestQuest& operator[](unsigned long n) { return *(begin() + n); }
    void _M_insert_overflow(CTestQuest* pos, const CTestQuest& x, const __false_type& tag, unsigned long fillLength, bool atEnd);   // 0x004ADB50
    void push_back(const CTestQuest& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CTestQuest(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

void InsertOverflow_004adb50(CTestQuest** start, CTestQuest** finish, CTestQuest** endOfStorage, CTestQuest* pos,
                             const CTestQuest& x, unsigned long fillLength, bool atEnd);
void AssignRange_00624a30(CTestQuest** start, CTestQuest** finish, CTestQuest** endOfStorage,
                          const CTestQuest* first, const CTestQuest* last);
// Stand-ins for the vector<CTestQuest> instantiations in retail's TU (_M_insert_overflow 0x004ADB50, operator= 0x00624A30):
// the bounds go to the extern through stand-in locals, so the caller's local vector does not escape.
#pragma auto_inline(off)
void CTestQuestVector::_M_insert_overflow(CTestQuest* pos, const CTestQuest& x, const __false_type& tag, unsigned long fillLength, bool atEnd)
{
    CTestQuest* start = _M_start;
    CTestQuest* finish = _M_finish;
    CTestQuest* endOfStorage = _M_end_of_storage;
    InsertOverflow_004adb50(&start, &finish, &endOfStorage, pos, x, fillLength, atEnd);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = endOfStorage;
}
CTestQuestVector& CTestQuestVector::operator=(const CTestQuestVector& other)
{
    CTestQuest* start = _M_start;
    CTestQuest* finish = _M_finish;
    CTestQuest* endOfStorage = _M_end_of_storage;
    AssignRange_00624a30(&start, &finish, &endOfStorage, other._M_start, other._M_finish);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = endOfStorage;
    return *this;
}
#pragma auto_inline(on)

struct CQuestManager {
    bool IsQuestActive(const CCharString& name);   // 0x004AF610
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC
struct CWorld {
    unsigned char _pad_0x00[0xc4];
    CTestQuestVector TestQuests;                   // +0xc4
};

struct CTCInventoryQuests {
    unsigned char _pad_0x000[0x160];
    bool TestQuestShowAll;                         // +0x160
    const CWorld* GetWorld() const;                // 0x00686A80 (CTCBase)
    void GetTestQuestsToDisplay(CTestQuestVector* quests);
};

void CTCInventoryQuests::GetTestQuestsToDisplay(CTestQuestVector* quests)
{
    *quests = GetWorld()->TestQuests;
    SortTestQuests(quests->begin(), quests->end(), CTestQuestLess());
    if (TestQuestShowAll)
        return;
    CTestQuestVector active;
    for (unsigned long i = 0; i < quests->size(); ++i)
        if (g_pQuestManager->IsQuestActive((*quests)[i].QuestName))
            active.push_back((*quests)[i]);
    *quests = active;
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_assigns = 0, g_overflows = 0, g_destroys = 0, g_sorts = 0, g_activeQueries = 0;
CCharString::CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) {}
CCharString::~CCharString() {}
CTestQuest::CTestQuest(const CTestQuest& other) : QuestName(other.QuestName) { memcpy(_pad_0x04, other._pad_0x04, sizeof(_pad_0x04)); }
static const char* NameOf(const CTestQuest& q) { return (const char*)q.QuestName.m_pStringData; }
void __fastcall _Destroy(CTestQuest* first, CTestQuest* last, const __false_type& tag) { ++g_destroys; }
void __fastcall SortTestQuests(CTestQuest* first, CTestQuest* last, CTestQuestLess compare)
{
    ++g_sorts;
    for (CTestQuest* i = first; i != last; ++i)
        for (CTestQuest* j = i + 1; j != last; ++j)
            if (strcmp(NameOf(*j), NameOf(*i)) < 0) {
                unsigned char t[sizeof(CTestQuest)]; memcpy(t, i, sizeof t); memcpy(i, j, sizeof t); memcpy(j, t, sizeof t);
            }
}
void InsertOverflow_004adb50(CTestQuest** start, CTestQuest** finish, CTestQuest** endOfStorage, CTestQuest* pos,
                             const CTestQuest& x, unsigned long fillLength, bool atEnd)
{
    ++g_overflows; CHECK(pos == *finish && fillLength == 1 && atEnd);
    long n = *finish - *start; long cap = n ? 2 * n : 1;
    CTestQuest* p = (CTestQuest*)malloc(cap * sizeof(CTestQuest));
    for (long i = 0; i < n; ++i) memcpy(p + i, *start + i, sizeof(CTestQuest));
    new (p + n) CTestQuest(x);
    if (*start) free(*start);
    *start = p; *finish = p + n + 1; *endOfStorage = p + cap;
}
void AssignRange_00624a30(CTestQuest** start, CTestQuest** finish, CTestQuest** endOfStorage,
                          const CTestQuest* first, const CTestQuest* last)
{
    ++g_assigns; long n = last - first;
    CTestQuest* p = n ? (CTestQuest*)malloc(n * sizeof(CTestQuest)) : 0;
    for (long i = 0; i < n; ++i) new (p + i) CTestQuest(first[i]);
    if (*start) free(*start);
    *start = p; *finish = p + n; *endOfStorage = p + n;
}
static CQuestManager g_qm; CQuestManager* g_pQuestManager = &g_qm;
bool CQuestManager::IsQuestActive(const CCharString& name)
{
    ++g_activeQueries; const char* s = (const char*)name.m_pStringData;
    return strcmp(s, "B") == 0 || strcmp(s, "D") == 0 || strcmp(s, "E") == 0;
}
static unsigned char g_worldRaw[sizeof(CWorld)];
const CWorld* CTCInventoryQuests::GetWorld() const { return (const CWorld*)g_worldRaw; }
int main()
{
    CWorld* world = (CWorld*)g_worldRaw;
    static const char* names[5] = { "D", "A", "E", "C", "B" };
    CTestQuest* src = (CTestQuest*)malloc(5 * sizeof(CTestQuest));
    for (int i = 0; i < 5; ++i) { memset(src + i, 0, sizeof(CTestQuest)); src[i].QuestName.m_pStringData = (void*)names[i]; src[i]._pad_0x04[0] = (unsigned char)(10 + i); }
    world->TestQuests._M_start = src; world->TestQuests._M_finish = src + 5; world->TestQuests._M_end_of_storage = src + 5;
    static unsigned char invRaw[sizeof(CTCInventoryQuests)];
    CTCInventoryQuests* inv = (CTCInventoryQuests*)invRaw;
    CTestQuestVector* out = (CTestQuestVector*)malloc(sizeof(CTestQuestVector));
    out->_M_start = out->_M_finish = out->_M_end_of_storage = 0;
    // filtered: B, D, E active, sorted
    inv->TestQuestShowAll = false;
    inv->GetTestQuestsToDisplay(out);
    CHECK(g_sorts == 1 && g_assigns == 2 && g_activeQueries == 5 && g_destroys == 1);
    CHECK(g_overflows == 3);                           // grows 0 -> 1 -> 2 -> 4
    CHECK(out->size() == 3);
    if (out->size() == 3) {
        CHECK(strcmp(NameOf((*out)[0]), "B") == 0 && strcmp(NameOf((*out)[1]), "D") == 0 && strcmp(NameOf((*out)[2]), "E") == 0);
        CHECK((*out)[0]._pad_0x04[0] == 14 && (*out)[1]._pad_0x04[0] == 10 && (*out)[2]._pad_0x04[0] == 12);
    }
    // show all: the sorted copy, no filtering
    inv->TestQuestShowAll = true;
    inv->GetTestQuestsToDisplay(out);
    CHECK(g_sorts == 2 && g_assigns == 3 && g_activeQueries == 5 && g_destroys == 1 && out->size() == 5);
    if (out->size() == 5)
        CHECK(strcmp(NameOf((*out)[0]), "A") == 0 && strcmp(NameOf((*out)[4]), "E") == 0);
    if (fails == 0) printf("0061a8a0_TEST PASS\n");
    return fails;
}
