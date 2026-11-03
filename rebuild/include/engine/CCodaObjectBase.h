#ifndef FABLE_CCODAOBJECTBASE_H
#define FABLE_CCODAOBJECTBASE_H
#include "CodaMemory.h"
// GameCODA 2.10 root class (owner decision 2026-11-03 D3; pass 11 spelled it CPKObject). Layout: vtable 0x012B5868, one
// virtual destructor slot (0x00C3C830 restores the vtable); child destructors restore it at offset zero.
// Allocation: the SDK Memory.obj CodaMalloc / CodaFree (0x00C3C360 / 0x00C3C370, byte-identical after one callback relocation
// each). Every retail `new` / `delete` of a derived class calls those two directly, so the class operators are inline forwarders.
class CCodaObjectBase {
public:
 virtual ~CCodaObjectBase();
 static void* __cdecl operator new(unsigned int size) { return CodaMalloc(size); }
 static void __cdecl operator delete(void* p) { CodaFree(p); }
};
#endif
