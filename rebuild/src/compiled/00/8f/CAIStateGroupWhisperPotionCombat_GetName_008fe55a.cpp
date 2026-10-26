// CAIStateGroup_WhisperPotionCombat::GetName() -- static: the AI state group name "SG_WHISPER_POTION_COMBAT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WhisperPotionCombat@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE55A: push esi; push -1; push "SG_WHISPER_POTION_COMBAT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221384 registers this GetName with Alloc 0x903479 (pushed before it); Alloc 0x903479 -> ctor 0x9031ba stores (last) the vtable of CAIStateGroup_WhisperPotionCombat at this+0; literal "SG_WHISPER_POTION_COMBAT"; PDB name set ?GetName@CAIStateGroup_WhisperPotionCombat@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WhisperPotionCombat@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe55a::build).
#define CAIStateGroup_WhisperPotionCombat CAIStateGroup_WhisperPotionCombatLayout
#include "engine/CAIStateGroup_WhisperPotionCombat.h"
#undef CAIStateGroup_WhisperPotionCombat

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WhisperPotionCombat : public CAIStateGroup_WhisperPotionCombatLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WhisperPotionCombat::GetName()
{
    return CCharString((const char*)0x0128A924, -1);   // "SG_WHISPER_POTION_COMBAT"
}
