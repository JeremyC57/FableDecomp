// CGraphicInfoFrame::UploadToVolumeTexture(CManagedTexture& texture, CBankFileAsyncData& data, unsigned long mipmapLevel) -- have the bank load this frame's graphic as a volume graphic into the texture (volume when the entry's data type is 2)
// Grade: DIFFER(61v61) -- register choice after GetEntryDataType: retail `mov ecx,[esi+0Ch] ; cmp eax,2 ; mov eax,[esp+14h] ; sete dl` (header in ecx, mipmap level in eax, bool in dl), ours header in edx, level in ecx, `sete al`
// Leaf: ?UploadToVolumeTexture@CGraphicInfoFrame@@ABE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FDBA0, 61 bytes, `ret 0Ch`. Name: Ego_r/FableWin PDB
//   ?UploadToVolumeTexture@CGraphicInfoFrame@@ABE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@K@Z -- the only 3-argument
//   CGraphicInfoFrame member that forwards to CGraphicDataBank::LoadVolumeGraphic (0x009FD9B0, the row just before it, as
//   in the Ego_r donor order 0x00B57EF0 / 0x00B580C0); `this` is a CGraphicInfoFrame (GraphicInfo +0x0C; CGraphicInfo
//   Bank +0x20, Index +0x24 -- PDB). The catalog label "CKilledThing::ModifyHeroStats" is propagated (that PDB member
//   takes one CTCHeroStats&). No direct callers (the body is unreferenced in retail).
// Callees: CBankFile::GetEntryDataType(ulong) const @ 0x009CC210, CGraphicDataBank::LoadVolumeGraphic @ 0x009FD9B0.
// Shape: the bank is read once (edi) for both calls; IsVolumeTexture() is the inlined `GetEntryDataType(Index) == 2`.
#define CBankFile CBankFileLayout
#include "engine/CBankFile.h"
#undef CBankFile
class CBankFile : public CBankFileLayout {
public:
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210
};
class CBankFileAsync : public CBankFile { };
class CManagedTexture;
class CBankFileAsyncData;
class CGraphicInfoFrame;

class CGraphicDataBank : public CBankFileAsync {
public:
    class CGraphicHeader {
    public:
        unsigned char _body[0x1c];
    };
private:
    friend class CGraphicInfoFrame;
    bool LoadVolumeGraphic(CManagedTexture& texture, CBankFileAsyncData& data, const CGraphicHeader& header,
                           bool a, unsigned long mipmapLevel, bool b);   // 0x009FD9B0
};

class CGraphicInfo {
public:
    CGraphicDataBank::CGraphicHeader Header;   // +0x00
    long              TextureType;             // +0x1c
    CGraphicDataBank* Bank;                    // +0x20
    long              Index;                   // +0x24
    CGraphicInfoFrame* Frames;                 // +0x28
};

class CGraphicInfoFrame {
public:
    long           _pad00;
    unsigned short FrameNum;                   // +0x04
    signed char    SmallMipmapLevel;           // +0x06
    void*          Data;                       // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;                // +0x0c
private:
    CGraphicDataBank* GetBank() const { return GraphicInfo->Bank; }
    unsigned long GetIndex() const { return GraphicInfo->Index; }
    bool IsVolumeTexture() const { return GetBank()->GetEntryDataType(GetIndex()) == 2; }
    bool UploadToVolumeTexture(CManagedTexture& texture, CBankFileAsyncData& data, unsigned long mipmapLevel) const;
public:
    void Touch();
};

bool CGraphicInfoFrame::UploadToVolumeTexture(CManagedTexture& texture, CBankFileAsyncData& data, unsigned long mipmapLevel) const
{
    return GetBank()->LoadVolumeGraphic(texture, data, GraphicInfo->Header, true, mipmapLevel, IsVolumeTexture());
}
