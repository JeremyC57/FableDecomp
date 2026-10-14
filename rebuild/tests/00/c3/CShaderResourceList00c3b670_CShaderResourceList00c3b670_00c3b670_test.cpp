#include <cstdio>
struct CShaderResourceBase_00c3b710 {
    unsigned char _body[4];
    CShaderResourceBase_00c3b710() { _body[0] = 0; }
    CShaderResourceBase_00c3b710(const CShaderResourceBase_00c3b710& other) { _body[0] = other._body[0]; }
};
struct CShaderResourceList_00c3b670 : CShaderResourceBase_00c3b710 {
    void*         m_pItems;
    unsigned long m_count;
    CShaderResourceList_00c3b670() : m_pItems(0), m_count(0) { }
    CShaderResourceList_00c3b670(const CShaderResourceList_00c3b670& other);
};
CShaderResourceList_00c3b670::CShaderResourceList_00c3b670(const CShaderResourceList_00c3b670& other)
    : CShaderResourceBase_00c3b710(other), m_pItems(0), m_count(0)
{
}
int main()
{
    int item = 1;
    CShaderResourceList_00c3b670 a;
    a._body[0] = 0x5a; a.m_pItems = &item; a.m_count = 3;
    CShaderResourceList_00c3b670 b(a);
    if (b._body[0] != 0x5a || b.m_pItems != 0 || b.m_count != 0) { std::printf("00c3b670_TEST FAIL\n"); return 1; }
    std::printf("00c3b670_TEST PASS\n");
    return 0;
}