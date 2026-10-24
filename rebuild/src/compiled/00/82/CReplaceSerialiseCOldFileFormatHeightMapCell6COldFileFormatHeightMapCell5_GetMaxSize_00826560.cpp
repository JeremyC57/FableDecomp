// CReplaceSerialise<COldFileFormatHeightMapCell6,COldFileFormatHeightMapCell5>::GetMaxSize(long version, long size) -- the largest buffer any height-map cell version up to 6 needs: walk down the version chain to the record's own version, then take the max with each level's sizeof
// Leaf: ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatHeightMapCell6@@VCOldFileFormatHeightMapCell5@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00826560, 95 bytes (+int3 pad), static __fastcall (version in ecx, size in edx), plain
//   `ret`. FableWin.pdb ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatHeightMapCell6@@VCOldFileFormatHeightMapCell5@@@@SIJJJ@Z.
// Callee: CReplaceSerialise<COldFileFormatHeightMapCell4,COldFileFormatHeightMapCell3>::GetMaxSize @ 0x008256A0 (twice).
// Callers: CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ReadFromFile 0x008275B0 (landed,
//   it cites this address) and 0x00826C4C.
// Layout: Ego_r.pdb sizes COldFileFormatHeightMapCell1..6 = 9, 10, 15, 17, 25, 21 bytes (pack 1): 0x15 and 0x19 here.
// Shape: the landed 0x00519630 template `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T)); return
//   MAX(TOld::GetMaxSize(version, size), sizeof(T));` with MAX the evaluate-twice macro and the whole chain in the TU:
//   VC inlines the version-5 level twice (the macro's compare and result) and keeps version 4 out of line, one call per
//   inlined copy; compares are unsigned (sizeof).
// Label: catalog "GetMaxSize" (module _global): it is the CReplaceSerialise<Cell6,Cell5> instance.
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static long __fastcall GetMaxSize(long version, long size)
    {
        if (version == GetClassVersionNumber())
            return MAX(size, sizeof(T));
        return MAX(TOld::GetMaxSize(version, size), sizeof(T));
    }
};
template <class T, long V> class CBaseSerialise {
public:
    unsigned long ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static long __fastcall GetMaxSize(long version, long size) { return MAX(size, sizeof(T)); }
};
class COldFileFormatHeightMapCell1 : public CBaseSerialise<COldFileFormatHeightMapCell1, 1> {
public:
    enum { VERSION = 1 };
    float Height;                                                               // +0x05 (9 bytes)
};
class COldFileFormatHeightMapCell2 : public CReplaceSerialise<COldFileFormatHeightMapCell2, COldFileFormatHeightMapCell1> {
public:
    enum { VERSION = 2 };
    float Height;                                                               // +0x05
    unsigned char TextureBlendAlpha;                                            // +0x09 (10 bytes)
};
class COldFileFormatHeightMapCell3 : public CReplaceSerialise<COldFileFormatHeightMapCell3, COldFileFormatHeightMapCell2> {
public:
    enum { VERSION = 3 };
    float Height;                                                               // +0x05
    unsigned char TextureBlendAlpha;                                            // +0x09
    unsigned char EngineTheme[3];                                               // +0x0A
    unsigned char EngineBlend[2];                                               // +0x0D (15 bytes)
};
class COldFileFormatHeightMapCell4 : public CReplaceSerialise<COldFileFormatHeightMapCell4, COldFileFormatHeightMapCell3> {
public:
    enum { VERSION = 4 };
    float Height;                                                               // +0x05
    unsigned char TextureBlendAlpha;                                            // +0x09
    unsigned char EngineTheme[3];                                               // +0x0A
    unsigned char EngineBlend[2];                                               // +0x0D
    unsigned char Passable;                                                     // +0x0F
    unsigned char CameraPassable;                                               // +0x10 (17 bytes)
};
class COldFileFormatHeightMapCell5 : public CReplaceSerialise<COldFileFormatHeightMapCell5, COldFileFormatHeightMapCell4> {
public:
    enum { VERSION = 5 };
    float Height;                                                               // +0x05
    unsigned char TextureBlendAlpha;                                            // +0x09
    unsigned char EngineTheme[3];                                               // +0x0A
    unsigned char EngineBlend[2];                                               // +0x0D
    unsigned char Passable;                                                     // +0x0F
    unsigned char CameraPassable;                                               // +0x10
    long SoundLayer;                                                            // +0x11
    long MinimapZone;                                                           // +0x15 (25 bytes)
};
class COldFileFormatHeightMapCell6 : public CReplaceSerialise<COldFileFormatHeightMapCell6, COldFileFormatHeightMapCell5> {
public:
    enum { VERSION = 6 };
    float Height;                                                               // +0x05
    unsigned char TextureBlendAlpha;                                            // +0x09
    unsigned char EngineTheme[3];                                               // +0x0A
    unsigned char EngineBlend[2];                                               // +0x0D
    unsigned char Passable;                                                     // +0x0F
    unsigned char CameraPassable;                                               // +0x10
    signed char SoundLayerPaletteIndex;                                         // +0x11
    signed char MinimapZone;                                                    // +0x12
    bool ShorePoint;                                                            // +0x13
    bool NavPreferred;                                                          // +0x14 (21 bytes)
};
#pragma pack(pop)
template long __fastcall CReplaceSerialise<COldFileFormatHeightMapCell6, COldFileFormatHeightMapCell5>::GetMaxSize(long, long);