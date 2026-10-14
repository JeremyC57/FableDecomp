#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x0048DAA2: exercises the same logic through a copy of the model (compiled separately from the source)
struct CCharString { const char* m_pData; operator const char*() const { return m_pData; } };
struct SEntryHandle_0048daa2 { long m_index; long m_tag; SEntryHandle_0048daa2() {} };
struct CGuiControlListBox {
    virtual void v00() {}
    virtual SEntryHandle_0048daa2 SelectEntryFromName(const char* name, long mode) { SEntryHandle_0048daa2 h; h.m_index = (long)std::strlen(name); h.m_tag = mode * 10; return h; }
    SEntryHandle_0048daa2 SelectEntryFromName(const CCharString& name, long mode) { return SelectEntryFromName((const char*)name, mode); }
};
static bool Run()
{
    CGuiControlListBox box; CCharString name; name.m_pData = "Potion";
    SEntryHandle_0048daa2 h = box.SelectEntryFromName(name, 3);
    return h.m_index == 6 && h.m_tag == 30;
}
int main()
{
    if (!Run()) { std::printf("0048daa2_TEST FAIL\n"); return 1; }
    std::printf("0048daa2_TEST PASS\n");
    return 0;
}