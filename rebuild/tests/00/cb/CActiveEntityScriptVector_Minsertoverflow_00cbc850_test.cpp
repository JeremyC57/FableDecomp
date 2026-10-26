// Behaviour test for 00cbc850: body copied from src/00cbc850.cpp (CActiveEntityScriptVector::_M_insert_overflow), callees stubbed.
#include <cstdio>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_malloc = 0, g_free = 0; static unsigned int g_mallocSize = 0; static void* g_freed = 0;
static void* my_malloc(unsigned int size) { ++g_malloc; g_mallocSize = size; return std::malloc(size); }
static void my_free(void* p) { ++g_free; g_freed = p; }
static void real_free(void* p) { std::free(p); }
#define malloc my_malloc
#define free my_free
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
class CActiveEntityScriptBase;
struct SCountBlock { long RefCount; };
struct CCountedPointer_CActiveEntityScriptBase {
    CActiveEntityScriptBase* Data; SCountBlock* Info;
    CCountedPointer_CActiveEntityScriptBase() : Data(0), Info(0) { }
    CCountedPointer_CActiveEntityScriptBase(const CCountedPointer_CActiveEntityScriptBase& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct SEntityScriptEntry {
    unsigned long KeyLow; unsigned long KeyHigh; CCountedPointer_CActiveEntityScriptBase Script;
    SEntityScriptEntry() : KeyLow(0), KeyHigh(0) { }
    SEntityScriptEntry(const SEntityScriptEntry& other) : KeyLow(other.KeyLow), KeyHigh(other.KeyHigh), Script(other.Script) { }
};
static int g_ucopy = 0, g_fill = 0, g_destroy = 0; static unsigned long g_fillN = 0;
static SEntityScriptEntry* __fastcall __uninitialized_copy(SEntityScriptEntry* first, SEntityScriptEntry* last, SEntityScriptEntry* result, const __false_type&)
{ ++g_ucopy; for (; first != last; ++first, ++result) new (result) SEntityScriptEntry(*first); return result; }
static SEntityScriptEntry* __fastcall __uninitialized_fill_n(SEntityScriptEntry* first, unsigned long n, const SEntityScriptEntry& x, const __false_type&)
{ ++g_fill; g_fillN = n; for (unsigned long i = 0; i < n; ++i) new (first + i) SEntityScriptEntry(x); return first + n; }
static void __fastcall _Destroy(SEntityScriptEntry* first, SEntityScriptEntry* last, const __false_type&) { ++g_destroy; (void)first; (void)last; }
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
struct CActiveEntityScriptVector {
    SEntityScriptEntry* _M_start; SEntityScriptEntry* _M_finish; SEntityScriptEntry* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static SEntityScriptEntry* allocate(unsigned long n) { return n != 0 ? (SEntityScriptEntry*)malloc(n * sizeof(SEntityScriptEntry)) : 0; }
    static void deallocate(SEntityScriptEntry* p) { if (p) free(p); }
    void _M_clear() { _Destroy(_M_start, _M_finish, __false_type()); deallocate(_M_start); }
    void _M_set(SEntityScriptEntry* s, SEntityScriptEntry* f, SEntityScriptEntry* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(SEntityScriptEntry* position, const SEntityScriptEntry& x, const __false_type&, unsigned long fill_len, bool atend);
};
void CActiveEntityScriptVector::_M_insert_overflow(SEntityScriptEntry* position, const SEntityScriptEntry& x,
                                                   const __false_type&, unsigned long fill_len, bool atend)
{
    const unsigned long old_size = size();
    const unsigned long len = old_size + (max)(old_size, fill_len);
    SEntityScriptEntry* new_start = allocate(len);
    SEntityScriptEntry* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
int main()
{
    SCountBlock block; block.RefCount = 1;
    SEntityScriptEntry old[2]; old[0].KeyLow = 1; old[1].KeyLow = 2;
    SEntityScriptEntry x; x.KeyLow = 7; x.KeyHigh = 8; x.Script.Info = &block;
    CActiveEntityScriptVector v; v._M_start = old; v._M_finish = old + 2; v._M_end_of_storage = old + 2;
    v._M_insert_overflow(old + 2, x, __false_type(), 1, true);          // append one: len 4
    CHECK(g_malloc == 1 && g_mallocSize == 4 * sizeof(SEntityScriptEntry) && g_ucopy == 1 && g_fill == 0);
    CHECK(g_destroy == 1 && g_free == 1 && g_freed == old);
    CHECK(v._M_finish - v._M_start == 3 && v._M_end_of_storage - v._M_start == 4 && v._M_start[2].KeyLow == 7 && v._M_start[2].KeyHigh == 8 && block.RefCount == 2);
    SEntityScriptEntry* second = v._M_start;
    v._M_insert_overflow(second + 1, x, __false_type(), 3, false);      // insert 3 in the middle: len 3 + max(3,3) = 6, tail copied
    CHECK(g_malloc == 2 && g_mallocSize == 6 * sizeof(SEntityScriptEntry) && g_fill == 1 && g_fillN == 3 && g_ucopy == 3 && g_destroy == 2 && g_freed == second);
    CHECK(v._M_finish - v._M_start == 6 && v._M_start[0].KeyLow == 1 && v._M_start[1].KeyLow == 7 && v._M_start[3].KeyLow == 7 && v._M_start[4].KeyLow == 2 && v._M_start[5].KeyLow == 7);
    CActiveEntityScriptVector e; e._M_start = 0; e._M_finish = 0; e._M_end_of_storage = 0;
    e._M_insert_overflow(0, x, __false_type(), 0, true);                 // empty + 0: no allocation, no free
    CHECK(g_malloc == 2 && g_free == 2 && e._M_start == 0 && e._M_finish == 0);
    real_free(second); real_free(v._M_start);
    if (fails == 0) printf("00cbc850_TEST PASS\n"); else printf("00cbc850_TEST FAIL (%d)\n", fails);
    return fails;
}