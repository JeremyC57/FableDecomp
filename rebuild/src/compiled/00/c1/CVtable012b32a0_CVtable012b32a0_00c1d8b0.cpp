// CVtable_012b32a0::~CVtable_012b32a0() -- empty virtual destructor of the libpk class with vtable 0x012B32A0: own vtable, then the base CVtable_012b3404 destructor (tail jump)
// Leaf: ??1CVtable_012b32a0@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D8B0, 11 bytes: `mov dword ptr [ecx],offset vtable 0x012B32A0 ; jmp 0x00C1EB10`.
// Identity: a destructor is named by the vtable it stores first. 0x012B32A0 has no complete-object locator (no-RTTI
//   libpk module, no PDB), so the class is named by its vtable as its landed constructor 0x00C1D880 is (CVtable_<va>, the
//   catalog's ??_7 convention). That constructor calls the base constructor 0x00C1EAA0 first, and the base destructor
//   0x00C1EB10 (CVtable_012b3404, named by its own stored vtable) is the jump target here.
//   The class's deleting destructor 0x00C1DD30 calls this row, then operator delete 0x00BFE9BC.
//   The catalog label "CChunkedFileChunk::~CChunkedFileChunk" is propagated across this byte-twin shape and is wrong.
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the out-of-line
//   base destructor.

class CVtable_012b3404 {
public:
    virtual ~CVtable_012b3404();                  // 0x00C1EB10
};

class CVtable_012b32a0 : public CVtable_012b3404 {
public:
    virtual ~CVtable_012b32a0();
};

CVtable_012b32a0::~CVtable_012b32a0()
{
}