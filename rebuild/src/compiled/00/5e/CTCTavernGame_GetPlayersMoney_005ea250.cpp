// CTCTavernGame::GetPlayersMoney() -- the tavern game def's "PlayersMoney" text followed by the player's money (less the stake when the previous state was 4, bet placed)
// Leaf: GetPlayersMoney
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA250, 182 bytes (`ret 4`: the CWideString result's hidden pointer).
// Callees: as GetBet 0x005EA1A0: CDefClassBase::GetSubDef_005ed4a0 @ 0x005ED4A0, CWideString() @ 0x0099AED0, FormatWideString
// @ 0x0099BA70 (L"%d"), CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CTCTavernGame::GetText
// @ 0x005E9E70, operator+ @ 0x0099BE70; the def's virtual slot 1.
// Layout (retail, not engine/CTCTavernGame.h: the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail, as the landed
// GetWinPhrase 0x005EA100): CTCTavernGame BaseBet +0x34 (PDB +0x3c), CurrentMoney +0x40 (PDB +0x48), PreviousTavernGameState +0xc4
// (engine/CTCTavernGame.h now has these retail STLport 4.6 offsets: TavernGameState +0xc0, PreviousTavernGameState +0xc4, as the
// landed SetStateAfterTime 0x005E9B70);
// CTavernGameDef PlayersMoney +0xf0.
// Shape: the twin of GetBet 0x005EA1A0: a zeroed CDefPointer filled by GetSubDef (same-TU stand-in body that writes only the
// out slot, so the def stays in edi for the release), the amount (`CurrentMoney`, less BaseBet in state 4) in a local, and
// `GetText(def->PlayersMoney) + IntToWideString(money)` as one expression (right operand first, own frame word per temporary).
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
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CTavernGameDef : public CDefBase { public:
    unsigned char _pad08[0xec - 0x08];
    unsigned long Bet;                                  // +0xec text id
    unsigned long PlayersMoney;                         // +0xf0 text id
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005ed4a0(CDefPointer<CTavernGameDef>& out);   // 0x005ED4A0
};
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005ed4a0(CDefPointer<CTavernGameDef>& out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CTavernGameDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x34 - 0x08];
    long BaseBet;                                       // +0x34
    long LastBet;                                       // +0x38
    long TotalWinnings;                                 // +0x3c
    long CurrentMoney;                                  // +0x40
    unsigned char _pad44[0xc4 - 0x44];
    long PreviousTavernGameState;                               // +0xc4 ETavernGameStates
    CWideString GetText(unsigned long textId);          // 0x005E9E70
    CWideString GetPlayersMoney();
};

CWideString CTCTavernGame::GetPlayersMoney()
{
    CDefPointer<CTavernGameDef> def;
    ParentThing->PDef->GetSubDef_005ed4a0(def);
    long money = CurrentMoney;
    if (PreviousTavernGameState == 4)
        money -= BaseBet;
    return GetText(def->PlayersMoney) + IntToWideString(money);
}