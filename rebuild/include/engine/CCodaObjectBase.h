#ifndef FABLE_CPKOBJECT_H
#define FABLE_CPKOBJECT_H
// Layout: libpk root vtable 012b5868; child destructors restore it at offset zero.
// Descriptive library identity, no donor PDB record. One virtual destructor slot.
class CPKObject {
public:
 virtual ~CPKObject();
 static void* __cdecl operator new(unsigned int);
 static void __cdecl operator delete(void*);
};
#endif
