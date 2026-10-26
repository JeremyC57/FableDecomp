// Behaviour test for 0x0051C850 std::vector<CNavigationAreaUpdate::CSwitchableBlockage>::push_back: with room the blockage is
// copy-constructed in place at _M_finish and _M_finish advances; when full, _M_insert_overflow(finish, x, tag, 1, true) runs.
#include <cstdio>
#include <new>
static int g_copies = 0, g_grows = 0;
struct CSwitchableBlockage {                         // CNavigationAreaUpdate::CSwitchableBlockage model (16 bytes)
    unsigned int UniqueIDLo, UniqueIDHi; void* LineList; unsigned int pad;
    CSwitchableBlockage() {}
    CSwitchableBlockage(const CSwitchableBlockage& o) : UniqueIDLo(o.UniqueIDLo), UniqueIDHi(o.UniqueIDHi), LineList(o.LineList) { ++g_copies; }
};
struct __false_type { __false_type() {} };
struct Vec {
    CSwitchableBlockage* _M_start; CSwitchableBlockage* _M_finish; CSwitchableBlockage* _M_end_of_storage;
    unsigned long lastFill; bool lastAtEnd; CSwitchableBlockage* lastPos;
    void _M_insert_overflow(CSwitchableBlockage* pos, const CSwitchableBlockage&, const __false_type&, unsigned long fill, bool atend)
    { ++g_grows; lastPos = pos; lastFill = fill; lastAtEnd = atend; }
    void push_back(const CSwitchableBlockage& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CSwitchableBlockage(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
int main()
{
    unsigned char raw[2 * sizeof(CSwitchableBlockage)];
    Vec v; v._M_start = (CSwitchableBlockage*)raw; v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 2;
    CSwitchableBlockage x; x.UniqueIDLo = 0x1234; x.UniqueIDHi = 7; x.LineList = &x;
    v.push_back(x); v.push_back(x); v.push_back(x);
    bool ok = sizeof(CSwitchableBlockage) == 16 && g_copies == 2 && g_grows == 1 && v._M_finish == v._M_start + 2
           && v._M_start[1].UniqueIDLo == 0x1234 && v._M_start[1].UniqueIDHi == 7 && v._M_start[1].LineList == &x
           && v.lastPos == v._M_start + 2 && v.lastFill == 1 && v.lastAtEnd;
    if (!ok) { std::printf("0051c850_TEST FAIL\n"); return 1; }
    std::printf("0051c850_TEST PASS\n");
    return 0;
}
