// Behaviour test for C3DMeshFile::GetRoot() @ 0x00AD1E80
// Returns a counted pointer to the file's root chunk (static cast of CChunkedFile::RootChunk) sharing its info block:
// the reference count goes up by one while the result lives, and the result is null for a file with no root.
#include <cstdio>

static int gDeleted = 0;

struct CPointerInfoModel {
    int RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};

struct CSuperChunkModel { int Tag; };
struct CRootChunkModel : public CSuperChunkModel { };

static void DeleteRoot(void* data) { delete (CRootChunkModel*)data; ++gDeleted; }

struct CSuperPointer {
    CSuperChunkModel* Data;
    CPointerInfoModel* Info;
    CSuperPointer() : Data(0), Info(0) { }
    CSuperPointer(const CSuperPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CSuperPointer() { Release(); }
    void Release()
    {
        if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
    }
};

struct CRootPointer {
    CRootChunkModel* Data;
    CPointerInfoModel* Info;
    CRootPointer() : Data(0), Info(0) { }
    CRootPointer(const CRootPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CRootPointer() { Release(); }
    void Release()
    {
        if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
    }
    void ShareData(CRootChunkModel* data, CPointerInfoModel* info)
    {
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info) ++info->RefCount;
        }
    }
    void SetStaticCast(const CSuperPointer& other) { ShareData(static_cast<CRootChunkModel*>(other.Data), other.Info); }
};

struct C3DMeshFileModel {
    CSuperPointer RootChunk;
    CSuperPointer GetChunkedFileRoot() { return RootChunk; }
    CRootPointer GetRoot()
    {
        CRootPointer root;
        root.SetStaticCast(GetChunkedFileRoot());
        return root;
    }
};

int main()
{
    int ok = 1;
    {
        C3DMeshFileModel file;
        CRootChunkModel* chunk = new CRootChunkModel;
        chunk->Tag = 42;
        file.RootChunk.Data = chunk;
        file.RootChunk.Info = new CPointerInfoModel;
        file.RootChunk.Info->RefCount = 1;
        file.RootChunk.Info->DeleteFunc = DeleteRoot;
        file.RootChunk.Info->Data = chunk;
        {
            CRootPointer root = file.GetRoot();
            if (root.Data != chunk || root.Info != file.RootChunk.Info || root.Data->Tag != 42) ok = 0;
            if (file.RootChunk.Info->RefCount != 2) ok = 0;
        }
        if (file.RootChunk.Info->RefCount != 1 || gDeleted != 0) ok = 0;
        C3DMeshFileModel empty;
        CRootPointer none = empty.GetRoot();
        if (none.Data != 0 || none.Info != 0) ok = 0;
    }
    if (gDeleted != 1) ok = 0;            // the last owner (the file) released the chunk
    if (!ok) { std::printf("00ad1e80_TEST FAIL\n"); return 1; }
    std::printf("00ad1e80_TEST PASS\n");
    return 0;
}