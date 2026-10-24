// _global::__uninitialized_fill_n(SThingRecord* first, unsigned int n, const SThingRecord& x, const __false_type&) -- STLport uninitialized fill of n 20-byte records holding an intelligent thing pointer: placement-copy x into each slot, return the end
// Grade: DIFFER(86v86) -- register swap only: retail keeps cur in ebx and x in edi (pushes ebx first, loads x after the three pushes); ours keeps cur in edi and x in ebx. Tried: cur local (this), stepping first directly, a memberwise SThingRecord copy ctor, the STLport value-type-pointer outer layer over a __forceinline tag version.
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00722BD0, 86 bytes (96 with padding), `ret 8`.
// Label correction: the catalog "Vector_Fill_CountedPtr_Pairs" is descriptive; the body is STLport
//   `__uninitialized_fill_n(first, n, x, __false_type) { for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur; }`,
//   called once (0x007231D2) by the fill-insert growth path `_M_insert_overflow` @ 0x00723110 of the same
//   vector<SThingRecord> (it handles fill_len == 1 inline and calls this for the rest).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::GetPItem() const @ 0x00A01B50,
//   ::SetPItem(CBaseObject*) @ 0x00A01B90 (landed CIntelligentPointer model of 0x006A3440; vtable 0x01238C6C is
//   CIntelligentPointer<CThing>).
// Element (20 bytes, descriptive): three plain words, then CIntelligentPointer<CThing> at +0x0c (vptr, item).  Its
//   implicit copy constructor copies the three words and runs the pointer's copy constructor (base default
//   constructor, vtable store, SetPItem(other.GetPItem())).
// Convention: /Gr (link-time __fastcall): first in ecx (ebx), n in edx (unsigned, `jbe`), x and the empty tag on
//   the stack; the record pointer and its embedded pointer (esi = cur + 0x0c) advance together.
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() {} };

class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {                        // 8 bytes: vptr, item block
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};

struct SThingRecord {                                   // 20 bytes
    long                       m_a;                     // +0x00
    long                       m_b;                     // +0x04
    long                       m_c;                     // +0x08
    CIntelligentPointer<CThing> m_thing;                // +0x0c
};

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

SThingRecord* __fastcall __uninitialized_fill_n(SThingRecord* first, unsigned int n, const SThingRecord& x, const __false_type&)
{
    SThingRecord* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
