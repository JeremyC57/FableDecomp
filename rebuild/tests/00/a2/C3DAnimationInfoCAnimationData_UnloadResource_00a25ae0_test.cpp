// Behaviour test for CAnimationData::UnloadResource @ 0x00a25ae0: true (kept) when it cannot unload; else release AnimInfo's data and return false.
#include <cstdio>
static int g_destroyed = 0;
class CIVCountedPointeeBase { public: virtual void v0() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; };
template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    void Release() { T* p = m_pObject; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_pObject = 0; } }
    CIVCountedPointer& operator=(T* object) { Release(); m_pObject = object; return *this; }
};
class C3DAnimationInfo { public: unsigned char _pad[0x0c]; CIVCountedPointer<CIVCountedPointeeBase> m_data; };
struct CAnimationData {
    virtual void v0() { } virtual void v1() { }
    virtual bool UnloadResource() { if (CanUnload()) { AnimInfo->m_data = 0; return false; } return true; }
    unsigned char _pad[0x34]; C3DAnimationInfo* AnimInfo; bool m_canUnload;
    bool CanUnload() { return m_canUnload; }
};
int main()
{
    CIVCountedPointeeBase block; block.m_refCount = 1;
    C3DAnimationInfo info; info.m_data.m_pObject = &block;
    CAnimationData d; d.AnimInfo = &info; d.m_canUnload = false;
    bool ok = d.UnloadResource() == true && info.m_data.m_pObject == &block && g_destroyed == 0;
    d.m_canUnload = true;
    if (!(d.UnloadResource() == false && info.m_data.m_pObject == 0 && g_destroyed == 1 && block.m_refCount == 0)) ok = false;
    if (!(d.UnloadResource() == false && g_destroyed == 1)) ok = false;
    if (!ok) { std::printf("00a25ae0_TEST FAIL\n"); return 1; }
    std::printf("00a25ae0_TEST PASS\n");
    return 0;
}