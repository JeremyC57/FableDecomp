// CAIStateGroup_FollowPlayer::GetName() -- static: the AI state group name "SG_MINION_FOLLOW_PLAYER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_FollowPlayer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDDF5: push esi; push -1; push "SG_MINION_FOLLOW_PLAYER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12206b4 registers this GetName with Alloc 0x902644 (pushed before it); Alloc 0x902644 -> ctor 0x900412 stores (last) the vtable of CAIStateGroup_FollowPlayer at this+0; literal "SG_MINION_FOLLOW_PLAYER"; PDB name set ?GetName@CAIStateGroup_FollowPlayer@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_FollowPlayer@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fddf5::build).
#define CAIStateGroup_FollowPlayer CAIStateGroup_FollowPlayerLayout
#include "engine/CAIStateGroup_FollowPlayer.h"
#undef CAIStateGroup_FollowPlayer

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_FollowPlayer : public CAIStateGroup_FollowPlayerLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_FollowPlayer::GetName()
{
    return CCharString((const char*)0x01289D5C, -1);   // "SG_MINION_FOLLOW_PLAYER"
}
