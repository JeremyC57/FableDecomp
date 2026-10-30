#pragma optimize("s", on)
// CExpressionDef::CExpressionDef() -- the CParentDefClassBase base, this class's vtable, then the twelve -1 words at +0x3c..+0x58, +0x64..+0x6c and +0x78, and the cleared word at +0x70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00456964, length 61 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370. Vtable 0x01233D1C = CExpressionDef (rtti-vtables.tsv). Shape: the size-optimised module (`or eax,-1`
// for the shared -1, `and dword ptr,0` for the clear); `this` is returned. Every store sits AFTER the vtable store, so the words are member objects with inline
// constructors (an index defaulting to -1, a cleared link): plain scalar initialisers would be scheduled before the vtable store (wave 24i).
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

struct CIndex_00456964 { long m_value; CIndex_00456964() : m_value(-1) { } };   // an index defaulting to "none"
struct CLink_00456964  { void* m_p;     CLink_00456964()  : m_p(0) { } };       // a cleared link

class CExpressionDef : public CParentDefClassBase { public:      // vtable 0x01233D1C
    CIndex_00456964 m_3c, m_40, m_44, m_48, m_4c, m_50, m_54, m_58;   // +0x3c .. +0x5c
    long _pad5c, _pad60;                           // +0x5c, +0x60 (not initialised)
    CIndex_00456964 m_64, m_68, m_6c;              // +0x64 .. +0x70
    CLink_00456964  m_70;                          // +0x70
    long _pad74;                                   // +0x74 (not initialised)
    CIndex_00456964 m_78;                          // +0x78
    virtual void v00();
    CExpressionDef();
};

CExpressionDef::CExpressionDef()
{
}