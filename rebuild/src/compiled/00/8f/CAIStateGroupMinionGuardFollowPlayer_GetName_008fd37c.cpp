// CAIStateGroup_MinionGuardFollowPlayer::GetName() -- static: the AI state group name "SG_MINION_GUARD_FOLLOW_PLAYER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardFollowPlayer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD37C: push esi; push -1; push "SG_MINION_GUARD_FOLLOW_PLAYER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12216a4 registers this GetName with Alloc 0x8fed9b (pushed before it); Alloc 0x8fed9b stores the vtable of CAIStateGroup_MinionGuardFollowPlayer into the new object; literal "SG_MINION_GUARD_FOLLOW_PLAYER"; PDB name set ?GetName@CAIStateGroup_MinionGuardFollowPlayer@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardFollowPlayer@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd37c::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardFollowPlayer
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardFollowPlayer::GetName()
{
    return CCharString((const char*)0x01288DA8, -1);   // "SG_MINION_GUARD_FOLLOW_PLAYER"
}
