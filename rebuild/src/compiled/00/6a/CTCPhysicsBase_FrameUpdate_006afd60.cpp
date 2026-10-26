// CTCPhysicsBase::FrameUpdate() -- per-frame physics bookkeeping: while the thing is (or was) moving, refresh the parent's graphic and run the moved hook; when physics is not active, finish a pending move and clear Moving; finally set the graphic appearance's persistence flags (2 while moving, 0 when still)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFD60, 122 bytes. CTCPhysicsBase vtable slot 9 (FrameUpdate).
// Callees: CTCPhysicsBase virtual slots 37 (+0x94, IsPhysicsEnabled: base 0x006B10C0 returns true, the CTCPhysicsStandard override 0x00724BA0 reads PhysicsEnabled), 87 (+0x15c, base empty) and 94 (+0x178, base
//   empty); CThing::<graphic refresh>(bool) @ 0x004C7590 (`mov ecx,[ecx+64h] ; test ; jmp 0x004BF9E0`; catalog
//   "CRenderTarget::DetachTarget" is propagated); CTCGraphicAppearance::SetActivePersistenceFlags(long) @ 0x004BFFD0 on the
//   parent's graphic appearance component (+0x64).
// Layout: engine/CTCPhysicsBase.h: +0x3c bit 0 Moving, bit 1 OldMoving. CThing +0x64 = PGraphicAppearance (retail offset).
// Shape: `Moving || OldMoving` on the two bit fields folds into `test al,3`; each persistence call arm keeps its own `pop ; ret`.
struct CTCGraphicAppearance {
    void SetActivePersistenceFlags(long flags); // 0x004BFFD0
};
class CThing { public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    void RefreshGraphic(bool flag);             // 0x004C7590
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008();
    virtual void FrameUpdate();   // slot 9 (+0x24)
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036();
    virtual bool IsPhysicsEnabled();   // slot 37 (+0x94)
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086();
    virtual void OnMoved();   // slot 87 (+0x15c)
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void OnMoveFinished();   // slot 94 (+0x178)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x34];
    unsigned char Moving : 1;                   // +0x3c bit 0
    unsigned char OldMoving : 1;                // +0x3c bit 1
};

void CTCPhysicsBase::FrameUpdate()
{
    if (IsPhysicsEnabled()) {
        if (Moving || OldMoving) {
            ParentThing->RefreshGraphic(true);
            OnMoveFinished();
        }
        if (Moving)
            OnMoved();
    } else if (Moving || OldMoving) {
        ParentThing->RefreshGraphic(true);
        OnMoveFinished();
        Moving = false;
    }
    CTCGraphicAppearance* graphic = ParentThing->PGraphicAppearance;
    if (graphic) {
        if (!(Moving || OldMoving))
            graphic->SetActivePersistenceFlags(0);
        else
            graphic->SetActivePersistenceFlags(2);
    }
}