// Enable2DSceneRenderingOnRejectingObject(CRenderTarget*, bool) -- asks the object (virtual slot 1) whether it accepts, then calls the setter @ 0x00C445C0 on the object when it does NOT, else on null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35F20, length 36 bytes. The chunk label "CDisplayViewManager::SetView" is propagated: the body is a
// free two-argument `__stdcall` function (`ret 8`, ecx never read) in the 0x00C3xxxx module, the negated twin of 0x00C34050.
// Callees: the object's virtual slot 1 (bool), the setter @ 0x00C445C0 (manifest "CEngine::Enable2DSceneRendering(bool)").
// Shape: the bool is widened to a mask and INVERTED (`neg al ; sbb eax,eax ; not eax ; and eax,esi`): the conditional is `accepts ? 0 : object`.
struct CRenderTarget {
    virtual void v00();
    virtual bool Accepts();                        // slot 1
    void Enable2DSceneRendering(bool enable);      // 0x00C445C0
};

void __stdcall Enable2DSceneRenderingOnRejectingObject(CRenderTarget* object, bool enable)
{
    CRenderTarget* target = object->Accepts() ? 0 : object;
    target->Enable2DSceneRendering(enable);
}