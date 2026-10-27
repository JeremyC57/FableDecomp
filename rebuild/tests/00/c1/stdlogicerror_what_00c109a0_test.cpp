// behaviour test for 00c109a0 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct Str { char _Alval; union { char _Buf[16]; char* _Ptr; } _Bx; unsigned int _Mysize; unsigned int _Myres;
    const char* c_str() const { return (16 <= _Myres ? _Bx._Ptr : _Bx._Buf); } };
struct LogicError { virtual ~LogicError() {} virtual const char* what() const { return _Str.c_str(); } Str _Str; };

int main()
{
    LogicError e; std::strcpy(e._Str._Bx._Buf, "short"); e._Str._Mysize = 5; e._Str._Myres = 15;
    if (!(std::strcmp(e.what(), "short") == 0)) { std::printf("00c109a0_TEST FAIL\n"); return 1; }
    static char longtext[] = "a much longer message text"; e._Str._Bx._Ptr = longtext; e._Str._Myres = 31;
    if (!(e.what() == longtext)) { std::printf("00c109a0_TEST FAIL\n"); return 1; }
    std::printf("00c109a0_TEST PASS\n");
    return 0;
}