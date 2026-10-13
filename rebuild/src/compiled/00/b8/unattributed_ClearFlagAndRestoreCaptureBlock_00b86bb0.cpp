// <unattributed>::ClearFlagAndRestoreCaptureBlock() -- Flag144 = false; g_RenderStateManager->RestoreCaptureBlock()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86BB0, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov byte ptr [ecx+144h],0 ; mov ecx,[013CB508h] ; jmp CRenderStateManager::RestoreCaptureBlock
// Clears the flag at +0x144 and tail-calls RestoreCaptureBlock (landed-trusted @ 0x00A05840) on the
// render-state-manager singleton at 0x013CB508.
struct CRenderStateManagerCalls {
    void RestoreCaptureBlock();             // 0x00A05840
};

extern CRenderStateManagerCalls* g_RenderStateManager;   // 0x013CB508 (.data)

struct CCaptureUser_Retail {
    unsigned char _pad_0x000[0x144];
    bool          Capturing;                // +0x144 (name from use)
    void ClearFlagAndRestoreCaptureBlock();
};

void CCaptureUser_Retail::ClearFlagAndRestoreCaptureBlock()
{
    this->Capturing = false;
    g_RenderStateManager->RestoreCaptureBlock();
}