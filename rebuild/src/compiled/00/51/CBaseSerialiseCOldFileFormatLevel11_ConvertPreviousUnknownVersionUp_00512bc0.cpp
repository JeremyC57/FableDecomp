// CBaseSerialise<COldFileFormatLevel1, 1>::ConvertPreviousUnknownVersionUp(const void* data) -- the oldest version cannot be converted up from anything: hand back a fresh default record in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CBaseSerialise@VCOldFileFormatLevel1@@$00@@SI?AV?$CCountedPointer@VCOldFileFormatLevel1@@@@PBX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00512BC0, 107 bytes, static `__fastcall` (the hidden CCountedPointer return slot in ecx,
//   data in edx, unused), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ConvertPreviousUnknownVersionUp@?$CBaseSerialise@VCOldFileFormatLevel1@@$00@@SI?AV?$CCountedPointer@VCOldFileFormatLevel1@@@@PBX@Z;
//   its only caller is the landed CReplaceSerialise<COldFileFormatLevel2, COldFileFormatLevel1>::ConvertPreviousUnknownVersionUp 0x00514790, which declares
//   it (the version-1 link); the allocation is 13 bytes (Ego_r.pdb COldFileFormatLevel1, pack 1) and the deleter CCountedPointer<COldFileFormatLevel1>::
//   DeleteData 0x0050D040. The manifest's "CEngineWorldMap::CreateNewBankMap" is propagated.
// Callees: operator new 0x00BFEA1A (the record, then the 0xC-byte count block), operator delete 0x00BFE9BC; the count
//   block's deleter (slot +4).
// Shape (family M107, representative 0x00512BC0): `CCountedPointer<T> result(new T); return result;` -- the inline
//   CBaseSerialise constructor stores SerialiseVersionNumber = VERSION before the count block is allocated; the return copy
//   bumps the count and the local's destructor drops it again (`inc ; mov ; dec ; mov ; mov`), releasing on zero (the landed
//   0x00514790 CCountedPointer model).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC
class C2DBoxI { public: long X0, Y0, X1, Y1; };
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
    static void __fastcall DeleteData(void* data);     // <COldFileFormatLevel1>: 0x0050D040
    CCountedPointer(T* p) : object(p), control(p ? new CCPPointerInfo(p, DeleteData) : 0) {}
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control)
    {
        if (control)
            ++control->refs;
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
    CBaseSerialise() { SerialiseVersionNumber = VERSION; }
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(const void* data);
};
class COldFileFormatLevel1 : public CBaseSerialise<COldFileFormatLevel1, 1> {    // Ego_r.pdb, 13 bytes
public:
    long MapDataOffset;                                   // +0x05
    long ThingDataOffset;                                 // +0x09
};
#pragma pack(pop)
template <class T, int VERSION>
CCountedPointer<T> __fastcall CBaseSerialise<T, VERSION>::ConvertPreviousUnknownVersionUp(const void* data)
{
    CCountedPointer<T> result(new T);
    return result;
}
template CCountedPointer<COldFileFormatLevel1> __fastcall CBaseSerialise<COldFileFormatLevel1, 1>::ConvertPreviousUnknownVersionUp(const void* data);