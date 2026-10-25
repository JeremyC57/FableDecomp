// CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh version-7 map header from an older record: convert the record up to version 6 first when it is older still, copy its fields forward (no sound theme entries yet) and hand back the new header in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader7@@VCOldFileFormatMapHeader6@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051B0A0, 320 bytes (0x133 + int3 padding), static __fastcall: the hidden
//   CCountedPointer return slot in ecx, data in edx, plain `ret`; 0x14-byte frame (the return slot, the version-6
//   temporary and held's count block).
// Identity: FableWin.pdb ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader7@@
//   VCOldFileFormatMapHeader6@@@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader7@@@@PAX@Z with
//   ?ConvertFromOld@COldFileFormatMapHeader7@@QAEXPBVCOldFileFormatMapHeader6@@@Z inlined; Ego_r.pdb
//   COldFileFormatMapHeader7 = 0x841E bytes (ThingUIDCount +5, WorldSizeX +0xD, WorldSizeY +0x11, UsingExtraMapCells
//   +0x15, LargeThemePalette +0x16, VersionNumber +0x8416, NoSoundThemeEntries +0x841A), COldFileFormatMapHeader6 =
//   the same without NoSoundThemeEntries; both open with the 5-byte CReplaceSerialise header (ByteLength,
//   SerialiseVersionNumber +4). Caller: the next link 0x0051B940 (H8 from H7, landed). The catalog label
//   "CEngineLandscapeMeshBuilder_CLayer_CopyConstructor_Type4" is wrong.
// Callees: operator new 0x00BFEA1A (0x841E-byte header, 0xC-byte count block), CFileLargeThemePalette ctor 0x0050EE40,
//   CReplaceSerialise<COldFileFormatMapHeader6,COldFileFormatMapHeader5>::ConvertPreviousUnknownVersionUp 0x0051A5A0
//   (same convention), the count blocks' deleter (slot +4, object in ecx), operator delete 0x00BFE9BC; the new
//   block's deleter is CCountedPointer<COldFileFormatMapHeader7>::DeleteData 0x0050CF90.
// Shape: the landed 0x00518040 template verbatim (`result(new T)`, a null `held`, `old = data`, the version byte vs
//   TOld::GetClassVersionNumber() = 6, `held = TOld::ConvertPreviousUnknownVersionUp(data); old = held.object;`,
//   `result.object->ConvertFromOld(old)`, return a copy). Here ConvertFromOld is a real inline member: the packed
//   (pack(1)) fields copy as dwords + a byte, the 0x8400-byte palette as one `rep movsd` (an implicit POD copy) and
//   the new NoSoundThemeEntries is zeroed.
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CFileLargeThemePalette {
public:
    unsigned char FileThemePaletteEntry[0x8400];       // CFileLargeThemePaletteEntry[256]
    CFileLargeThemePalette();                          // 0x0050EE40
};

struct CCPPointerInfo {
    unsigned long refs;                                // +0x0
    void (__fastcall* destroy)(void*);                 // +0x4
    void* object;                                      // +0x8
    CCPPointerInfo(void* p, void (__fastcall* d)(void*)) : refs(1), destroy(d), object(p) {}
};
template <class T> class CCountedPointer {
public:
    T*              object;                            // +0x0
    CCPPointerInfo* control;                           // +0x4
    static void __fastcall DeleteData(void* data);     // <COldFileFormatMapHeader7>: 0x0050CF90
    CCountedPointer() : object(0), control(0) {}
    CCountedPointer(T* p) : object(p), control(p ? new CCPPointerInfo(p, DeleteData) : 0) {}
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control)
    {
        if (control)
            ++control->refs;
    }
    CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* c = x.control;
        T* o = x.object;
        if (control != c) {
            if (control && --control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
            object = o;
            control = c;
            if (control)
                ++control->refs;
        }
        return *this;
    }
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
};

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
class COldFileFormatMapHeader5;
class COldFileFormatMapHeader6 : public CReplaceSerialise<COldFileFormatMapHeader6, COldFileFormatMapHeader5> {
public:
    enum { VERSION = 6 };
    unsigned __int64       ThingUIDCount;              // +0x05
    unsigned long          WorldSizeX;                 // +0x0D
    unsigned long          WorldSizeY;                 // +0x11
    bool                   UsingExtraMapCells;         // +0x15
    CFileLargeThemePalette LargeThemePalette;          // +0x16
    unsigned long          VersionNumber;              // +0x8416 (0x841A bytes in all)
};
class COldFileFormatMapHeader7 : public CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6> {
public:
    enum { VERSION = 7 };
    unsigned __int64       ThingUIDCount;              // +0x05
    unsigned long          WorldSizeX;                 // +0x0D
    unsigned long          WorldSizeY;                 // +0x11
    bool                   UsingExtraMapCells;         // +0x15
    CFileLargeThemePalette LargeThemePalette;          // +0x16
    unsigned long          VersionNumber;              // +0x8416
    unsigned long          NoSoundThemeEntries;        // +0x841A (0x841E bytes in all)
    COldFileFormatMapHeader7() {}
    void ConvertFromOld(const COldFileFormatMapHeader6* old)
    {
        ThingUIDCount = old->ThingUIDCount;
        WorldSizeX = old->WorldSizeX;
        WorldSizeY = old->WorldSizeY;
        UsingExtraMapCells = old->UsingExtraMapCells;
        LargeThemePalette = old->LargeThemePalette;
        VersionNumber = old->VersionNumber;
        NoSoundThemeEntries = 0;
    }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatMapHeader6> __fastcall
CReplaceSerialise<COldFileFormatMapHeader6, COldFileFormatMapHeader5>::ConvertPreviousUnknownVersionUp(void* data);   // 0x0051A5A0
template <class T, class TOld>
CCountedPointer<T> __fastcall CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held;
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

template CCountedPointer<COldFileFormatMapHeader7> __fastcall
CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp(void* data);