// CReplaceSerialise<COldFileFormatLevel2,COldFileFormatLevel1>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh version-2 level-file header from an older record: convert the record up to version 1 first when it is not version 1, copy MapDataOffset/ThingDataOffset across, zero BrushDataOffset and hand the new header back in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatLevel2@@VCOldFileFormatLevel1@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514790, 245 bytes, static __fastcall (the hidden CCountedPointer return slot in ecx,
//   data in edx), plain `ret`; 0x14-byte frame. FableWin.pdb names the template
//   `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatLevel2@@VCOldFileFormatLevel1@@@@SI?AV?$CCountedPointer@VCOldFileFormatLevel2@@@@PAX@Z`.
//   Catalog "CEngineWorldMap_CopyConstructor" is wrong (propagated).
// Callees: 0x00BFEA1A operator new (0x11-byte header, 0xC-byte count block), 0x00512BC0 CBaseSerialise<COldFileFormatLevel1,1>::ConvertPreviousUnknownVersionUp(const void*) (manifest "CEngineWorldMap::CreateNewBankMap", propagated) (the previous link, same
//   convention), the count blocks' deleter (slot +4, object in ecx), 0x00BFE9BC operator delete; the new block's deleter
//   is CCountedPointer<COldFileFormatLevel2>::DeleteData @ 0x0050CFA0.
// Layout: Ego_r.pdb COldFileFormatLevel2 (pack 1, 0x11 bytes): ByteLength +0, SerialiseVersionNumber +4, MapDataOffset +5, ThingDataOffset +9, BrushDataOffset +0xD.
// Shape: the landed 0x00518040 / 0x0051C2F0 template (`result(new T)`, a null `held`, `old = data`, the version byte vs
//   TOld::GetClassVersionNumber() = 1, `held = TOld::ConvertPreviousUnknownVersionUp(data); old = held.object;`,
//   `result.object->ConvertFromOld(old)` inline (two offsets copied, BrushDataOffset = 0), return a copy; held and result released in reverse order).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

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
    static void __fastcall DeleteData(void* data);     // <COldFileFormatLevel2>: 0x0050CFA0
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
template <class T, int VERSION> class CBaseSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(const void* data);
};
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
class COldFileFormatLevel1 : public CBaseSerialise<COldFileFormatLevel1, 1> {
public:
    long MapDataOffset;                                // +0x05
    long ThingDataOffset;                              // +0x09 (13 bytes)
};
class COldFileFormatLevel2 : public CReplaceSerialise<COldFileFormatLevel2, COldFileFormatLevel1> {
public:
    enum { VERSION = 2 };
    long MapDataOffset;                                // +0x05
    long ThingDataOffset;                              // +0x09
    long BrushDataOffset;                              // +0x0D (17 bytes)
    void ConvertFromOld(const COldFileFormatLevel1* old)
    {
        MapDataOffset = old->MapDataOffset;
        ThingDataOffset = old->ThingDataOffset;
        BrushDataOffset = 0;
    }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatLevel1> __fastcall
CBaseSerialise<COldFileFormatLevel1, 1>::ConvertPreviousUnknownVersionUp(const void* data);   // 0x00512BC0
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

template CCountedPointer<COldFileFormatLevel2> __fastcall
CReplaceSerialise<COldFileFormatLevel2, COldFileFormatLevel1>::ConvertPreviousUnknownVersionUp(void* data);