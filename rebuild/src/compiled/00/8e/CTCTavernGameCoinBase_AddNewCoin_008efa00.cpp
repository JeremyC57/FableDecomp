// CTCTavernGameCoinBase::AddNewCoin(const C2DVector& position) -- adds an owned coin (radius and density from the coin def, a fresh coin game piece), makes it the held coin at the given board position and places its mesh on the board
// Leaf: AddNewCoin
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EFA00, 250 bytes (`ret 4`; Ego_r.pdb `?AddNewCoin@CTCTavernGameCoinBase@@QAEXABVC3DVector@@@Z`, only X/Y are read; modelled as the C2DVector prefix: a three-float member-wise copy compiles 6 bytes longer).
// Callees: the owner's def holder (CThing+0x70) lookup 0x008F00F0 (CDefPointer<const CTavernGameCoinBaseDef> by out
//          pointer, this TU; manifest label "GetSubDef<CAICreatureWillPowerIndicatorDef>" is propagated);
//          CTCTavernGame::CreateGamePiece(long) 0x005EA9B0; Coin::Coin(float radius, float density, CThing*, bool owned)
//          0x008EF200; Coin::Coin(const Coin&) 0x008EFF40; vector<Coin>::_M_insert_overflow 0x008F0450 (the landed
//          InsertGrow spelling); ~CBaseIntelligentPointer 0x00A01C10 (the temporary coin's Thing);
//          CBaseIntelligentPointer::GetPItem() 0x00A01B50; CTCTavernGame::PositionMesh(CThing*, const C3DVector&, const
//          CRightHandedSet&, const C3DVector&, long, float) 0x005EABC0 (__fastcall global); the def's slot 1 on release.
// Layout (retail, as the landed UpdateMeshes 0x008EF2F0): CTCTavernGameCoinBase Coins +0xfc, Held +0x108, BoardPos +0x10c,
//          BoardRHSet +0x118; Coin (0x3c, packed) Pos +0x04, Owned +0x24, Thing +0x25; CTavernGameCoinBaseDef CoinIDensity
//          +0x28, CoinRadius +0x2c, CoinDefIndex +0x3c (PDB offsets hold).
// Shape: the coin is a temporary pushed through the inline STLport push_back (null-tested placement copy, or the
//        overflow insert with its empty tag homed on the argument slot), destroyed right after; Held is re-read for the
//        mesh call; the def stays in edi (the same-TU lookup stand-in below shows VC7.1 the out pointer does not escape)
//        and its release has no null test because it was dereferenced.
//        The tag shares the argument slot only with a same-TU noinline stand-in for _M_insert_overflow (the const-ref
//        taker of the coin; extern, the tag gets its own frame byte, frame 0x50). The mesh call reaches the coin's
//        thing pointer through a reference local, so its address is formed before the stack arguments (retail).
// PositionMesh (sweep3, FINDINGS chunk 481 "PositionMesh's 5th parameter is float"): Ego_r/FableWin
//   ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z -- a static __fastcall member taking
//   (CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float); C3DVector, CRightHandedSet and CThing
//   are classes (V in every PDB signature).
class CThing;
class CThing { };
class CThingObject : public CThing { };              // CThingObject derives from CThing (PositionMesh takes the base)
class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public: float m[6]; };

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* _vfptr;                               // +0x00
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
class Coin { public:                                   // 0x3c bytes
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
    Coin(float radius, float density, CThing* thing, bool owned);   // 0x008EF200
    Coin(const Coin& other);                    // 0x008EFF40
};
#pragma pack(pop)

struct SScalarTag { SScalarTag() { } };         // empty iterator-category tag (user ctor: no zero fill)
inline void* operator new(unsigned int, void* where) { return where; }
struct CCoinVector {                            // STLport vector<Coin> (12 bytes)
    Coin* _M_start;
    Coin* _M_finish;
    Coin* _M_end_of_storage;
    void _M_insert_overflow(Coin* position, const Coin& x, const SScalarTag& tag, unsigned long fill_len, bool atend);   // 0x008F0450
    void push_back(const Coin& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) Coin(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, SScalarTag(), 1, true);
    }
};

// Same-TU stand-in for _M_insert_overflow 0x008F0450 (not the retail body).
void InsertOverflow_008f0450(CCoinVector* v, Coin* position, const Coin* x);
#pragma auto_inline(off)
__declspec(noinline) void CCoinVector::_M_insert_overflow(Coin* position, const Coin& x, const SScalarTag& tag, unsigned long fill_len, bool atend)
{
    InsertOverflow_008f0450(this, position, &x);
}
#pragma auto_inline(on)

class CTavernGameCoinBaseDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_count;                      // +0x04
    unsigned char _pad_0x08[0x20];
    float         CoinIDensity;                 // +0x28
    float         CoinRadius;                   // +0x2c
    float         CoinEdgeRestitution;          // +0x30
    float         CoinEdgeFriction;             // +0x34
    float         CoinSurfaceFriction;          // +0x38
    long          CoinDefIndex;                 // +0x3c
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CDefPointer {                            // CDefPointer<const CTavernGameCoinBaseDef>
    CTavernGameCoinBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameCoinBaseDef* operator->() const { return Object; }
};

struct CThingDefHolder {
    bool GetDef(CDefPointer* out);              // 0x008F00F0
};
CTavernGameCoinBaseDef* FindRef_CThingDefHolder_GetDef_0(CThingDefHolder* holder);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x008F00F0 is in this TU, out of line, see the landed CDefClassBase_GetSubDef008f00f0): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call.
#pragma auto_inline(off)
bool CThingDefHolder::GetDef(CDefPointer* out)
{
    CTavernGameCoinBaseDef* def = FindRef_CThingDefHolder_GetDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

struct CThingDefOwner {
    unsigned char    _pad_0x00[0x70];
    CThingDefHolder* DefHolder;                 // +0x70
};

class CTCTavernGame { public:
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& orientation,
                                        const C3DVector& offset, float tilt, float angle);   // 0x005EABC0

    virtual void v00();
    CThingDefOwner* ParentThing;                // +0x04
    CThing* CreateGamePiece(long model);        // 0x005EA9B0 (CThingObject*)
};

struct CTCTavernGameCoinBase : CTCTavernGame {
    unsigned char   _pad_0x08[0xf4];
    CCoinVector     Coins;                      // +0xfc
    Coin*           Held;                       // +0x108
    C3DVector       BoardPos;                   // +0x10c
    CRightHandedSet BoardRHSet;                 // +0x118
    void AddNewCoin(const C2DVector& position);
};

void CTCTavernGameCoinBase::AddNewCoin(const C2DVector& position)
{
    CDefPointer def;
    ParentThing->DefHolder->GetDef(&def);
    Coins.push_back(Coin(def->CoinRadius, def->CoinIDensity, CreateGamePiece(def->CoinDefIndex), true));
    Held = Coins._M_finish - 1;
    Held->Pos = position;
    CBaseIntelligentPointer& thing = Held->Thing;
    CTCTavernGame::PositionMesh((CThingObject*)thing.GetPItem(), BoardPos, BoardRHSet, C3DVector(Held->Pos.X, Held->Pos.Y, 0.0f), 0.0f, 0.0f);
}