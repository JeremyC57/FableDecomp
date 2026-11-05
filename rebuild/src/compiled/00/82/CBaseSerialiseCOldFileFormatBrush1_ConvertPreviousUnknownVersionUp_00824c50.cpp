// CBaseSerialise<COldFileFormatBrush, 1>::ConvertPreviousUnknownVersionUp(const void* data) -- the oldest version cannot be converted up from anything: hand back a fresh default record in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CBaseSerialise@VCOldFileFormatBrush@@$00@@SI?AV?$CCountedPointer@VCOldFileFormatBrush@@@@PBX@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members object->Data, control->Info are the header CCountedPointer's (same offsets); the local CCPPointerInfo members refs->RefCount, destroy->DeleteFunc, object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00824C50, 107 bytes, static `__fastcall` (the hidden CCountedPointer return slot in ecx,
//   data in edx, unused), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ConvertPreviousUnknownVersionUp@?$CBaseSerialise@VCOldFileFormatBrush@@$00@@SI?AV?$CCountedPointer@VCOldFileFormatBrush@@@@PBX@Z;
//   its only caller is the landed CReplaceSerialise<CFileFormatBrush, COldFileFormatBrush>::ConvertPreviousUnknownVersionUp 0x00825590, which declares
//   it (the version-1 link); the allocation is 61 bytes (Ego_r.pdb COldFileFormatBrush, pack 1) and the deleter CCountedPointer<COldFileFormatBrush>::
//   DeleteData 0x00822CB0. The manifest's "CEngineWorldMap::CreateNewBankMap" is propagated.
// Callees: operator new 0x00BFEA1A (the record, then the 0xC-byte count block), operator delete 0x00BFE9BC; the count
//   block's deleter (slot +4).
// Shape (family M107, representative 0x00512BC0): `CCountedPointer<T> result(new T); return result;` -- the inline
//   CBaseSerialise constructor stores SerialiseVersionNumber = VERSION before the count block is allocated; the return copy
//   bumps the count and the local's destructor drops it again (`inc ; mov ; dec ; mov ; mov`), releasing on zero (the landed
//   0x00514790 CCountedPointer model).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC
class C2DBoxI { public: long X0, Y0, X1, Y1; };
#include "engine/CCountedPointer.h"
#pragma pack(push, 1)
template <class T, int VERSION> class CBaseSerialise {
public:
    CBaseSerialise() { SerialiseVersionNumber = VERSION; }
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(const void* data);
};
class COldFileFormatBrush : public CBaseSerialise<COldFileFormatBrush, 1> {    // Ego_r.pdb, 61 bytes
public:
    C2DBoxI GameArea;                                     // +0x05
    char ScriptName[32];                                  // +0x15
    float CreationTimeSeconds;                            // +0x35
    long CreationPattern;                                 // +0x39
};
#pragma pack(pop)
template <class T, int VERSION>
CCountedPointer<T> __fastcall CBaseSerialise<T, VERSION>::ConvertPreviousUnknownVersionUp(const void* data)
{
    CCountedPointer<T> result(new T);
    return result;
}
template CCountedPointer<COldFileFormatBrush> __fastcall CBaseSerialise<COldFileFormatBrush, 1>::ConvertPreviousUnknownVersionUp(const void* data);