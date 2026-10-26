// _global::UninitFillN_CGenericVar_009ee100(CGenericVar* first, unsigned int n, const CGenericVar& x, const __false_type&) -- STLport __uninitialized_fill_n<CGenericVar*>: copy-construct n generic script variables into raw storage, return first + n
// Leaf: UninitFillN_CGenericVar_009ee100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EE100, length 82 bytes, `ret 8`.
// Callers: one site, 0x009EFB12 in the landed vector<Element>::_M_insert_overflow 0x009EFA50 (its same-TU
//   `__uninitialized_fill_n` stand-in is this body; the element's word/string/word/word/byte/word copy matches).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Identity: the catalog label `_Uninit_fill_n<pair<CCharString, CScriptInfo>*, ...>` is propagated. The vtable stored at +0
//   is 0x0129C3E4 = CGenericVar (rtti-vtables.tsv); Ego_r.pdb CGenericVar (28 bytes): vptr +0x00, Type +0x04, ValString
//   +0x08, ValSLONG +0x0C, ValULONG +0x10, ValBool +0x14, ValFloat +0x18.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> esi as `cur`,
//   n in edx -> ebx counted down, &x and the tag reference on the stack). CGenericVar's implicit copy constructor stores
//   the vtable, copies Type, calls the out-of-line CCharString copy on `this+8`, then the four scalars (the float through
//   an integer register); placement-new null test; cur returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE };

class CGenericVar {                             // vtable 0x0129C3E4, 28 bytes
public:
    EGenericVarType Type;                       // +0x04
    CCharString     ValString;                  // +0x08
    long            ValSLONG;                   // +0x0C
    unsigned long   ValULONG;                   // +0x10
    bool            ValBool;                    // +0x14
    float           ValFloat;                   // +0x18
    virtual ~CGenericVar();
};

CGenericVar* __fastcall UninitFillN_CGenericVar_009ee100(CGenericVar* first, unsigned int n, const CGenericVar& x, const __false_type&)
{
    CGenericVar* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CGenericVar(x);
    return cur;
}