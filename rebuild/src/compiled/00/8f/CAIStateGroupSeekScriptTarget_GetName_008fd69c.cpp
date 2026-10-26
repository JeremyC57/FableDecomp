// CAIStateGroup_SeekScriptTarget::GetName() -- static: the AI state group name "SG_SEEK_SCRIPT_TARGET" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SeekScriptTarget@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD69C: push esi; push -1; push "SG_SEEK_SCRIPT_TARGET"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220114 registers this GetName with Alloc 0x9022d6 (pushed before it); Alloc 0x9022d6 -> ctor 0x8ff53a stores (last) the vtable of CAIStateGroup_SeekScriptTarget at this+0; literal "SG_SEEK_SCRIPT_TARGET"; PDB name set ?GetName@CAIStateGroup_SeekScriptTarget@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SeekScriptTarget@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd69c::build).
#define CAIStateGroup_SeekScriptTarget CAIStateGroup_SeekScriptTargetLayout
#include "engine/CAIStateGroup_SeekScriptTarget.h"
#undef CAIStateGroup_SeekScriptTarget

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SeekScriptTarget : public CAIStateGroup_SeekScriptTargetLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SeekScriptTarget::GetName()
{
    return CCharString((const char*)0x01289310, -1);   // "SG_SEEK_SCRIPT_TARGET"
}
