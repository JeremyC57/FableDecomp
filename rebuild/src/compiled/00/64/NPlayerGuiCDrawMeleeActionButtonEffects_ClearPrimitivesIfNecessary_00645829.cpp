#pragma optimize("s", on)
// NPlayerGui::CDrawMeleeActionButtonEffects::ClearPrimitivesIfNecessary() -- when not drawing, drop the melee-ability definition pointer; then the base clear (tail call)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645829, length 42 bytes.
// Callees: the def's virtual slot 1 (Destroy, when its count at +4 reaches zero), NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary @ 0x00644627 (sibling
// jump with `this`; manifest "CSystemManager::UninitWindow" is propagated). Size TU (`and dword ptr [esi+44h],0`). Shape: the inlined CDefPointer
// release (test, `dec [ecx+4] ; jne`, `call [eax+4]`, null) plus the unconditional null of the assignment from a default (null) def pointer -- the
// landed counted-pointer Unload shape, written out (the size TU does not inline a CDefPointer temporary's functions). PDB CDrawMeleeActionButtonEffects: Drawing (CDrawBase) +0x18, PMeleeAbilityDef +0x4c -> retail +0x44.
class CMeleeCombatAbilityDef { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

namespace NPlayerGui {

class CDrawBase { public:
    virtual void v00();
    unsigned char _pad04[0x14];                    // +0x04
    bool          Drawing;                         // +0x18
    unsigned char _pad19[0x2b];                    // +0x19 .. +0x44
    void ClearPrimitivesIfNecessary();             // 0x00644627
};

class CDrawMeleeActionButtonEffects : public CDrawBase { public:
    CMeleeCombatAbilityDef* PMeleeAbilityDef;      // +0x44 CDefPointer<const CMeleeCombatAbilityDef>::m_p
    void ClearPrimitivesIfNecessary();
};

void CDrawMeleeActionButtonEffects::ClearPrimitivesIfNecessary()
{
    if (!Drawing) {
        if (PMeleeAbilityDef) {                   // CDefPointer::Release, then the assignment's own null store
            PMeleeAbilityDef->Release();
            PMeleeAbilityDef = 0;
        }
        PMeleeAbilityDef = 0;
    }
    CDrawBase::ClearPrimitivesIfNecessary();
}

}