// Test for 0x00A35360 vector<pair<unsigned long, C3DMeshVolume> >::push_back: with spare capacity the element is copy-constructed at the end and the size grows; at full
// capacity the call goes to _M_insert_overflow(end, x, __false_type, 1, true) instead.
#include <stdio.h>
#include <new>
struct VolumePair { int Key; int Payload[3]; };
static int g_overflows = 0, g_fill = 0, g_atend = 0;
struct Vec {
    VolumePair* _M_start; VolumePair* _M_finish; VolumePair* _M_end_of_storage;
    void push_back(const VolumePair& x)
    {
        if (_M_finish != _M_end_of_storage) { new ((void*)_M_finish) VolumePair(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
    void _M_insert_overflow(VolumePair* pos, const VolumePair& x, unsigned int fill, bool atend) { ++g_overflows; g_fill = fill; g_atend = atend ? 1 : 0; }
};
int main()
{
    VolumePair buf[2]; Vec v; v._M_start = v._M_finish = buf; v._M_end_of_storage = buf + 2;
    VolumePair a = { 1, { 2, 3, 4 } }, b = { 5, { 6, 7, 8 } }, c = { 9, { 0, 0, 0 } };
    v.push_back(a); v.push_back(b);
    bool ok = v._M_finish == buf + 2 && buf[0].Key == 1 && buf[1].Payload[2] == 8 && g_overflows == 0;
    v.push_back(c);
    if (g_overflows != 1 || g_fill != 1 || g_atend != 1 || v._M_finish != buf + 2) ok = false;
    puts(ok ? "00a35360_TEST PASS" : "00a35360_TEST FAIL");
    return ok ? 0 : 1;
}