// NUISystem::CUIMiscThingsDef::Alloc() -- the definition factory: a new default-constructed CUIMiscThingsDef (0x260 bytes)
// Leaf: ?Alloc@CUIMiscThingsDef@NUISystem@@SIPAVCDefClassBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00432C40, 27 bytes, static `__fastcall`, plain `ret`.
// Identity (proven): the constructor 0x004304E0 stores vtable 0x01230EEC (RTTI .?AVCUIMiscThingsDef@NUISystem@@); operator new(0x260); Ego_r.pdb ?Alloc@CUIMiscThingsDef@NUISystem@@SIPAVCDefClassBase@@XZ. The address is
//   stored in the definition-type registration table (data references only, no direct callers).
// Callees: operator new 0x00BFEA1A, CUIMiscThingsDef::CUIMiscThingsDef() 0x004304E0 (tail jump).
// Shape (family M025, representative 0x00432C40): `return new CUIMiscThingsDef;` -- the null test of the new-expression, the constructor
//   reached by `jmp` with the object in ecx.
// Header debt: the def class is modelled by its retail size only (the constructor is out of line).
void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
namespace NUISystem {
// def vtable 0x01230EEC: slot 0 0x004317B0 (implicit inline dtor); real overrides from the def vtable scan
class CUIMiscThingsDef : public CDefClassBase {
public:
    CUIMiscThingsDef();                                              // 0x004304E0
    static CDefClassBase* __fastcall Alloc();
    unsigned char _members[0x23B];                    // retail sizeof 0x260
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00430900
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004321E0
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004308F0
};
CDefClassBase* __fastcall CUIMiscThingsDef::Alloc()
{
    return new CUIMiscThingsDef;
}
}