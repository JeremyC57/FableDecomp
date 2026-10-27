// CScriptDef::Alloc() -- the definition factory: a new default-constructed CScriptDef (0x1088 bytes)
// Leaf: ?Alloc@CScriptDef@@SIPAVCDefClassBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29F40, 27 bytes, static `__fastcall`, plain `ret`.
// Identity (proven): the constructor 0x00F28F90 is the landed CScriptDef::CScriptDef (vtable 0x012FB5C8); operator new(0x1088); Ego_r.pdb ?Alloc@CScriptDef@@SIPAVCDefClassBase@@XZ. The address is
//   stored in the definition-type registration table (data references only, no direct callers).
// Callees: operator new 0x00BFEA1A, CScriptDef::CScriptDef() 0x00F28F90 (tail jump).
// Shape (family M025, representative 0x00432C40): `return new CScriptDef;` -- the null test of the new-expression, the constructor
//   reached by `jmp` with the object in ecx.
// Header debt: the def class is modelled by its retail size only (the constructor is out of line).
void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A
class CDefClassBase {                                     // Ego_r.pdb: CResource + PDefManager, GlobalIndex, flags (0x25 bytes, packed)
public:
    unsigned char _base[0x25];
};
class CScriptDef : public CDefClassBase {
public:
    CScriptDef();                                              // 0x00F28F90
    static CDefClassBase* __fastcall Alloc();
    unsigned char _members[0x1063];                    // retail sizeof 0x1088
};
CDefClassBase* __fastcall CScriptDef::Alloc()
{
    return new CScriptDef;
}