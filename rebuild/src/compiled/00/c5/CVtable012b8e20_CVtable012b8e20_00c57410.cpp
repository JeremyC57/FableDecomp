// CVtable_012b8e20::~CVtable_012b8e20() -- empty virtual destructor of the GameCODA class with vtable 0x012B8E20: own vtable, then the base CVtable_012b8ed4 destructor (tail jump)
// Leaf: ??1CVtable_012b8e20@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C57410, 11 bytes: `mov dword ptr [ecx],offset vtable 0x012B8E20 ; jmp 0x00C577B0`.
// Identity: a destructor is named by the vtable it stores first. 0x012B8E20 has no complete-object locator (no-RTTI
//   GameCODA module, no PDB), so the class is named by its vtable as its landed constructor 0x00C573E0 is (CVtable_<va>, the
//   catalog's ??_7 convention). That constructor calls the base constructor 0x00C57780 first, and the base destructor
//   0x00C577B0 (CVtable_012b8ed4, named by its own stored vtable) is the jump target here.
//   The base destructor ends by restoring the GameCODA root vtable 0x012B5868 (CCodaObjectBase, owner decision D3), and the
//   class's deleting destructor 0x00C57440 frees through CodaFree 0x00C3C370, so the hierarchy roots at CCodaObjectBase.
//   The catalog label "CChunkedFileChunk::~CChunkedFileChunk" is propagated across this byte-twin shape and is wrong.
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the out-of-line
//   base destructor.
#include "engine/CCodaObjectBase.h"   // CCodaObjectBase (vtable 0x012B5868), CodaMalloc / CodaFree

class CVtable_012b8ed4 : public CCodaObjectBase {
public:
    virtual ~CVtable_012b8ed4();                  // 0x00C577B0
};

class CVtable_012b8e20 : public CVtable_012b8ed4 {
public:
    virtual ~CVtable_012b8e20();
};

CVtable_012b8e20::~CVtable_012b8e20()
{
}