// CCreatureAction_BalverineDropInFromSky::FrameUpdate() -- the balverine dropping in from the sky fades in: for the first half of the action its alpha rises from 0 to 255, after that it is fully opaque; then the combat-action FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_BalverineDropInFromSky@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC820, 114 bytes, tail jump to the base FrameUpdate.
// Identity: Ego_r.pdb `?FrameUpdate@CCreatureAction_BalverineDropInFromSky@@UAEXXZ`; the catalog label is right. Twin of the
//   CorneredJumpIntoSky 0x008CC7A0 (fade out over the second half).
// Callees: CCreatureActionBase::GetPercentageComplete() const 0x006925E0 (Ego_r.pdb `?GetPercentageComplete@CCreatureActionBase@@QBEMXZ`:
//   (FramesRunning - StartFrame - anim delay) / (TotalTime - StartFrame - anim delay); reloc_bindings carries invented
//   GetProgressRatio / GetProgress / GetAnimationFraction spellings), CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8),
//   NGraphicAppearance::CTCGraphicAppearance::SetAlpha(unsigned char) 0x004BFAC0 (Ego_r.pdb `QAEXE@Z`), _ftol2 0x00BFEA70,
//   CCombatActionBase::FrameUpdate 0x00859D80 (Ego_r.pdb UAEXXZ, tail jump). Floats 0.5f 0x0122F59C, 255.0f 0x01230014.
// Layout: CThing PTCGraphicAppearance +0x64 through the inline CThing::GetTCGraphicAppearance() (Ego_r.pdb QAE).
// Shape: `< 0.5f` (`test ah,5 ; jp`); the alpha fraction `p * 2.0f` is a named local computed before the creature pointer is
//   fetched (`fadd st,st`), scaled by 255 and truncated after it.
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);                     // 0x004BFAC0
};
}
class CThing { public:                                      // partial view, retail offsets
    virtual ~CThing();
    unsigned char                               _pad_0x04[0x64 - 0x04];
    NGraphicAppearance::CTCGraphicAppearance*   PTCGraphicAppearance;   // +0x64
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }
};
class CThingPhysical : public CThing { public: };
class CThingGameObject : public CThingPhysical { public: };
class CThingCreatureBase : public CThingGameObject { public: };

class CBaseObjectPointer;
class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char                            _pad_0x04[0x04];
    CIntelligentPointer<CThingCreatureBase>  PCreature;     // +0x08
    float GetPercentageComplete() const;                    // 0x006925E0
};
class CCombatActionBase : public CCreatureActionBase { public:
    virtual void FrameUpdate();                             // 0x00859D80
};
class CCreatureAction_BalverineDropInFromSky : public CCombatActionBase { public:
    virtual void FrameUpdate();
};

void CCreatureAction_BalverineDropInFromSky::FrameUpdate()
{
    if (GetPercentageComplete() < 0.5f) {
        float alpha = GetPercentageComplete() * 2.0f;
        PCreature->GetTCGraphicAppearance()->SetAlpha((unsigned char)(long)(alpha * 255.0f));
    } else {
        PCreature->GetTCGraphicAppearance()->SetAlpha(255);
    }
    CCombatActionBase::FrameUpdate();
}