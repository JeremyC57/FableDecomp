#include <cstdio>

// CPSCSingleSprite::ClearAllPrimitives: with a live group handle every primitive id in PrimIDList is
// removed through the engine, in order; with a dead handle nothing happens.
struct CPrimitiveGroupHandle {
    void* m_pLink;
    void* m_pGroup;
};
static long g_removed[8];
static int g_removedCount = 0;
static const CPrimitiveGroupHandle* g_lastGroup = 0;
struct CEngineInterface {
    virtual void RemovePrimitive(const CPrimitiveGroupHandle& group, long primitiveId)
    {
        g_lastGroup = &group;
        g_removed[g_removedCount++] = primitiveId;
    }
};
template<class T> struct CArray {
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
    T& operator[](unsigned long index) { return m_pData[index]; }
};
struct CPSCSingleSprite {
    CArray<long> PrimIDList;
    void ClearAllPrimitives(CEngineInterface* engine, const CPrimitiveGroupHandle& group);
};
void CPSCSingleSprite::ClearAllPrimitives(CEngineInterface* engine, const CPrimitiveGroupHandle& group)
{
    if (group.m_pGroup) {
        for (unsigned long i = 0; i < PrimIDList.Size(); ++i)
            engine->RemovePrimitive(group, PrimIDList[i]);
    }
}

int main()
{
    long ids[3] = { 17, 4, 99 };
    CPSCSingleSprite s;
    s.PrimIDList.m_pData = ids; s.PrimIDList.m_pLast = ids + 3; s.PrimIDList.m_pEnd = ids + 3;
    CEngineInterface engine;
    int groupObject = 0;
    CPrimitiveGroupHandle dead = { 0, 0 };
    CPrimitiveGroupHandle live = { 0, &groupObject };
    s.ClearAllPrimitives(&engine, dead);
    bool ok = g_removedCount == 0;
    s.ClearAllPrimitives(&engine, live);
    ok = ok && g_removedCount == 3 && g_removed[0] == 17 && g_removed[1] == 4 && g_removed[2] == 99 && g_lastGroup == &live;
    if (!ok) {
        std::printf("00b07570_TEST FAIL\n");
        return 1;
    }
    std::printf("00b07570_TEST PASS\n");
    return 0;
}