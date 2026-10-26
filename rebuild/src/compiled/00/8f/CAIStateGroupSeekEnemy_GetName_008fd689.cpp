// CAIStateGroup_SeekEnemy::GetName() -- static: the AI state group name "SG_SEEK_ENEMY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SeekEnemy@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD689: push esi; push -1; push "SG_SEEK_ENEMY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12200ec registers this GetName with Alloc 0x9022c0 (pushed before it); Alloc 0x9022c0 -> ctor 0x8ff4ec stores (last) the vtable of CAIStateGroup_SeekEnemy at this+0; literal "SG_SEEK_ENEMY"; PDB name set ?GetName@CAIStateGroup_SeekEnemy@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SeekEnemy@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd689::build).
#define CAIStateGroup_SeekEnemy CAIStateGroup_SeekEnemyLayout
#include "engine/CAIStateGroup_SeekEnemy.h"
#undef CAIStateGroup_SeekEnemy

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SeekEnemy : public CAIStateGroup_SeekEnemyLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SeekEnemy::GetName()
{
    return CCharString((const char*)0x0127813C, -1);   // "SG_SEEK_ENEMY"
}
