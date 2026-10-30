// CCutsceneDef::Alloc() -- the definition factory: a new default-constructed CCutsceneDef (0x9C bytes)
// Leaf: ?Alloc@CCutsceneDef@@SIPAVCDefClassBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2A0D0, 27 bytes, static `__fastcall`, plain `ret`.
// Identity (proven): the constructor 0x00F29D00 is the landed CCutsceneDef::CCutsceneDef (vtable 0x012FB6E0); operator new(0x9C); Ego_r.pdb ?Alloc@CCutsceneDef@@SIPAVCDefClassBase@@XZ. The address is
//   stored in the definition-type registration table (data references only, no direct callers).
// Callees: operator new 0x00BFEA1A, CCutsceneDef::CCutsceneDef() 0x00F29D00 (tail jump).
// Shape (family M025, representative 0x00432C40): `return new CCutsceneDef;` -- the null test of the new-expression, the constructor
//   reached by `jmp` with the object in ecx.
// Header debt: the def class is modelled by its retail size only (the constructor is out of line).
void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x012FB6E0: slot 0 0x00F29DA0 (implicit inline dtor); real overrides from the def vtable scan
class CCutsceneDef : public CDefClassBase {
public:
    CCutsceneDef();                                              // 0x00F29D00
    static CDefClassBase* __fastcall Alloc();
    unsigned char _members[0x77];                    // retail sizeof 0x9C
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00F2A1D0
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00F29FF0
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00F29D90
};
CDefClassBase* __fastcall CCutsceneDef::Alloc()
{
    return new CCutsceneDef;
}