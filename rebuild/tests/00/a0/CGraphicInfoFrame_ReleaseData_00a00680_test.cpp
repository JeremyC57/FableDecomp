// Behaviour test for CGraphicInfoFrame::ReleaseData @ 0x00a00680: release the counted pointer at +0x08 (Destroy at zero) and null it.
#include <cstdio>
#include <cstddef>
static int g_destroyed = 0;
class CIVCountedPointeeBase { public: virtual void v0() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; };
template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    void Release() { T* p = m_pObject; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_pObject = 0; } }
    CIVCountedPointer& operator=(T* object) { Release(); m_pObject = object; return *this; }
};
struct CFrameData : CIVCountedPointeeBase { };
struct CGraphicInfoFrame {
    unsigned char _pad[0x08];
    CIVCountedPointer<CFrameData> Data;
    void ReleaseData() { Data = 0; }
};
int main()
{
    bool ok = offsetof(CGraphicInfoFrame, Data) == 0x08;
    CFrameData block; block.m_refCount = 2;
    CGraphicInfoFrame obj; obj.Data.m_pObject = &block;
    obj.ReleaseData();
    if (!(obj.Data.m_pObject == 0 && block.m_refCount == 1 && g_destroyed == 0)) ok = false;
    obj.Data.m_pObject = &block;
    obj.ReleaseData();
    if (!(obj.Data.m_pObject == 0 && block.m_refCount == 0 && g_destroyed == 1)) ok = false;
    obj.ReleaseData();
    if (!(obj.Data.m_pObject == 0 && g_destroyed == 1)) ok = false;
    if (!ok) { std::printf("00a00680_TEST FAIL\n"); return 1; }
    std::printf("00a00680_TEST PASS\n");
    return 0;
}