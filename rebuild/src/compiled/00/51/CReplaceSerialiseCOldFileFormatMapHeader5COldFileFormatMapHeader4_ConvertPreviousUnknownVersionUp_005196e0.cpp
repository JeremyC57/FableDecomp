// CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh version-5 map header from an older record: convert the record up to version 4 first when it is not version 4, start ThingUIDCount at 1, copy the world size, extra-cells flag and large theme palette across and hand the new header back in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader5@@VCOldFileFormatMapHeader4@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005196E0, 287 bytes, static __fastcall (the hidden CCountedPointer return slot in ecx,
//   data in edx), plain `ret`; 0x14-byte frame. FableWin.pdb names the template
//   `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader5@@VCOldFileFormatMapHeader4@@@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader5@@@@PAX@Z`.
//   Catalog "CEngineLandscapeMeshBuilder_CLayer_CopyConstructor_Type2" is wrong (propagated).
// Callees: 0x00BFEA1A operator new (0x8416-byte header, 0xC-byte count block), 0x0050EE40 CFileLargeThemePalette ctor (+0x16), 0x00518040 CReplaceSerialise<COldFileFormatMapHeader4,COldFileFormatMapHeader3>::ConvertPreviousUnknownVersionUp (landed) (the previous link, same
//   convention), the count blocks' deleter (slot +4, object in ecx), 0x00BFE9BC operator delete; the new block's deleter
//   is CCountedPointer<COldFileFormatMapHeader5>::DeleteData @ 0x0050D0A0.
// Layout: Ego_r.pdb COldFileFormatMapHeader5 (pack 1, 0x8416 bytes): ThingUIDCount (ulong64) +5, WorldSizeX +0xD, WorldSizeY +0x11, UsingExtraMapCells +0x15, LargeThemePalette +0x16 (0x8400 bytes).
// Shape: the landed 0x00518040 / 0x0051C2F0 template (`result(new T)`, a null `held`, `old = data`, the version byte vs
//   TOld::GetClassVersionNumber() = 4, `held = TOld::ConvertPreviousUnknownVersionUp(data); old = held.object;`,
//   `result.object->ConvertFromOld(old)` inline (ThingUIDCount = 1, the world size, flag and palette copied -- the palette by the implicit `rep movsd` x0x2100), return a copy; held and result released in reverse order).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CFileLargeThemePalette {
public:
    unsigned char Storage[0x8400];
    CFileLargeThemePalette();                          // 0x0050EE40
};

class CCPPointerInfo { public:
    unsigned long refs;                                // +0x0
    void (__fastcall* destroy)(void*);                 // +0x4
    void* object;                                      // +0x8
    CCPPointerInfo(void* p, void (__fastcall* d)(void*)) : refs(1), destroy(d), object(p) {}
};
template <class T> class CCountedPointer {
public:
    T*              object;                            // +0x0
    CCPPointerInfo* control;                           // +0x4
    static void __fastcall DeleteData(void* data);     // <COldFileFormatMapHeader5>: 0x0050D0A0
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
class COldFileFormatMapHeader3;
class COldFileFormatMapHeader4 : public CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> {
public:
    enum { VERSION = 4 };
    unsigned long          WorldSizeX;                 // +0x05
    unsigned long          WorldSizeY;                 // +0x09
    bool                   UsingExtraMapCells;         // +0x0D
    CFileLargeThemePalette LargeThemePalette;          // +0x0E (0x840E bytes in all)
};
class COldFileFormatMapHeader5 : public CReplaceSerialise<COldFileFormatMapHeader5, COldFileFormatMapHeader4> {
public:
    enum { VERSION = 5 };
    unsigned __int64       ThingUIDCount;              // +0x05
    unsigned long          WorldSizeX;                 // +0x0D
    unsigned long          WorldSizeY;                 // +0x11
    bool                   UsingExtraMapCells;         // +0x15
    CFileLargeThemePalette LargeThemePalette;          // +0x16 (0x8416 bytes in all)
    COldFileFormatMapHeader5() {}
    void ConvertFromOld(const COldFileFormatMapHeader4* old)
    {
        ThingUIDCount = 1;
        WorldSizeX = old->WorldSizeX;
        WorldSizeY = old->WorldSizeY;
        UsingExtraMapCells = old->UsingExtraMapCells;
        LargeThemePalette = old->LargeThemePalette;
    }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatMapHeader4> __fastcall
CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3>::ConvertPreviousUnknownVersionUp(void* data);   // 0x00518040
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

template CCountedPointer<COldFileFormatMapHeader5> __fastcall
CReplaceSerialise<COldFileFormatMapHeader5, COldFileFormatMapHeader4>::ConvertPreviousUnknownVersionUp(void* data);