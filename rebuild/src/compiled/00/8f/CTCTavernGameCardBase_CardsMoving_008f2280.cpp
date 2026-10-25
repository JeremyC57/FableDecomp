// CTCTavernGameCardBase::CardsMoving() -- card-phase step while cards move: wait for the pack to settle, then advance the state (20 -> 21, 27 -> 6, anything else -> 0), or in state 26 gather the dealt cards back onto the pack position (stacked above the remaining pack), drop that many cards from the front of the pack, restack the pack and put the gathered cards on its end, then go to 27
// Leaf: ?CardsMoving@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2280, length 766 bytes (ret). Name: Ego_r.pdb; vtable slot 52 of
// CTCTavernGameCardBase (Blackjack's 0x007DA070 jumps here; the landed CTCTavernGameCardBase::FrameUpdate @ 0x008F1D40 calls
// it for states 20/26/27). The catalog label "CTCTavernGameCardBase_OnFrameUpdate" is a descriptive gap-scan name.
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20 (twice: the outer def
//          and the one inside the inlined GetPackPosition 0x008F0B80); CTCTavernGame::SetState(ETavernGameStates)
//          0x005E9B20; memmove 0x00BFEAE6 (vector::erase's __copy_trivial); vector<CCard*>::_M_range_insert<CCard**>(pos,
//          first, last, const forward_iterator_tag&) 0x008F28A0 (the out-of-line half of Pack.insert); the def's virtual
//          slot 1 (Destroy) on the last release. CCard::Move 0x008F0660 is inlined (immediately == false).
// Layout: CTCTavernGame::TavernGameState +0xc0; CTCTavernGameCardBase Cards +0x128, Pack +0x134 (STLport vector<CCard*>,
//         as the landed FrameUpdate); CCard (Ego_r.pdb, byte-packed) Moving +0x26, MoveFraction +0x20, LocalPos +0x27,
//         NewLocalPos +0x33, CurrentPos +0x3f; CTavernGameCardBaseDef (Ego_r.pdb) CardWidth +0x2c, CardHeight +0x30,
//         CardSeparation +0x5c, TableSeparation +0x60, PackOffset +0x64.
// Shape: the settle test is `Moving == true` (cmp byte, dl=1). Every state branch ends in its own `return`, which gives
//        retail's duplicated release + epilogue per branch (an if/else chain cross-jumps the SetState calls). GetPackPosition
//        and Move are same-TU inlines; GetCardDef is a same-TU noinline stand-in (as the landed GetPackPosition), which is
//        what drops the null test from the inner def's release and turns it into load/dec/store, while the outer def keeps
//        its null test. The restack height is `53 - i` over a second counter that VC strength-reduces into the decrementing
//        memory counter fild reads; its 53 is stored after the hoisted pack-position copies (a plain `height = 53; --height`
//        counter is stored before the loop guard). The erase loop is STLport `erase(begin())` (memmove of the tail in bytes,
//        then `_M_finish -= 1` with the -4 hoisted into ebx); the insert passes a random_access_iterator_tag temporary.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}

template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                  // +0x00
    _Tp* _M_finish;                                 // +0x04
    _Tp* _M_end_of_storage;                         // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_trivial(__position + 1, _M_finish, __position);
        --_M_finish;
        return __position;
    }
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_range_insert(__pos, __first, __last, random_access_iterator_tag());
    }
    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last,
                         const forward_iterator_tag&);   // 0x008F28A0
};
} // namespace std

struct C2DVector { float X, Y; };
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CCard {
public:
    void*     Thing;                          // +0x00 CThingObject*
    long      Name;                           // +0x04
    long      Value;                          // +0x08
    float     InitialRotation;                // +0x0c
    float     FinalRotation;                  // +0x10
    float     TurnSpeed;                      // +0x14
    float     TurnFraction;                   // +0x18
    float     MoveSpeed;                      // +0x1c
    float     MoveFraction;                   // +0x20
    bool      Turning;                        // +0x24
    bool      FaceDown;                       // +0x25
    bool      Moving;                         // +0x26
    C3DVector LocalPos;                       // +0x27
    C3DVector NewLocalPos;                    // +0x33
    C3DVector CurrentPos;                     // +0x3f
    unsigned char _pad4b;                     // +0x4b
    float     Width;                          // +0x4c
    float     Height;                         // +0x50
    long      Index;                          // +0x54
    void Move(const C3DVector& position, bool immediately);   // 0x008F0660
};
#pragma pack(pop)

void CCard::Move(const C3DVector& position, bool immediately)
{
    if (immediately) {
        LocalPos = position;
        NewLocalPos = position;
        return;
    }
    if (Moving) {
        LocalPos = CurrentPos;
    } else {
        CurrentPos = LocalPos;
        Moving = true;
    }
    NewLocalPos = position;
    MoveFraction = 0.0f;
}

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;             // slot 1
    mutable long RefCount;                    // +0x04
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CTavernGameCardBaseDef : public CDefClassBase {
public:
    unsigned char _pad_0x08[0x24];
    float     CardWidth;                      // +0x2c
    float     CardHeight;                     // +0x30
    unsigned char _pad_0x34[0x28];
    float     CardSeparation;                 // +0x5c
    float     TableSeparation;                // +0x60
    C2DVector PackOffset;                     // +0x64
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (DecRefToZero(Object->RefCount))
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

class CTCTavernGame {
public:
    virtual void v00();
    unsigned char _pad_0x04[0xbc];
    long          TavernGameState;            // +0xc0 ETavernGameStates
    unsigned char _pad_0xc4[0x2c];
    void SetState(long state);                // 0x005E9B20
};
class CTCTavernGameCardBase : public CTCTavernGame {
public:
    unsigned char _pad_0xf0[0x38];            // +0xf0 GamePositionRHSet, GamePositionPos, ...
    std::vector<CCard*> Cards;                // +0x128
    std::vector<CCard*> Pack;                 // +0x134
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out);   // 0x008F0B20
    C3DVector GetPackPosition();              // 0x008F0B80
    void CardsMoving();
};

void* FindCardDef();
// Stand-in body (the real GetCardDef is out of line in retail): it only writes the def through the out pointer, so VC7.1
// knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out)
{
    void* p = FindCardDef();
    if (!p)
        return;
    *(void**)out = p;
}
#pragma auto_inline(on)

C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset.X, def->CardHeight * def->PackOffset.Y, 0.0f);
}

void CTCTavernGameCardBase::CardsMoving()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    for (std::vector<CCard*>::iterator it = Pack.begin(); it != Pack.end(); ++it)
        if ((*it)->Moving == true)
            return;
    if (TavernGameState == 20) {
        SetState(21);
        return;
    }
    if (TavernGameState == 26) {
        C3DVector pack_pos = GetPackPosition();
        for (unsigned int i = 0; i < Cards.size(); ++i)
            Cards[i]->Move(C3DVector(pack_pos.X, pack_pos.Y, (Cards.size() - i) * def->CardSeparation + def->TableSeparation), false);
        for (unsigned int j = 0; j < Cards.size(); ++j)
            Pack.erase(Pack.begin());
        long i = 0;
        for (std::vector<CCard*>::iterator card = Pack.begin(); card != Pack.end(); ++card, ++i)
            (*card)->Move(C3DVector(pack_pos.X, pack_pos.Y, (53 - i) * def->CardSeparation + def->TableSeparation), false);
        Pack.insert(Pack.end(), Cards.begin(), Cards.end());
        SetState(27);
        return;
    }
    if (TavernGameState == 27) {
        SetState(6);
        return;
    }
    SetState(0);
}