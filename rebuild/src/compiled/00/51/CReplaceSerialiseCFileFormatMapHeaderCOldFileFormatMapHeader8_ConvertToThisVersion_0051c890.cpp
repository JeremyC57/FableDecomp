// CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertToThisVersion(void* data) -- upgrade a map header record in place: anything older than version 9 is converted up through the chain and the version-9 result copied over the buffer
// Leaf: ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@
//
// Retail 0x0051C890, 79 bytes (+ int3 padding to 80), static __fastcall (data in ecx), plain `ret`.  FableWin.pdb
//   ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@@@SIXPAX@Z.
// Callees: 0x0051C2F0 CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp
//   (landed; static __fastcall returning CCountedPointer<CFileFormatMapHeader> through the hidden pointer in ecx, data in
//   edx), the count block's deleter (its slot +4, object in ecx), 0x00BFE9BC operator delete.
// Caller: CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ReadFromFile 0x0051CBB0 (landed).
// Layout: Ego_r.pdb CFileFormatMapHeader (pack 1, 0x10822 bytes): ByteLength +0, SerialiseVersionNumber +4,
//   ThingUIDCount +5, WorldSizeX +0xD, WorldSizeY +0x11, UsingExtraMapCells +0x15, LargeThemePalette +0x16 (0x8400),
//   VersionNumber +0x8416, NoSoundThemeEntries +0x841A, AtmosThemePalette +0x841E (0x8400), StaticMapChecksum +0x1081E.
// Shape: the 0x00519510 template -- `if (version byte != 9) *(T*)data = *ConvertPreviousUnknownVersionUp(data);`, the
//   0x10822-byte implicit operator= as `rep movsd` x0x4208 + `movsw`, then the temporary CCountedPointer's release.
// Label: catalog "ResourceData_CopyIfDifferentBuffer" (propagated).
void __cdecl operator delete(void* block);

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
class CFileLargeThemePalette {
public:
    unsigned char Data[0x8400];
};
class COldFileFormatMapHeader7;
class COldFileFormatMapHeader8 : public CReplaceSerialise<COldFileFormatMapHeader8, COldFileFormatMapHeader7> {
public:
    enum { VERSION = 8 };
};
class CFileFormatMapHeader : public CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8> {
public:
    enum { VERSION = 9 };
    unsigned __int64       ThingUIDCount;          // +0x00005
    unsigned long          WorldSizeX;             // +0x0000D
    unsigned long          WorldSizeY;             // +0x00011
    bool                   UsingExtraMapCells;     // +0x00015
    CFileLargeThemePalette LargeThemePalette;      // +0x00016
    unsigned long          VersionNumber;          // +0x08416
    unsigned long          NoSoundThemeEntries;    // +0x0841A
    CFileLargeThemePalette AtmosThemePalette;      // +0x0841E
    unsigned long          StaticMapChecksum;      // +0x1081E
};
#pragma pack(pop)

template <class T, class TOld> void __fastcall CReplaceSerialise<T, TOld>::ConvertToThisVersion(void* data)
{
    if (((CReplaceSerialise<T, TOld>*)data)->SerialiseVersionNumber != GetClassVersionNumber()) {
        *(T*)data = *ConvertPreviousUnknownVersionUp(data);
    }
}

template void __fastcall CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8>::ConvertToThisVersion(void* data);