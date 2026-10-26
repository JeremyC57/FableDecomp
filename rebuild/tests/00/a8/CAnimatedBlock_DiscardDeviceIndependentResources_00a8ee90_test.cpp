// Behaviour test for CAnimatedBlock::DiscardDeviceIndependentResources @ 0x00a8ee90: release the counted pointer at +0x2c (Destroy at zero) and null it.
#include <cstdio>
#include <cstddef>
static int g_destroyed = 0;
class CIVCountedPointeeBase { public: virtual void v0() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; };
template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    void Release() { T* p = m_pObject; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_pObject = 0; } }
    CIVCountedPointer& operator=(T* object) { Release(); m_pObject = object; return *this; }
};
struct CDeviceIndependent : CIVCountedPointeeBase { };
struct CAnimatedBlock {
    unsigned char _pad[0x2c];
    CIVCountedPointer<CDeviceIndependent> Data;
    void DiscardDeviceIndependentResources() { Data = 0; }
};
int main()
{
    bool ok = offsetof(CAnimatedBlock, Data) == 0x2c;
    CDeviceIndependent block; block.m_refCount = 2;
    CAnimatedBlock obj; obj.Data.m_pObject = &block;
    obj.DiscardDeviceIndependentResources();
    if (!(obj.Data.m_pObject == 0 && block.m_refCount == 1 && g_destroyed == 0)) ok = false;
    obj.Data.m_pObject = &block;
    obj.DiscardDeviceIndependentResources();
    if (!(obj.Data.m_pObject == 0 && block.m_refCount == 0 && g_destroyed == 1)) ok = false;
    obj.DiscardDeviceIndependentResources();
    if (!(obj.Data.m_pObject == 0 && g_destroyed == 1)) ok = false;
    if (!ok) { std::printf("00a8ee90_TEST FAIL\n"); return 1; }
    std::printf("00a8ee90_TEST PASS\n");
    return 0;
}