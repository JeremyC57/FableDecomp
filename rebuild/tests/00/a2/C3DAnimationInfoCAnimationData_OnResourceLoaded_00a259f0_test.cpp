#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int g_log[16];
static int g_n = 0;
static void __fastcall DeleteFileData(void* data) { g_log[g_n++] = 3; }

struct CCPPointerInfo {
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04
    void*         Data;                                      // +0x08
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(int)
    {
        Release();
        Info = 0;
        Data = 0;
        return *this;
    }
};

template<class T> struct CIVCountedPointer {
    T* m_pObject;
    void Release()
    {
        T* p = m_pObject;
        if (p) {
            if (--p->m_refCount == 0)
                p->DeleteThis();
            m_pObject = 0;
        }
    }
    CIVCountedPointer& operator=(T* object)
    {
        Release();
        m_pObject = object;
        return *this;
    }
};

class CBankFileAsyncData;
struct CHandleBasedMemoryPool {
    void*         __vftable;                                 // +0x00
    unsigned char _pad04[0x25 - 4];
    bool          ShutdownFlag;                              // +0x25
};
struct CBankFileAsync {
    void DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& data);   // 0x009D67E0
};
struct CMeshDataBank : CBankFileAsync {
    unsigned char           _pad00[0x360];
    CHandleBasedMemoryPool* AnimationMemoryPool;             // +0x360 CCountedPointer<CMovableResourceMemoryPool>::Data
};

struct C3DAnimationInfo {
    class CAnimationData;
    void*                             __vftable;             // +0x00
    CMeshDataBank*                    PBank;                 // +0x04 the owning mesh data bank
    unsigned char                     _pad08[4];
    CIVCountedPointer<CAnimationData> Data;                  // +0x0c
};

class C3DAnimationInfo::CAnimationData {
public:
    virtual void v00();
    virtual void DeleteThis();                               // slot 1 (+4)
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void OnResourceLoaded();                         // slot 5
    long                                m_refCount;          // +0x04
    unsigned char                       _pad08[0x30 - 8];
    CCountedPointer<CBankFileAsyncData> FileData;            // +0x30
    C3DAnimationInfo*                   AnimInfo;            // +0x38
    void BuildSharedAnimation();                             // 0x00A24D70
};

void C3DAnimationInfo::CAnimationData::OnResourceLoaded()
{
    CMeshDataBank* bank = AnimInfo->PBank;
    if (bank->AnimationMemoryPool->ShutdownFlag) {
        bank->DiscardAsyncData(FileData);
        FileData = 0;
        AnimInfo->Data = 0;
    } else {
        BuildSharedAnimation();
    }
}

void CBankFileAsync::DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& data) { g_log[g_n++] = 1; }
void C3DAnimationInfo::CAnimationData::BuildSharedAnimation() { g_log[g_n++] = 2; }
void C3DAnimationInfo::CAnimationData::v00() { }
void C3DAnimationInfo::CAnimationData::DeleteThis() { g_log[g_n++] = 4; }
void C3DAnimationInfo::CAnimationData::v02() { }
void C3DAnimationInfo::CAnimationData::v03() { }
void C3DAnimationInfo::CAnimationData::v04() { }
int main()
{
    CHandleBasedMemoryPool pool; std::memset(&pool, 0, sizeof(pool));
    CMeshDataBank* bank = new CMeshDataBank; bank->AnimationMemoryPool = &pool;
    C3DAnimationInfo info; std::memset(&info, 0, sizeof(info)); info.PBank = bank;
    C3DAnimationInfo::CAnimationData* d = new C3DAnimationInfo::CAnimationData;
    d->m_refCount = 1; d->AnimInfo = &info; d->FileData.Data = 0; d->FileData.Info = 0;
    info.Data.m_pObject = d;
    d->OnResourceLoaded();                           // pool alive: build the shared animation
    if (g_n != 1 || g_log[0] != 2 || info.Data.m_pObject != d) { std::printf("00a259f0_TEST FAIL\n"); return 1; }
    g_n = 0; pool.ShutdownFlag = true;
    CCPPointerInfo* fileInfo = new CCPPointerInfo; fileInfo->RefCount = 1; fileInfo->DeleteFunc = DeleteFileData; fileInfo->Data = 0;
    d->FileData.Data = (CBankFileAsyncData*)fileInfo; d->FileData.Info = fileInfo;
    d->OnResourceLoaded();                           // shutting down: discard, release the file data, drop the info's reference
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 3 || g_log[2] != 4 || d->FileData.Data != 0 || d->FileData.Info != 0 ||
        info.Data.m_pObject != 0 || d->m_refCount != 0) { std::printf("00a259f0_TEST FAIL\n"); return 1; }
    std::printf("00a259f0_TEST PASS\n");
    return 0;
}