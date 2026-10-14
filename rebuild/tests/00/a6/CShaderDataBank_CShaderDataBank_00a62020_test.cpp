#include <stdio.h>
#include <stddef.h>
static int g_baseCalls, g_blockCalls;
struct CBankFile { virtual void v00() {} unsigned char _body[0x10c]; CBankFile() { ++g_baseCalls; } };
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) { } };
struct CShaderBankStateBlock { virtual void v00() {} CShaderBankStateBlock() { ++g_blockCalls; } };
struct CShaderDataBank : CBankFile {
    CRetailVector<void*> m_shaders; CShaderBankStateBlock m_stateBlock;
    virtual void v00() {}
    CShaderDataBank();
};
CShaderDataBank::CShaderDataBank() : CBankFile(), m_shaders(), m_stateBlock() { }
int main()
{
    CShaderDataBank b;
    bool ok = g_baseCalls == 1 && g_blockCalls == 1 && b.m_shaders.first == 0 && b.m_shaders.last == 0 && b.m_shaders.end == 0
        && offsetof(CShaderDataBank, m_shaders) == 0x110 && offsetof(CShaderDataBank, m_stateBlock) == 0x11c;
    if (!ok) { printf("00a62020_TEST FAIL\n"); return 1; }
    printf("00a62020_TEST PASS\n"); return 0;
}