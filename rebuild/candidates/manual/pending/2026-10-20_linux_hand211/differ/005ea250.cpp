// CTCTavernGame::GetPlayersMoney() -- the tavern game def's "PlayersMoney" text followed by the player's money (less the stake while a bet is placed, state 4)
// Leaf: GetPlayersMoney
// Grade: DIFFER(178v178) -- same as GetBet 0x005EA1A0: retail keeps four frame words and zeroes the out slot after the push; the lane shares the format local with the GetText temporary and releases the def before the amount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA250, 182 bytes (`ret 4`: the CWideString result's hidden pointer).
// Callees: as GetBet 0x005EA1A0: CDefClassBase::GetSubDef_005ed4a0 @ 0x005ED4A0, CWideString() @ 0x0099AED0, FormatWideString
// @ 0x0099BA70 (L"%d"), CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CTCTavernGame::GetText
// @ 0x005E9E70, operator+ @ 0x0099BE70; the def's virtual slot 1.
// Layout: CTCTavernGame BaseBet +0x34, CurrentMoney +0x40, TavernGameState +0xc4 (Ego_r.pdb +0xdc less the retail containers);
// CTavernGameDef PlayersMoney +0xf0.
// Shape: the twin of GetBet: the amount is `CurrentMoney`, less BaseBet in state 4, through the inlined IntToWideString body.
#include <stddef.h>
class CWideString {
public:
    void* m_pStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
void __cdecl FormatWideString(CWideString* text, const wchar_t* format, ...);   // 0x0099BA70
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
__forceinline CWideString __fastcall IntToWideString(long value)    // the body of 0x00405F00, inlined here
{
    CWideString text;
    FormatWideString(&text, L"%d", value);
    return text;
}
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CTavernGameDef : CDefBase {
    unsigned char _pad08[0xec - 0x08];
    unsigned long Bet;                                  // +0xec text id
    unsigned long PlayersMoney;                         // +0xf0 text id
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out);   // 0x005ED4A0
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CTCTavernGame {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x34 - 0x08];
    long BaseBet;                                       // +0x34
    long LastBet;                                       // +0x38
    long TotalWinnings;                                 // +0x3c
    long CurrentMoney;                                  // +0x40
    unsigned char _pad44[0xc4 - 0x44];
    long TavernGameState;                               // +0xc4 ETavernGameStates
    CWideString GetText(unsigned long textId);          // 0x005E9E70
    CWideString GetPlayersMoney();
};

CWideString CTCTavernGame::GetPlayersMoney()
{
    CDefOut<CTavernGameDef> defOut;
    ParentThing->PDef->GetSubDef_005ed4a0(defOut);
    long money = CurrentMoney;
    if (TavernGameState == 4)
        money -= BaseBet;
    CWideString amount = IntToWideString(money);
    CDefHolder<CTavernGameDef> def(defOut.Object);
    return GetText(def->PlayersMoney) + amount;
}
