// Behaviour test for 0x00A537F0: C3DAnimFile::GetRoot() const (root chunk as a fresh const counted pointer)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class CCPPointerInfo { public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);
    }
};
class CChunkedFileSuperChunk {
public:
    int Tag;
};
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { };
static int g_deleted = 0;
static void __fastcall DeleteChunk(void* data) { delete (C3DAnimFileRootChunk*)data; ++g_deleted; }
class CChunkedFile {
public:
    virtual ~CChunkedFile() { }
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;
    unsigned long                           Version;
};
class C3DAnimFile : public CChunkedFile {
public:
    CCountedPointer<const C3DAnimFileRootChunk> GetRoot() const;
};
CCountedPointer<const C3DAnimFileRootChunk> C3DAnimFile::GetRoot() const
{
    CCountedPointer<const C3DAnimFileRootChunk> root;
    root.SetStaticCast(CCountedPointer<const CChunkedFileSuperChunk>(RootChunk));
    return root;
}

int main()
{
    {
        C3DAnimFile file;
        C3DAnimFileRootChunk* chunk = new C3DAnimFileRootChunk;
        chunk->Tag = 77;
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 1;
        info->DeleteFunc = DeleteChunk;
        info->Data = chunk;
        file.RootChunk.Data = chunk;
        file.RootChunk.Info = info;
        {
            CCountedPointer<const C3DAnimFileRootChunk> root = file.GetRoot();
            Check(root.Data == chunk && root.Info == info && info->RefCount == 2 && root.Data->Tag == 77);
        }
        Check(info->RefCount == 1 && g_deleted == 0);
        C3DAnimFile empty;
        CCountedPointer<const C3DAnimFileRootChunk> none = empty.GetRoot();
        Check(none.Data == 0 && none.Info == 0);
        // a null block leaves the result empty even when Data is set
        C3DAnimFileRootChunk loose;
        empty.RootChunk.Data = &loose;
        CCountedPointer<const C3DAnimFileRootChunk> none2 = empty.GetRoot();
        Check(none2.Data == 0 && none2.Info == 0);
        empty.RootChunk.Data = 0;
    }
    Check(g_deleted == 1);
    if (g_fail) {
        printf("00a537f0_TEST FAIL\n");
        return 1;
    }
    printf("00a537f0_TEST PASS\n");
    return 0;
}