// _STL::__uninitialized_fill_n(AttackParticleAndEndPoints* first, unsigned int n, const AttackParticleAndEndPoints& x, const __false_type&) -- copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769750, 102 bytes (+int3 pad to 112), `ret 8`.
// Caller: 0x0076B760 (vector<AttackParticleAndEndPoints>::_M_insert_overflow, the fill_len > 1 path).
// Callees: CCharString copy constructor 0x0099EC30 (twice per element), CBaseIntelligentPointer ctor 0x00A01B10,
//   GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Shape: STLport `__uninitialized_fill_n(first, n, x, __false_type)`, /Gr (__fastcall: first in ecx, the unsigned
//   count in edx -- `test edx,edx ; jbe` -- x and the tag reference on the stack). The count runs down in ebp;
//   placement new null-tests each destination and the element's user copy constructor (0x00769040) is inlined.
class CThing;
class CBaseObject;

class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};

struct AttackParticleAndEndPoints {                     // 0x18 (Ego_r.pdb)
    CCharString StartDummy;                             // +0x00 (names as the landed 0x00769040)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        PeriodicFrames;                         // +0x0c
    CIntelligentPointer<CThing> ParticleEffect;         // +0x10
    AttackParticleAndEndPoints(const AttackParticleAndEndPoints& other)   // out of line @ 0x00769040, inlined here
        : StartDummy(other.StartDummy), EndDummy(other.EndDummy), ParticleType(other.ParticleType),
          PeriodicFrames(other.PeriodicFrames), ParticleEffect(other.ParticleEffect) { }
};

inline void* operator new(unsigned int, void* where) { return where; }

namespace _STL {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template AttackParticleAndEndPoints* __fastcall __uninitialized_fill_n<AttackParticleAndEndPoints*, unsigned int, AttackParticleAndEndPoints>(
    AttackParticleAndEndPoints*, unsigned int, const AttackParticleAndEndPoints&, const __false_type&);

}