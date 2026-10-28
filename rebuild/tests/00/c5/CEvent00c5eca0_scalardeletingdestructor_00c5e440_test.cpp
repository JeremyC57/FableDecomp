#include <stdio.h>
#include <stdlib.h>
#define CloseHandle TestCloseHandle
// CEvent_00c5eca0::scalar_deleting_destructor(unsigned int flags) -- Close an owned handle and release the holder when requested.
// Leaf: ??_GCEvent_00c5eca0@@UAEPAXI@Z
// Identity: own vtable012b90e8 only slot; 00c5ed90 embedded member+44. Existing descriptive libpk class names, not original PDB recovery.
// Layout: no active shared header; vptr0 and handle4, size8. Closure import and scalar delete match own body.
#include <new>
#ifndef _MSC_VER
#define __stdcall
#define __declspec(x)
#endif
typedef struct HKEY__* HKEY;
extern "C" int __stdcall CloseHandle(void*);
struct CEvent_00c5eca0{public:void* Handle;CEvent_00c5eca0():Handle(0){}virtual ~CEvent_00c5eca0(){if(Handle){CloseHandle(Handle);Handle=0;}}};
void* MakeHolder(){return new CEvent_00c5eca0;}
static int closed,deleted,errors;static void* expected;static void* expectedAllocation;
extern "C" int __stdcall CloseHandle(void* h){if((void*)h!=expected)++errors;++closed;return 0;}
void operator delete(void* p) throw(){if(p==expectedAllocation)++deleted;free(p);}
int main(){int token;expected=&token;CEvent_00c5eca0* p=static_cast<CEvent_00c5eca0*>(MakeHolder());expectedAllocation=p;p->Handle=(void*)expected;delete p;bool ok=closed==1&&deleted==1&&!errors;p=static_cast<CEvent_00c5eca0*>(MakeHolder());expectedAllocation=p;delete p;ok=ok&&closed==1&&deleted==2;union Storage{double align;unsigned char bytes[sizeof(CEvent_00c5eca0)];}storage;p=new(storage.bytes) CEvent_00c5eca0;p->Handle=(void*)expected;expectedAllocation=storage.bytes;p->~CEvent_00c5eca0();ok=ok&&closed==2&&deleted==2&&!errors;puts(ok?"00c5e440_TEST PASS":"00c5e440_TEST FAIL");return ok?0:1;}