// CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007449B0, oracle length 36 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0Ch] ; call CBaseIntelligentPointer::GetPItem ; test eax,eax ; je R ; mov byte ptr [esi+30h],1 ; mov dword ptr [esi+1Ch],0 ; mov ecx,esi ; pop esi ; jmp <on directly aware> ; R: pop esi ; ret
// engine/CTCPerceiveThing.h (PDB): +0x0C CIntelligentPointer<CThing const> PerceivedThing, +0x1C
// FramesSinceDirectlyPerceivedThing, +0x30 DirectAwareness. If the perceived thing still exists,
// flag direct awareness, reset the staleness counter and tail-call the reaction at 0x00744730.
#include "engine/CTCPerceiveThing.h"

struct CThing;
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ecx = the CIntelligentPointer object).
CThing* __fastcall IntelligentPointer_GetPItem_00a01b50(void* intelligentPointer);

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    void OnDirectlyAwareOfPerceivedThing();     // 0x00744730
    void SetAsDirectlyAwareOfPerceivedThing();
};

void CTCPerceiveThing_Methods::SetAsDirectlyAwareOfPerceivedThing()
{
    if (IntelligentPointer_GetPItem_00a01b50(&this->PerceivedThing[0])) {
        this->DirectAwareness = true;
        this->FramesSinceDirectlyPerceivedThing = 0;
        OnDirectlyAwareOfPerceivedThing();
    }
}