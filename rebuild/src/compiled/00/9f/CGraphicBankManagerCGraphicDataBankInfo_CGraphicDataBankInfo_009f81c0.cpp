// CGraphicBankManager::CGraphicDataBankInfo::~CGraphicDataBankInfo() -- implicit destructor: release the counted GraphicDataBank (+0x30), then the BankHandle string (+0x2C) by tail jump
// Leaf: ??1CGraphicDataBankInfo@CGraphicBankManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F81C0, 61 bytes, thiscall, `jmp` tail; no call or address reference (an unreferenced
//   COMDAT kept by the linker, no /OPT:REF).
// Identity: the CGraphicBankManager TU opens with CGraphicDataBankInfo's inline special members, in a row: the copy
//   constructor 0x009F80E0 (rep movsd 0xB dwords of Init, CCharString copy 0x0099EC30 at +0x2C, counted-pointer copy at
//   +0x30), the default constructor 0x009F8170 (Init fields -1 / flags, CCharString() at +0x2C, Data = Info = 0), then this
//   row, then the manager's own members (FrameEnd 0x009F8200 ..). The second body of this shape, 0x009F88C0, sits among
//   the list's function-template helpers (_Construct 0x009F8780, __fastcall: the TU is /Gr; _M_create_node 0x009F8860;
//   erase 0x009F8920): STLport _Destroy<CGraphicDataBankInfo>, which list::erase / clear use. VC7.1 emits the implicit
//   class members before the function-template instantiations (lane experiments exp/order1.cpp, exp/order2.cpp); the
//   CMeshBankManager TU repeats the pattern (0x00A0A3D0 / 0x00A0A3F0 vs 0x00A0A5D0). FableWin.pdb names
//   ??1CGraphicDataBankInfo@CGraphicBankManager@@QAE@XZ. The landed 0x009F88C0 carries this ??1 name: NAME_FIXES.tsv
//   respells it to the _Destroy instantiation. The manifest label (_Dest_val<..C3DSkeletalMorphTarget..>) is propagated
//   and wrong.
// Layout: Ego_r.pdb CGraphicDataBankInfo (0x38) = CGraphicDataBankInit Init +0 (0x2c, trivially destructible), CCharString
//   BankHandle +0x2c, CCountedPointer<CGraphicDataBank> GraphicDataBank +0x30 (no engine/CGraphicBankManager.h).
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CCharString::~CCharString 0x0099EAE0 (tail).
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};

class CGraphicDataBank;
class CGraphicDataBankInit { public: unsigned char _bytes[0x2c]; };   // pixel formats and flags, trivially destructible

class CGraphicBankManager {
public:
    class CGraphicDataBankInfo {                                // Ego_r.pdb, 0x38 bytes
    public:
        CGraphicDataBankInit              Init;                 // +0x00
        CCharString                       BankHandle;           // +0x2c
        CCountedPointer<CGraphicDataBank> GraphicDataBank;      // +0x30
    };
};

void EmitGraphicDataBankInfoDestructor(CGraphicBankManager::CGraphicDataBankInfo* info)   // emits the implicit ??1; not a retail function
{
    info->CGraphicBankManager::CGraphicDataBankInfo::~CGraphicDataBankInfo();
}