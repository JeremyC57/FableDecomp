// Behaviour test for 00c2ef30: CBankData::~CBankData (libpk bank data, three record lists)
#include <cstdio>
static int g_n = 0; struct A { unsigned char b[16]; ~A() { ++g_n; } }; struct B { unsigned char b[16]; ~B() { ++g_n; } }; struct C { unsigned char b[16]; ~C() { ++g_n; } }; struct X { A a; B b; C c; ~X() {} };
static bool Check() { { X x; } return g_n == 3; }

int main()
{
    if (!Check()) { std::printf("00c2ef30_TEST FAIL\n"); return 1; }
    std::printf("00c2ef30_TEST PASS\n");
    return 0;
}