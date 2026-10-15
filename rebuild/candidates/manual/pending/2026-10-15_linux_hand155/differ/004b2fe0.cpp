// CQuestManager::AddActivatedQuestName(const CCharString& questName) -- record a quest as between its start and end screens (once), counting it; false when it was already listed
// Grade: DIFFER(114v114) -- scheduling only: retail places `lea esi,[edi+4Ch]` (the vector reference) between the tag push and the key push, ours before the tag push. Tried: early-return form (merges the true tails, `add [m],4`), if/return-true form, a vector reference local (this one), member find arguments with the reference only for end(). The tag temporaries land in the argument slot here (no lane gap).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B2FE0, length 114 bytes (`ret 4`, bool).
// Layout (retail CQuestManager: STLport lists are one word, vectors three): BetweenStartAndEndScreenQuests
// (vector<CCharString>) +0x4c (PDB +0x6c), NumberOfQuestsBetweenStartAndFinishScreens +0x98 (PDB +0xf4).
// Callees: STLport __find(first, last, const CCharString&, random_access_iterator_tag) @ 0x004B8E40 (`__fastcall`),
// CCharString::CCharString(const CCharString&) @ 0x0099EC30 (push_back's in-place copy),
// vector<CCharString>::_M_insert_overflow(pos, const T&, __false_type, n, atend) @ 0x00433530.
// Shape: find() == end() with end() loaded once (ebx); push_back inlined with the null-tested placement copy.
// Leaf: AddActivatedQuestName
#include <stddef.h>
#include <new>

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
};

CCharString* __fastcall FindString(CCharString* first, CCharString* last, const CCharString& value,
                                   const random_access_iterator_tag& tag);   // 0x004B8E40

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

struct CQuestManager {
    unsigned char _pad00[0x4c];
    CStringVector BetweenStartAndEndScreenQuests;               // +0x4c
    unsigned char _pad58[0x40];
    long          NumberOfQuestsBetweenStartAndFinishScreens;   // +0x98
    bool AddActivatedQuestName(const CCharString& questName);
};

bool CQuestManager::AddActivatedQuestName(const CCharString& questName)
{
    CStringVector& quests = BetweenStartAndEndScreenQuests;
    if (find(quests.begin(), quests.end(), questName) == quests.end()) {
        ++NumberOfQuestsBetweenStartAndFinishScreens;
        quests.push_back(questName);
        return true;
    }
    return false;
}
