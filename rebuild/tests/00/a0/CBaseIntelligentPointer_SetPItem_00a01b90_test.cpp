// Behaviour test for CBaseIntelligentPointer::SetPItem (0x00A01B90): joins the object's shared record,
// creates one when missing, keeps the record for the same object and releases the old one otherwise.
#include <cstdio>

struct CBaseObjectPointer;
struct CBaseObject { CBaseObjectPointer* IntelligentPointer; };
struct CBaseObjectPointer { CBaseObject* PItem; long RefCount; };
struct CBaseIntelligentPointer {
    CBaseObjectPointer* m_pData;
    void ClearData()
    {
        if (m_pData != 0) {
            m_pData->RefCount--;
            if (m_pData->RefCount == 0 && m_pData->PItem == 0) delete m_pData;
            m_pData = 0;
        }
    }
    void SetPItem(CBaseObject* item)
    {
        if (m_pData != 0 && m_pData->PItem == item && item != 0) return;
        ClearData();
        if (item != 0) {
            CBaseObjectPointer* data = item->IntelligentPointer;
            if (data != 0) { m_pData = data; data->RefCount++; return; }
            m_pData = new CBaseObjectPointer;
            m_pData->PItem = item; m_pData->RefCount = 1;
            item->IntelligentPointer = m_pData;
        }
    }
};

int main()
{
    CBaseObject a = { 0 }, b = { 0 };
    CBaseIntelligentPointer p = { 0 }, q = { 0 };
    p.SetPItem(&a);
    if (!a.IntelligentPointer || a.IntelligentPointer->RefCount != 1 || p.m_pData != a.IntelligentPointer) { std::printf("00a01b90_TEST FAIL\n"); return 1; }
    q.SetPItem(&a);
    if (a.IntelligentPointer->RefCount != 2 || q.m_pData != p.m_pData) { std::printf("00a01b90_TEST FAIL\n"); return 1; }
    q.SetPItem(&a);
    if (a.IntelligentPointer->RefCount != 2) { std::printf("00a01b90_TEST FAIL\n"); return 1; }
    q.SetPItem(&b);
    if (a.IntelligentPointer->RefCount != 1 || !b.IntelligentPointer || q.m_pData != b.IntelligentPointer) { std::printf("00a01b90_TEST FAIL\n"); return 1; }
    q.SetPItem(0);
    if (q.m_pData != 0 || b.IntelligentPointer->RefCount != 0) { std::printf("00a01b90_TEST FAIL\n"); return 1; }
    delete a.IntelligentPointer; delete b.IntelligentPointer;
    std::printf("00a01b90_TEST PASS\n");
    return 0;
}