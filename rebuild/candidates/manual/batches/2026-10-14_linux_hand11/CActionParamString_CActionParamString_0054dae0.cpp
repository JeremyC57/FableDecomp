// NUISystem::CActionParamString::CActionParamString(const CWideString&) -- constructor: type tag 4 at +4, own vtable, the CWideString value copy-constructed at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054DAE0, length 48 bytes.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Vtable 0x01249520 = NUISystem::CActionParamString (rtti-vtables.tsv); the PDB layout is
// CActionParam {vptr, EActionParamType ActionParamType +4} then CWideString Value at +8 (4 = the
// string parameter type). The vtable store sinks to just before the member constructor call.
#include "fable_string.h"

struct CActionParam {
    virtual void v00();
    long ActionParamType;                                        // +0x04 EActionParamType
    CActionParam(long type) : ActionParamType(type) { }
};

struct CActionParamString : CActionParam {                       // vtable 0x01249520
    CWideString Value;                                           // +0x08
    virtual void v00();
    CActionParamString(const CWideString& value);
};

CActionParamString::CActionParamString(const CWideString& value)
    : CActionParam(4)
    , Value(value)
{
}
