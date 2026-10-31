// CGraphicInfoFrame::CFrameData::FreeTextureMemory(bool force) -- texture memory pressure: a frame unused long enough drops one mipmap level when allowed (true), or with `force` resets its texture and releases the frame data (true); otherwise only refresh the recorded resource size (false)
// Grade: DIFFER(231v231) -- eax/ecx/edx rotation: threshold chain GraphicInfo/bank in ecx/edx (retail edx/eax), SmallMipmapLevel in eax (retail ecx), texture manager loaded after the pushes (retail before)
// Leaf: ?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FE460, 231 bytes, thiscall `ret 4`. Ego_r PDB ?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z
//   (donor 0x00B58A80, right after ResetTexture 0x00B58A60 = retail 0x009FE440; Ego_r order lines up with retail).
// Callees: CResource::GetUnusedFrames() const 0x009FBF70 (through the inline PDB CManagedTexture::GetUnusedFrames and on `this`),
//   CTextureManager::ReduceMipmapLevel(CGraphicFrame&, CManagedTexture&, unsigned long) 0x00A69F70 (PDB takes references),
//   CFrameData::Sizeof() const 0x009FDBE0 (out of line in the reduce arm, inlined in the final arm), CResource::SetResourceSize
//   0x009FBF20, CFrameData::ResetTexture 0x009FE440, CGraphicInfoFrame::ReleaseData() const 0x00A00680 (probable: the only
//   void() const private CGraphicInfoFrame method in Ego_r; the body releases the CIVCountedPointer<CFrameData> Data at +8 --
//   the manifest label CAnimatedBlock::DiscardDeviceIndependentResources is a byte-twin propagation), CGraphicFrameData::Sizeof
//   0x009FCBF0, CManagedTexture::Sizeof 0x00A69E60.
// Layout (retail): CFrameData Frame +0x1C, ManagedTexture +0x20 (Data +0x24), LoadingManagedTexture +0x28, CurrentMipmapLevel
//   +0x38, GraphicInfoFrame +0x40; CGraphicInfoFrame SmallMipmapLevel +6, GraphicInfo +0xC; CGraphicInfo Header.IsFlatAnimatedGraphic
//   bit 0 of +0x12, Bank +0x20; CGraphicDataBank UnloadFrameDelay +0x26C, ReduceSizeFrameDelay +0x270, TextureManager +0x280
//   (PDB +0x2E4/+0x2E8/+0x2FC with the Dinkumware containers).
// Tried this session: PDB inline helpers GetBank()/CGraphicInfoFrame::ReduceMipmapLevel with the reversed threshold compare
//   (238v231), GetBank() for the first threshold only, GetBank() for both + the inline ReduceMipmapLevel (both = prior residue).
class CManagedTextureData;
class CGraphicFrameData {
public:
    unsigned long Sizeof() const;                   // 0x009FCBF0
};
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;
    unsigned long Sizeof() const { return FrameData ? FrameData->Sizeof() + sizeof(*this) : sizeof(*this); }
};
class CResource {
public:
    virtual void v00();
    unsigned long GetUnusedFrames() const;          // 0x009FBF70
    void SetResourceSize(unsigned long size);       // 0x009FBF20
};
class CManagedTextureData : public CResource { };
template <class T> class CIVCountedPointer {
public:
    T* Data;
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CManagedTexture {
public:
    virtual void v00();
    CIVCountedPointer<CManagedTextureData> Data;
    unsigned long GetUnusedFrames() { return Data->GetUnusedFrames(); }
    unsigned long Sizeof() const;                   // 0x00A69E60
};
class CTextureManager {
public:
    bool ReduceMipmapLevel(CGraphicFrame& frame, CManagedTexture& texture, unsigned long levels);   // 0x00A69F70
};
class CGraphicDataBank {
public:
    unsigned char _pad[0x26c];
    unsigned long UnloadFrameDelay;                 // +0x26C
    unsigned long ReduceSizeFrameDelay;             // +0x270
    unsigned char _pad274[0xc];
    CCountedPointer<CTextureManager> TextureManager;   // +0x280
};
class CGraphicInfo {
public:
    unsigned char _pad[0x12];
    unsigned char IsFlatAnimatedGraphic : 1;        // +0x12 (Header)
    unsigned char _pad13[0xd];
    CGraphicDataBank* Bank;                         // +0x20
};
class CGraphicInfoFrame {
public:
    virtual void v00();
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06
    unsigned char _pad07[5];
    CGraphicInfo* GraphicInfo;                      // +0x0C
    void ReleaseData() const;                       // 0x00A00680
    void ReduceMipmapLevel(CGraphicFrame& frame, CManagedTexture& texture, unsigned long levels) const
    {
        GraphicInfo->Bank->TextureManager->ReduceMipmapLevel(frame, texture, levels);
    }

    class CFrameData : public CResource {
    public:
        unsigned char _base[0x18];
        CGraphicFrame Frame;                        // +0x1C
        CManagedTexture ManagedTexture;             // +0x20
        CManagedTexture LoadingManagedTexture;      // +0x28
        void* FileData[2];                          // +0x30
        signed char CurrentMipmapLevel;             // +0x38
        signed char LoadingMipmapLevel;             // +0x39
        signed char TouchedMipmapLevel;             // +0x3A
        unsigned long TouchedFrame;                 // +0x3C
        CGraphicInfoFrame* GraphicInfoFrame;        // +0x40
        CGraphicDataBank* GetBank() const { return GraphicInfoFrame->GraphicInfo->Bank; }
        unsigned long Sizeof() const
        {
            unsigned long size = Frame.Sizeof() + sizeof(*this);
            if (!LoadingManagedTexture.Data.IsNull())
                size += LoadingManagedTexture.Sizeof();
            return size;
        }
        void ResetTexture();                        // 0x009FE440
        bool FreeTextureMemory(bool force);
    };
};

bool CGraphicInfoFrame::CFrameData::FreeTextureMemory(bool force)
{
    if (Frame.FrameData) {
        unsigned long reduceDelay = GetBank()->ReduceSizeFrameDelay;
        if (ManagedTexture.GetUnusedFrames() >= reduceDelay) {
            unsigned long unused = GetUnusedFrames();
            if (unused <= GraphicInfoFrame->GraphicInfo->Bank->UnloadFrameDelay)
                return false;
            unsigned long level = CurrentMipmapLevel + 1;
            if (!GraphicInfoFrame->GraphicInfo->IsFlatAnimatedGraphic && level <= (unsigned long)GraphicInfoFrame->SmallMipmapLevel) {
                GraphicInfoFrame->GraphicInfo->Bank->TextureManager->ReduceMipmapLevel(Frame, ManagedTexture, 1);
                CurrentMipmapLevel = (signed char)level;
                SetResourceSize(Sizeof());
                return true;
            }
            if (force) {
                ResetTexture();
                GraphicInfoFrame->ReleaseData();
                return true;
            }
        }
    }
    SetResourceSize(Sizeof());
    return false;
}
