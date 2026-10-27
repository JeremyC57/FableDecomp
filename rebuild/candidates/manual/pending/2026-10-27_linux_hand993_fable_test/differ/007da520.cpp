// CTCTavernGameCardBlackjack::GetRequiredScore() -- the def's "required score" text followed by the value of the dealer's hand (or "0" while no hand is being played)
// Grade: DIFFER(286v286) -- frame 0x14 v 0x10: the long->double `fild` temporary gets its own slot where retail reuses the slot the GetText result occupies later ([esp+0Ch]); a named long local gets the frame right but calls GetHandValue before the ToWideString hidden-result push; an inline (long) helper evaluates its argument first (283 bytes, edi dropped)
// Leaf: GetRequiredScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA520, 286 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardBlackjack vtable 0x0126F8F4 slot 45 (the GetRequiredScore slot, as ShoveHaPenny 0x007CF740 and
// CoinGolf 0x007D2C20). The manifest row says "CTCTavernGameCoinGolf::GetRequiredScore" (propagated; wrong class).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGameCardBlackjack::GetHandValue(vector<CCard*>*,
//          bool) 0x007DA230; CCharString::FormatDouble(long digits, double) 0x009A08B0 (static `__fastcall`, hidden result in
//          ecx); CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&, const wchar_t*)
//          0x0099BF30 (L"0" 0x01231640; both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the def's
//          virtual slot 1 when its count reaches zero.
// Layout (retail, below Ego_r.pdb: STLport vectors are 12 bytes): CTCTavernGameCardBlackjack::CurrentPlayerCards +0x33c
//          (PDB +0x368); CTavernGameDef::RequiredScore +0xbc; DealersCards +0x318 (PDB +0x338).
// Shape: both returns build straight into the hidden result; ToWideString's hidden-result push comes before the hand value
//        is computed (its slot then holds the `fild` source); the CDefPointer dtor releases through DecRef/GetRefCount and
//        clears the field (`mov edx,edi ; test edx,edx`), expanded at both returns.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
struct CWideString {
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
struct CCharString {
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);      // 0x0099BF30

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0xb4];
    unsigned long RequiredScore;                // +0xbc
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CTavernGameDef* operator->() const { return Object; }
};

struct CCard;
struct CCardVector {                            // STLport vector<CCard*> (12 bytes in retail)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
};

struct CTCTavernGame {
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
};

struct CTCTavernGameCardBlackjack : CTCTavernGame {
    unsigned char _pad_0x04[0x314];
    CCardVector   DealersCards;                 // +0x318
    CCardVector   PlayersCards;                 // +0x324 (vector<vector<CCard*> >)
    CCardVector   Bets;                         // +0x330 (vector<long>)
    CCardVector*  CurrentPlayerCards;           // +0x33c
    long GetHandValue(CCardVector* cards, bool includeFaceDown);   // 0x007DA230
    CWideString GetRequiredScore();
};

CWideString CTCTavernGameCardBlackjack::GetRequiredScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    if (CurrentPlayerCards)
        return GetText(def->RequiredScore)
             + CCharString::FormatDouble(0, GetHandValue(&DealersCards, false)).ToWideString();
    return GetText(def->RequiredScore) + L"0";
}
