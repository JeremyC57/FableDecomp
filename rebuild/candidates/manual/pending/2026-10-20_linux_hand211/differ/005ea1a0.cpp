// CTCTavernGame::GetBet() -- the tavern game def's "Bet" text followed by the current base bet
// Leaf: GetBet
// Grade: DIFFER(166v166) -- retail keeps four frame words (the format local and the GetText temporary apart), zeroes the out
// slot after the push and releases the def after the amount; the lane shares the two string words and releases the def first
// (holder before the amount loads the def too early; a CDefPointer reloads it; the amount as a temporary changes nothing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA1A0, 166 bytes (`ret 4`: the CWideString result's hidden pointer).
// Callees: CDefClassBase::GetSubDef_005ed4a0(CDefPointer&) @ 0x005ED4A0 ('CTavernGameDef'; result ignored), CWideString()
// @ 0x0099AED0, FormatWideString(CWideString*, const wchar_t*, ...) @ 0x0099BA70 (L"%d" @ 0x0122DE44), CWideString(const
// CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CTCTavernGame::GetText(unsigned long) @ 0x005E9E70, operator+(const
// CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall); the def's virtual slot 1.
// Layout: CTCTavernGame BaseBet +0x34 (retail, as UpdateValues 0x005EA010); CThing PDef +0x70; CTavernGameDef Bet +0xec.
// Shape: the amount is the inlined IntToWideString body (format into a local, copy out, destroy: no NRVO); the def is read
// from its out slot after it; `GetText(def->Bet) + amount` is built straight into the hidden result (the right operand's
// address pushed first).
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
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
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
    CWideString GetText(unsigned long textId);          // 0x005E9E70
    CWideString GetBet();
};

CWideString CTCTavernGame::GetBet()
{
    CDefOut<CTavernGameDef> defOut;
    ParentThing->PDef->GetSubDef_005ed4a0(defOut);
    CWideString amount = IntToWideString(BaseBet);
    CDefHolder<CTavernGameDef> def(defOut.Object);
    return GetText(def->Bet) + amount;
}
