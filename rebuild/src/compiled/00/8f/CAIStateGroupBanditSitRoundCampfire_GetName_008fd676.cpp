// CAIStateGroup_BanditSitRoundCampfire::GetName() -- static: the AI state group name "SG_BANDIT_SIT_ROUND_CAMPFIRE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditSitRoundCampfire@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD676: push esi; push -1; push "SG_BANDIT_SIT_ROUND_CAMPFIRE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f9e4 registers this GetName with Alloc 0x9022aa (pushed before it); Alloc 0x9022aa -> ctor 0x8ff4a2 stores (last) the vtable of CAIStateGroup_BanditSitRoundCampfire at this+0; literal "SG_BANDIT_SIT_ROUND_CAMPFIRE"; PDB name set ?GetName@CAIStateGroup_BanditSitRoundCampfire@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditSitRoundCampfire@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd676::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditSitRoundCampfire
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditSitRoundCampfire::GetName()
{
    return CCharString((const char*)0x012892F0, -1);   // "SG_BANDIT_SIT_ROUND_CAMPFIRE"
}
