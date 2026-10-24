// CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::GetMaxSize(long version, long size) -- map-header version chain: the scratch size needed to read a record of the given version and convert it up to version 5, the largest of the size and every header size from that version up
// Leaf: ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatMapHeader5@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515FA0, 100 bytes (padded to 112), plain `ret` (static `__fastcall`: version in
//   ecx, size in edx). PDB name ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatMapHeader5@@VCOldFileFormatMapHeader4@@@@SIJJJ@Z
//   (Ego_r.pdb name set). Callers 0x0051801F and 0x00519662 / 0x00519686 (the v6->v7 GetMaxSize 0x00519630, landed
//   with ReadFromFile 0x0051CBB0's model). Catalog label "GetMaxSize" is right, its class was missing.
// Layout: Ego_r.pdb sizes (pack 1): headers 1-3 are 5 bytes (ByteLength + SerialiseVersionNumber), header 4 0x840E
//   (+ WorldSizeX/Y, UsingExtraMapCells, the 0x8400-byte LargeThemePalette), header 5 0x8416 (+ the 64-bit
//   ThingUIDCount) -- the `cmp 840Eh` / `cmp 8416h` / `cmp 5` constants.
// Shape: the early-return template `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T)); return
//   MAX(TOld::GetMaxSize(version, size), sizeof(T));` -- the macro names the inner level twice, which is retail's tail
//   (`cmp ecx,4 ; je` / `cmp edx,5 ; ja` re-deriving the winning value); the v4, v3 and v2 levels and the version-1
//   CBaseSerialise (`MAX(size, sizeof(T))`, no version test) inline into the tests 5, 4, 3, 2 (unsigned compares:
//   sizeof is unsigned). A named-local form folds the v2 test away (53 bytes), the ternary-in-MAX form stops inlining
//   (72) or, force-inlined, duplicates every level (158).
#define MAX(a, b) ((a) > (b) ? (a) : (b))

class CFileLargeThemePalette {
public:
    unsigned char Data[0x8400];                     // Ego_r.pdb: 33792 bytes
};

#pragma pack(push, 1)
template <class T, int Version> class CBaseSerialise {
public:
    unsigned long ByteLength;                       // +0x00
    unsigned char SerialiseVersionNumber;           // +0x04
    static long __fastcall GetMaxSize(long, long size) { return MAX(size, sizeof(T)); }
};

template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                       // +0x00
    unsigned char SerialiseVersionNumber;           // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static long __fastcall GetMaxSize(long version, long size)
    {
        if (version == GetClassVersionNumber())
            return MAX(size, sizeof(T));
        return MAX(TOld::GetMaxSize(version, size), sizeof(T));
    }
};

class COldFileFormatMapHeader1 : public CBaseSerialise<COldFileFormatMapHeader1, 1> {
public:
    enum { VERSION = 1 };
};

class COldFileFormatMapHeader2 : public CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1> {
public:
    enum { VERSION = 2 };
};

class COldFileFormatMapHeader3 : public CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2> {
public:
    enum { VERSION = 3 };
};

class COldFileFormatMapHeader4 : public CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> {
public:
    enum { VERSION = 4 };
    unsigned long WorldSizeX;                       // +0x05
    unsigned long WorldSizeY;                       // +0x09
    bool UsingExtraMapCells;                        // +0x0D
    CFileLargeThemePalette LargeThemePalette;       // +0x0E
};

class COldFileFormatMapHeader5 : public CReplaceSerialise<COldFileFormatMapHeader5, COldFileFormatMapHeader4> {
public:
    enum { VERSION = 5 };
    unsigned __int64 ThingUIDCount;                 // +0x05
    unsigned long WorldSizeX;                       // +0x0D
    unsigned long WorldSizeY;                       // +0x11
    bool UsingExtraMapCells;                        // +0x15
    CFileLargeThemePalette LargeThemePalette;       // +0x16
};
#pragma pack(pop)

template long __fastcall CReplaceSerialise<COldFileFormatMapHeader5, COldFileFormatMapHeader4>::GetMaxSize(long version, long size);