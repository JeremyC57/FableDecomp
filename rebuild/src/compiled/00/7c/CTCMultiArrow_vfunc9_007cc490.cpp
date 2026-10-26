// CTCMultiArrow::vfunc_9() -- per-frame update: while volleys remain, count the effect timer down and create the next ammo effect; once spent and out of shots, remove the TC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC490, length 51 bytes.
// Callees: CTCMultiArrow::CreateAmmoEffect() @ 0x007CC2B0 (tail jump), CThing::RemoveTC(long type) @ 0x004C9840 (type 0x88 =
// this TC; manifest label "CBaseObject::OnPropertyChanged" is propagated).
// Retail CTCMultiArrow (PDB): NumberOfShots +0x10, CreateAmmoEffectCount +0x24, CreateAmmoEffectDelay +0x28,
// CreateAmmoEffectTimer +0x2c, CutSceneCancelled +0x44.
// Shape: the timer decrement folds into `dec dword ptr [ecx+2Ch] ; jne`, the reload from the delay precedes the tail jump.
class CThing { public:
    void RemoveTC(long type);                       // 0x004C9840
};

struct CTCBase_007cc490 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCMultiArrow : public CTCBase_007cc490 { public:
    const void*   PDef;                             // +0x0c
    long          NumberOfShots;                    // +0x10
    long          NumberInVolley;                   // +0x14
    long          NumOfVolleys;                     // +0x18
    long          SpellLevel;                       // +0x1c
    bool          RemovedEffect;                    // +0x20
    long          CreateAmmoEffectCount;            // +0x24
    long          CreateAmmoEffectDelay;            // +0x28
    long          CreateAmmoEffectTimer;            // +0x2c
    unsigned char AmmoEffects[0x0c];                // +0x30 std::vector<CIntelligentPointer<CThing> >
    unsigned char BowEffect[8];                     // +0x3c CIntelligentPointer<CThing>
    bool          CutSceneCancelled;                // +0x44
    void CreateAmmoEffect();                        // 0x007CC2B0
    virtual void vfunc_9();
};

void CTCMultiArrow::vfunc_9()
{
    if (CreateAmmoEffectCount > 0) {
        if (!CutSceneCancelled) {
            if (--CreateAmmoEffectTimer == 0) {
                CreateAmmoEffectTimer = CreateAmmoEffectDelay;
                CreateAmmoEffect();
            }
        }
    } else if (NumberOfShots <= 0) {
        ParentThing->RemoveTC(0x88);
    }
}