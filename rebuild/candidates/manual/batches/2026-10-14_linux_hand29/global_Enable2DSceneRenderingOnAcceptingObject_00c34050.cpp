// Enable2DSceneRenderingOnAcceptingObject(CRenderTarget*, bool) -- asks the object (virtual slot 1) whether it accepts, then calls the setter @ 0x00C41440 on the object or on null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34050, length 34 bytes. The chunk label "CDisplayViewManager::SetView" is propagated: the body is a
// free two-argument `__stdcall` function (`ret 8`, ecx never read) in the 0x00C3xxxx module, the sibling of the landed SetRangeOnAcceptingObject
// @ 0x00C33F20. Callees: the object's virtual slot 1 (bool), the setter @ 0x00C41440 (manifest "CEngine::Enable2DSceneRendering(bool)").
// Shape: the bool is widened to a mask (`neg al ; sbb eax,eax ; and eax,esi`) and the setter's `this` is the masked pointer -- a conditional
// (`accepts ? object : 0`) used as the call target; the argument is pushed before the mask is formed.
struct CRenderTarget {
    virtual void v00();
    virtual bool Accepts();                        // slot 1
    void Enable2DSceneRendering(bool enable);      // 0x00C41440
};

void __stdcall Enable2DSceneRenderingOnAcceptingObject(CRenderTarget* object, bool enable)
{
    CRenderTarget* target = object->Accepts() ? object : 0;
    target->Enable2DSceneRendering(enable);
}
