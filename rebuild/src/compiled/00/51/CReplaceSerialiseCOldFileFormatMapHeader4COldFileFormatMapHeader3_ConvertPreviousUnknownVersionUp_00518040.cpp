// CReplaceSerialise<COldFileFormatMapHeader4,COldFileFormatMapHeader3>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh version-4 map header from an older record: convert the record up to version 3 first when it is older still, then hand back the new header in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader4@@VCOldFileFormatMapHeader3@@
//
// Retail 0x00518040, 225 bytes (then int3 padding), static __fastcall: the hidden CCountedPointer return slot in ecx,
//   data in edx, plain `ret`; 0xc-byte frame (the new object word and the version-3 temporary).
// Callees: 0x00BFEA1A operator new (0x840E-byte header, 0xC-byte count block), 0x0050EE40 CFileLargeThemePalette ctor
//   (+0xE), 0x00516010 CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp
//   (same convention), the count blocks' deleter (slot +4, object in ecx), 0x00BFE9BC operator delete; the block's
//   deleter is 0x0050D120 CCountedPointer<COldFileFormatMapHeader4>::DeleteData.
// Callers: 0x005196E0 (the next link, CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::
//   ConvertPreviousUnknownVersionUp; catalog "..._CopyConstructor_Type2").
// Identity: FableWin.pdb `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader4@@
//   VCOldFileFormatMapHeader3@@@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader4@@@@PAX@Z` and
//   `?ConvertFromOld@COldFileFormatMapHeader4@@QAEXPBVCOldFileFormatMapHeader3@@@Z` (name set); Ego_r.pdb
//   COldFileFormatMapHeader4 = 0x840E bytes {WorldSizeX +5, WorldSizeY +9, UsingExtraMapCells +0xD,
//   LargeThemePalette +0xE}, COldFileFormatMapHeader3 = 5 bytes (header only, the same size GetMaxSize 0x00519630 uses).
// Shape: the landed 0x0051C2F0 template (`result(new T)`, a null `held`, `old = data`, version byte vs
//   TOld::GetClassVersionNumber() = 3, `held = TOld::ConvertPreviousUnknownVersionUp(data); old = held.object;`,
//   `result.object->ConvertFromOld(old)`, return a copy). Version 3 has no fields, so ConvertFromOld is empty and
//   inlined away: no call follows the version test, `old` is dead and only held's count block survives the inline
//   operator= (`mov eax,[eax+4]`). Copy-out, then held and result are released in reverse order.
// Label: catalog "CEngineLandscapeMeshBuilder_CLayer_CopyConstructor_Type1" is wrong.
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
    static void __fastcall DeleteData(void* data);     // <COldFileFormatMapHeader4>: 0x0050D120
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
class COldFileFormatMapHeader2;
class COldFileFormatMapHeader3 : public CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2> {
public:
    enum { VERSION = 3 };                              // 5 bytes: the serialise header only
};
class COldFileFormatMapHeader4 : public CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> {
public:
    enum { VERSION = 4 };
    unsigned long          WorldSizeX;                 // +0x05
    unsigned long          WorldSizeY;                 // +0x09
    bool                   UsingExtraMapCells;         // +0x0D
    CFileLargeThemePalette LargeThemePalette;          // +0x0E (0x840E bytes in all)
    COldFileFormatMapHeader4() {}
    void ConvertFromOld(const COldFileFormatMapHeader3*) {}   // version 3 carries no fields to bring forward
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatMapHeader3> __fastcall
CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(void* data);   // 0x00516010
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

template CCountedPointer<COldFileFormatMapHeader4> __fastcall
CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3>::ConvertPreviousUnknownVersionUp(void* data);