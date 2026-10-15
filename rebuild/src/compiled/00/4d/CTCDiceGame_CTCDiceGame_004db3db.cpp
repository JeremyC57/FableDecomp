// CTCDiceGame::CTCDiceGame(CThing* parent) -- CTCBase(parent), NumberOfPlayers = 0, own vtable, null CurrentPlayer pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB3DB, length 38 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0
// (out of line, called with 0).
// Vtable 0x0124093C = CTCDiceGame (rtti-vtables.tsv). PDB CTCDiceGame (0x18 bytes): NumberOfPlayers +0x0c,
// CurrentPlayer (CIntelligentPointer<CThing>) +0x10.
// Shape: /Os (pass-through base argument, `and dword ptr [esi+0Ch],0` zero store); the scalar
// initialiser is stored before the vtable store, the pointer member's out-of-line constructor follows it.
#pragma optimize("s", on)
struct CThing;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

struct CIntelligentPointer_CThing {                         // vptr + PData
    void* __vftable;
    void* PData;
    CIntelligentPointer_CThing(CThing* thing);              // 0x004A96F0
};

struct CTCDiceGame : CTCBase_Calls {
    long                       NumberOfPlayers;             // +0x0c
    CIntelligentPointer_CThing CurrentPlayer;               // +0x10
    virtual void v00();                                     // own vtable 0x0124093C
    CTCDiceGame(CThing* parent);
};

CTCDiceGame::CTCDiceGame(CThing* parent)
    : CTCBase_Calls(parent)
    , NumberOfPlayers(0)
    , CurrentPlayer(0)
{
}