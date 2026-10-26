// CCreatureAction_ShopkeeperGetOut::PerformAction() -- put the carried goods up for sale: build a goods place from the stored position and orientation and hand it to the shop with the goods id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842D40, length 101 bytes (+ int3 padding).
// Callees: CTCShop::AddSaleItem(long, const CTCShop::GoodsPlace&) @ 0x00772BB0.
// Shape: a 0x24-byte frame holding the GoodsPlace local {C3DVector pos; CRightHandedSet rhset} (Ego_r.pdb: 36 bytes), filled by two
// whole-struct copies, each through its own base register (`lea eax,[ecx+0B0h]`, `lea edx,[ecx+0BCh]`: struct members copy through a
// re-based pointer); then the shop word is loaded straight into ecx after the goods argument. No callee-saved registers.
// Layout: the engine header (PDB, sizeof 0xe0) places TCShop/Goods/Pos/RHSet at +0xb4..+0xc8; retail CCreatureActionBase is 0x0c bytes
// narrower (0xa8), so they sit at +0xa8/+0xac/+0xb0/+0xbc here and `this` is modelled locally with the PDB member names.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:                    // 24 bytes
    C3DVector Up;
    C3DVector Forward;
};
class CTCShop { public:
    struct GoodsPlace {                     // 36 bytes
        C3DVector pos;
        CRightHandedSet rhset;
    };
    void AddSaleItem(long goods, const GoodsPlace& place);   // 0x00772BB0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _body[0xa8 - 4];
};
class CCreatureAction_ShopkeeperGetOut : public CCreatureActionBase { public:   // retail sizeof 0xd4
    CTCShop* TCShop;                        // +0xa8
    long Goods;                             // +0xac
    C3DVector Pos;                          // +0xb0
    CRightHandedSet RHSet;                  // +0xbc
    virtual void PerformAction();
};

void CCreatureAction_ShopkeeperGetOut::PerformAction()
{
    CTCShop::GoodsPlace place;
    place.pos = Pos;
    place.rhset = RHSet;
    TCShop->AddSaleItem(Goods, place);
}