// Grade: DIFFER(729v732) -- SetState(0x15) block placed after epilogue; wait-path card copy reg order
// CTCTavernGameCardBlackjack::DealCards() -- deal the two opening cards of the hand being dealt: place each at its slot (growing the dealt-area bounds), wait for it to land or send a stray one back, then open the first bet (state 0x12) or finish the deal (state 0x15)
// Leaf: ?DealCards@CTCTavernGameCardBlackjack
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DAC10, 732 bytes (+ int3 padding), plain `ret`; vtable slot 51. Ego_r.pdb
//   `?DealCards@CTCTavernGameCardBlackjack@@UAEXXZ`.
// Callees: CTCTavernGameCardBase::GetCardDef 0x008F0B20 (same-TU stand-in, as the landed GetPackPosition 0x008F0B80);
//   CTCTavernGameCardBlackjack::GetPosition(long, long) 0x007DA760; CTCTavernGameCardBase::GetPackPosition 0x008F0B80;
//   vector<long>::_M_insert_overflow 0x00440990; CTCTavernGame::SetState 0x005E9B20 / PlaySound 0x005EA950;
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CCard::Move(const C3DVector&, bool) 0x008F0660.
//   Globals: the table-height factor 0x01383C94, 1e-4f 0x0129BA3C.
// Layout (retail, as the landed vfunc_53 0x007DA120): BaseBet +0x34, TavernGameState +0xc0, MinX/MaxX/MinY/MaxY +0x118,
//   DealIndex +0x310, PlayersCards +0x324, Bets +0x330, CurrentDealingCards +0x340; CCard as CTCTavernGameCardBase::DealCards.
struct C3DVector {
    float X, Y, Z;
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
    bool IsPlaced() const { return Placed; }
};
#pragma pack(pop)
void MoveCardTo_008f0660(CCard* card, float x, float y, float z, bool snap);
#pragma auto_inline(off)
// Same-TU read-only stand-in for CCard::Move (0x008F0660): it only reads the target.
__declspec(noinline) void CCard::Move(const C3DVector& to, bool snap)
{
    MoveCardTo_008f0660(this, to.X, to.Y, to.Z, snap);
}
#pragma auto_inline(on)

template<class T> struct CVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
inline void* operator new(unsigned int, void* p) { return p; }
struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00440990
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
void* GrowVectorStorage_00440990(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (instantiated in retail's TU): reads the value and never touches the tag.
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_00440990(this, fillLength);
    if (p)
        *p = x;
}
#pragma auto_inline(on)

extern float g_CardTableHeight_01383c94;                // 0x01383C94
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

#include <math.h>

struct CTCTavernGameCardBlackjack {
    void* __vfptr;
    unsigned char _pad04[0x34 - 0x04];
    long BaseBet;                                       // +0x34
    unsigned char _pad38[0xc0 - 0x38];
    long TavernGameState;                               // +0xc0
    unsigned char _padc4[0x118 - 0xc4];
    float MinX, MaxX, MinY, MaxY;                       // +0x118
    unsigned char _pad128[0x310 - 0x128];
    long DealIndex;                                     // +0x310
    unsigned char _pad314[0x324 - 0x314];
    CVector< CVector<CCard*> > PlayersCards;            // +0x324
    vector<long> Bets;                                  // +0x330
    CVector<CCard*>* CurrentPlayerCards;                // +0x33c
    CVector<CCard*>* CurrentDealingCards;               // +0x340
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F0B20
    C3DVector GetPackPosition();                        // 0x008F0B80
    C3DVector GetPosition(long hand, long card);        // 0x007DA760
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                          // 0x005E9B20
    void DealCards();
};

void CTCTavernGameCardBlackjack::DealCards()
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    long state = TavernGameState;
    long hands = state == 0x13 ? PlayersCards.size() : 0;
    C3DVector position;
    bool placed = false;
    if (DealIndex != -1)
        placed = (*CurrentDealingCards)[DealIndex]->Placed;
    if (!placed) {
        if (++DealIndex == 2) {
            if (state == 0x13) {
                DealIndex = -1;
                CurrentDealingCards = PlayersCards.begin();
                Bets.push_back(BaseBet);
                SetState(0x12);
                return;
            } else {
                SetState(0x15);
                return;
            }
        }
        position = GetPosition(hands, DealIndex);
        position.Z = (*CurrentDealingCards)[DealIndex]->OldPosition.Z;
        if (position.X < MinX)
            MinX = position.X;
        if (position.X > MaxX)
            MaxX = position.X;
        if (position.Y < MinY)
            MinY = position.Y;
        if (position.Y > MaxY)
            MaxY = position.Y;
    } else {
        CCard* card = (*CurrentDealingCards)[DealIndex];
        if (fabs(card->Target.Z - (g_CardTableHeight_01383c94 * def->HeightScale + def->HeightOffset)) > g_epsilon_0129ba3c) {
            C2DVector pack(GetPackPosition());
            C2DVector cardPos((*CurrentDealingCards)[DealIndex]->Position);
            if (fabs(pack.X - cardPos.X) > def->CardWidth || fabs(pack.Y - cardPos.Y) > def->CardHeight) {
                position = (*CurrentDealingCards)[DealIndex]->Target;
                position.Z = g_CardTableHeight_01383c94 * def->HeightScale + def->HeightOffset;
                goto move;
            }
        }
        goto done;
    }
move:
    {
        CCharString sound("SOUND_CARD_DEAL");
        PlaySound(sound);
    }
    (*CurrentDealingCards)[DealIndex]->Move(position, false);
done:
    ;
}
