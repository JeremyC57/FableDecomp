// Behaviour test for 006b5ed0: STLport vector<pair<long,float> >::insert (append, shift the tail, or grow through _M_insert_overflow; model copy).
#include <cstdio>
#include <new>
template <class T1, class T2> struct pair { T1 first; T2 second; };
typedef pair<long, float> SW;
struct __false_type { __false_type() { } };
static SW g_store[8];
static int g_overflows = 0;
struct vector {
    SW* _M_start; SW* _M_finish; SW* _M_end_of_storage;
    void _M_insert_overflow(SW* pos, const SW& x, const __false_type&, unsigned int fill_len, bool atend = false)
    {   // stand-in for 0x006B5BE0: open two more slots in the static store and insert there
        ++g_overflows;
        if (fill_len != 1 || atend) return;
        _M_end_of_storage += 2;
        for (SW* p = _M_finish; p != pos; --p) p[0] = p[-1];
        *pos = x; ++_M_finish;
    }
    SW* insert(SW* pos, const SW& x)
    {
        unsigned int n = pos - _M_start;
        if (_M_finish != _M_end_of_storage) {
            if (pos == _M_finish) { new ((void*)_M_finish) SW(x); ++_M_finish; }
            else {
                new ((void*)_M_finish) SW(*(_M_finish - 1)); ++_M_finish;
                SW copy = x;
                SW* last = _M_finish - 2; SW* result = _M_finish - 1;
                for (int k = last - pos; k > 0; --k) *--result = *--last;
                *pos = copy;
            }
        } else _M_insert_overflow(pos, x, __false_type(), 1UL);
        return _M_start + n;
    }
};
int main()
{
    vector v; v._M_start = v._M_finish = g_store; v._M_end_of_storage = g_store + 3;
    SW a = {10, 1.0f}, b = {30, 3.0f}, c = {20, 2.0f}, d = {5, 0.5f};
    int fail = 0;
    if (v.insert(v._M_finish, a) != g_store) fail = 1;           // append into an empty vector
    if (v.insert(v._M_finish, b) != g_store + 1) fail = 1;       // append
    if (v.insert(g_store + 1, c) != g_store + 1) fail = 1;       // middle insert shifts (30) up
    if (v._M_finish != g_store + 3 || g_overflows != 0) fail = 1;
    if (v.insert(g_store, d) != g_store || g_overflows != 1) fail = 1;   // full: grows
    long want[4] = {5, 10, 20, 30};
    for (int i = 0; i < 4; ++i) if (g_store[i].first != want[i] || g_store[i].second != want[i] / 10.0f) fail = 1;
    if (fail) { std::printf("006b5ed0_TEST FAIL\n"); return 1; }
    std::printf("006b5ed0_TEST PASS\n");
    return 0;
}