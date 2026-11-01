// std::_Destroy<CMeshDataBankInfo>(CMeshBankManager::CMeshDataBankInfo* __pointer) -- STLport single-element destroy of a mesh-bank record: the inlined implicit ~CMeshDataBankInfo (release MeshDataBank, then BankHandle)
// Leaf: ??$_Destroy@VCMeshDataBankInfo@CMeshBankManager@@@std@@YIXPAVCMeshDataBankInfo@CMeshBankManager@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0A5D0, 60 bytes, __fastcall (pointer in ecx), `jmp` tail; no call or address
//   reference (an unreferenced COMDAT kept by the linker, no /OPT:REF).
// Identity: the CMeshBankManager TU (~CMeshBankManager 0x00A0A150 .. ??_G 0x00A0A8A0) keeps Ego_r.pdb
//   CMeshBankManager::MeshDataBanks, a list<CMeshBankManager::CMeshDataBankInfo> (CCharString BankHandle +0,
//   CCountedPointer<CMeshDataBank> MeshDataBank +4; its erase 0x00A0A650 inlines this release + ~CCharString at node+8,
//   its _Construct 0x00A0A4A0 copies the 12-byte record and is __fastcall, so the TU builds /Gr). The TU holds two bodies
//   of this shape: 0x00A0A3F0 right after the record's inline default constructor 0x00A0A3D0 (the class-member COMDATs:
//   ~CMeshDataBankInfo) and this one among the list's function-template helpers (after _Construct 0x00A0A4A0, before
//   erase 0x00A0A650) = STLport _Destroy(_Tp*), which list::clear / erase use. The CGraphicBankManager TU repeats the
//   pattern exactly (ctor 0x009F8170 + dtor 0x009F81C0; _Construct 0x009F8780, _Destroy 0x009F88C0, erase 0x009F8920),
//   and VC7.1 emits function-template instantiations after the implicit members they use (lane experiments
//   exp/order1.cpp, exp/order2.cpp). ~C3DMeshVolume, the other {CCharString, CCountedPointer} record, is 0x00A32690.
//   The manifest label (_Dest_val<..C3DSkeletalMorphTarget..>) is propagated and wrong.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CCharString::~CCharString 0x0099EAE0 (tail).
// Layout: engine/CMeshBankManager.h keeps MeshDataBanks opaque and has no nested CMeshDataBankInfo, so the nested record
//   is declared here from Ego_r.pdb (header debt, NOTES.md).
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};

class CMeshDataBank;

class CMeshBankManager {
public:
    class CMeshDataBankInfo {                                   // Ego_r.pdb, 12 bytes
    public:
        CCharString                    BankHandle;              // +0x00
        CCountedPointer<CMeshDataBank> MeshDataBank;            // +0x04
    };
};

namespace std {
// STLport 4.6 stl/_construct.h
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
    __pointer;
    __pointer->~_Tp();
}
template void _Destroy<CMeshBankManager::CMeshDataBankInfo>(CMeshBankManager::CMeshDataBankInfo*);
}