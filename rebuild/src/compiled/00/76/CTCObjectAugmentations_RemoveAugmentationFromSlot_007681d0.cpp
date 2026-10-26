// CTCObjectAugmentations::RemoveAugmentationFromSlot(long) -- releases the slot's def pointer, then re-attaches the particles and sets the augmentation sounds up again
// Leaf: ?RemoveAugmentationFromSlot@CTCObjectAugmentations@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007681D0, length 61 bytes.
// Callees: the def's virtual slot 1 (destroy on the last reference), CTCObjectAugmentations::AttachAugmentationParticles()
// @ 0x007677E0, CTCObjectAugmentations::SetupAugmentationSounds() @ 0x00766C00 (both private in Ego_r/FableWin; the manifest's
// "CTCObjectAugmentations_UpdateParticleMorphs" is a forge autoname).
// Retail CTCObjectAugmentations: Augmentations (std::vector<CDefPointer<const CThingBaseDef> >) first pointer at +0x14.
// Shape: the slot address in edi; CDefPointer `operator=(0)` = the counted release through a LOCAL copy (`dec dword ptr
// [ecx+4] ; jne ; call [edx+4]`) with the null store inside the test, then a second unconditional null store.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
};

struct CThingBaseDefPointer {                       // CDefPointer<const CThingBaseDef>
    CDefBase* m_pDef;
    void Release()
    {
        if (m_pDef) {
            CDefBase* def = m_pDef;
            if (--def->m_refCount == 0)
                def->Destroy();
            m_pDef = 0;
        }
    }
    CThingBaseDefPointer& operator=(int) { Release(); m_pDef = 0; return *this; }
};

struct CTCBase_007681d0 {
    virtual void v00();
    unsigned char _pad04[8];
};

class CTCObjectAugmentations : public CTCBase_007681d0 { public:
    const void*           PDef;                     // +0x0c
    bool                  LoadedFromSaveGame;       // +0x10
    CThingBaseDefPointer* m_pAugmentationsFirst;    // +0x14 Augmentations (vector)
    CThingBaseDefPointer* m_pAugmentationsLast;     // +0x18
    CThingBaseDefPointer* m_pAugmentationsEnd;      // +0x1c
    void RemoveAugmentationFromSlot(long slot);
private:
    void AttachAugmentationParticles();             // 0x007677E0
    void SetupAugmentationSounds();                 // 0x00766C00
};

void CTCObjectAugmentations::RemoveAugmentationFromSlot(long slot)
{
    m_pAugmentationsFirst[slot] = 0;
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}