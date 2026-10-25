// CGenericVar::CGenericVar() -- default constructor of a generic script/console variable: own vtable, an empty value string, type none
// Leaf: ??0CGenericVar@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EA150, 28 bytes, __fastcall `ret`, returns `this`.
// Identity (label correction): the catalog's "pair<unsigned_long,C3DMeshVolume>" is propagated. The body stores vtable
//   0x0129C3E4 (RTTI .?AVCGenericVar@@, six slots from 0x009EA230 down), default-constructs the CCharString at +8 and
//   zeroes the dword at +4: Ego_r.pdb CGenericVar (vptr, Type +4, ValString +8, ValSLONG +0xC, ValULONG +0x10, ValBool
//   +0x14, ValFloat +0x18); the PDB name sets have ??0CGenericVar@@QAE@XZ and the virtual ??1CGenericVar@@UAE@XZ. No direct
//   callers (reached through the inlined constructors of its users).
// Callee: CCharString::CCharString() @ 0x0099E4B0.
// Shape: the string member is constructed first, then the body sets Type (the landed CConsoleCommandParameters models
//   use the same `CGenericVar() { Type = GENERIC_VAR_TYPE_NONE; }`); the value fields are left unset.
class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE = 0 };

class CGenericVar {                                     // vtable 0x0129C3E4
public:
    EGenericVarType Type;                               // +0x04
    CCharString     ValString;                          // +0x08
    long            ValSLONG;                           // +0x0C
    unsigned long   ValULONG;                           // +0x10
    bool            ValBool;                            // +0x14
    float           ValFloat;                           // +0x18
    CGenericVar();
    virtual ~CGenericVar();
};

CGenericVar::CGenericVar()
{
    Type = GENERIC_VAR_TYPE_NONE;
}