#include <cstdio>
class CEnginePrimitiveHandle;
static int g_last = 0, g_destroy = 0;
class CPrim {
public:
    long RefCount;
    CPrim() : RefCount(0) { }
    virtual ~CPrim() { }
    virtual void Destroy() { ++g_destroy; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_last; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CPrim* Object;
    CIVCountedPointer() : Object(0) { }
    CPrim* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CPrim* old = Object;
        if (old != other.Object) {
            if (old) { old->Release(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    void Clear() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0; }
    CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) { Clear(); Data = other.Data; return *this; }
};
struct SNode { SNode* m_pNext; SNode* m_pPrev; CEnginePrimitiveHandle m_value; };
struct CIter { unsigned char m_used; SNode* m_pNode; };
struct CGroup {
    CIter* m_first; CIter* m_last;
    unsigned long GetCount() const { return (unsigned long)(m_last - m_first); }
    void GetChildPrimitiveHandle(unsigned long id, CEnginePrimitiveHandle& handle)
    {
        if (id < GetCount()) {
            CIter& entry = m_first[id];
            if (entry.m_used)
                handle = entry.m_pNode->m_value;
        }
    }
};
int main()
{
    CPrim p; p.RefCount = 1;
    SNode n; n.m_value.Data.Object = &p;
    CIter it[2]; it[0].m_used = 0; it[0].m_pNode = &n; it[1].m_used = 1; it[1].m_pNode = &n;
    CGroup g; g.m_first = it; g.m_last = it + 2;
    CEnginePrimitiveHandle out;
    g.GetChildPrimitiveHandle(0, out);           // unused slot
    g.GetChildPrimitiveHandle(7, out);           // out of range
    if (out.Data.Object != 0 || p.RefCount != 1) { std::printf("00bbc3a0_TEST FAIL\n"); return 1; }
    g.GetChildPrimitiveHandle(1, out);
    if (out.Data.Object != &p || p.RefCount != 2) { std::printf("00bbc3a0_TEST FAIL\n"); return 1; }
    std::printf("00bbc3a0_TEST PASS\n");
    return 0;
}
