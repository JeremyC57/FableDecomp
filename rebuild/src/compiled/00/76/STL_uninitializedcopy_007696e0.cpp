// std::__uninitialized_copy(AttackParticleAndEndPoints* first, AttackParticleAndEndPoints* last, AttackParticleAndEndPoints* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end of the constructed range
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007696E0, 106 bytes (+int3 pad to 112), `ret 8`.
// Callers: 0x0076B760 (vector<AttackParticleAndEndPoints>::_M_insert_overflow, twice). Callees: CCharString copy constructor 0x0099EC30 (twice per element),
//   CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Twin: 0x007698C0 is the byte-identical <const T*, T*> instantiation (separate COMDATs per
//   iterator constness).
// Shape: STLport `__uninitialized_copy(first, last, result, __false_type)`, /Gr (__fastcall: first in ecx, last in
//   edx, result and the tag reference on the stack). Placement new null-tests each destination; the element's
//   user copy constructor (out of line at 0x00769040) is inlined: two string copies, two dword moves, then the
//   intelligent pointer's copy (base ctor, vtable 0x01238C6C, SetPItem(GetPItem())). The nested pointer member
//   gives the second induction register (`lea edi,[ebx+10h]`).
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
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

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template AttackParticleAndEndPoints* __fastcall __uninitialized_copy<AttackParticleAndEndPoints*, AttackParticleAndEndPoints*>(
    AttackParticleAndEndPoints*, AttackParticleAndEndPoints*, AttackParticleAndEndPoints*, const __false_type&);

}