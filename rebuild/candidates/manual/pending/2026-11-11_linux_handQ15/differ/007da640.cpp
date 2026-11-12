// CTCTavernGameCardBlackjack::GetBet() -- the def's "bet" text followed by the bet on the player's current hand (when there are bets and the hand index is valid), else the game's base bet
// Grade: DIFFER(ours v retail) -- regraded as-is in Q15 (FABLE_HDR_OVERLAY=$A/hdr): ERROR: the draft's leaf ?GetBet@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ is not in its object (q.py graded the largest function); earlier grade 274v274 frame-slot permutation; NOT PORTED this pass -- the draft is still a local model, so it is not landable even if it matched; port onto the shared engine headers first (see NOTES.md)
// Earlier grade note: DIFFER(275v274) -- the if/else with two += statements gives retail's frame (text +8, inner +0xC, def +0x10, result +0x14) but VC7.1 pushes the out-of-line GFIntToWideString result in its arm (retail joins both arms with eax = &result before one push) and keeps Bets in ecx (retail esi); the draft's forceinline helper gets that join but permutes the frame
// Leaf: ?GetBet@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA640, 274 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardBlackjack vtable 0x0126F8F4 slot 47 (Ego_r.pdb `?GetBet@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ`).
// Callees: CTCTavernGame::GetDef(CDefPointer*) 0x005EA000; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          GFGFIntToWideString(long) 0x00405F00 (Ego_r ?GFIntToWideString@@YI?AVCWideString@@J@Z) (an inline `__fastcall` helper emitted out of line: hidden result in ecx,
//          value in edx; manifest label "CGuiControlNumber::SetSLongValue" is propagated); CWideString() 0x0099AED0;
//          CWideString::Sprintf(const wchar_t*, ...) 0x0099BA70 (__cdecl member, L"%d" 0x0122DE44); CWideString(const
//          CWideString&) 0x0099B720; CWideString::operator+=(const CWideString&) 0x0099B8D0; ~CWideString 0x0099B510;
//          the def's virtual slot 1 when its count reaches zero.
// Layout (retail, STLport vectors 12 bytes): CTCTavernGame::BaseBet +0x34; CTCTavernGameCardBlackjack::PlayersCards
//          (vector<vector<CCard*> >, 12-byte elements) +0x324, Bets (vector<long>) +0x330, CurrentPlayerCardsIndex +0x354
//          (PDB +0x380); CTavernGameDef::Bet +0xec.
// Shape: `Bets.size() != 0` is `test ecx,0FFFFFFFCh` on the byte difference; the index test is signed `>= 0` then an
//        unsigned compare against size(); a `__forceinline` helper returns one of two inline GFIntToWideString results:
//        VC7.1 expands it once (the fallback arm) and calls the out-of-line copy from the first; both build into the
//        helper's single result slot, the `+=` takes eax from either arm; the named text is copied out (no NRVO).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
struct CWideString {
    void* m_data;
    CWideString();                              // 0x0099AED0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    void __cdecl Sprintf(const wchar_t* format, ...);          // 0x0099BA70 ?Sprintf@CWideString@@QAAXPBGZZ
};
inline CWideString __fastcall GFIntToWideString(long value)                        // 0x00405F00 (out-of-line copy)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}

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
    unsigned char _pad_0x08[0xe4];
    unsigned long Bet;                          // +0xec
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
struct CHandVector {                            // STLport vector<vector<CCard*> >
    CCardVector* _M_start;
    CCardVector* _M_finish;
    CCardVector* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CLongVector {                            // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long i) { return _M_start[i]; }
};

struct CTCTavernGame {
    virtual void v00();
    void GetDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x04[0x30];
    long          BaseBet;                      // +0x34
};

struct CTCTavernGameCardBlackjack : CTCTavernGame {
    unsigned char _pad_0x38[0x2e0];
    CCardVector   DealersCards;                 // +0x318
    CHandVector   PlayersCards;                 // +0x324
    CLongVector   Bets;                         // +0x330
    CCardVector*  CurrentPlayerCards;           // +0x33c
    CCardVector*  CurrentDealingCards;          // +0x340
    void*         NewlyDealtCard;               // +0x344
    float         MostMoneyWon;                 // +0x348
    float         MoneyWonThisSitting;          // +0x34c
    long          Answer;                       // +0x350
    long          CurrentPlayerCardsIndex;      // +0x354
    CWideString GetBet();
    __forceinline CWideString GetCurrentBetString()   // inline helper (no retail body of its own)
    {
        if (Bets.size() != 0 && CurrentPlayerCardsIndex >= 0 && (unsigned long)CurrentPlayerCardsIndex < PlayersCards.size())
            return GFIntToWideString(Bets[CurrentPlayerCardsIndex]);
        return GFIntToWideString(BaseBet);
    }
};

CWideString CTCTavernGameCardBlackjack::GetBet()
{
    CDefPointer def;
    GetDef(&def);
    CWideString text = GetText(def->Bet);
    if (Bets.size() != 0 && CurrentPlayerCardsIndex >= 0 && (unsigned long)CurrentPlayerCardsIndex < PlayersCards.size())
        text += GFIntToWideString(Bets[CurrentPlayerCardsIndex]);
    else
        text += GFIntToWideString(BaseBet);
    return text;
}
