#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventoryAbilities::InitialiseExpressions (0x005F3E3E): same logic over a copy of the model.
static int g_live = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_live; }
    ~CCharString() { --g_live; }
};
struct CDefinitionManager {
    long GetNumberOfDefsOfType(const CCharString& typeName) { return std::strcmp(typeName.m_pData, "EXPRESSION") == 0 ? 5 : 0; }
};
static CDefinitionManager g_defs;
struct CLongVector {
    long data[8]; unsigned long count;
    void resize(unsigned long size) { while (count < size) data[count++] = 0; count = size; }
};
struct CTCInventoryAbilities_Retail {
    CLongVector ReceivedExpressionOrder;
    CDefinitionManager* GetDefinitions() { return &g_defs; }
    void InitialiseExpressions() { ReceivedExpressionOrder.resize(GetDefinitions()->GetNumberOfDefsOfType("EXPRESSION")); }
};
int main()
{
    CTCInventoryAbilities_Retail a; a.ReceivedExpressionOrder.count = 1; a.ReceivedExpressionOrder.data[0] = 7;
    a.InitialiseExpressions();
    if (a.ReceivedExpressionOrder.count != 5 || a.ReceivedExpressionOrder.data[0] != 7 || a.ReceivedExpressionOrder.data[4] != 0 || g_live != 0) { std::printf("005f3e3e_TEST FAIL\n"); return 1; }
    std::printf("005f3e3e_TEST PASS\n");
    return 0;
}