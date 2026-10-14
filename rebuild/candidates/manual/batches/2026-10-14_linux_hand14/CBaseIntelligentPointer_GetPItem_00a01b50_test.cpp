#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CBaseObject;
struct CBaseObjectPointer { CBaseObject* PItem; long RefCount; };
static int g_deleted = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObjectPointer* m_pData;
    CBaseIntelligentPointer() : m_pData(0) {}
    void ClearData()
    {
        if (m_pData != 0) {
            m_pData->RefCount--;
            if (m_pData->RefCount == 0 && m_pData->PItem == 0) { ++g_deleted; delete m_pData; }
            m_pData = 0;
        }
    }
    CBaseObject* GetPItem()
    {
        if (m_pData != 0) {
            if (m_pData->PItem != 0) return m_pData->PItem;
            ClearData();
        }
        return 0;
    }
    ~CBaseIntelligentPointer() { ClearData(); }
};

static bool Check()
{
    CBaseIntelligentPointer p;
    if (p.GetPItem() != 0) return false;
    CBaseObjectPointer* rec = new CBaseObjectPointer; rec->PItem = (CBaseObject*)0x1234; rec->RefCount = 1;
    p.m_pData = rec;
    if (p.GetPItem() != (CBaseObject*)0x1234 || p.m_pData != rec) return false;
    rec->PItem = 0;
    if (p.GetPItem() != 0 || p.m_pData != 0 || g_deleted != 1) return false;    // dead record dropped and freed
    return true;
}
int main()
{
    if (!Check()) { std::printf("00a01b50_TEST FAIL\n"); return 1; }
    std::printf("00a01b50_TEST PASS\n");
    return 0;
}
