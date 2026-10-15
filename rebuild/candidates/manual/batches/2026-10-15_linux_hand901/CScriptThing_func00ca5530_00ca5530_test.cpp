#include <cstdio>
#include <cstring>
// Behaviour of 0x00CA5530 (chunk 901 piece): returns the address of the sub-object at +0xa8
struct CLibrarySubObject_Retail { long m_first; };
struct CLibraryObject_Retail {
    unsigned char _pad_0x00[0xa8];
    CLibrarySubObject_Retail m_subA8;
    CLibrarySubObject_Retail* func_00ca5530();
};
CLibrarySubObject_Retail* CLibraryObject_Retail::func_00ca5530() { return &m_subA8; }
int main()
{
    CLibraryObject_Retail o;
    if (o.func_00ca5530() != &o.m_subA8 || (char*)o.func_00ca5530() - (char*)&o != 0xa8) { std::printf("00ca5530_TEST FAIL\n"); return 1; }
    std::printf("00ca5530_TEST PASS\n"); return 0;
}
