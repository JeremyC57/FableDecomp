// std::_Construct(CCardDef* p, const CCardDef& value) -- copy-construct the definition record in place
// Leaf: ??$_Construct@VCCardDef@@V1@@std@@YIXPAVCCardDef@@ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EC08A, 37 bytes, `__fastcall` (p ecx, value edx), plain `ret`.
// Identity (proven): stores vtable 0x01243A44 (RTTI .?AVCCardDef@@); the copied members match Ego_r.pdb CCardDef; its callers are vector<CCardDef>'s operator= 0x004EBF32, _M_allocate_and_copy 0x004EC10F, _M_fill_insert 0x004FA277 and _M_insert_overflow 0x004FA434 (catalogued so).
// Callees: CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) 0x0046EE98 (out of line).
// Shape (family M101, representative 0x004EAC30): STLport `new (__p) _T1(__val);` with CCardDef's implicit copy constructor
//   inline: the base copy, the vtable store, then the members through eax -- a size-optimised module, so the member-wise
//   copy constructor is spelled out and `__forceinline` (VC7.1 /Os does not inline it otherwise).
// Header debt: the def base is modelled by its retail size (0x39 bytes, packed) with its out-of-line copy constructor.
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class CCardDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CCardDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CCardDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CCardDef@@UAEKXZ
    __forceinline CCardDef(const CCardDef& other) : CParentDefClassBase(other), CardName(other.CardName), CardVal(other.CardVal) { }
    long CardName;                                        // +0x3c
    long CardVal;                                         // +0x40
};
namespace std {
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
template void __fastcall _Construct<CCardDef, CCardDef>(CCardDef* __p, const CCardDef& __val);
}