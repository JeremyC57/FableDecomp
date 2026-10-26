// CAIStateGroup_MinionListenToStory::GetName() -- static: the AI state group name "SG_MINION_LISTEN_TO_STORY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionListenToStory@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD5BB: push esi; push -1; push "SG_MINION_LISTEN_TO_STORY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fcdc registers this GetName with Alloc 0x8ff31a (pushed before it); Alloc 0x8ff31a stores the vtable of CAIStateGroup_MinionListenToStory into the new object; literal "SG_MINION_LISTEN_TO_STORY"; PDB name set ?GetName@CAIStateGroup_MinionListenToStory@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionListenToStory@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd5bb::build).
#define CAIStateGroup_MinionListenToStory CAIStateGroup_MinionListenToStoryLayout
#include "engine/CAIStateGroup_MinionListenToStory.h"
#undef CAIStateGroup_MinionListenToStory

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionListenToStory : public CAIStateGroup_MinionListenToStoryLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionListenToStory::GetName()
{
    return CCharString((const char*)0x01289160, -1);   // "SG_MINION_LISTEN_TO_STORY"
}
