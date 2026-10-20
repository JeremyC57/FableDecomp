// Grade: DIFFER(283v290) -- register allocation in the filter loop: retail keeps the local vector's finish in ebx (reloaded only after the overflow insert) and homes the strength-reduced element offset in the dead `quests` slot; ours keeps the offset in ebx and the finish in memory (the loop-top alignment `jmp` adds bytes). Same result without the _M_insert_overflow stand-in (probe p27).
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
// Same-TU stand-in: _M_insert_overflow is compiled above (not the real body; it only READS the value and grows through
// `this`) so VC7.1 knows the local vector does not escape and keeps its finish pointer in ebx across IsQuestActive,
// as retail does -- review at landing.
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
CTestQuest* GrowTestQuestStorage(unsigned long count);   // stand-in callee of the stand-in below

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

#pragma auto_inline(off)
void CTestQuestVector::_M_insert_overflow(CTestQuest* pos, const CTestQuest& x, const __false_type& tag, unsigned long fillLength, bool atEnd)
{
    if (*(void**)&x != pos)
        return;
    CTestQuest* storage = GrowTestQuestStorage(fillLength);
    _M_start = storage;
    _M_finish = storage + fillLength;
    _M_end_of_storage = _M_finish;
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
