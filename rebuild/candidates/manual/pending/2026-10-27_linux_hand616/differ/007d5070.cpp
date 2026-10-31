// CTCTavernGameCardSorting::CardsMainGame() -- the sorting main loop: run the timed-game clock (a timed game past its target time is lost: lose sound and the round-over OK message, state 0x18); once every card is face up and settled, stack them back on the table with the deal sound and enter state 0x1a
// Grade: DIFFER(539v539) -- one store: retail emits fstp Z [esp+2Ch] before the X=0 store [esp+24h] of the Move target, ours stores X first | tried (chunk 616, after the 2026-10-30 sweep's header migration and offset/scale sum order): Ego_r respelling (virtual UAE leaf, GetDef / protected PlaySoundW / SetStateAfterOK / ETavernGameStates, member names) = same; named float z (both zero stores hoisted above the fild); ctor body assignments; user copy ctor; named C3DVector local; Z-then-X-then-Y member assignments (all same 1-insn residue) | next: lane gap: store-vs-fstp scheduling (fleet residue class)
// Leaf: ?CardsMainGame@CTCTavernGameCardSorting@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D5070, 539 bytes (+ int3 padding), plain `ret`; virtual (Ego_r.pdb
//   ?CardsMainGame@CTCTavernGameCardSorting@@UAEXXZ next to the CardPairs / CardBlackjack overrides).
// Callees (Ego_r.pdb spellings): CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   CTCTavernGame::PlaySoundW (protected; <windows.h> renamed the PlaySound declaration) 0x005EA950; CCard::Move 0x008F0660;
//   CTCTavernGameCardBase::GetCardDef 0x008F0B20; CTCTavernGame::SetState 0x005E9B20 / GetText 0x005E9E70 /
//   GetDef 0x005EA000 (donor order: right after GetText in both binaries; body `mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ; jmp`
//   like GetCardDef's) / SetStateAfterOK (protected) 0x005EB080.
//   Globals: frames per second 0x01375550 (the landed g_framesPerSecond), 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 2^32 0x0122DCB4.
// Layout: Ego_r member names with STLport container sizes. CTCTavernGame (engine/CTCTavernGame.h, vptr stored as a data
//   member, so it cannot be a polymorphic base here): GameWon +0x21, GameFinished +0x28, TargetTime +0x98.
//   CTCTavernGameCardBase: Cards +0x128 (vector<CCard*>), Counter +0x154, RoundFinished +0x158, GameType +0x300.
//   CCard is #pragma pack(1) in the PDB (C3DVector LocalPos at +0x27): Turning +0x24, FaceDown +0x25.
//   CTavernGameDef WinPhrase +0xa4 / LosePhrase +0xa8 / Continue +0xf8; CTavernGameCardBaseDef CardSeparation +0x5c /
//   TableSeparation +0x60 (engine/CTavernGameCardBaseDef.h names; that header also stores the vptr as data).
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"

class CTavernGameDef : public CSubDefClassBase {            // RTTI .?AVCTavernGameDef@@ : CSubDefClassBase
public:
    unsigned char _pad[0xa4 - sizeof(CSubDefClassBase)];
    unsigned long WinPhrase;                    // +0xa4
    unsigned long LosePhrase;                   // +0xa8
    unsigned char _padAC[0xf8 - 0xac];
    unsigned long Continue;                     // +0xf8
};
class CTavernGameCardBaseDef : public CSubDefClassBase {    // RTTI .?AVCTavernGameCardBaseDef@@ : CSubDefClassBase
public:
    unsigned char _pad[0x5c - sizeof(CSubDefClassBase)];
    float CardSeparation;                       // +0x5c
    float TableSeparation;                      // +0x60
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(push, 1)
class CCard {
public:
    unsigned char _pad0[0x24];
    bool Turning;                               // +0x24
    bool FaceDown;                              // +0x25
    void Move(const C3DVector& to, bool snap);  // 0x008F0660
};
#pragma pack(pop)
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};
}
extern long g_framesPerSecond;                  // 0x01375550

class CTCTavernGame {                           // Layout: local (see header comment)
public:
    enum ETavernGameStates { };
    virtual ~CTCTavernGame();
    unsigned char _pad04[0x21 - 0x04];
    bool GameWon;                               // +0x21
    unsigned char _pad22[0x28 - 0x22];
    bool GameFinished;                          // +0x28
    unsigned char _pad29[0x98 - 0x29];
    float TargetTime;                           // +0x98
    unsigned char _pad9c[0xf0 - 0x9c];
    void SetState(ETavernGameStates state);     // 0x005E9B20
    CWideString GetText(unsigned long textID);  // 0x005E9E70
    void GetDef(CDefPointer<const CTavernGameDef>* def);   // 0x005EA000
protected:
    void SetStateAfterOK(CWideString title, CWideString text, ETavernGameStates state);   // 0x005EB080
    unsigned long PlaySoundW(const CCharString& name);   // 0x005EA950
};
class CTCTavernGameCardBase : public CTCTavernGame {
public:
    enum ECardGameType { };
    unsigned char _padF0[0x128 - 0xf0];
    std::vector<CCard*> Cards;                  // +0x128
    unsigned char _pad134[0x154 - 0x134];
    float Counter;                              // +0x154
    bool RoundFinished;                         // +0x158
    unsigned char _pad159[0x300 - 0x159];
    ECardGameType GameType;                     // +0x300
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* def);   // 0x008F0B20
};
class CTCTavernGameCardSorting : public CTCTavernGameCardBase {
public:
    virtual void CardsMainGame();
};

void CTCTavernGameCardSorting::CardsMainGame()
{
    if (!RoundFinished) {
        switch (GameType) {
        case 1:
            Counter += 1.0f / (float)g_framesPerSecond;
            break;
        }
    }
    if (Counter < 0.0f)
        Counter = 0.0f;
    for (CCard** it = Cards.begin(); it != Cards.end(); ++it) {
        if (!(*it)->FaceDown) {
            switch (GameType) {
            case 1:
                if (Counter > TargetTime) {
                    RoundFinished = true;
                    GameFinished = true;
                    GameWon = false;
                    {
                        CCharString sound("TAVERN_GAME_LOSE_GAME");
                        PlaySoundW(sound);
                    }
                    CDefPointer<const CTavernGameDef> def(NULL_DEF);
                    GetDef(&def);
                    unsigned long text = GameWon ? def->WinPhrase : def->LosePhrase;
                    SetStateAfterOK(GetText(text), GetText(def->Continue), (ETavernGameStates)0x18);
                }
                break;
            }
            return;
        }
    }
    RoundFinished = true;
    for (CCard** it = Cards.begin(); it != Cards.end(); ++it)
        if ((*it)->Turning == true)
            return;
    unsigned long i = 0;
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    for (CCard** it = Cards.begin(); it != Cards.end(); ++it, ++i) {
        {
            CCharString sound("TAVERN_GAME_CARD_DEAL");
            PlaySoundW(sound);
        }
        (*it)->Move(C3DVector(0.0f, 0.0f, (float)(Cards.size() - i) * def->CardSeparation + def->TableSeparation), false);
    }
    SetState((ETavernGameStates)0x1a);
}
