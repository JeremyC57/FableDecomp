// PKCDeferredBank::~PKCDeferredBank() -- destructor of the libpk deferred-bank object (vtable 0x012B29F8): delete every data object holder A yields, delete the controller and the game state (clearing the pointers), then the members die in reverse order
// Leaf: ??1PKCDeferredBank@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14F90, 248 bytes (+ int3 padding), plain `ret`; called only by its scalar
//   deleting destructor 0x00C15A10. Identity: the destructor stores 0x012B29F8 first, the vtable of PKCDeferredBank (landed
//   ctor 0x00C14E40, naming wave 13; libpk has no RTTI/PDB, names descriptive as that ctor's model). The manifest label
//   "CTCDiceGame::~CTCDiceGame" is propagated.
// Callees: holder A's virtual slot 11 (+0x2c, next data object), PKCDeferredBank::DeleteData 0x00C14A50 (landed), the
//   controller / game-state destructors 0x00C2BE10 / 0x00C2A9B0 with operator delete 0x00BFE9BC, the holders' storage
//   destructor PKCTRRedblackTree::~PKCTRRedblackTree 0x00C19250 (bound spelling; the ctor model calls the class CDiceRuleSet).
// Layout (the landed ctor): base shell (vtable 0x012B27E0), parent +0x04, game state +0x08, controller +0x0c, holder A +0x10
//   (vtable 0x012B29B0, base 0x012B1AB0, storage +8), holder B +0x40 (0x012B28D8, same base), seed pair +0x70, three quads
//   +0x78 / +0x84 / +0x90 (base vtable 0x012B2828: the derived quad's dtor store is dead).
// Shape: /EHsc destructor (the member destructors are the unwind states); `while (data = HolderA.Next()) DeleteData(data)`;
//   each owned pointer deleted then cleared; members die in reverse order with their own vtable resets.
class CThing;
class CData_00c14a50;                                        // landed DeleteData parameter type
class PKCTRRedblackTree {                                    // 0x28 bytes
public:
    ~PKCTRRedblackTree();                                    // 0x00C19250
    unsigned char _body[0x28];
};
class CDiceRuleHolderBase {                                  // vtable 0x012B1AB0
public:
    virtual ~CDiceRuleHolderBase() { }
    PKCTRRedblackTree* Rules;                                // +0x04
};
class CDiceRuleHolderA : public CDiceRuleHolderBase {        // vtable 0x012B29B0
public:
    virtual ~CDiceRuleHolderA() { }
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CData_00c14a50* Next();                          // slot 11 (+0x2c)
    PKCTRRedblackTree Storage;                               // +0x08
};
class CDiceRuleHolderB : public CDiceRuleHolderBase {        // vtable 0x012B28D8
public:
    virtual ~CDiceRuleHolderB() { }
    PKCTRRedblackTree Storage;                               // +0x08
};
class CDiceSeedPair { public: long A, B; };
class CDiceQuadBase {                                        // vtable 0x012B2828
public:
    virtual ~CDiceQuadBase() { }
};
class CDiceQuad : public CDiceQuadBase {                     // vtable 0x012B4630
public:
    virtual ~CDiceQuad() { }
    short V[4];
};
class CDiceGameState { public: ~CDiceGameState(); };         // 0x00C2A9B0
class CDiceController { public: ~CDiceController(); };       // 0x00C2BE10
class CTCBaseShell {                                         // vtable 0x012B27E0
public:
    virtual ~CTCBaseShell() { }
};
class PKCDeferredBank : public CTCBaseShell {                // vtable 0x012B29F8
public:
    virtual ~PKCDeferredBank();
    void DeleteData(CData_00c14a50* data);                   // 0x00C14A50
    CThing*          ParentThing;                            // +0x04
    CDiceGameState*  State;                                  // +0x08
    CDiceController* Controller;                             // +0x0c
    CDiceRuleHolderA HolderA;                                // +0x10
    CDiceRuleHolderB HolderB;                                // +0x40
    CDiceSeedPair    Seed;                                   // +0x70
    CDiceQuad        QuadA;                                  // +0x78
    CDiceQuad        QuadB;                                  // +0x84
    CDiceQuad        QuadC;                                  // +0x90
    long             Field9C;                                // +0x9c
    long             FieldA0;                                // +0xa0
};

PKCDeferredBank::~PKCDeferredBank()
{
    CData_00c14a50* data;
    while ((data = HolderA.Next()) != 0)
        DeleteData(data);
    delete Controller;
    Controller = 0;
    delete State;
    State = 0;
}