// CTCActivationReceptorBase::Deactivate() -- clear the receptor's Activated flag (+0x0C)
// Leaf: ?Deactivate@CTCActivationReceptorBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F7700 (`mov byte ptr [ecx+0Ch],0` / `ret`); slot 29 of every receptor
// vtable (base of CTCActivationReceptorDoor / Chest); Ego_r.pdb ?Deactivate@CTCActivationReceptorBase@@UAEXXZ.
// Layout: Ego_r.pdb CTCActivationReceptorBase (Activated +0x0C). engine/CTCActivationReceptorBase.h has no virtuals, so the
// polymorphic class is modelled locally. Re-modelled in naming wave 15 (was the stand-in `C007f7700::M`, leaf
// `??0CXMVPlaybackInfo@`).
class CTCActivationReceptorBase { public:
    virtual void Deactivate();                  // slot 29
    unsigned char _pad04[0x08];                 // +0x04 CTCBase body
    bool          Activated;                    // +0x0c
};

void CTCActivationReceptorBase::Deactivate()
{
    Activated = false;
}
