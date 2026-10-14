#include <cstdio>
static int g_deleted = 0;
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { ++g_deleted; } unsigned long IVRefCount; };
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& o) { if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->IVRefCount; } return *this; }
};
struct CMesh { unsigned char _pad[0x68]; CIVCountedPointer m_meshEffect; bool GetMeshEffect(CIVCountedPointer& e) { e = m_meshEffect; return true; } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1;
    CMesh m; m.m_meshEffect.Object = &b;
    CIVCountedPointer out; out.Object = &a;
    bool ok = m.GetMeshEffect(out);
    if (!ok || out.Object != &b || b.IVRefCount != 2 || g_deleted != 1 || a.IVRefCount != 0) { std::printf("00ba8070_TEST FAIL\n"); return 1; }
    m.GetMeshEffect(out);                       // same pointee: no change
    if (out.Object != &b || b.IVRefCount != 2 || g_deleted != 1) { std::printf("00ba8070_TEST FAIL\n"); return 1; }
    std::printf("00ba8070_TEST PASS\n"); return 0;
}
