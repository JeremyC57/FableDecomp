// Behaviour test for 0x00A69A80 CManagedTextureData::ReleaseTexture(): Allocated and Owner are cleared, the counted volume
// texture is released (deleted on the last reference), the D3D volume header is deleted and cleared, then SetAsLRU runs.
#include <cstdio>

static int g_volumeDeleted = 0;
static int g_headerDeleted = 0;
static int g_lruCalls = 0;
static int g_infoDeleted = 0;

class CCPPointerInfo {
public:
    unsigned int RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
    ~CCPPointerInfo() { ++g_infoDeleted; }
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
class CVolumeTexture { public: int Id; };
static void DeleteVolume(void* data) { ++g_volumeDeleted; delete (CVolumeTexture*)data; }
struct IDirect3DVolumeTexture9 { int Dummy; ~IDirect3DVolumeTexture9() { ++g_headerDeleted; } };
template <class T> class scoped_ptr {
    T* ptr;
public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { delete ptr; }
    void reset(T* p = 0) { scoped_ptr<T> tmp(p); tmp.swap(*this); }
    void swap(scoped_ptr& b) { T* t = b.ptr; b.ptr = ptr; ptr = t; }
    T* get() const { return ptr; }
};
class CResource {
public:
    void SetAsLRU() { ++g_lruCalls; }
};
class CManagedTextureData : public CResource {
public:
    void ReleaseTexture()
    {
        Allocated = false;
        Owner = 0;
        VolumeTexture.ReleaseData();
        D3DVolumeHeader.reset();
        SetAsLRU();
    }
    CCountedPointer<CVolumeTexture> VolumeTexture;
    void* Owner;
    scoped_ptr<IDirect3DVolumeTexture9> D3DVolumeHeader;
    bool Allocated;
};

int main()
{
    CManagedTextureData data;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 2;
    info->DeleteFunc = DeleteVolume;
    info->Data = new CVolumeTexture;
    data.VolumeTexture.Data = (CVolumeTexture*)info->Data;
    data.VolumeTexture.Info = info;
    data.Owner = &data;
    data.Allocated = true;
    data.D3DVolumeHeader.reset(new IDirect3DVolumeTexture9);
    data.ReleaseTexture();
    if (data.Allocated || data.Owner || data.VolumeTexture.Data || data.VolumeTexture.Info || data.D3DVolumeHeader.get()
        || info->RefCount != 1 || g_volumeDeleted != 0 || g_headerDeleted != 1 || g_lruCalls != 1) {
        std::printf("00a69a80_TEST FAIL\n");
        return 1;
    }
    data.VolumeTexture.Data = (CVolumeTexture*)info->Data;
    data.VolumeTexture.Info = info;
    data.ReleaseTexture();
    if (g_volumeDeleted != 1 || g_infoDeleted != 1 || g_headerDeleted != 1 || g_lruCalls != 2) {
        std::printf("00a69a80_TEST FAIL\n");
        return 1;
    }
    std::printf("00a69a80_TEST PASS\n");
    return 0;
}