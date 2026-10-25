// CConsoleCommandBase::`scalar deleting destructor'(unsigned int) -- free the parameter-type vector, unwind CConsoleInputBase (its vtable, the Name string), flag-tested delete
// Leaf: ??_GCConsoleCommandBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00414FC0, 61 bytes, thiscall, `ret 4`.
// Identity: slot 0 of the CConsoleCommandBase vtable 0x0122E638 (rtti-vtables.tsv); the second vptr store 0x0122E5B0 is
//   CConsoleInputBase's. The catalog label CWaterPatchMesh::`scalar_deleting_destructor' is propagated. It sits right
//   after the landed out-of-line ~CConsoleCommandBase 0x00414F90 (same TU), whose body it inlines.
// Callees: free 0x00BFEA14 (ParamTypes is an STLport vector<EGenericVarType> over the malloc allocator),
//   ~CCharString 0x0099EAE0 (Name, +0x04), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CConsoleInputBase (vptr, Name +0x04) and CConsoleCommandBase (ParamTypes +0x08, a
//   CConsoleCommandParameters wrapping one vector) with a 12-byte STLport vector (PDB 16, so engine/CConsoleCommandBase.h
//   is 4 bytes too large): the same local model as the landed destructor 0x00414F90.
// Shape: both destructors are user-declared (each stores its vptr); CConsoleCommandParameters' is implicit; the
//   out-of-line ~CConsoleCommandBase defined in the TU emits the vtable and VC7.1 inlines it into the deleting destructor.
#include <stdlib.h>
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
enum EGenericVarType { GENERIC_VAR_TYPE_NONE };
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CConsoleCommandParameters {
public:
    CMallocVector<EGenericVarType> ParamTypes;           // +0x00
};
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase() {}
    CCharString Name;                                    // +0x04
};
class CConsoleCommandBase : public CConsoleInputBase {
public:
    virtual ~CConsoleCommandBase();
    CConsoleCommandParameters ParamTypes;                // +0x08
};
CConsoleCommandBase::~CConsoleCommandBase()
{
}