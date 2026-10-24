// CReplaceSerialise<COldFileFormatMapHeader3,COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(void* data) -- map-header version chain, step 2 -> 3: allocate a fresh version-3 header, convert the serialised data up to version 2 first unless it already is version 2, return the new header
// Grade: DIFFER(214v208) -- register allocation only (as 0x005148D0): retail spills the hidden result pointer and keeps result.Object in ebp, result.Info in esi, held.Info in ebx, data in edi; ours keeps the hidden pointer in ebp, data in ebx and spills result.Object behind an edi zero register
// Leaf: ?ConvertPreviousUnknownVersionUp@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00516010, 208 bytes, plain `ret` (static `__fastcall`: hidden result in ecx, data in
//   edx). PDB name ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader3@@VCOldFileFormatMapHeader2@@@@
//   SI?AV?$CCountedPointer@VCOldFileFormatMapHeader3@@@@PAX@Z (Ego_r.pdb name set). Caller 0x005180A0 (the version-4
//   step). Catalog label "CEngineWorldMap_MoveConstructor_Type2" is descriptive.
// Callees: operator new 0x00BFEA1A (the 5-byte header, then the 12-byte counted-pointer info), the previous step
//   CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp
//   0x005148D0, the info's deleter CCountedPointer<COldFileFormatMapHeader3>::DeleteData 0x0050D180 (`__fastcall`, `delete p`;
//   manifest "operator_new" is propagated), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb COldFileFormatMapHeader3 / 2 are 5 bytes (CReplaceSerialise: ulong ByteLength +0, uchar
//   SerialiseVersionNumber +4; pack 1); CCountedPointer {Object, Info}, the info {RefCount, Deleter, Object}.
// Model: the landed sibling step 0x0051B940 (v7 -> v8): a raw-pointer counted-pointer ctor, a null `held` pointer,
//   the conditional `held = previous step(data)` (the operator= folds to a ref-count increment), ConvertFromOld (empty
//   for these memberless versions, so only held's info survives), `return result;` copying into the hidden result,
//   then held and result released (reverse declaration order; the call temporary's release re-reads its info).
#pragma pack(push, 1)
template <class TNew, class TOld> class CReplaceSerialise;

class COldFileFormatMapHeader2;
class COldFileFormatMapHeader3;
#pragma pack(pop)

void operator delete(void* p);                      // 0x00BFE9BC

struct CCPPointerInfo {
    unsigned long RefCount;                         // +0x00
    void (__fastcall* Deleter)(void* object);       // +0x04
    void* Object;                                   // +0x08
    __forceinline CCPPointerInfo(void* p, void (__fastcall* deleter)(void*)) : RefCount(1), Deleter(deleter), Object(p) { }
};

template <class T> class CCountedPointer {
public:
    T* Object;                                      // +0x00
    CCPPointerInfo* Info;                           // +0x04
    static void __fastcall DeleteData(void* data);
    __forceinline CCountedPointer(T* p) : Object(p), Info(p ? new CCPPointerInfo(p, DeleteData) : 0) { }
    __forceinline CCountedPointer(const CCountedPointer& x) : Object(x.Object), Info(x.Info) { if (Info) ++Info->RefCount; }
    __forceinline CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* info = x.Info;
        T* object = x.Object;
        if (Info != info) {
            if (Info && --Info->RefCount == 0) { Info->Deleter(Info->Object); operator delete(Info); }
            Object = object;
            Info = info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
    __forceinline ~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->Deleter(Info->Object); operator delete(Info); } }
};

#pragma pack(push, 1)
template <class TNew, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                       // +0x00
    unsigned char SerialiseVersionNumber;           // +0x04
    static CCountedPointer<TNew> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};

class COldFileFormatMapHeader1;

class COldFileFormatMapHeader2 : public CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1> {
};                                                  // its step: 0x005148D0

class COldFileFormatMapHeader3 : public CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2> {
public:
    __forceinline COldFileFormatMapHeader3() { }
    __forceinline void ConvertFromOld(const COldFileFormatMapHeader2*) { }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatMapHeader3> __fastcall
CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<COldFileFormatMapHeader3> result(new COldFileFormatMapHeader3);
    CCountedPointer<COldFileFormatMapHeader2> held((COldFileFormatMapHeader2*)0);
    COldFileFormatMapHeader2* old = (COldFileFormatMapHeader2*)data;
    if (((COldFileFormatMapHeader2*)data)->SerialiseVersionNumber != 2) {
        held = CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(data);
        old = held.Object;
    }
    result.Object->ConvertFromOld(old);
    return result;
}
