#ifndef FABLE_CCODAOBJECTBASE_H
#define FABLE_CCODAOBJECTBASE_H
#include "CodaMemory.h"
#if defined(FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23) && !defined(_MSC_VER)
#include <stddef.h>
#endif
// GameCODA 2.10 root class (owner decision 2026-11-03 D3; pass 11 spelled it CPKObject). Layout: vtable 0x012B5868, one
// virtual destructor slot (0x00C3C830 restores the vtable); child destructors restore it at offset zero.
// Allocation: the SDK Memory.obj CodaMalloc / CodaFree (0x00C3C360 / 0x00C3C370, byte-identical after one callback relocation
// each). Every retail `new` / `delete` of a derived class calls those two directly, so the class operators are inline forwarders.
class CCodaObjectBase {
public:
 virtual ~CCodaObjectBase();
#if defined(FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23) && !defined(_MSC_VER)
 // Host-size allocation signature; native ABI and default declaration stay unchanged.
 // CodaMalloc returns null on failure; preserve the native caller null check on LP64.
 static void* operator new(size_t size) throw() { return CodaMalloc((unsigned int)size); }
#else
 static void* __cdecl operator new(unsigned int size) { return CodaMalloc(size); }
#endif
 static void __cdecl operator delete(void* p) { CodaFree(p); }
#ifdef FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#if defined(FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23) && !defined(_MSC_VER)
 static void* operator new[](size_t size) throw() { return CodaMalloc((unsigned int)size); }
#else
 static void* __cdecl operator new[](unsigned int size) {return CodaMalloc(size);}
#endif
 static void __cdecl operator delete[](void* p) {CodaFree(p);}
#endif
};
#ifdef FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
// NF404 opt-in (rows 0x00C3C830 ??1CCodaObjectBase@@UAE@XZ, 0x00C5ECA0 / 0x00C5ED90 / 0x00C5E3D0 with FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS):
// retail's destructor is an inline member. ~CSalOutputKS 0x00C5ECA0 expands it in place (the closing 0x012B5868 vptr store), and every
// original GameCODA 2.10 SDK CODAApi.lib object that emits it (Object.obj, ObjectList.obj, SalPlatform.obj, SalRender.obj, SalSwBaseVoice.obj,
// SalSw3DVoice.obj, ...) carries ??1CCodaObjectBase@@UAE@XZ as a 7-byte pick-any (Any) COMDAT: 0x00C3C830 is that copy.
inline CCodaObjectBase::~CCodaObjectBase(){}
#endif
#endif
