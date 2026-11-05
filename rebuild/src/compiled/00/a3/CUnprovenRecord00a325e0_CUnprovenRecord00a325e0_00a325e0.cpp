// CUnprovenRecord_00a325e0::CUnprovenRecord_00a325e0() -- default constructor: construct the CCharString member, then null the counted pointer at +4 (Data, Info)
// Leaf: ??0CUnprovenRecord_00a325e0@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a325e0, 20 bytes, thiscall, returns this, plain `ret`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT); its own bytes construct a CCharString at +0 and zero the two dwords at +4/+8 -- a default-constructed CCountedPointer (Data, Info) -- so the record is {CCharString, CCountedPointer<T>} (Ego_r.pdb has several: CMeshDataBankInfo, C3DMeshVolume, pair<CCharString, CCountedPointer<..> >; nothing picks one; the catalog's C3DSkeletalMorphTarget has a CArray there instead).
// Callee: CCharString::CCharString() 0x0099E4B0.
// Shape (family L038): push esi / mov esi,ecx / call the string ctor / one zeroed register stored twice / mov eax,esi.
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    ~CCharString();
private:
    void* PStringData;                                    // +0x00
};
#include "engine/CCountedPointer.h"
class CUnprovenPointee_00a325e0;                             // descriptive: the pointee is unproven
class CUnprovenRecord_00a325e0 {                                              // descriptive: the record type is unproven
public:
    CCharString Name;                                     // +0x00
    CCountedPointer<CUnprovenPointee_00a325e0> Pointer;      // +0x04
    CUnprovenRecord_00a325e0();
};
CUnprovenRecord_00a325e0::CUnprovenRecord_00a325e0()
{
}