// Behaviour test for 0x00D18CA0 vector<NScript::CQ_BountyHuntScript::TeleportData>::_M_insert_overflow: the STLport grow path --
// new capacity size + max(size, fill_len), prefix copied, fill_len copies of x, suffix copied unless atend, old block
// cleared (every element destroyed, block freed), new block adopted.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies, g_dtors, g_frees;

struct TeleportData {                       // the record, with a counting copy constructor
    int defName; unsigned int appearanceSeed; float health; int scriptName; int teleportTo; int teleportMkr;
    TeleportData() : defName(0), appearanceSeed(0), health(0.0f), scriptName(0) {}
    TeleportData(const TeleportData& o) : defName(o.defName), appearanceSeed(o.appearanceSeed), health(o.health), scriptName(o.scriptName) { ++g_copies; }
    ~TeleportData() { ++g_dtors; }
};

struct __false_type { };

template <class T> struct vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static const unsigned int& (max)(const unsigned int& a, const unsigned int& b) { return a < b ? b : a; }
    void _M_clear()
    {
        for (T* p = _M_start; p != _M_finish; ++p) p->~T();
        if (_M_start) { std::free(_M_start); ++g_frees; }
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (max)(old_size, fill_len);
        T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* new_finish = new_start;
        for (T* p = _M_start; p != position; ++p, ++new_finish) new (new_finish) T(*p);
        if (fill_len == 1) { new (new_finish) T(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) T(x);
        if (!atend)
            for (T* p = position; p != _M_finish; ++p, ++new_finish) new (new_finish) T(*p);
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    int bad = 0;
    vec<TeleportData> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    TeleportData x; x.defName = 7; x.health = 2.5f;
    // push_back into an empty vector: capacity 0 + max(0, 1) = 1
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || v._M_start[0].defName != 7 || g_copies != 1) bad |= 1;
    // second push_back: capacity 1 + 1 = 2, one prefix copy + the new element, old element destroyed, old block freed
    x.defName = 8;
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);
    if (v.size() != 2 || v._M_end_of_storage - v._M_start != 2 || v._M_start[1].defName != 8 || g_copies != 3 || g_dtors != 1 || g_frees != 1) bad |= 2;
    // middle insert of 3 copies, not at end: capacity 2 + max(2, 3) = 5; order 7, 9, 9, 9, 8
    x.defName = 9;
    v._M_insert_overflow(v._M_start + 1, x, __false_type(), 3, false);
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5) bad |= 4;
    if (v._M_start[0].defName != 7 || v._M_start[1].defName != 9 || v._M_start[3].defName != 9 || v._M_start[4].defName != 8) bad |= 8;
    if (g_copies != 8 || g_dtors != 3 || g_frees != 2) bad |= 16;
    if (sizeof(TeleportData) != 24) bad |= 32;
    v._M_clear();
    if (bad) { std::printf("00d18ca0_TEST FAIL (%d)\n", bad); return 1; }
    std::printf("00d18ca0_TEST PASS\n");
    return 0;
}