// Behaviour test for 0x0099C550 std::_String_base<unsigned short,std::allocator<unsigned short> >::_M_throw_length_error() const: prints the class name, then
// aborts (puts / abort are recording stubs here).
#include <cstdio>
#include <cstring>
static int g_n = 0; static const char* g_s = 0;
extern "C" int __cdecl myputs(const char* s) { g_s = s; return 0; }
extern "C" void __cdecl myabort() { ++g_n; }
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
};
template <class _Tp, class _Alloc> void _String_base<_Tp, _Alloc>::_M_throw_length_error() const
{
    myputs("basic_string");
    myabort();
}
}
int main()
{
    std::_String_base<unsigned short, std::allocator<unsigned short> > s;
    s._M_start = s._M_finish = s._M_end_of_storage = 0;
    s._M_throw_length_error();
    if (g_n != 1 || !g_s || std::strcmp(g_s, "basic_string") != 0) { std::printf("0099c550_TEST FAIL\n"); return 1; }
    std::printf("0099c550_TEST PASS\n");
    return 0;
}
