// CTCTavernGameCoinBase::EnterGame() -- tavern-game EnterGame, then create the three arrow pieces from the coin def's arrow model
// Leaf: ?EnterGame@CTCTavernGameCoinBase
// Retail: Fable.exe @ 0x008EF390, 105 bytes.
// Callees: CTCTavernGame::EnterGame() 0x005EC0F0 (non-virtual base call); the owner's def holder (CThing+0x70) lookup
//          0x008F00F0 (CDefPointer<const CTavernGameCoinBaseDef> by out pointer; the inlined form of the forwarder that
//          0x005EA000 is for the tavern-game def); CTCTavernGame::CreateGamePiece(long) 0x005EA9B0 (x3); the def's
//          virtual slot 1 when its count reaches 0.
// Layout: retail CTCTavernGameCoinBase = Ego_r.pdb minus 0x18: Arrow +0xf0, Arrow1 +0xf4, Arrow2 +0xf8 (PDB +0x108..);
//         CTavernGameCoinBaseDef::ArrowDefIndex +0x40 (PDB offset holds).
// Shape: the def getter is an inline member through ParentThing->+0x70; the taken def is copied out of the escaped out
//        slot once into a holder (edi) whose dtor releases it (`dec [edi+4] ; jne`, no null test).
class CThingObject;

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};
class CTavernGameCoinBaseDef : public CDefPointee { public:
    unsigned char _pad_0x08[0x38];
    long ArrowDefIndex;                         // +0x40
};
struct CDefPointer {
    CTavernGameCoinBaseDef* Object;
    CDefPointer() : Object(0) {}
};
struct CDefReference {                          // holder of the taken def: releases it without re-reading the out slot
    CTavernGameCoinBaseDef* Object;
    CDefReference(CTavernGameCoinBaseDef* object) : Object(object) {}
    ~CDefReference()
    {
        if (--Object->m_count == 0)
            Object->Destroy();
    }
    CTavernGameCoinBaseDef* operator->() const { return Object; }
};

struct CThingDefHolder {
    void GetDef(CDefPointer* out);              // 0x008F00F0
};
void* FindDef_CThingDefHolder_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) void CThingDefHolder::GetDef(CDefPointer* out)
{
    void* def = FindDef_CThingDefHolder_GetDef_0();
    if (!def)
        return;
    *(void**)out = def;
    
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad_0x00[0x70];
    CThingDefHolder* DefHolder;                 // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    void EnterGameBase();                       // 0x005EC0F0 (CTCTavernGame::EnterGame)
    CThingObject* CreateGamePiece(long model);  // 0x005EA9B0
};

struct CTCTavernGameCoinBase : CTCTavernGame {
    unsigned char _pad_0x08[0xe8];
    CThingObject* Arrow;                        // +0xf0
    CThingObject* Arrow1;                       // +0xf4
    CThingObject* Arrow2;                       // +0xf8
    void GetCoinDef(CDefPointer* out) { ParentThing->DefHolder->GetDef(out); }
    void EnterGame();
};

void CTCTavernGameCoinBase::EnterGame()
{
    EnterGameBase();
    CDefPointer out;
    GetCoinDef(&out);
    CDefReference def(out.Object);
    Arrow = CreateGamePiece(def.Object->ArrowDefIndex);
    Arrow1 = CreateGamePiece(def.Object->ArrowDefIndex);
    Arrow2 = CreateGamePiece(def.Object->ArrowDefIndex);
}