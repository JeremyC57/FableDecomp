// CVtable_012b8d98::~CVtable_012b8d98() -- empty virtual destructor of the GameCODA class with vtable 0x012B8D98: own vtable, then the base CVtable_012b8df8 destructor (tail jump)
// Leaf: ??1CVtable_012b8d98@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C56D60, 11 bytes: `mov dword ptr [ecx],offset vtable 0x012B8D98 ; jmp 0x00C57310`.
// Identity: a destructor is named by the vtable it stores first. 0x012B8D98 has no complete-object locator (no-RTTI
//   GameCODA module, no PDB), so the class is named by its vtable as its landed constructor 0x00C56D30 is (CVtable_<va>, the
//   catalog's ??_7 convention). That constructor calls the base constructor 0x00C572F0 first, and the base destructor
//   0x00C57310 (CVtable_012b8df8, named by its own stored vtable) is the jump target here.
//   The base destructor ends by restoring the GameCODA root vtable 0x012B5868 (CCodaObjectBase, owner decision D3), and the
//   class's deleting destructor 0x00C56E40 frees through CodaFree 0x00C3C370, so the hierarchy roots at CCodaObjectBase.
//   The catalog label "CChunkedFileChunk::~CChunkedFileChunk" is propagated across this byte-twin shape and is wrong.
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the out-of-line
//   base destructor.
#include "engine/CCodaObjectBase.h"   // CCodaObjectBase (vtable 0x012B5868), CodaMalloc / CodaFree

class CVtable_012b8df8 : public CCodaObjectBase {
public:
    virtual ~CVtable_012b8df8();                  // 0x00C57310
};

class CVtable_012b8d98 : public CVtable_012b8df8 {
public:
    virtual ~CVtable_012b8d98();
};

CVtable_012b8d98::~CVtable_012b8d98()
{
}