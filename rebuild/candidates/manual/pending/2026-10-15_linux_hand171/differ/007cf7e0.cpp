// CTCTavernGameShoveHaPenny::NewRound() -- coin-base NewRound, clear the board, find the obstacles, add the first coin at the board's centre line on the def's minimum coin position, then go to state 15
// Grade: DIFFER(127v127) -- (1) the out slot's zero store is scheduled before `mov ecx,[ecx+70h] ; push eax` (retail: after the push, as with a this-call getter); (2) the start vector's Y (def->PositionCoinMinimum) is copied through edx after X, retail loads it with fld first and stores X then Y from the x87 stack. The holder release (def in edi, dec [edi+4]) matches. Tried: CDefPointer with/without empty dtor, direct ParentThing->DefHolder->GetSubDef call, `0.5f * x`, C3DVector user copy ctor, body-assigned 3-float ctor.
// Leaf: ?NewRound@CTCTavernGameShoveHaPenny
// Retail: Fable.exe @ 0x007CF7E0, 127 bytes.
// Callees: CTCTavernGameCoinBase::NewRound() 0x008EF180 (non-virtual base call; manifest "CXMVPlaybackInfo" ctor label is a
//          propagation); CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer*) 0x007D1260 on the owner's
//          def holder (CThing+0x70; manifest "<CAICreatureWillPowerIndicatorDef>" is a propagated template label);
//          CTCTavernGameCoinBase::ClearBoard() 0x008EF950, ::FindObstacles() 0x008EFB00, ::AddNewCoin(const C3DVector&)
//          0x008EFA00; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20; the def's slot 1 on release. Float pool
//          0x0122F59C = 0.5.
// Layout: retail CTCTavernGameCoinBase::BoardSize +0x130 (PDB +0x14c; retail BoardPos +0x10c, BoardRHSet +0x118);
//         CTavernGameShoveHaPennyDef::PositionCoinMinimum +0x1c8 (PDB offset holds).
// Shape: the sub-def is copied once out of the escaped out slot (after the two board calls) into a holder that releases it
//        (`dec [edi+4] ; jne`, no null test); the start position is a 3-float temporary.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};
struct CTavernGameShoveHaPennyDef : CDefPointee {
    unsigned char _pad_0x08[0x1c0];
    float PositionCoinMinimum;                  // +0x1c8
};
struct CDefPointer {
    CTavernGameShoveHaPennyDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() {}
};
struct CDefReference {                          // holder of the taken def: releases it without re-reading the out slot
    CTavernGameShoveHaPennyDef* Object;
    CDefReference(CTavernGameShoveHaPennyDef* object) : Object(object) {}
    ~CDefReference()
    {
        if (--Object->m_count == 0)
            Object->Destroy();
    }
    CTavernGameShoveHaPennyDef* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);           // 0x007D1260
};
struct CThing {
    unsigned char _pad_0x00[0x70];
    CDefClassBase* DefHolder;                   // +0x70
};

struct CTCTavernGameCoinBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad_0x08[0x128];
    float BoardSizeX;                           // +0x130 BoardSize.X
    void NewRound();                            // 0x008EF180
    void ClearBoard();                          // 0x008EF950
    void FindObstacles();                       // 0x008EFB00
    void AddNewCoin(const C3DVector& pos);      // 0x008EFA00
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame)
};

struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    void GetShoveDef(CDefPointer* out) { ParentThing->DefHolder->GetSubDef(out); }
    void NewRound();
};

void CTCTavernGameShoveHaPenny::NewRound()
{
    CTCTavernGameCoinBase::NewRound();
    CDefPointer out;
    GetShoveDef(&out);
    ClearBoard();
    FindObstacles();
    CDefReference def(out.Object);
    AddNewCoin(C3DVector(0.5f * BoardSizeX, def->PositionCoinMinimum, 0.0f));
    SetState(15);
}
