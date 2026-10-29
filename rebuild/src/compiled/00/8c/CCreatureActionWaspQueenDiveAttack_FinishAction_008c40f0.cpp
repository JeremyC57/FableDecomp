// CCreatureAction_WaspQueenDiveAttack::FinishAction() -- end of the wasp queen's dive: remove her hit-on-collision component; if the action ran its full time, turn her round half a turn in the XY plane and reset the physics interpolation; then the combat-action FinishAction
// Leaf: ?FinishAction@CCreatureAction_WaspQueenDiveAttack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C40F0, 119 bytes, tail jump to the base FinishAction.
// Identity: Ego_r.pdb `?FinishAction@CCreatureAction_WaspQueenDiveAttack@@UAEXXZ`; the catalog label is right.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, four times), CThing::RemoveTC(ETCInterfaceType)
//   0x004C9840 (Ego_r.pdb `?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z`, the only RemoveTC; the landed 0x008C4020 spells it
//   with `long`), CTCHitOnCollision is interface type 200 (the DiveAttack Initialise 0x008C4250 adds it), the creature's
//   physics component (CThing PhysicsTC +0x60) virtual slots 63 GetFacingAngleXY() (+0xFC), 66 SetFacingAngleXY(float)
//   (+0x108) and 80 ResetInterpolation() (+0x140), CCombatActionBase::FinishAction 0x00859A20 (Ego_r.pdb protected MAEXXZ,
//   tail jump). Float 0.5f 0x0122F59C.
// Slots (CTCPhysicsBase vtable 0x0125D2C4 and CTCPhysicsStandard 0x01265F84): slot 63 of CTCPhysicsStandard (0x00723B30) is
//   atan2(Forward.X, Forward.Y) scaled to turns (the XY facing angle); slot 66 (0x00723760) rebuilds the facing from (its
//   argument, slot 64, slot 65) -- SetFacingAngleXY; slot 67 (0x007237E0) from (slot 63, argument, slot 65) -- SetFacingAngleYZ;
//   slot 80 (0x006AF790) copies the position into the previous position (ResetInterpolation; Ego_r.pdb lists all these as
//   UBEMXZ / UAEXM@Z / UAEXXZ members of CTCPhysicsBase).
// Layout: CCreatureActionBase (Ego_r.pdb, no containers before them): PCreature +0x08, TotalTime +0x10, FramesRunning +0x1c.
// Shape: the TC type is pushed before the creature pointer is fetched; the facing angle is a named local (spilled across
//   the setter's GetPItem; one expression evaluates the setter's object first); the component comes through the inline
//   CThing::GetPhysicsTC() (straight into ecx; the raw member goes through eax).
enum ETCInterfaceType { };
class CCharString;
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
namespace NTCMessages { class CMessageBase; }
class C3DVector { public:
    float X, Y, Z;
};
class CRightHandedSet { public:
    C3DVector Up, Forward;
};
class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:     // retail vtable: 28 slots (editor-only virtuals absent)
    virtual ~CTCBase();                                                          // 0  (0x00686830)
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);            // 1
public:
    virtual void OnRemove();                                                     // 2
    virtual void ConstructFromParams(const CTCParamsBase& params);               // 3
    virtual void OnCreate();                                                     // 4
    virtual void InitialActivate();                                              // 5
    virtual void OnInitialActivate();                                            // 6
    virtual void RenderUpdate(float elapsed);                                    // 7
    virtual bool HasFrameUpdate() const;                                         // 8
    virtual void FrameUpdate();                                                  // 9
    virtual void BulletTimeFrameUpdate();                                        // 10
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 11
    virtual void Copy(const CTCBase& other);                                     // 12
    virtual void OnDie();                                                        // 13
    virtual void OnKill();                                                       // 14
    virtual void OnLevelUnload();                                                // 15
    virtual void OnSetInLimbo(bool inLimbo);                                     // 16
    virtual bool Validate(CCharString& error) const;                             // 17
    virtual bool IsFreeForDeletion() const;                                      // 18
    virtual bool IsSerialiseCreatable() const;                                   // 19
    virtual bool IsSerialisableForHeroState() const;                             // 20
    virtual ETCInterfaceType GetInterfaceType() const = 0;                       // 21
    virtual CCharString GetClassTypeName() const = 0;                            // 22
    virtual bool IsComponentSerialisable() const;                                // 23
    virtual bool IsDriver() const;                                               // 24
    virtual bool IsCopyable() const;                                             // 25
    virtual void GlobalUpdate(bool force);                                       // 26
    virtual void HandleMessage(const NTCMessages::CMessageBase& message);        // 27
    CThing* ParentThing;                                                         // +0x04
    bool    ToBeRemoved;                                                         // +0x08
    bool    IsInGlobalUpdateTCs;                                                 // +0x09
};
class CTCPhysicsBase : public CTCBase { public:            // vtable 0x0125D2C4
    // slots 28-62 are not called here; no PDB-proven names in slot order (placeholders, NOTES.md "Descriptive")
    virtual void CUnprovenSlot28_0125d2c4();
    virtual void CUnprovenSlot29_0125d2c4();
    virtual void CUnprovenSlot30_0125d2c4();
    virtual void CUnprovenSlot31_0125d2c4();
    virtual void CUnprovenSlot32_0125d2c4();
    virtual void CUnprovenSlot33_0125d2c4();
    virtual void CUnprovenSlot34_0125d2c4();
    virtual void CUnprovenSlot35_0125d2c4();
    virtual void CUnprovenSlot36_0125d2c4();
    virtual void CUnprovenSlot37_0125d2c4();
    virtual void CUnprovenSlot38_0125d2c4();
    virtual void CUnprovenSlot39_0125d2c4();
    virtual void CUnprovenSlot40_0125d2c4();
    virtual void CUnprovenSlot41_0125d2c4();
    virtual void CUnprovenSlot42_0125d2c4();
    virtual void CUnprovenSlot43_0125d2c4();
    virtual void CUnprovenSlot44_0125d2c4();
    virtual void CUnprovenSlot45_0125d2c4();
    virtual void CUnprovenSlot46_0125d2c4();
    virtual void CUnprovenSlot47_0125d2c4();
    virtual void CUnprovenSlot48_0125d2c4();
    virtual void CUnprovenSlot49_0125d2c4();
    virtual void CUnprovenSlot50_0125d2c4();
    virtual void CUnprovenSlot51_0125d2c4();
    virtual void CUnprovenSlot52_0125d2c4();
    virtual void CUnprovenSlot53_0125d2c4();
    virtual void CUnprovenSlot54_0125d2c4();
    virtual void CUnprovenSlot55_0125d2c4();
    virtual void CUnprovenSlot56_0125d2c4();
    virtual void CUnprovenSlot57_0125d2c4();
    virtual void CUnprovenSlot58_0125d2c4();
    virtual void CUnprovenSlot59_0125d2c4();
    virtual void CUnprovenSlot60_0125d2c4();
    virtual void CUnprovenSlot61_0125d2c4();
    virtual void CUnprovenSlot62_0125d2c4();
    virtual float GetFacingAngleXY() const;                                      // 63 (+0xFC)
    virtual float GetFacingAngleYZ() const;                                      // 64 (+0x100)
    virtual float GetFacingAngleXZ() const;                                      // 65 (+0x104)
    virtual void SetFacingAngleXY(float angle);                                  // 66 (+0x108)
    virtual void SetFacingAngleYZ(float angle);                                  // 67 (+0x10C)
    virtual void CUnprovenSlot68_0125d2c4();                                     // 68
    // overloads sit in reverse declaration order: () -> slot 70 (0x006B1410), (float) -> slot 69 (0x006B1430)
    virtual C3DVector GetFacingVector() const;                                   // 70 (+0x118)
    virtual C3DVector GetFacingVector(float time) const;                         // 69 (+0x114)
    virtual CRightHandedSet GetRHSet(float time) const;                          // 71 (+0x11C, 0x006B1450)
    virtual const CRightHandedSet& GetLastRHSet() const;                         // 72 (+0x120, 0x006B1290)
    virtual void CUnprovenSlot73_0125d2c4(); virtual void CUnprovenSlot74_0125d2c4(); virtual void CUnprovenSlot75_0125d2c4();
    virtual void CUnprovenSlot76_0125d2c4(); virtual void CUnprovenSlot77_0125d2c4(); virtual void CUnprovenSlot78_0125d2c4();
    virtual void CUnprovenSlot79_0125d2c4();
    virtual void ResetInterpolation();                                           // 80 (+0x140, 0x006AF790)
};

class CThing { public:                                      // partial view, retail offsets
    virtual ~CThing();
    unsigned char      _pad_0x04[0x60 - 0x04];
    CTCPhysicsBase*    PhysicsTC;                           // +0x60
    void RemoveTC(ETCInterfaceType type);                   // 0x004C9840
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }    // Ego_r.pdb ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ (inline)
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
    long                                     TotalTime;     // +0x10
    long                                     ActionTime;    // +0x14
    long                                     AnimDelayTime; // +0x18
    long                                     FramesRunning; // +0x1c
};
class CCombatActionBase : public CCreatureActionBase { public:
protected:
    virtual void FinishAction();                            // 0x00859A20
};
class CCreatureAction_WaspQueenDiveAttack : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_WaspQueenDiveAttack::FinishAction()
{
    PCreature->RemoveTC((ETCInterfaceType)200);
    if (FramesRunning >= TotalTime) {
        float angle = PCreature->GetPhysicsTC()->GetFacingAngleXY();
        PCreature->GetPhysicsTC()->SetFacingAngleXY(angle + 0.5f);
        PCreature->GetPhysicsTC()->ResetInterpolation();
    }
    CCombatActionBase::FinishAction();
}