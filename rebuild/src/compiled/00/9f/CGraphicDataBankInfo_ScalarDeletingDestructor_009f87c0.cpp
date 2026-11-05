// CGraphicDataBankInfo::`scalar deleting destructor'(unsigned int) -- release the GraphicDataBank counted pointer, destroy BankHandle, free the object when bit 0 of the flag is set
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pData->Data, m_pInfo->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F87C0, length 82 bytes.
// Callees: 0x00BFE9BC operator delete x2, 0x0099EAE0 CCharString::~CCharString, CCPPointerInfo::DeleteFunc through the block
// The manifest label (C3DMeshFilePartitionChunk::vector_deleting_destructor) is propagated; the body is the
// PDB's CGraphicDataBankInfo (sizeof 0x38: CGraphicDataBankInit +0x00, CCharString BankHandle +0x2c,
// CCountedPointer<CGraphicDataBank> GraphicDataBank +0x30), sitting between the CGraphicBankManager bodies at
// 0x009F86C0..0x009F88C0. No vtable is stored: the class is not polymorphic, and VC7.1 emits this `??_G` for a
// non-polymorphic class with a non-trivial destructor in any translation unit that deletes it.
//
#include "rebuild_abi.h"

class CCharString { public:
    CCharString();                                      // 0x0099E4B0 default
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

void __cdecl operator delete(void* p);                  // 0x00BFE9BC (retail CRT)
#include "engine/CCountedPointer.h"

class CGraphicDataBank;
class CGraphicDataBankInit { public: unsigned char _bytes[0x2c]; };     // trivially destructible init block
struct CGraphicDataBankInfo {
    CGraphicDataBankInit               Init;              // +0x00
    CCharString                        BankHandle;        // +0x2c
    CCountedPointer<CGraphicDataBank>  GraphicDataBank;   // +0x30   sizeof == 0x38
};
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBankInfo) == 0x38);
// The deletion that makes VC7.1 emit the scalar deleting destructor in this translation unit.
void DestroyGraphicDataBankInfo(CGraphicDataBankInfo* info)
{
    delete info;
}