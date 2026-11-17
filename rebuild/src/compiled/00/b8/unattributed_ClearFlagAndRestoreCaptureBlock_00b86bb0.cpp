// <unattributed>::ClearFlagAndRestoreCaptureBlock() -- the embedded CRenderStateCaptureBlock (+0x144) restored: Open = false; StateManager->RestoreCaptureBlock()
// Leaf: ?ClearFlagAndRestoreCaptureBlock@CCaptureUser_Retail@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86BB0, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov byte ptr [ecx+144h],0 ; mov ecx,[013CB508h] ; jmp CRenderStateManager::RestoreCaptureBlock
// Clears the flag at +0x144 and tail-calls RestoreCaptureBlock (landed-trusted @ 0x00A05840) on the
// render-state-manager singleton at 0x013CB508.
// Name (round 7, NF53): the callee is ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ (Ego_r.pdb; engine/CRenderStateManager.h) and the
//   0x013CB508 pointer is the private static ?StateManager@CRenderStateCaptureBlock@@0PAVCRenderStateManager@@A (the landed
//   ~CRenderStateManager clears it); the +0x144 byte is that block's Open flag (Ego_r.pdb CRenderStateCaptureBlock: one bool), so the
//   body is the inline CRenderStateCaptureBlock::Restore (engine/CRenderStateCaptureBlock.h, FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE +
//   _RESTORE_OPEN_FIRST: the flag store before the tail call). Was a local CRenderStateManagerCalls view and the address-named
//   g_RenderStateManager. The owner class is still unattributed (the manifest label is propagated), so its name stays descriptive.
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_RESTORE_OPEN_FIRST
#include "engine/CRenderStateCaptureBlock.h"

struct CCaptureUser_Retail {
    unsigned char            _pad_0x000[0x144];
    CRenderStateCaptureBlock CaptureBlock;      // +0x144
    void ClearFlagAndRestoreCaptureBlock();
};

void CCaptureUser_Retail::ClearFlagAndRestoreCaptureBlock()
{
    CaptureBlock.Restore();
}
