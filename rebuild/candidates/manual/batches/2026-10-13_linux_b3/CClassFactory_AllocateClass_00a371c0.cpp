// CClassFactory::AllocateClass(CCharString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A371C0, oracle length 42 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; push eax ; mov esi,ecx ; call CClassFactory::GetClassNumberFromName ; mov esi,[esi+4] ; lea eax,[eax+eax*4] ; shl eax,2 ; mov ecx,[eax+esi+0Ch] ; test ecx,ecx ; je Z ; call ecx ; pop esi ; ret 4 ; Z: xor eax,eax ; pop esi ; ret 4
// engine/CClassFactory.h (PDB): +0x04 vector<CClassTypeInfo> ClassTypeInfo (retail: `first` at
// +0x04). Each 20-byte CClassTypeInfo carries its allocator function at +0x0C. The class name is
// resolved to a type number (0x00A37060, __thiscall) and that entry's allocator, if registered,
// creates the instance; an unregistered type yields null.
#include "engine/CClassFactory.h"

struct CClass;
struct CCharString;

struct CClassTypeInfoEntry {
    unsigned char _pad_0x00[0x0C];
    CClass* (__cdecl* Allocate)();     // +0x0C allocator callback
    unsigned char _pad_0x10[4];        // 20-byte stride
};

struct CClassFactory_Methods : CClassFactory {
    long GetClassNumberFromName(CCharString* name);   // 0x00A37060
    CClass* AllocateClass(CCharString* name);
};

CClass* CClassFactory_Methods::AllocateClass(CCharString* name)
{
    long typeNumber = GetClassNumberFromName(name);
    CClassTypeInfoEntry* entry = &(*(CClassTypeInfoEntry**)&this->ClassTypeInfo[0])[typeNumber];
    if (entry->Allocate)
        return entry->Allocate();
    return 0;
}
