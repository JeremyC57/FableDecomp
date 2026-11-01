// Behaviour test for CManagedTextureData::~CManagedTextureData (0x00A6B5B0): the scoped D3D volume header is deleted, then the
// VolumeTexture and Texture counted pointers are released (deleted on their last reference), then ~CResource.
#include <cstdio>
#include <cstring>
static char g_log[64];
static int g_logLen;
static void Log(char c) { if (g_logLen < 63) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; } }
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer { public:                     // engine/CCountedPointer.h destructor
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Share(T* data, CCPPointerInfo* info) { Data = data; Info = info; ++info->RefCount; }
};
struct CPayload { char Tag; };
static void DeletePayload(void* data) { Log(((CPayload*)data)->Tag); }

struct IVolume { ~IVolume() { Log('v'); } };
template <class T> struct scoped_ptr { T* ptr; ~scoped_ptr() { delete ptr; } };
class CResource { public: ~CResource() { Log('r'); } };
class CManagedTextureData : public CResource { public:
    unsigned short Width, Height;
    CCountedPointer<CPayload> Texture;
    CCountedPointer<CPayload> VolumeTexture;
    void* Owner;
    void* D3DHeader;
    scoped_ptr<IVolume> D3DVolumeHeader;
};
int main()
{
    CPayload texture = { 'T' }, volume = { 'V' };
    CCPPointerInfo* textureInfo = new CCPPointerInfo; textureInfo->RefCount = 1; textureInfo->DeleteFunc = DeletePayload; textureInfo->Data = &texture;
    CCPPointerInfo* volumeInfo = new CCPPointerInfo; volumeInfo->RefCount = 1; volumeInfo->DeleteFunc = DeletePayload; volumeInfo->Data = &volume;
    CManagedTextureData* data = new CManagedTextureData;
    data->Texture.Data = &texture; data->Texture.Info = textureInfo;
    data->VolumeTexture.Data = &volume; data->VolumeTexture.Info = volumeInfo;
    data->D3DVolumeHeader.ptr = new IVolume;
    delete data;
    if (std::strcmp(g_log, "vVTr") != 0) { std::printf("00a6b5b0_TEST FAIL\n"); return 1; }
    std::printf("00a6b5b0_TEST PASS\n");
    return 0;
}