// SetRangeOnAcceptingObject(CRangedObject*, long, long) -- asks the object (virtual slot 1) whether it accepts, and calls the two-word setter @ 0x00C417A0 on the object or on null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C33F20, length 39 bytes. The chunk label ("CCreatureAction_TrollWhackGroundBase::Initialise") is propagated:
// the body is a free three-argument `__stdcall` function (`ret 0Ch`, ecx never read) in the 0x00C3xxxx module, not a creature action.
// Callees: the object's virtual slot 1 (bool result), the setter @ 0x00C417A0 (`mov [this+1D0h],a ; mov [this+1D4h],b ; or [this+1D8h],40000h`; its
// manifest label "CShaderRenderManager::SetProjectionTransform" is propagated).
// Shape: the bool is widened to a mask (`neg al ; sbb eax,eax ; and eax,esi`) and the setter's `this` is the masked pointer -- a conditional cast
// (`accepts ? object : 0`) whose result is then used as the call target; the second argument is pushed before the mask is formed.
struct CRangedObject {
    virtual void v00();
    virtual bool Accepts();                 // slot 1
    void SetRange(long first, long last);   // 0x00C417A0 (stores +0x1d0 / +0x1d4 and sets bit 18 of +0x1d8)
};

void __stdcall SetRangeOnAcceptingObject(CRangedObject* object, long first, long last)
{
    CRangedObject* target = object->Accepts() ? object : 0;
    target->SetRange(first, last);
}