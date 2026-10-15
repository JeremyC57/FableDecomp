#include <cstdio>
#include <cstring>
class CEnginePrimitiveHandle;
static int g_lastHandleCalls = 0, g_destroyCalls = 0;
class CEnginePersistentPrimitiveBase {
public:
    long RefCount;
    CEnginePersistentPrimitiveBase() : RefCount(0) { }
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Destroy() { ++g_destroyCalls; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_lastHandleCalls; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    void Clear() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0; }
};
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)std::memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct CPODVector {
    unsigned char* m_start; unsigned char* m_finish; unsigned char* m_endOfStorage;
    unsigned char* erase(unsigned char* first, unsigned char* last)
    { unsigned char* i = (unsigned char*)CopyTrivial(last, m_finish, first); m_finish = i; return first; }
    void clear() { erase(m_start, m_finish); }
};
struct COwner {
    CPODVector* m_childData;
    CEnginePrimitiveHandle* m_hStart; CEnginePrimitiveHandle* m_hFinish;
    unsigned long size() const { return (unsigned long)(m_hFinish - m_hStart); }
    void ClearChildPrimitive(unsigned long index)
    {
        if (size() > index) {
            m_hStart[index].Clear();
            m_childData[index].clear();
        }
    }
};
int main()
{
    CEnginePersistentPrimitiveBase a, b;
    a.RefCount = 1; b.RefCount = 3;
    CEnginePrimitiveHandle h[2];
    h[0].Data.Object = &a; h[1].Data.Object = &b;
    unsigned char buf0[4], buf1[4];
    CPODVector v[2];
    v[0].m_start = buf0; v[0].m_finish = buf0 + 3; v[0].m_endOfStorage = buf0 + 4;
    v[1].m_start = buf1; v[1].m_finish = buf1 + 2; v[1].m_endOfStorage = buf1 + 4;
    COwner o; o.m_childData = v; o.m_hStart = h; o.m_hFinish = h + 2;
    o.ClearChildPrimitive(5);                       // out of range: nothing
    if (h[0].Data.Object != &a || v[0].m_finish != buf0 + 3) { std::printf("00af7170_TEST FAIL\n"); return 1; }
    o.ClearChildPrimitive(0);                       // unique: notify + destroy
    if (h[0].Data.Object != 0 || g_lastHandleCalls != 1 || g_destroyCalls != 1 || v[0].m_finish != buf0) { std::printf("00af7170_TEST FAIL\n"); return 1; }
    o.ClearChildPrimitive(1);                       // shared: no notify, count drops
    if (h[1].Data.Object != 0 || g_lastHandleCalls != 1 || g_destroyCalls != 1 || b.RefCount != 2 || v[1].m_finish != buf1) { std::printf("00af7170_TEST FAIL\n"); return 1; }
    std::printf("00af7170_TEST PASS\n");
    return 0;
}