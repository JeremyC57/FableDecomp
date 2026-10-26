// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::GetName() -- static: the AI state group name "SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD63E: push esi; push -1; push "SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f994 registers this GetName with Alloc 0x902294 (pushed before it); Alloc 0x902294 -> ctor 0x8ff418 stores (last) the vtable of CAIStateGroup_MinionGuardReactToPlayerWithWeapon at this+0; literal "SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON"; PDB name set ?GetName@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd63e::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardReactToPlayerWithWeapon
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardReactToPlayerWithWeapon::GetName()
{
    return CCharString((const char*)0x01289260, -1);   // "SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON"
}
