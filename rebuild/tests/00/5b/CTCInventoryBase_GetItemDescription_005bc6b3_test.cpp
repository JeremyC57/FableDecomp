#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventoryBase::GetItemDescription (0x005BC6B3): same logic over a copy of the model.
static int g_live = 0;
class CWideString { public:
    const char* m_pData;
    CWideString(const char* s) : m_pData(s) { ++g_live; }
    CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_live; }
    const CWideString& operator=(const CWideString& other) { m_pData = other.m_pData; return *this; }
    ~CWideString() { --g_live; }
};
static CWideString GetItemDefDescription(long defIndex) { return CWideString(defIndex == 3 ? "A sturdy iron sword." : ""); }
struct CTCInventoryBase_Retail {
    void GetItemDescription(CWideString* out, long defIndex, bool detailed) { *out = GetItemDefDescription(defIndex); }
};
int main()
{
    CTCInventoryBase_Retail inv;
    {
        CWideString out("old");
        inv.GetItemDescription(&out, 3, true);
        if (std::strcmp(out.m_pData, "A sturdy iron sword.") != 0 || g_live != 1) { std::printf("005bc6b3_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("005bc6b3_TEST FAIL\n"); return 1; }
    std::printf("005bc6b3_TEST PASS\n");
    return 0;
}