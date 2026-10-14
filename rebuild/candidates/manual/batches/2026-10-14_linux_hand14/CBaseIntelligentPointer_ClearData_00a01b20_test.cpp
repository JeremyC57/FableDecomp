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
    CBaseIntelligentPointer p; p.ClearData();
    if (p.m_pData != 0) return false;
    CBaseObjectPointer* rec = new CBaseObjectPointer; rec->PItem = (CBaseObject*)0x1234; rec->RefCount = 2;
    p.m_pData = rec; p.ClearData();
    if (p.m_pData != 0 || rec->RefCount != 1 || g_deleted != 0) return false;   // still referenced elsewhere
    p.m_pData = rec; p.ClearData();
    if (rec->RefCount != 0 || g_deleted != 0) return false;                      // count hit zero but the object is alive
    rec->PItem = 0; rec->RefCount = 1; p.m_pData = rec; p.ClearData();
    if (g_deleted != 1 || p.m_pData != 0) return false;                          // orphaned record freed
    return true;
}
int main()
{
    if (!Check()) { std::printf("00a01b20_TEST FAIL\n"); return 1; }
    std::printf("00a01b20_TEST PASS\n");
    return 0;
}
