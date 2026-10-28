#include <stdio.h>
#include <stdlib.h>
#define CloseHandle TestCloseHandle
// CMutex_00c5eca0::scalar_deleting_destructor(unsigned int flags) -- Close an owned handle and release the holder when requested.
// Leaf: ??_GCMutex_00c5eca0@@UAEPAXI@Z
// Identity: own vtable012b90ec only slot; 00c5ed90 embedded member+52. Existing descriptive libpk class names, not original PDB recovery.
// Layout: no active shared header; vptr0 and handle4, size8. Closure import and scalar delete match own body.
#include <new>
#ifndef _MSC_VER
#define __stdcall
#define __declspec(x)
#endif
typedef struct HKEY__* HKEY;
extern "C" int __stdcall CloseHandle(void*);
struct CMutex_00c5eca0{public:void* Handle;CMutex_00c5eca0():Handle(0){}virtual ~CMutex_00c5eca0(){if(Handle){CloseHandle(Handle);Handle=0;}}};
void* MakeHolder(){return new CMutex_00c5eca0;}
static int closed,deleted,errors;static void* expected;static void* expectedAllocation;
extern "C" int __stdcall CloseHandle(void* h){if((void*)h!=expected)++errors;++closed;return 0;}
void operator delete(void* p) throw(){if(p==expectedAllocation)++deleted;free(p);}
int main(){int token;expected=&token;CMutex_00c5eca0* p=static_cast<CMutex_00c5eca0*>(MakeHolder());expectedAllocation=p;p->Handle=(void*)expected;delete p;bool ok=closed==1&&deleted==1&&!errors;p=static_cast<CMutex_00c5eca0*>(MakeHolder());expectedAllocation=p;delete p;ok=ok&&closed==1&&deleted==2;union Storage{double align;unsigned char bytes[sizeof(CMutex_00c5eca0)];}storage;p=new(storage.bytes) CMutex_00c5eca0;p->Handle=(void*)expected;expectedAllocation=storage.bytes;p->~CMutex_00c5eca0();ok=ok&&closed==2&&deleted==2&&!errors;puts(ok?"00c5e4a0_TEST PASS":"00c5e4a0_TEST FAIL");return ok?0:1;}