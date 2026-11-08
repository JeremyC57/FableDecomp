#ifndef FABLE_ENGINE_CANIMCOMPONENTSTRIKESPEEDMULTIPLIER_H
#define FABLE_ENGINE_CANIMCOMPONENTSTRIKESPEEDMULTIPLIER_H
// Retail opt-in (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): RTTI CAnimComponentStrikeSpeedMultiplier : CAnimComponentBase, vtable 0x01255800;
// retail sizeof 0xC (`push Ch ; call operator new` in StatRegisterClass; Ego_r.pdb sizeof 12: the animation components are
// byte-packed, CAnimComponentCombatMisc 10 and CAnimComponentCombatFlourishTargetOffset 17 bytes). The constructor is inline in retail
// (FableWin.pdb ??0CAnimComponentStrikeSpeedMultiplier@@QAE@XZ): IVRefCount = 1, then this vtable. FableWin.pdb
// ?StatGetTokenName@CAnimComponentStrikeSpeedMultiplier@@SI?AVCCharString@@XZ ("StrikeSpeedMultiplier", the token RegisteredComponentsByToken keys it by) and
// ?StatGetType@CAnimComponentStrikeSpeedMultiplier@@SI?AW4EAnimComponent@@XZ (ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER = 9, the RegisteredComponentDefaults key) are static __fastcall, inline in retail.
// The overrides are declared only (Ego_r.pdb / FableWin.pdb ?GetType / ?Initialise / ?Alloc / ?Clone / ?TransferBinaryIn / ?TransferBinaryOut /
// ?SetDefaultValue@CAnimComponentStrikeSpeedMultiplier@@...).
#include "rebuild_abi.h"
#include "engine/CAnimComponentBase.h"
#pragma pack(push, 1)
class CAnimComponentStrikeSpeedMultiplier : public CAnimComponentBase {
public:
    virtual EAnimComponent GetType() const;
    virtual bool Initialise(CPersistContext& context, const CCharString& token);
    virtual CAnimComponentBase* Alloc() const;
    virtual CAnimComponentBase* Clone() const;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    virtual void SetDefaultValue();
    static CCharString __fastcall StatGetTokenName() { return CCharString("StrikeSpeedMultiplier"); }
    static EAnimComponent __fastcall StatGetType() { return ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER; }
    float AnimationSpeedMultiplier;               // +0x08
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CAnimComponentStrikeSpeedMultiplier) == 0xc);
#endif // FABLE_ENGINE_CANIMCOMPONENTSTRIKESPEEDMULTIPLIER_H
