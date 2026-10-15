#include <cstdio>
#include <cstring>
// Behaviour of 0x00CA5520 (chunk 901 piece): returns the member at +0xa1
struct CLibraryObject_Retail {
    unsigned char _pad_0x00[0xa1];
    unsigned char m_byteA1;
    unsigned char func_00ca5520();
};
unsigned char CLibraryObject_Retail::func_00ca5520() { return m_byteA1; }
int main()
{
    CLibraryObject_Retail o; std::memset(&o, 0xAB, sizeof o); o.m_byteA1 = 0xC7;
    if (o.func_00ca5520() != 0xC7) { std::printf("00ca5520_TEST FAIL\n"); return 1; }
    std::printf("00ca5520_TEST PASS\n"); return 0;
}
