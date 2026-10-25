// std::__copy(Coin*, Coin*, Coin*, const random_access_iterator_tag&, int*) -- assign the coin range [first, last) onto result in order and return the end of the destination
// Leaf: ??$__copy@PAVCoin@@
// Retail 0x008F02E0, 0xA1 bytes, __fastcall (first in ecx, last in edx; result, the tag by const reference and the
//   null distance pointer on the stack, `ret 0Ch`). Manifest label "_Move<Coin*,Coin*>" is descriptive; callers are
//   CTCTavernGameCoinBase::ClearBoard 0x008EF950 (landed: the inline `erase(begin(), end())`) and the coin vector's
//   erase/assign helpers 0x008F0390 / 0x008F03B0.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 and SetPItem @ 0x00A01B90 (CIntelligentPointer::operator=).
// Layout: Coin (Ego_r.pdb, 0x3C, matches retail; the landed Coin copy ctor 0x008EFF40 model): Mass base (vptr, Pos, Vel,
//   IMass, APos, AVel, AIMass), Owned +0x24, Thing CIntelligentPointer<CThing> at the odd offset +0x25 (pointer classes
//   `pack(1)`, Coin not), Radius +0x30, Owner +0x34, Sound +0x38.
// Shape: STLport 4.x random-access `__copy`: `for (n = last - first; n > 0; --n) *result++ = *first++` with Coin's
//   compiler-generated operator= inlined (the vptr is not assigned: eight Mass words, the Owned byte, the pointer's
//   SetPItem(GetPItem()), three words); VC divides the byte distance by 0x3C with the 0x88888889 multiply and
//   precomputes the returned `result + n`.
class CBaseObject;
class CThing;

class C2DVector {
public:
    float X;                                    // +0x00
    float Y;                                    // +0x04
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};

class Mass {                                    // vtable 0x01288230, 0x24 bytes
public:
    C2DVector Pos;                              // +0x04
    C2DVector Vel;                              // +0x0C
    float IMass;                                // +0x14
    float APos;                                 // +0x18
    float AVel;                                 // +0x1C
    float AIMass;                               // +0x20
    virtual ~Mass() {}
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {                 // 8 bytes, byte-aligned (Coin::Thing sits at the odd offset +0x25)
public:
    CBaseObject* m_pItem;                       // +0x04
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
#pragma pack(pop)

class Coin : public Mass {                      // vtable 0x01288238, 0x3C bytes (Ego_r.pdb)
public:
    bool                        Owned;          // +0x24
    CIntelligentPointer<CThing> Thing;          // +0x25
    float                       Radius;         // +0x30
    int                         Owner;          // +0x34
    unsigned long               Sound;          // +0x38
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template Coin* __fastcall __copy(Coin*, Coin*, Coin*, const random_access_iterator_tag&, int*);
}