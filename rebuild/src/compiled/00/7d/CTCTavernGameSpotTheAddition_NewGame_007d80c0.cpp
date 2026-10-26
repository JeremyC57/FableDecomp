// CTCTavernGameSpotTheAddition::NewGame() -- tavern-game NewGame, round/time/cover state cleared, the cover fully raised, first round flagged, then state 7
// Leaf: ?NewGame@CTCTavernGameSpotTheAddition
// Retail: Fable.exe @ 0x007D80C0, 115 bytes; CTCTavernGameSpotTheAddition vtable 0x0126F814 slot 31 (NewGame; the catalog's
//         "vfunc_31").
// Callees: CTCTavernGame::NewGame() 0x005E9A70 (non-virtual base call); CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>
//          (CDefPointer*) 0x007D9A80 on the owner's def holder (CThing+0x70; manifest template label propagated);
//          CTCTavernGameSpotTheAddition::PositionCover(float) 0x007D7F10; CTCTavernGame::SetState(ETavernGameStates)
//          0x005E9B20; the def's slot 1 on release.
// Layout: retail = Ego_r.pdb minus 0x18: CurrentRound +0xf0, CurrentTime +0xf4, CoverTimer +0xfc, UpdatedKnickKnacks +0x100,
//         Selected +0x148, FirstRound (bool) +0x14c; CTCTavernGame::GameWon +0x21 (PDB +0x29).
// Shape: the sub-def is fetched (and then unused) through an inline accessor; one ebx zero serves the out slot, the stores
//        and the release's null test (CDefPointer dtor `if (Object) Object->Release()`, the pointee's inline Release).
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
class CTavernGameSpotTheAdditionDef : public CDefPointee { public:};
struct CDefPointer {
    CTavernGameSpotTheAdditionDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->m_count == 0)
            Object->Destroy();
    }
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointer* out);           // 0x007D9A80
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad_0x00[0x70];
    CDefClassBase* DefHolder;                   // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad_0x08[0x19];
    bool    GameWon;                            // +0x21
    unsigned char _pad_0x22[0xce];
    void NewGameBase();                         // 0x005E9A70 (CTCTavernGame::NewGame)
    void SetState(long state);                  // 0x005E9B20
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long    CurrentRound;                       // +0xf0
    float   CurrentTime;                        // +0xf4
    CThing* Cover;                              // +0xf8
    float   CoverTimer;                         // +0xfc
    bool    UpdatedKnickKnacks;                 // +0x100
    unsigned char _pad_0x101[0x47];
    long    Selected;                           // +0x148
    bool    FirstRound;                         // +0x14c
    void GetSpotDef(CDefPointer* out) { ParentThing->DefHolder->GetSubDef(out); }
    void PositionCover(float fraction);         // 0x007D7F10
    void NewGame();
};

void CTCTavernGameSpotTheAddition::NewGame()
{
    NewGameBase();
    CDefClassBase* defClass = ParentThing->DefHolder;
    CDefPointer def;
    defClass->GetSubDef(&def);
    CurrentTime = 0.0f;
    CurrentRound = 0;
    GameWon = false;
    UpdatedKnickKnacks = false;
    Selected = 0;
    CoverTimer = 0.0f;
    PositionCover(1.0f);
    FirstRound = true;
    SetState(7);
}