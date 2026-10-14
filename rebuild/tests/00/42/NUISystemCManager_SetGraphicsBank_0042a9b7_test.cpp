#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CGraphicDataBank { int x; }; struct CCPPointerInfo { long m_references; };
struct CCountedPointer {
    CGraphicDataBank* m_pObject; CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pObject(0), m_pInfo(0) { }
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pInfo(o.m_pInfo) { if (m_pInfo) ++m_pInfo->m_references; }
    ~CCountedPointer() { if (m_pInfo) --m_pInfo->m_references; }
    void ShareData(CGraphicDataBank* object, CCPPointerInfo* info) { if (m_pInfo != info) { if (m_pInfo) --m_pInfo->m_references; m_pObject = object; m_pInfo = info; if (info) ++info->m_references; } }
    CCountedPointer& operator=(const CCountedPointer& o) { ShareData(o.m_pObject, o.m_pInfo); return *this; }
};
struct CManager { unsigned char _pad[0x10]; CCountedPointer m_graphicsBank; void SetGraphicsBank(CCountedPointer bank) { m_graphicsBank = bank; } };

int main()
{
    CGraphicDataBank bank; CCPPointerInfo info; info.m_references = 1;
    { CCountedPointer p; p.m_pObject = &bank; p.m_pInfo = &info; CManager m; m.SetGraphicsBank(p);
      if (m.m_graphicsBank.m_pObject != &bank || m.m_graphicsBank.m_pInfo != &info || info.m_references != 2) { std::printf("0042a9b7_TEST FAIL\n"); return 1; }
      m.SetGraphicsBank(p); if (info.m_references != 2) { std::printf("0042a9b7_TEST FAIL\n"); return 1; } }
    if (info.m_references != 0) { std::printf("0042a9b7_TEST FAIL\n"); return 1; }
    std::printf("0042a9b7_TEST PASS\n");
    return 0;
}