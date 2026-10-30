// Grade: DIFFER(539v539) -- header-moved (CSubDefClassBase/CDefClassBase + engine/CDefPointer.h verify block, ENullDef ctor): same 1-insn residue, Move target X=0 store scheduled before fstp Z
// CTCTavernGameCardSorting::CardsMainGame() -- the sorting main loop: run the timed-game clock (a timed game past its target time is lost: lose sound and the round-over OK message, state 0x18); once every card is face up and settled, stack them back on the table with the deal sound and enter state 0x1a
// Leaf: ?CardsMainGame@CTCTavernGameCardSorting
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D5070, 544 bytes (+ int3 padding), plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CTCTavernGame::PlaySound 0x005EA950; CCard::Move 0x008F0660; CTCTavernGameCardBase::GetCardDef 0x008F0B20; CTCTavernGame::SetState 0x005E9B20;
//   CTCTavernGame::GetTavernGameDef 0x005EA000 / GetText 0x005E9E70 / SetStateAfterOK 0x005EB080. Globals: the frame rate
//   0x01375550 (int), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout (retail): GameWon +0x21, NumRounds +0x24, GameFinished +0x28, TargetTime +0x98, GameStarted +0xa2, Cards +0x128,
//   Counter +0x154, TimerStopped +0x158, GameType +0x300, SelectedCard +0x308, AlternateSelectedCard +0x318; CCard Turning
//   +0x24, FaceDown +0x25; CTavernGameDef win/lose text +0xa4/+0xa8, body text +0xf8.
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};


class CTavernGameDef : public CSubDefClassBase {            // RTTI .?AVCTavernGameDef@@ : CSubDefClassBase
public:
    unsigned char _pad[0xa4 - sizeof(CSubDefClassBase)];
    unsigned long WinText;                      // +0xa4
    unsigned long LoseText;                     // +0xa8
    unsigned char _padAC[0xf8 - 0xac];
    unsigned long OKBodyText;                   // +0xf8
};
class CTavernGameCardBaseDef : public CDefClassBase {      // RTTI .?AVCTavernGameCardBaseDef@@
public:
    unsigned char _pad[0x5c - sizeof(CDefClassBase)];
    float HeightScale;                          // +0x5c
    float HeightOffset;                         // +0x60
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(push, 1)
struct CCard {
    unsigned char _pad0[0x24];
    bool Turning;                               // +0x24
    bool FaceDown;                              // +0x25
    void Move(const C3DVector& to, bool snap);  // 0x008F0660
};
#pragma pack(pop)
struct CCardVector {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
extern long g_FrameRate_01375550;               // 0x01375550

struct CTCTavernGameCardSorting {
    virtual void v00();
    unsigned char _pad04[0x21 - 0x04];
    bool GameWon;                               // +0x21
    unsigned char _pad22[0x28 - 0x22];
    bool GameFinished;                          // +0x28
    unsigned char _pad29[0x98 - 0x29];
    float TargetTime;                           // +0x98
    unsigned char _pad9c[0x128 - 0x9c];
    CCardVector Cards;                          // +0x128
    unsigned char _pad134[0x154 - 0x134];
    float Counter;                              // +0x154
    bool TimerStopped;                          // +0x158
    unsigned char _pad159[0x300 - 0x159];
    long GameType;                              // +0x300
    void GetTavernGameDef(CDefPointer<const CTavernGameDef>* out);          // 0x005EA000
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out);        // 0x008F0B20
    CWideString GetText(unsigned long textID);  // 0x005E9E70
    void SetStateAfterOK(CWideString title, CWideString text, long state);   // 0x005EB080
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                  // 0x005E9B20
    void CardsMainGame();
};

void CTCTavernGameCardSorting::CardsMainGame()
{
    if (!TimerStopped) {
        switch (GameType) {
        case 1:
            Counter += 1.0f / (float)g_FrameRate_01375550;
            break;
        }
    }
    if (Counter < 0.0f)
        Counter = 0.0f;
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it) {
        if (!(*it)->FaceDown) {
            switch (GameType) {
            case 1:
                if (Counter > TargetTime) {
                    TimerStopped = true;
                    GameFinished = true;
                    GameWon = false;
                    {
                        CCharString sound("TAVERN_GAME_LOSE_GAME");
                        PlaySound(sound);
                    }
                    CDefPointer<const CTavernGameDef> def(NULL_DEF);
                    GetTavernGameDef(&def);
                    unsigned long text = GameWon ? def->WinText : def->LoseText;
                    SetStateAfterOK(GetText(text), GetText(def->OKBodyText), 0x18);
                }
                break;
            }
            return;
        }
    }
    TimerStopped = true;
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it)
        if ((*it)->Turning == true)
            return;
    unsigned long i = 0;
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it, ++i) {
        {
            CCharString sound("TAVERN_GAME_CARD_DEAL");
            PlaySound(sound);
        }
        (*it)->Move(C3DVector(0.0f, 0.0f, (float)(Cards.size() - i) * def->HeightScale + def->HeightOffset), false);
    }
    SetState(0x1a);
}
