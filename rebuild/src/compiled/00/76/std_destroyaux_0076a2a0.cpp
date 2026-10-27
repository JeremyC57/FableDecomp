// std::__destroy_aux(AttackParticleAndEndPoints* first, AttackParticleAndEndPoints* last, const __false_type&) -- destroy every record of [first, last)
// Leaf: ??$__destroy_aux@PAUAttackParticleAndEndPoints@@@std@@YIXPAUAttackParticleAndEndPoints@@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076A2A0, 51 bytes, `__fastcall` (first ecx, last edx, the tag by reference), `ret 4`.
// Identity (probable): 24-byte records destroyed in CTCObjectAugmentations' code (callers AttachAugmentationParticles 0x007677E0 and the vector helpers 0x0076A990..0x0076B320) with an intelligent pointer at +0x10 and CCharStrings at +4 / +0; Ego_r.pdb CTCObjectAugmentations holds vector<vector<AttackParticleAndEndPoints> > and AttackParticleAndEndPoints is 24 bytes with ParticleEffect, a CIntelligentPointer<CThing>, at +0x10.
// Callees: the member destructors (CCharString::~CCharString 0x0099EAE0, CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10).
// Shape (family M134, representative 0x00567350): STLport `for (; __first != __last; ++__first) _Destroy(&*__first);` with
//   the record's implicit destructor inline (members in reverse order); speed TU (the `lea ebx,[ebx]` loop alignment).
class CCharString {
public:
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;
};
class CBaseObjectPointer;
class CBaseIntelligentPointer {                           // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObjectPointer* PData;
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
struct AttackParticleAndEndPoints {                       // Ego_r.pdb, 24 bytes (the first 16 bytes unnamed there)
    CCharString _unnamed_0x0;                             // +0x0
    CCharString _unnamed_0x4;                             // +0x4
    unsigned long _unnamed_0x8;                           // +0x8
    unsigned long _unnamed_0xc;                           // +0xC
    CIntelligentPointer<CThing> ParticleEffect;           // +0x10
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template void __fastcall std::__destroy_aux<AttackParticleAndEndPoints*>(AttackParticleAndEndPoints*, AttackParticleAndEndPoints*, const std::__false_type&);