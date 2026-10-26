// CTCTavernGame::GetBet() -- the tavern game def's "Bet" text followed by the current base bet
// Leaf: GetBet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA1A0, 166 bytes (`ret 4`: the CWideString result's hidden pointer).
// Callees: CDefClassBase::GetSubDef_005ed4a0(CDefPointer&) @ 0x005ED4A0 ('CTavernGameDef'; result ignored), CWideString()
// @ 0x0099AED0, FormatWideString(CWideString*, const wchar_t*, ...) @ 0x0099BA70 (L"%d" @ 0x0122DE44), CWideString(const
// CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CTCTavernGame::GetText(unsigned long) @ 0x005E9E70, operator+(const
// CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall); the def's virtual slot 1.
// Layout (retail, not engine/CTCTavernGame.h: the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail, as the landed
// GetWinPhrase 0x005EA100): CTCTavernGame BaseBet +0x34 (PDB +0x3c; as UpdateValues 0x005EA010); CThing PDef +0x70; CTavernGameDef Bet +0xec.
// Shape: the def is a zeroed CDefPointer filled by GetSubDef (a same-TU stand-in body that writes only the out slot, so VC7.1
// knows the def pointer does not escape and keeps it in edi for the release); `GetText(def->Bet) + IntToWideString(BaseBet)`
// is one expression, so the right operand (the inlined IntToWideString body: format into a local, copy out, destroy) is
// built first and the GetText temporary gets its own frame word; both temporaries die before the def is released.
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
    CWideString GetText(unsigned long textId);          // 0x005E9E70
    CWideString GetBet();
};

CWideString CTCTavernGame::GetBet()
{
    CDefPointer<CTavernGameDef> def;
    ParentThing->PDef->GetSubDef_005ed4a0(def);
    return GetText(def->Bet) + IntToWideString(BaseBet);
}