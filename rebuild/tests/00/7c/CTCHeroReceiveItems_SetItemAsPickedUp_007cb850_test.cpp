// Behaviour test for 0x007CB850 CTCHeroReceiveItems::SetItemAsPickedUp: an item is appended once; repeats are ignored;
// a full vector grows.
#include <cstdio>
#include <cstdlib>

struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    void push_back(const long& x)
    {
        if (_M_finish == _M_end_of_storage) {
            unsigned int n = (unsigned int)(_M_finish - _M_start);
            unsigned int cap = n ? 2 * n : 1;
            long* s = (long*)std::malloc(cap * sizeof(long));
            for (unsigned int i = 0; i < n; ++i) s[i] = _M_start[i];
            std::free(_M_start);
            _M_start = s; _M_finish = s + n; _M_end_of_storage = s + cap;
        }
        *_M_finish++ = x;
    }
};
template <class It, class T> It find(It first, It last, const T& val)
{
    for (; first != last; ++first)
        if (*first == val) return first;
    return last;
}
struct CTCHeroReceiveItems {
    CLongVector PickedUpItems;
    void SetItemAsPickedUp(long item)
    {
        if (find(PickedUpItems.begin(), PickedUpItems.end(), item) == PickedUpItems.end())
            PickedUpItems.push_back(item);
    }
};

int main()
{
    CTCHeroReceiveItems tc; tc.PickedUpItems._M_start = tc.PickedUpItems._M_finish = tc.PickedUpItems._M_end_of_storage = 0;
    tc.SetItemAsPickedUp(5);
    tc.SetItemAsPickedUp(9);
    tc.SetItemAsPickedUp(5);
    tc.SetItemAsPickedUp(12);
    tc.SetItemAsPickedUp(9);
    long* v = tc.PickedUpItems._M_start;
    bool ok = tc.PickedUpItems._M_finish - v == 3 && v[0] == 5 && v[1] == 9 && v[2] == 12;
    std::free(v);
    if (!ok) { std::printf("007cb850_TEST FAIL\n"); return 1; }
    std::printf("007cb850_TEST PASS\n");
    return 0;
}