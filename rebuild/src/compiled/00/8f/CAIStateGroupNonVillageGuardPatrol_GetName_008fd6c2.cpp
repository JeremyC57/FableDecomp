// CAIStateGroup_NonVillageGuardPatrol::GetName() -- static: the AI state group name "SG_NON_VILLAGE_GUARD_PATROL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_NonVillageGuardPatrol@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD6C2: push esi; push -1; push "SG_NON_VILLAGE_GUARD_PATROL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12200c4 registers this GetName with Alloc 0x902302 (pushed before it); Alloc 0x902302 -> ctor 0x8ff5d6 stores (last) the vtable of CAIStateGroup_NonVillageGuardPatrol at this+0; literal "SG_NON_VILLAGE_GUARD_PATROL"; PDB name set ?GetName@CAIStateGroup_NonVillageGuardPatrol@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_NonVillageGuardPatrol@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd6c2::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_NonVillageGuardPatrol
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_NonVillageGuardPatrol::GetName()
{
    return CCharString((const char*)0x01289340, -1);   // "SG_NON_VILLAGE_GUARD_PATROL"
}
