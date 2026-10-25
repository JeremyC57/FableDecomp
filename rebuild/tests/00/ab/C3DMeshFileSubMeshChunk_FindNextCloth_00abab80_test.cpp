// behaviour test for 0x00ABAB80 C3DMeshFileSubMeshChunk::FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous): returns the next "CLTH" sub-chunk after `previous` (skipping another type), add-refs it while held, releases everything after, and a null pointer after the last one
#include <cstdio>
#include <cstring>
#if defined(_MSC_VER)
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CCPPointerInfo {
public:
    long RefCount;
    void (FASTCALL* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data; Info = info;
            if (info) ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other) { ShareData(static_cast<T*>(other.Data), other.Info); }
};
static int gLiveChunks = 0;
class CChunkedFileChunk {
public:
    char ChunkType[5];
    CChunkedFileChunk(const char* type) { std::strncpy(ChunkType, type, 4); ChunkType[4] = 0; ++gLiveChunks; }
    virtual ~CChunkedFileChunk() { --gLiveChunks; }
};
static void FASTCALL DeleteChunk(void* p) { delete (CChunkedFileChunk*)p; }
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    CCountedPointer<CChunkedFileChunk> Sub[4];
    int NoSub;
    CChunkedFileSuperChunk() : CChunkedFileChunk("SUPR"), NoSub(0) {}
    void Add(CChunkedFileChunk* c)
    {
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 0; info->DeleteFunc = &DeleteChunk; info->Data = c;
        Sub[NoSub++].ShareData(c, info);
    }
    int Find(const char* type, int from) const
    {
        for (int i = from; i < NoSub; ++i)
            if (std::strcmp(Sub[i].Data->ChunkType, type) == 0) return i;
        return -1;
    }
    int IndexOf(const CChunkedFileChunk* c) const
    {
        for (int i = 0; i < NoSub; ++i) if (Sub[i].Data == c) return i;
        return NoSub;
    }
    const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const
    {
        int i = Find(type, 0);
        return i < 0 ? CCountedPointer<CChunkedFileChunk>() : Sub[i];
    }
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type)
    {
        int i = Find(type, 0);
        return i < 0 ? CCountedPointer<CChunkedFileChunk>() : Sub[i];
    }
    const CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const
    {
        int i = Find(previous.Data->ChunkType, IndexOf(previous.Data) + 1);
        return i < 0 ? CCountedPointer<CChunkedFileChunk>() : Sub[i];
    }
    CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous)
    {
        int i = Find(previous.Data->ChunkType, IndexOf(previous.Data) + 1);
        return i < 0 ? CCountedPointer<CChunkedFileChunk>() : Sub[i];
    }
};
class C3DMeshFileClothChunk : public CChunkedFileChunk {
public:
    C3DMeshFileClothChunk() : CChunkedFileChunk("CLTH") {}
    static const char* FASTCALL GetStaticChunkType() { return "CLTH"; }
};
class COtherChunk : public CChunkedFileChunk {
public:
    COtherChunk() : CChunkedFileChunk("ZZZZ") {}
};
class C3DMeshFileSubMeshChunk : public CChunkedFileSuperChunk {
public:
    CCountedPointer<C3DMeshFileClothChunk> FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous);
};
CCountedPointer<C3DMeshFileClothChunk> C3DMeshFileSubMeshChunk::FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous)
{
    CCountedPointer<C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}
static int Check()
{
    {
        C3DMeshFileSubMeshChunk owner;
        C3DMeshFileClothChunk* first = new C3DMeshFileClothChunk;
        C3DMeshFileClothChunk* second = new C3DMeshFileClothChunk;
        owner.Add(new COtherChunk);
        owner.Add(first);
        owner.Add(new COtherChunk);
        owner.Add(second);
        C3DMeshFileSubMeshChunk& ref = owner;
        CCountedPointer<const C3DMeshFileClothChunk> prev;
        prev.SetStaticCast(owner.Sub[1]);
        {
            CCountedPointer<C3DMeshFileClothChunk> c = ref.FindNextCloth(prev);
            if (c.Data != second || c.Info != owner.Sub[3].Info || c.Info->RefCount != 2 || prev.Info->RefCount != 2) return 1;
        }
        {
            CCountedPointer<const C3DMeshFileClothChunk> last;
            last.SetStaticCast(owner.Sub[3]);
            CCountedPointer<C3DMeshFileClothChunk> c = ref.FindNextCloth(last);
            if (c.Data != 0 || c.Info != 0) return 2;
        }
        prev.ReleaseData(); prev.Data = 0; prev.Info = 0;
        if (owner.Sub[1].Info->RefCount != 1 || owner.Sub[3].Info->RefCount != 1) return 9;
    }
    if (gLiveChunks != 0) return 10;
    {
        C3DMeshFileSubMeshChunk empty;
        empty.Add(new COtherChunk);
        if (empty.NoSub != 1) return 11;
    }
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("00abab80_TEST FAIL\n"); return 1; }
    std::printf("00abab80_TEST PASS\n");
    return 0;
}