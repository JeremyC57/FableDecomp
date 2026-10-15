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
    CIter* m_first; unsigned long m_lastAdded;
    void AddChildPrimitiveWithID(unsigned long id, int, unsigned long) { m_lastAdded = id; }
    void SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, int primitive, unsigned long flags)
    {
        AddChildPrimitiveWithID(id, primitive, flags);
        handle = m_first[id].m_pNode->m_value;
    }
};
int main()
{
    CPrim p; p.RefCount = 1;
    SNode n; n.m_value.Data.Object = &p;
    CIter it[2]; it[1].m_used = 1; it[1].m_pNode = &n;
    CGroup g; g.m_first = it; g.m_lastAdded = 0;
    CEnginePrimitiveHandle out;
    g.SetChildPrimitive(1, out, 0, 0);
    if (g.m_lastAdded != 1 || out.Data.Object != &p || p.RefCount != 2) { std::printf("00bbc310_TEST FAIL\n"); return 1; }
    std::printf("00bbc310_TEST PASS\n");
    return 0;
}
