// _global::_Cons_val(CGenericVar*, const CGenericVar&) -- STL construct helper: placement-copy-construct a generic (typed) variable into raw container storage
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009edfe0, 61 bytes.
// Callee: CCharString copy ctor 0x0099EC30.
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (esi), source in edx (edi); the
// placement new's null test guards the inlined IMPLICIT copy constructor of a polymorphic class: the
// vtable store 0x0129C3E4 (RTTI CGenericVar), then the members in order (the bool as a byte, the
// float as a dword).
// Layout: Ego_r.pdb CGenericVar (0x1C) = vptr +0, EGenericVarType Type +4, CCharString ValString +8,
// long ValSLONG +0x0C, ulong ValULONG +0x10, bool ValBool +0x14, float ValFloat +0x18 (the landed
// CGenericVar::ReadFromParser 0x00A64360 models +0 as a plain dword; it is the vptr here).
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_009edfe0 {

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE };

struct CGenericVar {                                    // vtable 0x0129C3E4
    virtual void v00();
    EGenericVarType Type;                               // +0x04
    CCharString     ValString;                          // +0x08
    long            ValSLONG;                           // +0x0C
    unsigned long   ValULONG;                           // +0x10
    bool            ValBool;                            // +0x14
    float           ValFloat;                           // +0x18
};

void __fastcall _Cons_val(CGenericVar* ptr, const CGenericVar& value)
{
    new ((void*)ptr) CGenericVar(value);
}

}