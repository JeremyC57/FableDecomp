// CTCTavernGameCardSorting::EnterGame() -- base EnterGame, one card in play, create the pointer piece from the card def's model and place it at the game position raised by the pointer height
// Retail: Fable.exe @ 0x007D4E30, 153 bytes (CTCTavernGameCardSorting vtable 0x0126F5B4).
// Callees: CTCTavernGameCardBase::EnterGame() 0x008F1E30; CTCTavernGameCardBase::GetCardDef(CDefPointer*) 0x008F0B20;
//          CTCTavernGame::CreateGamePiece(long model) 0x005EA9B0 (returns CThingObject*);
//          CTCTavernGame::PositionMesh(CThing*, const C3DVector& pos, const CRightHandedSet& rh, const C3DVector& offset, long, long)
//          0x005EABC0 (__fastcall global); the def's virtual slot 1 when its count reaches 0.
// Layout (retail CTCTavernGameCardBase, 0x18..0x24 below Ego_r.pdb): GamePositionRHSet +0xf0, GamePositionPos +0x108,
//          NumCards +0x300, Pointer +0x140. Def (engine/CTavernGameCardBaseDef.h offsets hold in retail): PointerModel
//          +0x3c, PointerHeightOffset +0x48.
// Shape: NumCards = 1 precedes the def temporary (its store sits before the zeroing of the out pointer); the offset vector
//        is a 3-float temporary; the CDefPointer dtor is null-tested and calls the def's inline ReleaseTemporary().
// Leaf: EnterGame
// PositionMesh (sweep3, FINDINGS chunk 481 "PositionMesh's 5th parameter is float"): Ego_r/FableWin
//   ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z -- a static __fastcall member taking
//   (CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float); C3DVector, CRightHandedSet and CThing
//   are classes (V in every PDB signature).
class CThing { };
class CThingObject : public CThing { };              // CThingObject derives from CThing (PositionMesh takes the base)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public: float m[6]; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x34];
    long  PointerModel;                         // +0x3c
    float MoveSpeed;                            // +0x40
    float TurnSpeed;                            // +0x44
    float PointerHeightOffset;                  // +0x48
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};

class CTCTavernGame {
public:
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float tilt, float angle);   // 0x005EABC0
};

struct CTCTavernGameCardBase {
    virtual void v00();
    void EnterGameBase();                       // 0x008F1E30 (CTCTavernGameCardBase::EnterGame)
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
    CThingObject* CreateGamePiece(long model);  // 0x005EA9B0
    unsigned char   _pad_0x04[0xec];
    CRightHandedSet GamePositionRHSet;          // +0xf0
    C3DVector       GamePositionPos;            // +0x108
    unsigned char   _pad_0x114[0x2c];
    CThingObject*   Pointer;                    // +0x140
    unsigned char   _pad_0x144[0x1bc];
    long            NumCards;                   // +0x300
};

struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    void EnterGame();
};

void CTCTavernGameCardSorting::EnterGame()
{
    EnterGameBase();
    NumCards = 1;
    CDefPointer def;
    GetCardDef(&def);
    Pointer = CreateGamePiece(def->PointerModel);
    CTCTavernGame::PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeightOffset), 0.0f, 0.0f);
}