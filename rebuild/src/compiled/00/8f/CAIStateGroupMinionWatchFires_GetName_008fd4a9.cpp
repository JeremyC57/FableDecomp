// CAIStateGroup_MinionWatchFires::GetName() -- static: the AI state group name "SG_MINION_WATCH_FIRES" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionWatchFires@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD4A9: push esi; push -1; push "SG_MINION_WATCH_FIRES"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fafc registers this GetName with Alloc 0x9020ca (pushed before it); Alloc 0x9020ca -> ctor 0x8ff04d stores (last) the vtable of CAIStateGroup_MinionWatchFires at this+0; literal "SG_MINION_WATCH_FIRES"; PDB name set ?GetName@CAIStateGroup_MinionWatchFires@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionWatchFires@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd4a9::build).
#define CAIStateGroup_MinionWatchFires CAIStateGroup_MinionWatchFiresLayout
#include "engine/CAIStateGroup_MinionWatchFires.h"
#undef CAIStateGroup_MinionWatchFires

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionWatchFires : public CAIStateGroup_MinionWatchFiresLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionWatchFires::GetName()
{
    return CCharString((const char*)0x01288FD8, -1);   // "SG_MINION_WATCH_FIRES"
}
