// CConsoleCommandBase::~CConsoleCommandBase() -- free the parameter-type vector, then unwind CConsoleInputBase (its Name string)
// Leaf: ??1CConsoleCommandBase@@UAE@XZ
// Retail: Fable.exe @ 0x00414F90, 40 bytes. Catalog label CCompressedMemoryDataInputStream::~... is wrong: the first vptr
// store is 0x0122E638, the RTTI vtable of CConsoleCommandBase (slot 1 ProcessCommandLine, slot 2 GetFormattedName), and the
// second 0x0122E5B0 is CConsoleInputBase's; the only non-deleting callers are CConsoleCommandBase-derived destructors
// (e.g. the deleting destructor 0x00415050). Body: `if (_M_start) free()` for ParamTypes (+0x08, STLport
// vector<EGenericVarType> on the malloc allocator, free = 0x00BFEA14), then the inline ~CConsoleInputBase stores its vptr
// and tail-jumps to ~CCharString (0x0099EAE0) on Name (+0x04).
// Layout: Ego_r.pdb CConsoleInputBase (vptr, Name +0x04) and CConsoleCommandBase (ParamTypes +0x08, a
// CConsoleCommandParameters wrapping one vector) with a 12-byte STLport vector (PDB 16). No engine/ headers.
// Shape: both destructors are user-declared (each stores its vptr); CConsoleCommandParameters' is implicit.
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