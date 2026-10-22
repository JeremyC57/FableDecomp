// Grade: DIFFER(536v540) -- volume arm register allocation: retail keeps the file data in ebx, the mip level in the dead param slot and the bank in a frame word, &LoadingManagedTexture in ebp across both arms; ours swaps them
// CGraphicInfoFrame::CFrameData::UploadFrame(bool) -- upload the pending async read: a volume texture through LoadVolumeGraphic, else a frame through LoadGraphicFrame; on success adopt the loading texture and mip level; drop the file data, then resize and re-register the resource
// Leaf: UploadFrame
void operator delete(void* p);                         // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CCountedPointerWords {
    void*           Data;
    CCPPointerInfo* Info;
};
struct CGraphicFrameData {
    long                 RefCount;                     // +0x00
    long                 _pad04;
    CCountedPointerWords PImp;                         // +0x08
    unsigned char        _pad10[0xa];
    unsigned char        Flags1a;                      // +0x1a
    void Uninitialise();                               // 0x009FCBA0
    unsigned long Sizeof();                            // 0x009FCBF0
};
struct CGraphicFrame {
    CGraphicFrameData* Data;
    CGraphicFrame() : Data(0) { }
    __forceinline void Release()
    {
        CGraphicFrameData* d = Data;
        if (d && --d->RefCount == 0) {
            if (d->Flags1a & 0x10)
                d->Uninitialise();
            if (d->PImp.Info) {
                --d->PImp.Info->RefCount;
                if (d->PImp.Info->RefCount == 0) {
                    d->PImp.Info->DeleteFunc(d->PImp.Info->Data);
                    delete d->PImp.Info;
                }
            }
            d->PImp.Data = 0;
            d->PImp.Info = 0;
            delete d;
        }
    }
    __forceinline ~CGraphicFrame() { Release(); }
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& o)
    {
        Release();
        Data = o.Data;
        if (Data)
            ++Data->RefCount;
        return *this;
    }
};
struct CManagedTexture {
    void* Word0;
    void* Handle;
    CManagedTexture& operator=(const CManagedTexture& o);   // 0x00A69B20
    void ClearHandle();                                // 0x00A69AD0
    unsigned long Sizeof();                            // 0x00A69E60
};
struct CBankFileAsyncData;
struct CGraphicInfo;
struct CResource;
struct CResourceBank { void AddResource(CResource* r); };   // 0x009FC380
enum ETextureType { TEXTURE_TYPE_NONE };
struct CGraphicDataBank {
    unsigned char _pad[0x164];
    CResourceBank Resources;                           // +0x164
    unsigned long GetEntryDataType(unsigned long id);  // 0x009CC210
    bool LoadVolumeGraphic(CManagedTexture* tex, CBankFileAsyncData* data, CGraphicInfo* header, bool a, unsigned long level, bool b);   // 0x009FD9B0
    bool LoadGraphicFrame(CGraphicFrame* frame, CManagedTexture* tex, CBankFileAsyncData* data, CGraphicInfo* info, bool a, unsigned long level, ETextureType type, bool b);   // 0x009FED30
};
struct CGraphicInfo {
    unsigned char     _pad[0x12];
    unsigned char     Flags12;                         // +0x12
    unsigned char     _pad13[9];
    ETextureType      TextureType;                     // +0x1c
    CGraphicDataBank* Bank;                            // +0x20
    unsigned long     EntryID;                         // +0x24
};
struct CGraphicInfoFrame {
    unsigned char _pad[0xc];
    CGraphicInfo* Info;                                // +0x0c
};
struct CResource {
    unsigned char _pad[0x1c];
    void SetResourceSize(unsigned long size);          // 0x009FBF20
};
struct CFrameData : CResource {
    CGraphicFrame        Frame;                        // +0x1c
    CManagedTexture      ManagedTexture;               // +0x20
    CManagedTexture      LoadingManagedTexture;        // +0x28
    CCountedPointerWords FileData;                     // +0x30
    signed char          CurrentMipmapLevel;           // +0x38
    signed char          LoadingMipmapLevel;           // +0x39
    signed char          TouchedMipmapLevel;           // +0x3a
    unsigned long        TouchedFrame;                 // +0x3c
    CGraphicInfoFrame*   GraphicInfoFrame;             // +0x40
    bool UploadFrame(bool force);
};

bool CFrameData::UploadFrame(bool force)
{
    bool result = false;
    CGraphicInfo* info = GraphicInfoFrame->Info;
    if (info->Bank->GetEntryDataType(info->EntryID) == 4) {
        long level = LoadingMipmapLevel;
        CGraphicInfoFrame* gif = GraphicInfoFrame;
        CGraphicDataBank* bank = gif->Info->Bank;
        bool isType2 = bank->GetEntryDataType(gif->Info->EntryID) == 2;
        CBankFileAsyncData* data = (CBankFileAsyncData*)FileData.Data;
        if (bank->LoadVolumeGraphic(&LoadingManagedTexture, data, gif->Info, true, level, isType2)) {
            ManagedTexture = LoadingManagedTexture;
            LoadingManagedTexture.ClearHandle();
            CurrentMipmapLevel = LoadingMipmapLevel;
            result = true;
        }
    } else {
        CGraphicFrame frame;
        CGraphicInfo* gi = GraphicInfoFrame->Info;
        if (gi->Bank->LoadGraphicFrame(&frame, &LoadingManagedTexture, (CBankFileAsyncData*)FileData.Data, gi, true, LoadingMipmapLevel, gi->TextureType, force)) {
            if (!(GraphicInfoFrame->Info->Flags12 & 1)) {
                Frame = frame;
                ManagedTexture = LoadingManagedTexture;
                LoadingManagedTexture.ClearHandle();
                CurrentMipmapLevel = LoadingMipmapLevel;
            }
            result = true;
        }
    }
    if (FileData.Info) {
        --FileData.Info->RefCount;
        if (FileData.Info->RefCount == 0) {
            FileData.Info->DeleteFunc(FileData.Info->Data);
            delete FileData.Info;
        }
    }
    FileData.Info = 0;
    FileData.Data = 0;
    unsigned long size = (Frame.Data ? Frame.Data->Sizeof() + 4 : 4) + 0x44;
    if (LoadingManagedTexture.Handle)
        size += LoadingManagedTexture.Sizeof();
    SetResourceSize(size);
    GraphicInfoFrame->Info->Bank->Resources.AddResource(this);
    return result;
}
