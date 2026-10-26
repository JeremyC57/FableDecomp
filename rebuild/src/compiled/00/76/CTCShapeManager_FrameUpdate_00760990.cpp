// CTCShapeManager::FrameUpdate() -- find the persistent closed shape the hero stands in (inside the closed-shape bounds first) and select it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760990, 219 bytes (plain `ret`).
// Callees: CTCBase::GetWorld @ 0x00686A70, CWorld::GetCurrentPlayer @ 0x0049D9D0 (PlayerManager +0xc -> GetMainPlayer),
// CPlayer::GetControlledCreature @ 0x00487DD0, CThing::GetPosition() const @ 0x004C73D0 (returns const C3DVector&);
// NShape::CShape virtual slot 1 GetShapeType (+0x4: 0 for CClosedShapeBase 0x00765040, 1 for CTrack 0x007650C0; the name
// of the landed EditUpdateDelete 0x00762D80) and slot 4 IsPosInShape(const C3DVector&) (+0x10, CBasicShape 0x007611C0).
// Layout (retail; the PDB's 16-byte vector is 12 here, as the landed GetPrincipalVector 0x00760910): PersistantShapes
// +0x0c/+0x10, SelectedShape +0x18, Activated +0x1c, NewShapeActivated +0x1d, LeavingShape +0x1e, ClosedShapeBounds
// (C2DBoxF) +0x20; CThing's dead flag is bit 0 of the byte at +0x91.
// Shape: the position is copied into a frame C3DVector (dword copies interleaved with the first compare); the shape loop
// re-reads _M_finish after the virtual calls; the hit and the miss each store the iterator and the Activated byte.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; };
class C2DBoxF { public: C2DVector TopLeft; C2DVector BottomRight; };
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                              // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CPlayer { public:
    CThing* GetControlledCreature();                    // 0x00487DD0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer();                        // 0x0049D9D0
};
namespace NShape {
class CShape { public:
    virtual void v00();
    virtual long GetShapeType() const;                  // slot 1
    virtual void v02();
    virtual void v03();
    virtual bool IsPosInShape(const C3DVector& pos);    // slot 4
};
}
class CCPPointerInfo;
struct CShapePointer {                                  // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                                 // 0x00686A70
};
class CTCShapeManager : public CTCBase { public:
    CShapePointer* PersistantShapes_first;              // +0x0c
    CShapePointer* PersistantShapes_last;               // +0x10
    CShapePointer* PersistantShapes_end;                // +0x14
    CShapePointer* SelectedShape;                       // +0x18
    bool           Activated;                           // +0x1c
    bool           NewShapeActivated;                   // +0x1d
    bool           LeavingShape;                        // +0x1e
    unsigned char  _pad1f;
    C2DBoxF        ClosedShapeBounds;                   // +0x20
    void FrameUpdate();
};

void CTCShapeManager::FrameUpdate()
{
    NewShapeActivated = false;
    LeavingShape = false;
    CThing* hero = GetWorld()->GetCurrentPlayer()->GetControlledCreature();
    if (hero && !(hero->Flags91 & 1)) {
        C3DVector pos = hero->GetPosition();
        if (pos.X >= ClosedShapeBounds.TopLeft.X && pos.X < ClosedShapeBounds.BottomRight.X &&
            pos.Y >= ClosedShapeBounds.TopLeft.Y && pos.Y < ClosedShapeBounds.BottomRight.Y) {
            for (CShapePointer* it = PersistantShapes_first; it != PersistantShapes_last; ++it) {
                NShape::CShape* shape = it->Data;
                if (!shape->GetShapeType() && shape->IsPosInShape(pos)) {
                    SelectedShape = it;
                    Activated = true;
                    return;
                }
            }
        }
    }
    SelectedShape = PersistantShapes_last;
    Activated = false;
}