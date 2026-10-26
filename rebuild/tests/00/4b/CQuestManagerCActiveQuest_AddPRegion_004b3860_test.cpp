// Behaviour test for 004b3860: src/004b3860.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CQuestManager::CActiveQuest::AddPRegion(const CRegion* region)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::CActiveQuest::AddPRegion(const CRegion* region) -- remember a region the quest uses (once); a null region clears the list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B3860, length 146 bytes (`ret 4`).
// Identity: the landed CQuestManager::AddQuestRegion 0x004B3900 calls this as CActiveQuest::AddPRegion (catalog
// "CQuestManager::AddRegionToQuestArray"). Retail CActiveQuest: Regions (vector<const CRegion*>, 12 bytes) at +0x10.
// Callees: STLport __find(first, last, const T&, random_access_iterator_tag) @ 0x004B7550 (`__fastcall`),
// vector::_M_insert_overflow(pos, const T&, __true_type, n, atend) @ 0x004BA640, memmove @ 0x00BFEAE6.
// Shape: STLport vector inlines: find() == end() (end() loaded once into ebx), push_back's in-place construct with
// the null test, and clear() = erase(begin(), end()) through __copy_trivial (memmove of zero bytes, `cmp eax,eax`).
// The two out-of-line callees are same-TU noinline stand-ins. The _M_insert_overflow stand-in forwards to the retail
// address. The find stand-in is a plain counted find that writes no memory, the way retail's LTCG-compiled __find
// (0x004B7550, `__fastcall`) is known to the caller. That lets region stay in ebp across the call while its own
// argument slot serves as both the const-ref value and the empty tag (`lea eax,[esp+14h]` twice).
// Leaf: AddPRegion
#include <stddef.h>

extern "C" void* __cdecl memmove(void* dest, const void* src, size_t count);   // 0x00BFEAE6

class CRegion;

struct __true_type { __true_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

typedef const CRegion* PCRegion;

#pragma auto_inline(off)
__declspec(noinline) PCRegion* __fastcall FindRegion(PCRegion* first, PCRegion* last, const PCRegion& value,
                                const random_access_iterator_tag&)       // 0x004B7550 stand-in
{
    long n = last - first;
    for (; n > 0; --n, ++first)
        if (*first == value)
            break;
    return first;
}
#pragma auto_inline(on)

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return result;
    size_t n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}

struct CRegionVector {                              // STLport vector<const CRegion*>
    PCRegion* _M_start;
    PCRegion* _M_finish;
    PCRegion* _M_end_of_storage;
    PCRegion* begin() { return _M_start; }
    PCRegion* end() { return _M_finish; }
    void _M_insert_overflow(PCRegion* pos, const PCRegion& x, const __true_type&, size_t n, bool atend);
    void push_back(const PCRegion& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    PCRegion* erase(PCRegion* first, PCRegion* last)
    {
        PCRegion* i = (PCRegion*)CopyTrivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

void InsertOverflow_004ba640(CRegionVector* v, PCRegion* pos, const PCRegion* x, size_t n, bool atend);
#pragma auto_inline(off)
__declspec(noinline) void CRegionVector::_M_insert_overflow(PCRegion* pos, const PCRegion& x, const __true_type&, size_t n, bool atend)   // 0x004BA640 stand-in
{
    InsertOverflow_004ba640(this, pos, &x, n, atend);
}
#pragma auto_inline(on)

inline PCRegion* find(PCRegion* first, PCRegion* last, const PCRegion& value)
{
    return FindRegion(first, last, value, random_access_iterator_tag());
}

class CQuestManager { public:
    class CActiveQuest { public:
        long          ID;                           // +0x00
        void*         PScriptInfo;                  // +0x04
        void*         PScript;                      // +0x08
        void*         PScriptInfoData;              // +0x0c
        CRegionVector Regions;                      // +0x10
        void AddPRegion(const CRegion* region);
    };
};

void CQuestManager::CActiveQuest::AddPRegion(const CRegion* region)
{
    if (region) {
        if (find(Regions.begin(), Regions.end(), region) == Regions.end())
            Regions.push_back(region);
    } else
        Regions.clear();
}

// ---- stubs and driver ----
class CRegion { public: int id; };
static int g_overflows = 0;
static const CRegion* g_big[16];
void InsertOverflow_004ba640(CRegionVector* v, PCRegion* pos, const PCRegion* x, size_t n, bool atend)
{
    ++g_overflows;
    CHECK(pos == v->_M_finish && n == 1 && atend);
    long size = v->_M_finish - v->_M_start;
    for (long i = 0; i < size; ++i) g_big[i] = v->_M_start[i];
    g_big[size] = *x;
    v->_M_start = g_big; v->_M_finish = g_big + size + 1; v->_M_end_of_storage = g_big + 16;
}
int main()
{
    CRegion r1 = { 1 }, r2 = { 2 }, r3 = { 3 };
    static const CRegion* store[2];
    CQuestManager::CActiveQuest q;
    q.Regions._M_start = store; q.Regions._M_finish = store; q.Regions._M_end_of_storage = store + 2;
    q.AddPRegion(&r1);
    CHECK(q.Regions._M_finish - q.Regions._M_start == 1 && store[0] == &r1 && g_overflows == 0);
    q.AddPRegion(&r1);                                   // already held: unchanged
    CHECK(q.Regions._M_finish - q.Regions._M_start == 1 && g_overflows == 0);
    q.AddPRegion(&r2);
    CHECK(q.Regions._M_finish - q.Regions._M_start == 2 && store[1] == &r2 && g_overflows == 0);
    q.AddPRegion(&r3);                                   // full: overflow insert at the end
    CHECK(g_overflows == 1 && q.Regions._M_start == g_big && q.Regions._M_finish - q.Regions._M_start == 3 && g_big[2] == &r3);
    q.AddPRegion(&r2);                                   // found in the grown storage: unchanged
    CHECK(g_overflows == 1 && q.Regions._M_finish - q.Regions._M_start == 3);
    q.AddPRegion(0);                                     // null clears the list
    CHECK(q.Regions._M_finish == q.Regions._M_start && q.Regions._M_start == g_big);
    if (fails == 0) printf("004b3860_TEST PASS\n");
    return fails;
}
