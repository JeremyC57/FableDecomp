// CGraphicDataBankInfo::`scalar deleting destructor'(unsigned int) -- release the GraphicDataBank counted pointer, destroy BankHandle, free the object when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F87C0, length 82 bytes.
// Callees: 0x00BFE9BC operator delete x2, 0x0099EAE0 CCharString::~CCharString, CCPPointerInfo::DeleteFunc through the block
// The manifest label (C3DMeshFilePartitionChunk::vector_deleting_destructor) is propagated; the body is the
// PDB's CGraphicDataBankInfo (sizeof 0x38: CGraphicDataBankInit +0x00, CCharString BankHandle +0x2c,
// CCountedPointer<CGraphicDataBank> GraphicDataBank +0x30), sitting between the CGraphicBankManager bodies at
// 0x009F86C0..0x009F88C0. No vtable is stored: the class is not polymorphic, and VC7.1 emits this `??_G` for a
// non-polymorphic class with a non-trivial destructor in any translation unit that deletes it.
//
#include "rebuild_abi.h"

struct CCharString {
    CCharString();                                      // 0x0099E4B0 default
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

void __cdecl operator delete(void* p);                  // 0x00BFE9BC (retail CRT)
struct CCPPointerInfo {                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // called with Data in ecx and nothing on the stack
    void* Data;
};
template<class T> struct CCountedPointer {
    T*             m_pData;                              // PDB: Data
    CCPPointerInfo* m_pInfo;                             // PDB: Info
    CCountedPointer() : m_pData(0), m_pInfo(0) {}
    // The shared block is released through a re-read of m_pInfo after the decrement (the store through
    // the block pointer may alias the member), the block's delete function is called on its Data and
    // the block itself is freed with a plain (untested) operator delete.
    void ReleaseInfo()
    {
        if (m_pInfo) {
            --m_pInfo->RefCount;
            if (m_pInfo->RefCount == 0) {
                m_pInfo->DeleteFunc(m_pInfo->Data);
                delete m_pInfo;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); m_pData = 0; m_pInfo = 0; }
    void Reset()       { ReleaseInfo(); m_pInfo = 0; m_pData = 0; }
};

struct CGraphicDataBank;
struct CGraphicDataBankInit { unsigned char _bytes[0x2c]; };     // trivially destructible init block
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
