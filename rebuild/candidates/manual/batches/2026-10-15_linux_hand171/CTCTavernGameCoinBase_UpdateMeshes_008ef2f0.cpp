// CTCTavernGameCoinBase::UpdateMeshes() -- place every live, owned coin's mesh on the board at its 2D position, its angle turned into revolutions
// Leaf: UpdateMeshes
// Retail: Fable.exe @ 0x008EF2F0, 158 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (x3: alive test, dead test, mesh); PositionMesh(CThingObject*,
//          const C3DVector& pos, const CRightHandedSet& rh, const C3DVector& offset, long, float) 0x005EABC0 (__fastcall
//          global). Float pool 0x01237F08 = 1/(2*pi).
// Layout: retail CTCTavernGameCoinBase = Ego_r.pdb minus 0x18: Coins (vector<Coin>, 12 bytes) +0xfc, BoardPos +0x10c,
//         BoardRHSet +0x118. Coin (0x3c bytes, Ego_r.pdb names from +0x24): physics part Pos +0x04, Velocity +0x0c, Angle
//         +0x18; Owned +0x24, Thing (CIntelligentPointer<CThing>, unaligned) +0x25. Retail CThing Dead flag +0x91 bit 0.
// Shape: the board offset and the mesh are named locals built before the call (the mesh's GetPItem precedes the float
//        argument); the loop keeps the coin and &coin->Thing in two
//        strength-reduced registers.
struct CThingObject;
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet { float m[6]; };

struct CThing {
    unsigned char _pad_0x00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* _vfptr;                               // +0x00
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct Coin {                                   // 0x3c bytes
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    C2DVector Velocity;                         // +0x0c
    float     _pad_0x14;
    float     Angle;                            // +0x18
    unsigned char _pad_0x1c[0x8];
    bool      Owned;                            // +0x24
    CBaseIntelligentPointer Thing;              // +0x25
    unsigned char _pad_0x2d[0x3];
    float     Radius;                           // +0x30
    int       Owner;                            // +0x34
    unsigned long Sound;                        // +0x38
};
#pragma pack(pop)

void __fastcall PositionMesh(CThingObject* piece, const C3DVector& pos, const CRightHandedSet& rh,
                             const C3DVector& offset, long a, float angle);   // 0x005EABC0

struct CTCTavernGameCoinBase {
    unsigned char   _pad_0x00[0xfc];
    Coin*           CoinsBegin;                 // +0xfc
    Coin*           CoinsEnd;                   // +0x100
    Coin*           CoinsEndOfStorage;          // +0x104
    Coin*           Held;                       // +0x108
    C3DVector       BoardPos;                   // +0x10c
    CRightHandedSet BoardRHSet;                 // +0x118
    void UpdateMeshes();
};

void CTCTavernGameCoinBase::UpdateMeshes()
{
    for (Coin* coin = CoinsBegin; coin != CoinsEnd; ++coin) {
        if (coin->Thing.GetPItem() && !coin->Thing.GetPItem()->IsDead() && coin->Owned) {
            C3DVector offset(coin->Pos.X, coin->Pos.Y, 0.0f);
            CThingObject* mesh = (CThingObject*)coin->Thing.GetPItem();
            PositionMesh(mesh, BoardPos, BoardRHSet, offset, 0, coin->Angle * 0.15915494f);
        }
    }
}
