// std::_Construct(CMeshBankManager::CMeshDataBankInfo*, const CMeshBankManager::CMeshDataBankInfo&) -- placement copy of a mesh-bank record: bank handle string plus a counted pointer whose count is bumped
// Retail: Fable.exe @ 0x006C3170, 37 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N006c3170 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
#include "engine/CCountedPointer.h"
struct CMeshDataBank;
struct CMeshDataBankInfo {                       // CMeshBankManager::CMeshDataBankInfo (PDB, sizeof 0xC)
    CCharString                     BankHandle;      // +0x00
    CCountedPointer<CMeshDataBank>  MeshDataBank;    // +0x04 Data, +0x08 Info
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CMeshDataBankInfo* p, const CMeshDataBankInfo& value)
{
    if (p != 0)
        new (p) CMeshDataBankInfo(value);
}
}