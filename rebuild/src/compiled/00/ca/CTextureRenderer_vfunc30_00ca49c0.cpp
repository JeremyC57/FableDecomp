// CTextureRenderer::vfunc30() -- DirectShow renderer flush-end shape: reset the position pass-through's media times (when one is attached), call virtual slot 28 on itself, return S_OK
// Leaf: ?vfunc30@CTextureRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA49C0, length 26 bytes (+ int3 padding). CTextureRenderer vtable
// 0x0129D08C slot 30 (also referenced from the no-RTTI vtables at 0x012BF510 / 0x012BF650).
// Callees: CRendererPosPassThru::ResetMediaTime @ 0x00CADDC0 (EnterCriticalSection, zero the two 64-bit media times
// +0x38..+0x44, m_bReset = 1 at +0x48, LeaveCriticalSection; the manifest's ctor label there is propagated), this's
// virtual slot 28 (`call [eax+70h]`, no arguments).
// Label correction: the manifest's "CThingFilter_HasNavigationInformation::operator()" is propagated -- the body takes
// no argument (`ret`), reads a pointer member at +0x50 and returns 0; the vtable slot names the class.
// Layout: local model (no engine header for CTextureRenderer): the position pass-through pointer at +0x50.
struct CRendererPosPassThru {
    long ResetMediaTime();                          // 0x00CADDC0
};

struct CTextureRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long vfunc28();                         // slot 28 (+0x70)
    virtual void v29();
    virtual long vfunc30();                         // slot 30
    unsigned char _pad04[0x4c];
    CRendererPosPassThru* m_pPosition;              // +0x50
};

long CTextureRenderer::vfunc30()
{
    if (m_pPosition)
        m_pPosition->ResetMediaTime();
    vfunc28();
    return 0;
}