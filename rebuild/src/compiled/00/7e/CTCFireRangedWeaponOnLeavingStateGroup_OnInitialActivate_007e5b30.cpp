// CTCFireRangedWeaponOnLeavingStateGroup::OnInitialActivate() -- suspend the owner creature's AI, with this component as the suspender
// Leaf: ?OnInitialActivate@CTCFireRangedWeaponOnLeavingStateGroup@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5B30, 12 bytes, `call ; ret`.
// Identity: CTCFireRangedWeaponOnLeavingStateGroup vtable 0x0123E464 slot 6 (RTTI
//   .?AVCTCFireRangedWeaponOnLeavingStateGroup@@). Neither PDB lists the class's methods (FableWin.pdb has only its
//   GFDebugPDynamicCast), so the name comes from the CTCBase slot.
// CTCBase virtual slots (Ego_r.pdb donor order, matched to the CTCBase vtable 0x0125BE0C): 2 OnRemove, 3 ConstructFromParams,
//   4 OnCreate, 5 InitialActivate, 6 OnInitialActivate, 7 RenderUpdate, 8 HasFrameUpdate, 9 FrameUpdate,
//   10 BulletTimeFrameUpdate, 11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill (the editor-only
//   EditFrameUpdate / EditorDrawPreparePrimitivesForRendering of the debug build are absent from retail).
// Callees: CThingAICreature::SuspendAI(bool suspend, void* suspender) @ 0x00833A10 (landed; Ego_r.pdb
//   `?SuspendAI@CThingAICreature@@QAEX_NPAX@Z`; catalog label CThingAnimatable::SetSuspendAnimation is propagated).
// Shape: `this` is pushed first as the suspender (`push ecx`), then `push 1`, the parent thing is loaded into ecx; the
//   callee's two stack arguments rule out a tail jump. The parent thing is an AI creature here (static downcast, offset 0:
//   CThingAICreature derives from CThing through single inheritance).
class CThing { public:
    void* __vftable;
};
class CThingAICreature : public CThing { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10
};

class CTCFireRangedWeaponOnLeavingStateGroup {
public:
    virtual void OnInitialActivate();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    bool    QuitOut;                            // +0x0c (Ego_r.pdb)
};

void CTCFireRangedWeaponOnLeavingStateGroup::OnInitialActivate()
{
    static_cast<CThingAICreature*>(ParentThing)->SuspendAI(true, this);
}