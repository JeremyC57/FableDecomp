// CMessageEvent::~CMessageEvent() -- the AI message event's implicit destructor: PTarget (+0x50) and POriginator (+0x48)
// intelligent pointers destroyed out of line, then the CBaseObject base destructor by tail call
// Leaf: ??1CMessageEvent@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687210, oracle length 27 bytes.
// Name (NF197, 2026-11-20): Ego_r.pdb 0x0049C737 + FableWin.pdb 0x01A77DE8 ??1CMessageEvent@@UAE@XZ; the CMessageEvent vtable
//   0x0125BE8C slot 0 (scalar deleting dtor 0x006872B0) expands this exact body (lea ecx,[esi+0x50] / [esi+0x48] -> 0x00A01C10,
//   then 0x0099A430), and engine/CMessageEvent.h's layout puts POriginator / PTarget (CIntelligentPointer<CThing>) at +0x48 / +0x50
//   with CBaseObject (protected virtual dtor ??1CBaseObject@@MAE@XZ, 0x0099A430) as the base. Was the generated stand-in
//   CAggregate_00687210 (members CMember_0099a430 / CBaseIntelligentPointer). The destructor is compiler-generated: retail stores
//   no vptr (a user-written empty body does, `mov [esi],vtable`), so the row emits the implicit COMDAT (nf86 scaffolding).
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x50] ; call ~CBaseIntelligentPointer ; lea ecx,[esi+0x48] ;
//   call ~CBaseIntelligentPointer ; mov ecx,esi ; pop esi ; jmp ~CBaseObject
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMESSAGEEVENT
#include "engine/CMessageEvent.h"

namespace {
// Fidelity: scaffolding, not retail -- the qualified explicit destructor call makes VC7.1 emit the implicit ~CMessageEvent COMDAT.
void EmitMessageEventDestructor(CMessageEvent* event)
{
    event->CMessageEvent::~CMessageEvent();
}
}
