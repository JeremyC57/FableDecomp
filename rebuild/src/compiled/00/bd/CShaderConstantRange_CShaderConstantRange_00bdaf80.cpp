// CShaderConstantRange::CShaderConstantRange(long first, long count) -- CBase, the first register at +4, own vtable, the count at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDAF80, length 34 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0. Vtable 0x0129A04C = CShaderConstantRange (rtti-vtables.tsv); the chunk
// label "CVertexShaderData::CVertexShaderData" is propagated over this byte-identical shell.
// Shape: first argument stored at +4 before the vtable store (`__declspec(novtable)` argument base), the
// second at +8 after it; `ret 8`.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct __declspec(novtable) CShaderConstantRangeBase : CBaseClass {
    long m_first;                             // +0x04
    CShaderConstantRangeBase(long first) : CBaseClass(), m_first(first) { }
};

class CShaderConstantRange : public CShaderConstantRangeBase { public:      // vtable 0x0129A04C
    long m_count;                             // +0x08
    virtual void v00();
    CShaderConstantRange(long first, long count);
};

CShaderConstantRange::CShaderConstantRange(long first, long count)
    : CShaderConstantRangeBase(first), m_count(count)
{
}