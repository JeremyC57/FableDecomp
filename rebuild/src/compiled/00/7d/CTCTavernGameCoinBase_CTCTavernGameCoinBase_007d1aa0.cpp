// CTCTavernGameCoinBase::~CTCTavernGameCoinBase() -- compiler-generated destructor: destroy the Coins vector, then CTCTavernGame
// Leaf: ??1CTCTavernGameCoinBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D1AA0, 65 bytes, tail jump to ~CTCTavernGame.
// Identity: called by the scalar deleting destructors of CTCTavernGameCoinBase 0x007D2200 (vtable 0x0126F224 slot 0),
//   CTCTavernGameShoveHaPenny 0x007D2240 and CTCTavernGameCoinGolf 0x007D4D90 (both derive from CoinBase); Ego_r.pdb
//   `??1CTCTavernGameCoinBase@@UAE@XZ`. It stores no vtable (implicit destructor). The catalog's `Catch@0047e8cc` label is
//   propagated: this is a plain thiscall function with its own prologue, not a catch block.
// Layout: Ego_r.pdb CTCTavernGameCoinBase with retail 12-byte vectors: Arrow/Arrow1/Arrow2 +0xf0..+0xf8, Coins +0xfc
//   (STLport vector<Coin>, 0x3c-byte elements with a virtual destructor: Ego_r `??_GCoin@@UAEPAXI@Z`), Held +0x108.
// Callees: Coin's virtual destructor (slot 0, flag 0), free @ 0x00BFEA14, CTCTavernGame::~CTCTavernGame @ 0x005EDA50 (tail jump).
// Shape: the members after Coins are trivially destructible, so only the vector's `_Destroy` loop and deallocate run, then the base.
//   Emitted through a qualified destructor call in a helper (GOTCHAS: an implicit dtor is only emitted out of line that way).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14 (CRT import thunk)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)        // STLport __destroy_aux(first, last, __false_type)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CThing;
class CThingObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class Coin {                                    // Ego_r: 60 bytes, virtual destructor
public:
    virtual ~Coin();
    unsigned char Unnamed04[0x20];              // +0x04 (PDB padding)
    bool          Owned;                        // +0x24
    CIntelligentPointer<CThing> Thing;          // +0x28
    float         Radius;                       // +0x30
    int           Owner;                        // +0x34
    unsigned long Sound;                        // +0x38
};

class CTCTavernGame {                           // retail size 0xf0
public:
    virtual ~CTCTavernGame();                   // 0x005EDA50
    unsigned char Unnamed04[0xec];              // +0x04 (CTCBase + tavern-game state, not touched here)
};

class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; };

class CTCTavernGameCoinBase : public CTCTavernGame {
public:
    CThingObject*    Arrow;                     // +0xf0
    CThingObject*    Arrow1;                    // +0xf4
    CThingObject*    Arrow2;                    // +0xf8
    std::vector<Coin> Coins;                    // +0xfc
    Coin*            Held;                      // +0x108
    C3DVector        BoardPos;                  // +0x10c
    unsigned char    BoardRHSet[0x18];          // +0x118 (CRightHandedSet)
    C3DVector        BoardSize;                 // +0x130
    C2DVector        MousePosEffect;            // +0x13c
};

void DestroyCoinBase_007d1aa0(CTCTavernGameCoinBase* game)
{
    game->CTCTavernGameCoinBase::~CTCTavernGameCoinBase();
}