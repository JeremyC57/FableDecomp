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
    CBaseObjectPointer* rec = new CBaseObjectPointer; rec->PItem = 0; rec->RefCount = 1;
    { CBaseIntelligentPointer p; p.m_pData = rec; }
    if (g_deleted != 1) return false;
    CBaseObjectPointer* rec2 = new CBaseObjectPointer; rec2->PItem = (CBaseObject*)1; rec2->RefCount = 3;
    { CBaseIntelligentPointer p; p.m_pData = rec2; }
    if (g_deleted != 1 || rec2->RefCount != 2) return false;
    delete rec2;
    return true;
}
int main()
{
    if (!Check()) { std::printf("00a01c10_TEST FAIL\n"); return 1; }
    std::printf("00a01c10_TEST PASS\n");
    return 0;
}
