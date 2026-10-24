// CReplaceSerialise<COldFileFormatHeightMapCell2,COldFileFormatHeightMapCell1>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh version-2 height-map cell from an older record: convert it up to version 1 first when it is older still, copy the height and default the texture blend alpha to 0x80, and hand the new cell back in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatHeightMapCell2@@VCOldFileFormatHeightMapCell1@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00825700, 236 bytes, static __fastcall: the hidden CCountedPointer return slot in ecx,
//   data in edx, plain `ret`; 0x14-byte frame (the result slot copy, the 8-byte temporary and held's object word).
// Callees: operator new @ 0x00BFEA1A (the 10-byte cell, the 0xC-byte count block), CBaseSerialise<COldFileFormatHeightMapCell1,1>::
//   ConvertPreviousUnknownVersionUp @ 0x00824D30 (same convention; manifest "CEngineWorldMap::CreateNewBankMap" is
//   propagated), the count blocks' deleter (slot +4, object in ecx), operator delete @ 0x00BFE9BC; the new block's
//   deleter is 0x00822E60 CCountedPointer<COldFileFormatHeightMapCell2>::DeleteData.
// Identity: FableWin.pdb `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatHeightMapCell2@@
//   VCOldFileFormatHeightMapCell1@@...` and `?ConvertFromOld@COldFileFormatHeightMapCell2@@...` (name set); Ego_r.pdb
//   COldFileFormatHeightMapCell2 = 10 bytes {Height float +5, TextureBlendAlpha +9}, COldFileFormatHeightMapCell1 = 9
//   bytes {Height +5}, CBaseSerialise<Cell1, 1> (version 1).
// Label: the catalog's "CAFile_ConstructPrimitive" is descriptive and wrong.
// Shape: the landed 0x00518040 / 0x0051C2F0 template: `CCountedPointer<T> result(new T); CCountedPointer<TOld> held((TOld*)0);
//   TOld* old = data; if (old version != TOld::GetClassVersionNumber()) { held = TOld::ConvertPreviousUnknownVersionUp(data);
//   old = held.object; } result.object->ConvertFromOld(old); return result;` with ConvertFromOld inline (`Height =
//   old->Height; TextureBlendAlpha = 0x80;`). The cell has no constructor, so VC keeps result.object in ebx and homes
//   held's object word in the frame (`mov [esp+1Ch],ebp`; compare the constructor-bearing brush twin 0x00825590).
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
    static void __fastcall DeleteData(void* data);     // <COldFileFormatHeightMapCell2>: 0x00822E60
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
template <class T, long V> class CBaseSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return (unsigned short)V; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
class COldFileFormatHeightMapCell1 : public CBaseSerialise<COldFileFormatHeightMapCell1, 1> {
public:
    float Height;                                      // +0x05 (9 bytes)
};
class COldFileFormatHeightMapCell2 : public CReplaceSerialise<COldFileFormatHeightMapCell2, COldFileFormatHeightMapCell1> {
public:
    enum { VERSION = 2 };
    float         Height;                              // +0x05
    unsigned char TextureBlendAlpha;                   // +0x09 (10 bytes)
    void ConvertFromOld(const COldFileFormatHeightMapCell1* old)
    {
        Height = old->Height;
        TextureBlendAlpha = 0x80;
    }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatHeightMapCell1> __fastcall
CBaseSerialise<COldFileFormatHeightMapCell1, 1>::ConvertPreviousUnknownVersionUp(void* data);   // 0x00824D30
template <class T, class TOld>
CCountedPointer<T> __fastcall CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held((TOld*)0);
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

template CCountedPointer<COldFileFormatHeightMapCell2> __fastcall
CReplaceSerialise<COldFileFormatHeightMapCell2, COldFileFormatHeightMapCell1>::ConvertPreviousUnknownVersionUp(void* data);