// CAIStateGroup_StealthSeekEnemy::GetName() -- static: the AI state group name "SG_STEALTH_SEEK_ENEMY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_StealthSeekEnemy@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD6AF: push esi; push -1; push "SG_STEALTH_SEEK_ENEMY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122013c registers this GetName with Alloc 0x9022ec (pushed before it); Alloc 0x9022ec -> ctor 0x8ff588 stores (last) the vtable of CAIStateGroup_StealthSeekEnemy at this+0; literal "SG_STEALTH_SEEK_ENEMY"; PDB name set ?GetName@CAIStateGroup_StealthSeekEnemy@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_StealthSeekEnemy@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd6af::build).
#define CAIStateGroup_StealthSeekEnemy CAIStateGroup_StealthSeekEnemyLayout
#include "engine/CAIStateGroup_StealthSeekEnemy.h"
#undef CAIStateGroup_StealthSeekEnemy

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_StealthSeekEnemy : public CAIStateGroup_StealthSeekEnemyLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_StealthSeekEnemy::GetName()
{
    return CCharString((const char*)0x01289328, -1);   // "SG_STEALTH_SEEK_ENEMY"
}
