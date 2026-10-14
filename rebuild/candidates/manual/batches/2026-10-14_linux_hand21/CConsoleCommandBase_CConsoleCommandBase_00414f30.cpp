// CConsoleCommandBase::CConsoleCommandBase(const CCharString&, const long&) -- the inlined CConsoleInputBase(name) (its vtable, the Name copy), own vtable, the parameter-type list built from the single type
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00414F30, length 49 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CConsoleCommandParameters::
// CConsoleCommandParameters(long) @ 0x009EBB30 (a one-element vector constructor taking the 4-byte type
// by value; the manifest calls it "Vector_PushElementOrReallocate"). Vtables: 0x0122E5B0 =
// CConsoleInputBase (the inlined base constructor's store), 0x0122E638 = CConsoleCommandBase
// (rtti-vtables.tsv); the manifest's "CConsoleClasslessCommand" label is propagated.
// Layout = Ego_r.pdb: CConsoleInputBase {vptr, CCharString Name +4}, ParamTypes at +8 -- a 12-byte
// retail vector (engine/CConsoleCommandBase.h reserves 16, hence the `_Retail` model). The second
// argument arrives by reference and its dword is pushed for the list constructor (`mov edx,[ecx]`).
#include "engine/CConsoleCommandBase.h"

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
};

struct CConsoleInputBase {                                      // vtable 0x0122E5B0
    virtual void v00();
    CCharString Name;                                           // +0x04
    CConsoleInputBase(const CCharString& name) : Name(name) { }
};

struct CConsoleCommandParameters_Retail {                       // 12 bytes: a retail std::vector of parameter types
    long* first; long* last; long* end_of_storage;
    CConsoleCommandParameters_Retail(long type);                // 0x009EBB30: one element
};

struct CConsoleCommandBase_Retail : CConsoleInputBase {
    CConsoleCommandParameters_Retail ParamTypes;                // +0x08
    virtual void v00();                                         // own vtable 0x0122E638
    CConsoleCommandBase_Retail(const CCharString& name, const long& type);
};

FABLE_STATIC_ASSERT(sizeof(CConsoleCommandBase_Retail) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CConsoleCommandBase) == 0x18);       // the PDB vector is 4 bytes larger

CConsoleCommandBase_Retail::CConsoleCommandBase_Retail(const CCharString& name, const long& type)
    : CConsoleInputBase(name)
    , ParamTypes(type)
{
}
