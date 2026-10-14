#include <cstdio>
struct CAnimationData { unsigned long m_size; unsigned long Sizeof() { return m_size; } };
struct C3DAnimationInfo { virtual void v00() { } virtual void v01() { } virtual unsigned long Sizeof() { return (Data ? Data->Sizeof() + sizeof(Data) : sizeof(Data)) + 0x14; } void* Bank; unsigned long Index; CAnimationData* Data; };
int main()
{
    CAnimationData d; d.m_size = 100; C3DAnimationInfo i; i.Data = &d;
    if (i.Sizeof() != 100 + 0x18) { std::printf("00a265b0_TEST FAIL\n"); return 1; }
    i.Data = 0; if (i.Sizeof() != 0x18) { std::printf("00a265b0_TEST FAIL\n"); return 1; }
    std::printf("00a265b0_TEST PASS\n"); return 0;
}
