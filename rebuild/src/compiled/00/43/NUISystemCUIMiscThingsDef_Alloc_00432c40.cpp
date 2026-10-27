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
class CDefClassBase {                                     // Ego_r.pdb: CResource + PDefManager, GlobalIndex, flags (0x25 bytes, packed)
public:
    unsigned char _base[0x25];
};
namespace NUISystem {
class CUIMiscThingsDef : public CDefClassBase {
public:
    CUIMiscThingsDef();                                              // 0x004304E0
    static CDefClassBase* __fastcall Alloc();
    unsigned char _members[0x23B];                    // retail sizeof 0x260
};
CDefClassBase* __fastcall CUIMiscThingsDef::Alloc()
{
    return new CUIMiscThingsDef;
}
}