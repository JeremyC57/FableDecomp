// Behaviour test for CPrimitiveBlock_00ac5740::DiscardDeviceIndependentResources @ 0x00ac5740: release the counted pointer at +0x34 (Destroy at zero) and null it.
#include <cstdio>
#include <cstddef>
static int g_destroyed = 0;
struct CIVCountedPointeeBase { virtual void v0() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; };
template<class T> struct CIVCountedPointer {
    T* m_pObject;
    void Release() { T* p = m_pObject; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_pObject = 0; } }
    CIVCountedPointer& operator=(T* object) { Release(); m_pObject = object; return *this; }
};
struct CDeviceIndependent : CIVCountedPointeeBase { };
struct CPrimitiveBlock_00ac5740 {
    unsigned char _pad[0x34];
    CIVCountedPointer<CDeviceIndependent> Data;
    void DiscardDeviceIndependentResources() { Data = 0; }
};
int main()
{
    bool ok = offsetof(CPrimitiveBlock_00ac5740, Data) == 0x34;
    CDeviceIndependent block; block.m_refCount = 2;
    CPrimitiveBlock_00ac5740 obj; obj.Data.m_pObject = &block;
    obj.DiscardDeviceIndependentResources();
    if (!(obj.Data.m_pObject == 0 && block.m_refCount == 1 && g_destroyed == 0)) ok = false;
    obj.Data.m_pObject = &block;
    obj.DiscardDeviceIndependentResources();
    if (!(obj.Data.m_pObject == 0 && block.m_refCount == 0 && g_destroyed == 1)) ok = false;
    obj.DiscardDeviceIndependentResources();
    if (!(obj.Data.m_pObject == 0 && g_destroyed == 1)) ok = false;
    if (!ok) { std::printf("00ac5740_TEST FAIL\n"); return 1; }
    std::printf("00ac5740_TEST PASS\n");
    return 0;
}
