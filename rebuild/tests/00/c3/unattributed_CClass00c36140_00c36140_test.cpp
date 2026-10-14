#include <cstdio>
extern "C" __declspec(dllimport) long __stdcall CloseHandle(void*);
struct X { void* h; ~X() { CloseHandle(h); } };
int main()
{
    { X x; x.h = 0; }

    std::printf("00c36140_TEST PASS\n");
    return 0;
}