// Behaviour test for 0x0098D820 _STL::vector<C4DVector>::push_back: with room the value is copied to finish and finish
// advances by one element; when full the overflow path gets (finish, value, 1, true).
#include <cstdio>
#include <new>
struct C4DVector { float X, Y, Z, W; };
struct __false_type { __false_type() { } };
static int g_overflowCalls = 0; static const C4DVector* g_overflowValue = 0; static unsigned int g_fill = 0; static bool g_atEnd = false;
struct Vec {
    C4DVector* _M_start; C4DVector* _M_finish; C4DVector* _M_end_of_storage;
    void _M_insert_overflow(C4DVector* position, const C4DVector& x, const __false_type&, unsigned int fillLen, bool atEnd)
    {
        ++g_overflowCalls; g_overflowValue = &x; g_fill = fillLen; g_atEnd = atEnd;
        if (position != _M_finish) g_fill = 99;
    }
    void push_back(const C4DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C4DVector(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
int main()
{
    int fails = 0;
    C4DVector storage[2];
    Vec v; v._M_start = storage; v._M_finish = storage; v._M_end_of_storage = storage + 2;
    C4DVector a = { 1.0f, 2.0f, 3.0f, 4.0f }, b = { 5.0f, 6.0f, 7.0f, 8.0f };
    v.push_back(a);
    v.push_back(b);
    if (v._M_finish != storage + 2 || storage[0].W != 4.0f || storage[1].X != 5.0f || g_overflowCalls != 0) ++fails;
    v.push_back(a);
    if (g_overflowCalls != 1 || g_overflowValue != &a || g_fill != 1 || !g_atEnd || v._M_finish != storage + 2) ++fails;
    std::printf(fails ? "0098d820_TEST FAIL\n" : "0098d820_TEST PASS\n");
    return fails ? 1 : 0;
}