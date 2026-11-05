// std::swap(CBindingPair& a, CBindingPair& b) -- exchange two (name, entity script binding) pairs through a temporary copy
// Leaf: ??$swap@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA8E0, 194 bytes (`ret`, both references in ecx/edx).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport std::swap over (name, entity script binding) = std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > (0x0C bytes, CCompareString -> CCharString::operator< 0x009E6530 (??MCCharString@@QBE_NABV0@@Z, naming wave 20)); reached through
//   iter_swap (tail `jmp` at 0x00CBB2C0) and __unguarded_partition 0x00CBB8B9. The manifest label "swap<C3DMeshVolume>"
//   is propagated and wrong.
// Callees: CCharString copy/assign/dtor 0x0099EC30 / 0x0099EFB0 / 0x0099EAE0, pair operator= 0x00CBA700 (retail
//   emits the implicit pair assignment out of line), operator delete 0x00BFE9BC.
// Shape: `_Tp __tmp = __a; __a = __b; __b = __tmp;` with the counted pointer's inline copy/assign/release.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator<(const CCharString& other) const;      // 0x009E6530
};
#include "engine/CCountedPointer.h"
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class V> class CCompareString {                  // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.operator<(b.first); }
};
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CBindingPair;   // 12 bytes
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CBindingLess;
namespace std {
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<CBindingPair>(CBindingPair&, CBindingPair&);
}