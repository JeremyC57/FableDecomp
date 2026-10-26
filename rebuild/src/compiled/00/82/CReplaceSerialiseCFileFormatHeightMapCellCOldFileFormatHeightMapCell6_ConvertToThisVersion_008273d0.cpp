// CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ConvertToThisVersion(void* data) -- upgrade a height-map cell record in place: anything older than version 7 is converted up through the chain and the version-7 result copied over the buffer
// Leaf: ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatHeightMapCell@@VCOldFileFormatHeightMapCell6@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008273D0, 102 bytes, static __fastcall (data in ecx), plain `ret`. FableWin.pdb
//   ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatHeightMapCell@@VCOldFileFormatHeightMapCell6@@@@SIXPAX@Z.
// Callees: CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ConvertPreviousUnknownVersionUp
//   @ 0x008271E0 (static __fastcall, hidden CCountedPointer result in ecx, data in edx; manifest
//   "CAFile_ConvertNavConnectorNode" is descriptive), the count block's deleter (slot +4, object in ecx), operator delete
//   @ 0x00BFE9BC.
// Caller: CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ReadFromFile 0x008275B0 (landed; it
//   cites this address).
// Label: the catalog's "ConvertToThisVersion" (module _global) is this instance.
// Layout: Ego_r.pdb CFileFormatHeightMapCell, 0x15 bytes packed (as the landed 0x008275B0).
// Shape: the landed 0x00519510 template: `if (version byte != GetClassVersionNumber()) *(T*)data =
//   *ConvertPreviousUnknownVersionUp(data);` -- the 21-byte implicit operator= is five register dword moves and a byte
//   (below the `rep movsd` threshold), the temporary's destructor runs the deleter and frees the block on zero.
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CCPPointerInfo { public:
    unsigned long refs;                        // +0x0
    void (__fastcall* destroy)(void*);         // +0x4
    void* object;                              // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              object;                    // +0x0
    CCPPointerInfo* control;                   // +0x4
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
        }
    }
    T& operator*() const { return *object; }
};

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
    static void __fastcall ConvertToThisVersion(void* data);
};
class COldFileFormatHeightMapCell6;
class CFileFormatHeightMapCell : public CReplaceSerialise<CFileFormatHeightMapCell, COldFileFormatHeightMapCell6> {
public:
    enum { VERSION = 7 };
    float Height;                                                               // +0x05
    unsigned char TextureBlendAlpha;                                            // +0x09
    unsigned char EngineTheme[3];                                               // +0x0A
    unsigned char EngineBlend[2];                                               // +0x0D
    unsigned char Passable;                                                     // +0x0F
    unsigned char CameraPassable;                                               // +0x10
    signed char SoundLayerPaletteIndex;                                         // +0x11
    signed char MinimapZone;                                                    // +0x12
    bool ShorePoint;                                                            // +0x13
    bool NavPreferred;                                                          // +0x14
};
#pragma pack(pop)

template <class T, class TOld> void __fastcall CReplaceSerialise<T, TOld>::ConvertToThisVersion(void* data)
{
    if (((CReplaceSerialise<T, TOld>*)data)->SerialiseVersionNumber != GetClassVersionNumber()) {
        *(T*)data = *ConvertPreviousUnknownVersionUp(data);
    }
}

template void __fastcall CReplaceSerialise<CFileFormatHeightMapCell, COldFileFormatHeightMapCell6>::ConvertToThisVersion(void* data);