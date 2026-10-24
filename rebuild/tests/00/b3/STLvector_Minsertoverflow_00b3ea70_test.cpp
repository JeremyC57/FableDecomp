#include <cstdio>
#include <cstdlib>
// Behaviour test for vector<CTextureManagerPoolInfo>::_M_insert_overflow @ 0x00B3EA70: grow to old + max(old, fill_len),
// copy the head, one x (or fill_len copies), the tail unless at the end, free the old block.
inline void* operator new(size_t, void* p, int) { return p; }
class CTextureManagerPoolInfo {
public:
    unsigned long Width;
    unsigned long Height;
    long          FormatIndex;
    bool          UseMipmaps;
    unsigned long Count;
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
template <class T> T* UninitializedCopy(T* first, T* last, T* result)
{
    T* cur = result;
    for ( ; first != last; ++first, ++cur)
        new (cur, 0) T(*first);
    return cur;
}
template <class T> T* UninitializedFillN(T* first, unsigned int n, const T& x)
{
    T* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur, 0) T(x);
    return cur;
}
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + Max(old_size, fill_len);
        T* new_start = len != 0 ? (T*)malloc(len * sizeof(T)) : 0;
        T* new_finish = UninitializedCopy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish, 0) T(x);
            ++new_finish;
        } else
            new_finish = UninitializedFillN(new_finish, fill_len, x);
        if (!atend)
            new_finish = UninitializedCopy(position, _M_finish, new_finish);
        if (_M_start != 0)
            free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};
static CTextureManagerPoolInfo Info(unsigned long w)
{
    CTextureManagerPoolInfo i;
    i.Width = w; i.Height = w * 2; i.FormatIndex = (long)w; i.UseMipmaps = (w & 1) != 0; i.Count = w + 1;
    return i;
}
int main()
{
    vector<CTextureManagerPoolInfo> v;
    v._M_start = (CTextureManagerPoolInfo*)malloc(3 * sizeof(CTextureManagerPoolInfo));
    for (int i = 0; i < 3; ++i) v._M_start[i] = Info(i);
    v._M_finish = v._M_start + 3;
    v._M_end_of_storage = v._M_finish;
    v._M_insert_overflow(v._M_finish, Info(9), 1, true);          // push_back at capacity
    if (v.size() != 4 || v._M_end_of_storage - v._M_start != 6) { std::printf("00b3ea70_TEST FAIL\n"); return 1; }
    if (v._M_start[3].Width != 9 || v._M_start[2].Count != 3) { std::printf("00b3ea70_TEST FAIL\n"); return 1; }
    v._M_insert_overflow(v._M_start + 1, Info(7), 5, false);      // insert 5 copies in the middle
    if (v.size() != 9 || v._M_end_of_storage - v._M_start != 9) { std::printf("00b3ea70_TEST FAIL\n"); return 1; }
    static const unsigned long expect[9] = { 0, 7, 7, 7, 7, 7, 1, 2, 9 };
    for (int i = 0; i < 9; ++i)
        if (v._M_start[i].Width != expect[i] || v._M_start[i].Height != expect[i] * 2) { std::printf("00b3ea70_TEST FAIL\n"); return 1; }
    free(v._M_start);
    std::printf("00b3ea70_TEST PASS\n");
    return 0;
}