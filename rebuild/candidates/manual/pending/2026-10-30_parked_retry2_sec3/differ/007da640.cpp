// CTCTavernGameCardBlackjack::GetBet() -- the def's "bet" text followed by the bet on the player's current hand (when there are bets and the hand index is valid), else the game's base bet
// Grade: DIFFER(274v274) -- frame-slot permutation (retail text/inner/def/temp, ours inner/temp/text/def) + this reuse for Bets; this sweep: moved onto engine/CDefPointer.h (text inlined with the proposed ENullDef ctor; the release now matches retail's CIVCountedPointeeBase::Release shape), fable_string.h CWideString with CWideString::Sprintf (was FormatWideString); tried a ternary += (316, worse)
// Leaf: GetBet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA640, 274 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardBlackjack vtable 0x0126F8F4 slot 47 (Ego_r.pdb `?GetBet@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ`).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          IntToWideString(long) 0x00405F00 (an inline `__fastcall` helper emitted out of line: hidden result in ecx,
//          value in edx; manifest label "CGuiControlNumber::SetSLongValue" is propagated); CWideString() 0x0099AED0;
//          FormatWideString(CWideString*, const wchar_t*, ...) 0x0099BA70 (__cdecl, L"%d" 0x0122DE44); CWideString(const
//          CWideString&) 0x0099B720; CWideString::operator+=(const CWideString&) 0x0099B8D0; ~CWideString 0x0099B510;
//          the def's virtual slot 1 when its count reaches zero.
// Layout (retail, STLport vectors 12 bytes): CTCTavernGame::BaseBet +0x34; CTCTavernGameCardBlackjack::PlayersCards
//          (vector<vector<CCard*> >, 12-byte elements) +0x324, Bets (vector<long>) +0x330, CurrentPlayerCardsIndex +0x354
//          (PDB +0x380); CTavernGameDef::Bet +0xec.
// Shape: `Bets.size() != 0` is `test ecx,0FFFFFFFCh` on the byte difference; the index test is signed `>= 0` then an
//        unsigned compare against size(); a `__forceinline` helper returns one of two inline IntToWideString results:
//        VC7.1 expands it once (the fallback arm) and calls the out-of-line copy from the first; both build into the
//        helper's single result slot, the `+=` takes eax from either arm; the named text is copied out (no NRVO).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
#include "fable_string.h"
inline CWideString __fastcall IntToWideString(long value)                        // 0x00405F00 (out-of-line copy)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}

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
    unsigned char _pad[0xec - sizeof(CSubDefClassBase)];
    unsigned long Bet;                          // +0xec
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
    void GetTavernGameDef(CDefPointer<const CTavernGameDef>* out);    // 0x005EA000 (ParentThing's base def GetSubDef, tail call)
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
            return IntToWideString(Bets[CurrentPlayerCardsIndex]);
        return IntToWideString(BaseBet);
    }
};

CWideString CTCTavernGameCardBlackjack::GetBet()
{
    CDefPointer<const CTavernGameDef> def(NULL_DEF);
    GetTavernGameDef(&def);
    CWideString text = GetText(def->Bet);
    text += GetCurrentBetString();
    return text;
}
