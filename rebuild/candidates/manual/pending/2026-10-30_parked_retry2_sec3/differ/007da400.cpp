// CTCTavernGameCardBlackjack::GetCurrentScore() -- the def's "current score" text followed by the value of the player's current hand (or "0" while no hand is being played)
// Grade: DIFFER(ours v retail) -- 280v280: frame 0x14 v 0x10, the long->double conversion temporary gets its own slot (retail shares it with the GetText result slot); this sweep: moved onto engine/CDefPointer.h + CSubDefClassBase (header text inlined with the proposed ENullDef ctor), bytes unchanged
// Leaf: GetCurrentScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA400, 280 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardBlackjack vtable 0x0126F8F4 slot 44 (the GetCurrentScore slot, as ShoveHaPenny 0x007CF690 and
// CoinGolf 0x007D2B70). The manifest row says "CTCTavernGameCoinGolf::GetRequiredScore" (propagated; wrong class and slot).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGameCardBlackjack::GetHandValue(vector<CCard*>*,
//          bool) 0x007DA230; CCharString::FormatDouble(long digits, double) 0x009A08B0 (static `__fastcall`, hidden result in
//          ecx); CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&, const wchar_t*)
//          0x0099BF30 (L"0" 0x01231640; both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the def's
//          virtual slot 1 when its count reaches zero.
// Layout (retail, below Ego_r.pdb: STLport vectors are 12 bytes): CTCTavernGameCardBlackjack::CurrentPlayerCards +0x33c
//          (PDB +0x368); CTavernGameDef::CurrentScore +0xb8.
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
    unsigned char _pad[0xb8 - sizeof(CSubDefClassBase)];
    unsigned long CurrentScore;                 // +0xb8
};

struct CCard;
struct CCardVector {                            // STLport vector<CCard*> (12 bytes in retail)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
};

struct CTCTavernGame {
    virtual void v00();
    void GetTavernGameDef(CDefPointer<const CTavernGameDef>* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
};

struct CTCTavernGameCardBlackjack : CTCTavernGame {
    unsigned char _pad_0x04[0x314];
    CCardVector   DealersCards;                 // +0x318
    CCardVector   PlayersCards;                 // +0x324 (vector<vector<CCard*> >)
    CCardVector   Bets;                         // +0x330 (vector<long>)
    CCardVector*  CurrentPlayerCards;           // +0x33c
    long GetHandValue(CCardVector* cards, bool includeFaceDown);   // 0x007DA230
    CWideString GetCurrentScore();
};

CWideString CTCTavernGameCardBlackjack::GetCurrentScore()
{
    CDefPointer<const CTavernGameDef> def(NULL_DEF);
    GetTavernGameDef(&def);
    if (CurrentPlayerCards)
        return GetText(def->CurrentScore)
             + CCharString::FormatDouble(0, GetHandValue(CurrentPlayerCards, false)).ToWideString();
    return GetText(def->CurrentScore) + L"0";
}
