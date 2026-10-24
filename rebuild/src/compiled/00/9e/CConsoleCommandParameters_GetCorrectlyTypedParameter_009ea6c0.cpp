// CConsoleCommandParameters::GetCorrectlyTypedParameter(long) -- a default generic variable typed as parameter <index>
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EA6C0, 124 bytes (128 with padding). PDB: ?GetCorrectlyTypedParameter@CConsoleCommandParameters@@QBE?AVCGenericVar@@J@Z.
// Catalog label "Command_CreateAnonymousObject" is wrong. Single caller 0x009EBEF4.
// Callees: CCharString() 0x0099E4B0, CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0; vtable 0x0129C3E4.
// Shape: a default-constructed local (vptr, ValString ctor, then the dead Type = 0 body store), its Type set from
//   ParamTypes._M_start[index], returned by value through the hidden pointer (implicit copy ctor: vptr, Type, string
//   copy, then the dwords / bool byte / float dword), then the local's inline virtual dtor (vptr re-store + ~CCharString).
// Layout: Ego_r.pdb CGenericVar (0x1C); CConsoleCommandParameters = STLport vector<EGenericVarType> ParamTypes +0.
// Leaf: ?GetCorrectlyTypedParameter@CConsoleCommandParameters@@QBE?AVCGenericVar@@J@Z
class CCharString
{
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE };

class CGenericVar                                       // vtable 0x0129C3E4
{
public:
    EGenericVarType Type;                               // +0x04
    CCharString     ValString;                          // +0x08
    long            ValSLONG;                           // +0x0C
    unsigned long   ValULONG;                           // +0x10
    bool            ValBool;                            // +0x14
    float           ValFloat;                           // +0x18

    CGenericVar() { Type = GENERIC_VAR_TYPE_NONE; }
    virtual ~CGenericVar() {}
};

namespace _STL {
template <class _Tp> class vector
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp& operator[](unsigned int n) const { return *(_M_start + n); }
};
}

class CConsoleCommandParameters
{
public:
    _STL::vector<EGenericVarType> ParamTypes;           // +0x00
    CGenericVar GetCorrectlyTypedParameter(long index) const;
};

CGenericVar CConsoleCommandParameters::GetCorrectlyTypedParameter(long index) const
{
    CGenericVar var;
    var.Type = ParamTypes[index];
    return var;
}