// Grade: DIFFER(904v904) -- 38 lines: scratch reg rotation in flip branch (vector ptr ecx vs edx, flag cl vs al)
// CTCTavernGameCardBlackjack::DealCard() -- deal one card from the pack to the hand being dealt (state 0x25), turn it face up once it lands (state 0x16), or send a stray landed card back to its slot
// Leaf: ?DealCard@CTCTavernGameCardBlackjack
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DAEF0, 904 bytes (+ int3 padding), plain `ret`.
// Callees: CTCTavernGame::GetDef(CDefPointer<CTavernGameDef>*) 0x005EA000 (out of line here); vector<CCard*>::
//   _M_insert_overflow 0x007D5D60 (same-TU template stand-in, unnamed tag); CTCTavernGame::SetState 0x005E9B20 /
//   PlaySound 0x005EA950; CTCTavernGameCardBlackjack::GetPosition(long, long) 0x007DA760; CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; CCard::Move 0x008F0660 / Turn 0x008F0620; CTCTavernGameCardBase::GetCardDef 0x008F0B20 /
//   GetPackPosition 0x008F0B80. Constants: 2^32 0x0122DCB4, 1.0f 0x0122DED8, 1/3 0x0125A4DC, the card layer height
//   0x01383C98 (2.0f), 1e-4f 0x0129BA3C.
// Layout (retail, as the landed vfunc_53 0x007DA120): MinX..MaxY +0x118, Cards +0x128, Pack +0x134, DealIndex +0x310,
//   CurrentDealingCards +0x340, NewlyDealtCard +0x344, CurrentDealingCardsIndex +0x358, CurrentPackCardIndex +0x35c.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CTavernGameDef : CDefPointee {
};
struct CTavernGameCardBaseDef : CDefPointee {
    unsigned char _pad08[0x2c - 0x08];
    float CardWidth;                                    // +0x2c
    float CardHeight;                                   // +0x30
    unsigned char _pad34[0x5c - 0x34];
    float HeightScale;                                  // +0x5c
    float HeightOffset;                                 // +0x60
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

#pragma pack(push, 1)
struct CCard {
    unsigned char _pad0[0x20];
    long State;                                         // +0x20
    unsigned char _pad24[2];
    bool Placed;                                        // +0x26
    C3DVector OldPosition;                              // +0x27
    C3DVector Target;                                   // +0x33
    C3DVector Position;                                 // +0x3f
    void Move(const C3DVector& to, bool snap);          // 0x008F0660
    void Turn();                                        // 0x008F0620
};
#pragma pack(pop)

inline void* operator new(unsigned int, void* p) { return p; }
struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x007D5D60
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
void* GrowVectorStorage_007d5d60(void* vector, unsigned long count);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_007d5d60(this, fillLength);
    if (p)
        *p = x;
}
#pragma auto_inline(on)

extern float g_CardLayerHeight_01383c98;                // 0x01383C98 (2.0f)
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

#include <math.h>

struct CTCTavernGameCardBlackjack {
    void* __vfptr;
    unsigned char _pad04[0x118 - 0x04];
    float MinX, MaxX, MinY, MaxY;                       // +0x118
    vector<CCard*> Cards;                               // +0x128
    vector<CCard*> Pack;                                // +0x134
    unsigned char _pad140[0x310 - 0x140];
    long DealIndex;                                     // +0x310
    unsigned char _pad314[0x340 - 0x314];
    vector<CCard*>* CurrentDealingCards;                // +0x340
    CCard* NewlyDealtCard;                              // +0x344
    unsigned char _pad348[0x358 - 0x348];
    long CurrentDealingCardsIndex;                      // +0x358
    long CurrentPackCardIndex;                          // +0x35c
    void GetDef(CDefPointer<CTavernGameDef>* out);      // 0x005EA000
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F0B20
    C3DVector GetPackPosition();                        // 0x008F0B80
    C3DVector GetPosition(long hand, long card);        // 0x007DA760
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                          // 0x005E9B20
    void DealCard();
};

void CTCTavernGameCardBlackjack::DealCard()
{
    CDefPointer<CTavernGameDef> gameDef;
    GetDef(&gameDef);
    if (DealIndex == -1) {
        DealIndex = CurrentDealingCards->size();
        Cards.push_back(Pack[CurrentPackCardIndex]);
        NewlyDealtCard = Pack[CurrentPackCardIndex];
        CurrentDealingCards->push_back(Pack[CurrentPackCardIndex++]);
        SetState(0x25);
        C3DVector position = GetPosition(CurrentDealingCardsIndex, DealIndex);
        position.Z = (*CurrentDealingCards)[DealIndex]->OldPosition.Z;
        if (position.X < MinX)
            MinX = position.X;
        if (position.X > MaxX)
            MaxX = position.X;
        if (position.Y < MinY)
            MinY = position.Y;
        if (position.Y > MaxY)
            MaxY = position.Y;
        {
            CCharString sound("TAVERN_GAME_CARD_DEAL");
            PlaySound(sound);
        }
        (*CurrentDealingCards)[DealIndex]->Move(position, false);
    } else if (!(*CurrentDealingCards)[DealIndex]->Placed) {
        {
            CCharString sound("TAVERN_GAME_CARD_FLIP");
            PlaySound(sound);
        }
        (*CurrentDealingCards)[DealIndex]->Turn();
        SetState(0x16);
        return;
    } else {
        CDefPointer<CTavernGameCardBaseDef> def;
        GetCardDef(&def);
        CCard* card = (*CurrentDealingCards)[DealIndex];
        float z = (((float)CurrentDealingCards->size() - 1.0f) * 0.33333334f + g_CardLayerHeight_01383c98) * def->HeightScale +
                  def->HeightOffset;
        if (card->Target.Z - z > g_epsilon_0129ba3c) {
            C2DVector pack(GetPackPosition());
            C2DVector cardPos(card->Position);
            if (fabs(pack.X - cardPos.X) > def->CardWidth || fabs(pack.Y - cardPos.Y) > def->CardHeight) {
                C3DVector to(card->Target.X, card->Target.Y, z);
                {
                    CCharString sound("TAVERN_GAME_CARD_DEAL");
                    PlaySound(sound);
                }
                card->Move(to, false);
            }
        }
    }
}
