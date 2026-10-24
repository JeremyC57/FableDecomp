// CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(void* data) -- map-header version chain, step 1 -> 2: allocate a fresh version-2 header, convert the serialised data up to version 1 first unless it already is version 1, return the new header
// Grade: DIFFER(214v209) -- register allocation only: retail spills the hidden result pointer to the frame and keeps result.Object in ebx, result.Info in esi, held.Info in ebp (no zero register, `test reg,reg`, the version byte loaded into al before `xor ebp,ebp`); ours keeps the hidden pointer in ebp and spills result.Object (zero register ebx, `cmp reg,ebx`, a null-path store) -- the landed v7->v8 step 0x0051B940 has the ours-shape in retail too
// Leaf: ?ConvertPreviousUnknownVersionUp@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005148D0, 208 bytes, plain `ret` (static `__fastcall`: hidden result in ecx, data in
//   edx). PDB name ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader2@@VCOldFileFormatMapHeader1@@@@
//   SI?AV?$CCountedPointer@VCOldFileFormatMapHeader2@@@@PAX@Z (Ego_r.pdb name set). Caller 0x00516060 (the version-3
//   step). Catalog label "CEngineWorldMap_MoveConstructor" is descriptive.
// Callees: operator new 0x00BFEA1A (the 5-byte header, then the 12-byte counted-pointer info), the previous step
//   CBaseSerialise<COldFileFormatMapHeader1,1>::ConvertPreviousUnknownVersionUp 0x00512C70
//   (manifest "CEngineWorldMap::CreateNewBankMap" is propagated), the info's deleter CCountedPointer<COldFileFormatMapHeader2>::DeleteData 0x0050D1F0 (`__fastcall`, `delete p`;
//   manifest "operator_new" is propagated), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb COldFileFormatMapHeader2 / 1 are 5 bytes (CReplaceSerialise: ulong ByteLength +0, uchar
//   SerialiseVersionNumber +4; pack 1); CCountedPointer {Object, Info}, the info {RefCount, Deleter, Object}.
// Model: the landed sibling step 0x0051B940 (v7 -> v8): a raw-pointer counted-pointer ctor, a null `held` pointer,
//   the conditional `held = previous step(data)` (the operator= folds to a ref-count increment), ConvertFromOld (empty
//   for these memberless versions, so only held's info survives), `return result;` copying into the hidden result,
//   then held and result released (reverse declaration order; the call temporary's release re-reads its info).
#pragma pack(push, 1)
template <class TNew, class TOld> class CReplaceSerialise;

class COldFileFormatMapHeader1;
class COldFileFormatMapHeader2;
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

template <class T, int Version> class CBaseSerialise {
public:
    unsigned long ByteLength;                       // +0x00
    unsigned char SerialiseVersionNumber;           // +0x04
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(const void* data);   // 0x00512C70
};

class COldFileFormatMapHeader1 : public CBaseSerialise<COldFileFormatMapHeader1, 1> {
};

class COldFileFormatMapHeader2 : public CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1> {
public:
    __forceinline COldFileFormatMapHeader2() { }
    __forceinline void ConvertFromOld(const COldFileFormatMapHeader1*) { }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatMapHeader2> __fastcall
CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<COldFileFormatMapHeader2> result(new COldFileFormatMapHeader2);
    CCountedPointer<COldFileFormatMapHeader1> held((COldFileFormatMapHeader1*)0);
    COldFileFormatMapHeader1* old = (COldFileFormatMapHeader1*)data;
    if (((COldFileFormatMapHeader1*)data)->SerialiseVersionNumber != 1) {
        held = CBaseSerialise<COldFileFormatMapHeader1, 1>::ConvertPreviousUnknownVersionUp(data);
        old = held.Object;
    }
    result.Object->ConvertFromOld(old);
    return result;
}
