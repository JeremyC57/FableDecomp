// CTCTavernGameCardPairs::EnterGame() -- enter the card game, then create the pointer game piece from the card def and place it at the game position, raised by the def's pointer height
// Leaf: vfunc_29
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D60C0, length 143 bytes (padded row 144); CTCTavernGameCardPairs vtable 0x0126F6FC slot 29
// (an EnterGame override: it starts with CTCTavernGameCardBase::EnterGame).
// Callees: CTCTavernGameCardBase::EnterGame @ 0x008F1E30, CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F0B20,
// CTCTavernGame::CreateGamePiece(long) @ 0x005EA9B0, CTCTavernGame::PositionMesh(CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float)
// __fastcall @ 0x005EABC0, the def's virtual slot 1 (destroy).
// Layout (retail CTCTavernGameCardBase, see 0x008F1CD0): GamePositionRHSet +0xF0, GamePositionPos +0x108, Pointer +0x140. Card def: pointer
// piece def index +0x3C, pointer height +0x48.
// Shape: null-tested CDefPointer dtor with the def's inline ReleaseTemporary(); the offset is a `C3DVector(0, 0, height)` temporary (Z via
// x87 before the pushes, the zero words stored after); the new piece goes from eax both into Pointer and into ecx.
// PositionMesh (sweep3, FINDINGS chunk 481 "PositionMesh's 5th parameter is float"): Ego_r/FableWin
//   ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z -- a static __fastcall member taking
//   (CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float); C3DVector, CRightHandedSet and CThing
//   are classes (V in every PDB signature).
class CThing;
class CThing { };
class CThingObject : public CThing { };              // CThingObject derives from CThing (PositionMesh takes the base)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRightHandedSet { public: float m[6]; };
class CTCTavernGame {
public:
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float tilt, float angle);   // 0x005EABC0
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameCardBaseDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad08[0x3c - 0x08];
    long  PointerDef;                           // +0x3c
    unsigned char _pad40[0x48 - 0x40];
    float PointerHeight;                        // +0x48
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};
struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];
    CRightHandedSet GamePositionRHSet;          // +0xf0
    C3DVector GamePositionPos;                  // +0x108
    long  GamePositionActionPointIndex;         // +0x114
    float MinX, MaxX, MinY, MaxY;               // +0x118
    void* Cards[3];                             // +0x128
    void* Pack[3];                              // +0x134
    CThingObject* Pointer;                      // +0x140
    void EnterGame();                           // 0x008F1E30
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
    CThingObject* CreateGamePiece(long defIndex);   // 0x005EA9B0 (CTCTavernGame)
};
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    void vfunc_29();
};

void CTCTavernGameCardPairs::vfunc_29()
{
    CTCTavernGameCardBase::EnterGame();
    CDefPointer def;
    GetCardDef(&def);
    Pointer = CreateGamePiece(def->PointerDef);
    CTCTavernGame::PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeight), 0.0f, 0.0f);
}