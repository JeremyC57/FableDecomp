// CReplaceSerialise<COldFileFormatHeightMapCell6,COldFileFormatHeightMapCell5>::ConvertPreviousUnknownVersionUp(void* data) -- build a version-6 cell (21 bytes): copies Height, TextureBlendAlpha, Passable, CameraPassable, narrows MinimapZone to a byte, clears SoundLayerPaletteIndex / ShorePoint and block-copies EngineTheme / EngineBlend (NavPreferred is left unset); an older record is first converted up to version 5 through the previous link; the new cell is handed back in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatHeightMapCell6@@VCOldFileFormatHeightMapCell5@@@@SI?AV?$CCountedPointer@VCOldFileFormatHeightMapCell6@@@@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00826F10, 290 bytes, static __fastcall (the hidden CCountedPointer return slot in ecx,
//   data in edx), plain `ret`.
// Identity (proven): Ego_r.pdb and FableWin.pdb hold ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatHeightMapCell6@@VCOldFileFormatHeightMapCell5@@@@SI?AV?$CCountedPointer@VCOldFileFormatHeightMapCell6@@@@PAX@Z
//   and ?ConvertFromOld@COldFileFormatHeightMapCell6@@QAEXPBVCOldFileFormatHeightMapCell5@@@Z; the allocation is 21 bytes = sizeof(COldFileFormatHeightMapCell6) (Ego_r.pdb, pack 1); the
//   version test compares the record's SerialiseVersionNumber with 5 and the conversion it calls, 
//   CReplaceSerialise<COldFileFormatHeightMapCell5, ..>::ConvertPreviousUnknownVersionUp 0x00826C90, is the previous link of the chain
//   (Cell2 0x00825700 landed -> Cell3 0x00825E00 -> Cell4 0x008265C0 -> Cell5 0x00826C90 -> Cell6 0x00826F10 -> current
//   0x008271E0, each the only caller of the one before); the new block's deleter is CCountedPointer<COldFileFormatHeightMapCell6>::DeleteData 0x00822CA0.
// Label: the catalog's "CAFile_*" name is descriptive and wrong.
// Callees: operator new 0x00BFEA1A (the cell, then the 0xC-byte count block), 0x00826C90 (the previous link), the count blocks'
//   deleter (slot +4, object in ecx), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb COldFileFormatHeightMapCell6 / COldFileFormatHeightMapCell5 members (pack 1); CCountedPointer {Data, Info} and CCPPointerInfo {RefCount, DeleteFunc,
//   Data} (Ego_r.pdb names).
// Shape: the landed CReplaceSerialise<COldFileFormatHeightMapCell2, COldFileFormatHeightMapCell1> 0x00825700 template with
//   COldFileFormatHeightMapCell6::ConvertFromOld inline.
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC
#include <string.h>
#pragma intrinsic(memcpy, memset)

class CCPPointerInfo { public:                         // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                            // +0x0
    void (__fastcall* DeleteFunc)(void*);              // +0x4 (the object reaches it in ecx in retail)
    void* Data;                                        // +0x8
    CCPPointerInfo(void* p, void (__fastcall* d)(void*)) : RefCount(1), DeleteFunc(d), Data(p) {}
};
template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x0
    CCPPointerInfo* Info;                              // +0x4
    static void __fastcall DeleteData(void* data);
    CCountedPointer(T* p) : Data(p), Info(p ? new CCPPointerInfo(p, DeleteData) : 0) {}
    CCountedPointer(const CCountedPointer& x) : Data(x.Data), Info(x.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* c = x.Info;
        T* o = x.Data;
        if (Info != c) {
            if (Info && --Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
            Data = o;
            Info = c;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
};

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return (unsigned short)(TOld::GetClassVersionNumber() + 1); }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
template <class T, long V> class CBaseSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return (unsigned short)V; }
};
class COldFileFormatHeightMapCell1 : public CBaseSerialise<COldFileFormatHeightMapCell1, 1> {
public:
    float Height;                                      // +0x05 (9 bytes)
};
class COldFileFormatHeightMapCell2 : public CReplaceSerialise<COldFileFormatHeightMapCell2, COldFileFormatHeightMapCell1> {
public:
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09 (10 bytes)
};
class COldFileFormatHeightMapCell3 : public CReplaceSerialise<COldFileFormatHeightMapCell3, COldFileFormatHeightMapCell2> {
public:
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09
    unsigned char EngineTheme[3];                      // +0x0A
    unsigned char EngineBlend[2];                      // +0x0D (15 bytes)
    void ConvertFromOld(const COldFileFormatHeightMapCell2* old)
    {
        Height = old->Height;
        TextureBlendAlpha = old->TextureBlendAlpha;
        EngineTheme[0] = 1;
        memset(&EngineTheme[1], 0, 2);
        EngineBlend[0] = 0xff;
        EngineBlend[1] = 0;
    }
};
class COldFileFormatHeightMapCell4 : public CReplaceSerialise<COldFileFormatHeightMapCell4, COldFileFormatHeightMapCell3> {
public:
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09
    unsigned char EngineTheme[3];                      // +0x0A
    unsigned char EngineBlend[2];                      // +0x0D
    unsigned char Passable;                            // +0x0F
    unsigned char CameraPassable;                      // +0x10 (17 bytes)
    void ConvertFromOld(const COldFileFormatHeightMapCell3* old)
    {
        Height = old->Height;
        TextureBlendAlpha = old->TextureBlendAlpha;
        EngineTheme[0] = old->EngineTheme[0];
        EngineTheme[1] = old->EngineTheme[1];
        EngineTheme[2] = old->EngineTheme[2];
        EngineBlend[0] = old->EngineBlend[0];
        EngineBlend[1] = old->EngineBlend[1];
        Passable = 0;
        CameraPassable = 0;
    }
};
class COldFileFormatHeightMapCell5 : public CReplaceSerialise<COldFileFormatHeightMapCell5, COldFileFormatHeightMapCell4> {
public:
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09
    unsigned char EngineTheme[3];                      // +0x0A
    unsigned char EngineBlend[2];                      // +0x0D
    unsigned char Passable;                            // +0x0F
    unsigned char CameraPassable;                      // +0x10
    long          SoundLayer;                          // +0x11
    long          MinimapZone;                         // +0x15 (25 bytes)
    void ConvertFromOld(const COldFileFormatHeightMapCell4* old)
    {
        Height = old->Height;
        TextureBlendAlpha = old->TextureBlendAlpha;
        EngineTheme[0] = old->EngineTheme[0];
        EngineTheme[1] = old->EngineTheme[1];
        EngineTheme[2] = old->EngineTheme[2];
        EngineBlend[0] = old->EngineBlend[0];
        EngineBlend[1] = old->EngineBlend[1];
        Passable = old->Passable;
        CameraPassable = old->CameraPassable;
        SoundLayer = 0;
        MinimapZone = 0;
    }
};
class COldFileFormatHeightMapCell6 : public CReplaceSerialise<COldFileFormatHeightMapCell6, COldFileFormatHeightMapCell5> {
public:
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09
    unsigned char EngineTheme[3];                      // +0x0A
    unsigned char EngineBlend[2];                      // +0x0D
    unsigned char Passable;                            // +0x0F
    unsigned char CameraPassable;                      // +0x10
    signed char   SoundLayerPaletteIndex;              // +0x11
    signed char   MinimapZone;                         // +0x12
    bool          ShorePoint;                          // +0x13
    bool          NavPreferred;                        // +0x14 (21 bytes; not set by this conversion)
    void ConvertFromOld(const COldFileFormatHeightMapCell5* old)
    {
        Height = old->Height;
        TextureBlendAlpha = old->TextureBlendAlpha;
        Passable = old->Passable;
        CameraPassable = old->CameraPassable;
        SoundLayerPaletteIndex = 0;
        MinimapZone = (signed char)old->MinimapZone;
        ShorePoint = false;
        memcpy(EngineTheme, old->EngineTheme, sizeof(EngineTheme));
        memcpy(EngineBlend, old->EngineBlend, sizeof(EngineBlend));
    }
};
class CFileFormatHeightMapCell : public CReplaceSerialise<CFileFormatHeightMapCell, COldFileFormatHeightMapCell6> {
public:
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09
    unsigned char EngineTheme[3];                      // +0x0A
    unsigned char EngineBlend[2];                      // +0x0D
    unsigned char Passable;                            // +0x0F
    unsigned char CameraPassable;                      // +0x10
    signed char   SoundLayerPaletteIndex;              // +0x11
    signed char   MinimapZone;                         // +0x12
    bool          ShorePoint;                          // +0x13
    bool          NavPreferred;                        // +0x14 (21 bytes)
    void ConvertFromOld(const COldFileFormatHeightMapCell6* old)
    {
        Height = old->Height;
        TextureBlendAlpha = old->TextureBlendAlpha;
        Passable = old->Passable;
        CameraPassable = old->CameraPassable;
        SoundLayerPaletteIndex = old->SoundLayerPaletteIndex;
        MinimapZone = old->MinimapZone;
        ShorePoint = old->ShorePoint;
        memcpy(EngineTheme, old->EngineTheme, sizeof(EngineTheme));
        memcpy(EngineBlend, old->EngineBlend, sizeof(EngineBlend));
        NavPreferred = false;
    }
};
#pragma pack(pop)

template <class T, class TOld>
CCountedPointer<T> __fastcall CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held((TOld*)0);
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.Data;
    }
    result.Data->ConvertFromOld(old);
    return result;
}

template <>
CCountedPointer<COldFileFormatHeightMapCell5> __fastcall
CReplaceSerialise<COldFileFormatHeightMapCell5, COldFileFormatHeightMapCell4>::ConvertPreviousUnknownVersionUp(void* data);   // 0x00826C90
template CCountedPointer<COldFileFormatHeightMapCell6> __fastcall CReplaceSerialise<COldFileFormatHeightMapCell6, COldFileFormatHeightMapCell5>::ConvertPreviousUnknownVersionUp(void* data);