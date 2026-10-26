// Behaviour test for CTCShapeManager::FrameUpdate (0x00760990): the hero inside the bounds selects the first closed shape
// containing it; open shapes, shapes not containing the hero, a dead hero, no hero or out-of-bounds select end().
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; };
class C2DBoxF { public: C2DVector TopLeft; C2DVector BottomRight; };
class CThing { public:
    unsigned char Flags91;
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
class CPlayer { public:
    CThing* Creature;
    CThing* GetControlledCreature() { return Creature; }
};
class CWorld { public:
    CPlayer* Player;
    CPlayer* GetCurrentPlayer() { return Player; }
};
namespace NShape {
class CShape { public:
    long Open; float MinX, MaxX; int Asked;
    virtual void v00() { }
    virtual long GetShapeType() const { return Open; }
    virtual void v02() { }
    virtual void v03() { }
    virtual bool IsPosInShape(const C3DVector& pos) { ++Asked; return pos.X >= MinX && pos.X <= MaxX; }
};
}
class CCPPointerInfo;
struct CShapePointer {
    NShape::CShape* Data;
    CCPPointerInfo* Info;
};
class CTCShapeManager { public:
    virtual void v00() { }
    CThing*        ParentThing;
    bool           ToBeRemoved;
    bool           IsInGlobalUpdateTCs;
    unsigned char  _pad0a[2];
    CShapePointer* PersistantShapes_first;
    CShapePointer* PersistantShapes_last;
    CShapePointer* PersistantShapes_end;
    CShapePointer* SelectedShape;
    bool           Activated;
    bool           NewShapeActivated;
    bool           LeavingShape;
    unsigned char  _pad1f;
    C2DBoxF        ClosedShapeBounds;
    CWorld* World;
    CWorld* GetWorld() { return World; }
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

static void Shape(NShape::CShape& s, long open, float lo, float hi) { s.Open = open; s.MinX = lo; s.MaxX = hi; s.Asked = 0; }

int main()
{
    int bad = 0;
    NShape::CShape a, b, c;
    Shape(a, 0, 0.0f, 1.0f); Shape(b, 1, 0.0f, 10.0f); Shape(c, 0, 4.0f, 6.0f);
    CShapePointer v[3]; v[0].Data = &a; v[1].Data = &b; v[2].Data = &c;
    CThing hero; hero.Flags91 = 0; hero.Pos.X = 5.0f; hero.Pos.Y = 5.0f; hero.Pos.Z = 0.0f;
    CPlayer player; player.Creature = &hero;
    CWorld world; world.Player = &player;
    CTCShapeManager m; m.World = &world;
    m.PersistantShapes_first = v; m.PersistantShapes_last = v + 3; m.PersistantShapes_end = v + 3;
    m.ClosedShapeBounds.TopLeft.X = 0.0f; m.ClosedShapeBounds.TopLeft.Y = 0.0f;
    m.ClosedShapeBounds.BottomRight.X = 10.0f; m.ClosedShapeBounds.BottomRight.Y = 10.0f;
    m.NewShapeActivated = true; m.LeavingShape = true; m.Activated = false; m.SelectedShape = 0;
    m.FrameUpdate();
    if (m.SelectedShape != v + 2 || !m.Activated || m.NewShapeActivated || m.LeavingShape) bad |= 1;
    if (a.Asked != 1 || b.Asked != 0 || c.Asked != 1) bad |= 2;        // the open shape is never asked
    hero.Pos.X = 10.0f; m.FrameUpdate();                                 // on the right edge: outside
    if (m.SelectedShape != v + 3 || m.Activated) bad |= 4;
    hero.Pos.X = 5.0f; hero.Pos.Y = -1.0f; m.FrameUpdate();
    if (m.SelectedShape != v + 3 || m.Activated) bad |= 8;
    hero.Pos.Y = 0.0f; m.FrameUpdate();                                  // on the top edge: inside
    if (m.SelectedShape != v + 2 || !m.Activated) bad |= 16;
    hero.Flags91 = 1; m.FrameUpdate();
    if (m.SelectedShape != v + 3 || m.Activated) bad |= 32;
    hero.Flags91 = 0; player.Creature = 0; m.FrameUpdate();
    if (m.SelectedShape != v + 3 || m.Activated) bad |= 64;
    player.Creature = &hero; hero.Pos.X = 8.0f; m.FrameUpdate();          // inside the bounds, in no closed shape
    if (m.SelectedShape != v + 3 || m.Activated) bad |= 128;
    if (bad) { std::printf("00760990_TEST FAIL\n"); return 1; }
    std::printf("00760990_TEST PASS\n");
    return 0;
}