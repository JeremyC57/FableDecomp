// CCreatureActionBase::IsPreventingNormalMovement() const -- true unless the creature treats this action's animation as a combination anim
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692A20, length 37 bytes. Vtable slot 49 of the CCreatureActionBase family
// (rtti-vtables.tsv lists it through CCreatureAction_ThrowGameObject, 0x01259044).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), virtual GetAnimationIndex (slot 7, +0x1c; the
// base's 0x00692870), CThingCreatureBase::IsAnimCombinationAnim(long) @ 0x00662820.
// Shape: the creature is fetched into a named local (edi) BEFORE the virtual call (the argument of the member call is
// evaluated after the object expression only when the object is already a local), then `return !result` on a bool:
// `neg al ; sbb eax,eax ; inc eax`.
struct CThingCreatureBase_Calls {         // engine/CThingCreatureBase.h exists but is not used: only this call is modelled
    bool IsAnimCombinationAnim(long animationIndex);   // 0x00662820
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase_Calls* m_pItem;      // +0x04
    CThingCreatureBase_Calls* GetPItem() const;   // 0x00A01B50
};

// engine/CCreatureActionBase.h exists but is not used: it is the PDB build's flat pack(1) layout (0xb4 bytes, three
// containers 4 bytes wider than retail's 0xa8, no vtable or methods); the retail `this` is modelled locally.
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual long GetAnimationIndex() const; // slot 7 (+0x1c)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    bool IsPreventingNormalMovement() const;
};

bool CCreatureActionBase::IsPreventingNormalMovement() const
{
    CThingCreatureBase_Calls* creature = PCreature.GetPItem();
    return !creature->IsAnimCombinationAnim(GetAnimationIndex());
}