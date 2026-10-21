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

struct CCharStringData {                            // the shared string block
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

struct CCharString {
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

struct CQuestManager {
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