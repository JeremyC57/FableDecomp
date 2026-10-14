#include <cstdio>
struct CIndexBuffer { virtual void v00() { } void* PD3DIndexBuffer; unsigned long ByteLength; long PrimitiveType; void* PAllocatedBuffer; CIndexBuffer() : PD3DIndexBuffer(0), ByteLength(0) { } };
struct CLockedIndexBuffer : CIndexBuffer { unsigned char _pad_0x14[0x18]; unsigned long m_lockOffset; CLockedIndexBuffer() : m_lockOffset(0x80000000) { } };
int main()
{
    CLockedIndexBuffer b;
    if (sizeof(CIndexBuffer) != 0x14 || (char*)&b.m_lockOffset != (char*)&b + 0x2c || b.m_lockOffset != 0x80000000 || b.PD3DIndexBuffer != 0 || b.ByteLength != 0) { std::printf("00c59ba0_TEST FAIL\n"); return 1; }
    std::printf("00c59ba0_TEST PASS\n"); return 0;
}