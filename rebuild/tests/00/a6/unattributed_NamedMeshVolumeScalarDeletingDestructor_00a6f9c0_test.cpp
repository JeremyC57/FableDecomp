#include <cstdio>

// Deleting a {CCharString, C3DMeshVolume} record: the volume's counted pointer is released (the block's
// delete function runs when the count reaches zero, the block is freed), the volume's name and then the
// key string are destroyed, the record is freed.
static int g_stringDtors = 0, g_deleteFuncCalls = 0;
static void* g_deletedData = 0;

struct CCharString {
    char* m_pString;
    ~CCharString() { ++g_stringDtors; }
};
static void DeleteFunc(void* data) { ++g_deleteFuncCalls; g_deletedData = data; }
struct CCPPointerInfo {
    unsigned long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              m_pData;
    CCPPointerInfo* m_pInfo;
    void ReleaseInfo()
    {
        if (m_pInfo) {
            --m_pInfo->RefCount;
            if (m_pInfo->RefCount == 0) {
                m_pInfo->DeleteFunc(m_pInfo->Data);
                delete m_pInfo;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); m_pData = 0; m_pInfo = 0; }
};
struct CContainmentVolume { int m_dummy; };
struct C3DMeshVolume {
    CCharString                         Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
};
struct CNamedMeshVolume {
    CCharString   first;
    C3DMeshVolume second;
};
void DestroyNamedMeshVolume(CNamedMeshVolume* record)
{
    delete record;
}

int main()
{
    CContainmentVolume volume;
    CNamedMeshVolume* r1 = new CNamedMeshVolume;
    r1->first.m_pString = 0; r1->second.Name.m_pString = 0;
    r1->second.ConvexVolume.m_pData = &volume;
    r1->second.ConvexVolume.m_pInfo = new CCPPointerInfo;
    r1->second.ConvexVolume.m_pInfo->RefCount = 2;          // shared: only the count drops
    r1->second.ConvexVolume.m_pInfo->DeleteFunc = DeleteFunc;
    r1->second.ConvexVolume.m_pInfo->Data = &volume;
    CCPPointerInfo* shared = r1->second.ConvexVolume.m_pInfo;
    DestroyNamedMeshVolume(r1);
    bool ok = g_stringDtors == 2 && g_deleteFuncCalls == 0 && shared->RefCount == 1;
    CNamedMeshVolume* r2 = new CNamedMeshVolume;            // last reference: delete function runs
    r2->first.m_pString = 0; r2->second.Name.m_pString = 0;
    r2->second.ConvexVolume.m_pData = &volume;
    r2->second.ConvexVolume.m_pInfo = shared;
    DestroyNamedMeshVolume(r2);
    ok = ok && g_stringDtors == 4 && g_deleteFuncCalls == 1 && g_deletedData == &volume;
    if (!ok) {
        std::printf("00a6f9c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a6f9c0_TEST PASS\n");
    return 0;
}