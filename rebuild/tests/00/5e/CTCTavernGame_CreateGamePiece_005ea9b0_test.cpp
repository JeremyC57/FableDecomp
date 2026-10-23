// Behaviour test for CTCTavernGame::CreateGamePiece (0x005EA9B0): the new piece is hidden, flagged, owned by the
// game's thing through both owned-entity components, has the 0x5b flag set and is deactivated.
#include <cstdio>

struct CThing;
struct COwnerA { CThing* owner; };
struct COwnerB { CThing* owner; void SetPOwner(CThing* t) { owner = t; } };
struct CTC5B { unsigned char flags; };

struct CThing {
    bool visible, active;
    unsigned char flags91;
    int tcCount;
    COwnerA a; COwnerB b; CTC5B c;
    void AddTC() { ++tcCount; }
};

struct CGame {
    CThing* ParentThing;
    CThing pieceStore;
    CThing* CreateGamePiece()
    {
        CThing* piece = &pieceStore;
        piece->visible = true; piece->active = true; piece->flags91 = 0; piece->tcCount = 0;
        piece->a.owner = 0; piece->b.owner = 0; piece->c.flags = 1;
        piece->visible = false;
        piece->flags91 |= 2;
        piece->AddTC();
        piece->a.owner = ParentThing;
        piece->AddTC();
        piece->b.SetPOwner(ParentThing);
        piece->c.flags |= 2;
        piece->active = false;
        return piece;
    }
};

int main()
{
    CThing owner;
    CGame g;
    g.ParentThing = &owner;
    CThing* p = g.CreateGamePiece();
    if (p->visible || p->active || p->flags91 != 2 || p->tcCount != 2 || p->a.owner != &owner || p->b.owner != &owner
        || p->c.flags != 3) {
        std::printf("005ea9b0_TEST FAIL\n");
        return 1;
    }
    std::printf("005ea9b0_TEST PASS\n");
    return 0;
}