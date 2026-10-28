#include <stdio.h>
#include <stdlib.h>
// CRegistryKey::scalar_deleting_destructor(unsigned int flags) -- Close an owned handle and release the holder when requested.
// Leaf: ??_GCRegistryKey@@UAEPAXI@Z
// Identity: own vtable012befb0 only slot; Own constructor00c73790, destructor00c739a0 and registry Create00c737d0. Existing descriptive libpk class names, not original PDB recovery.
// Layout: no active shared header; vptr0 and handle4, size8. Closure import and scalar delete match own body.
#include <new>
#ifndef _MSC_VER
#define __stdcall
#define __declspec(x)
#endif
typedef struct HKEY__* HKEY;
extern "C" long __stdcall RegCloseKey(HKEY);
class CRegistryKey{public:HKEY Handle;CRegistryKey():Handle(0){}virtual ~CRegistryKey(){if(Handle){RegCloseKey(Handle);Handle=0;}}};
void* MakeHolder(){return new CRegistryKey;}
static int closed,deleted,errors;static void* expected;static void* expectedAllocation;
extern "C" long __stdcall RegCloseKey(HKEY h){if((void*)h!=expected)++errors;++closed;return 0;}
void operator delete(void* p) throw(){if(p==expectedAllocation)++deleted;free(p);}
int main(){int token;expected=&token;CRegistryKey* p=static_cast<CRegistryKey*>(MakeHolder());expectedAllocation=p;p->Handle=(HKEY)expected;delete p;bool ok=closed==1&&deleted==1&&!errors;p=static_cast<CRegistryKey*>(MakeHolder());expectedAllocation=p;delete p;ok=ok&&closed==1&&deleted==2;union Storage{double align;unsigned char bytes[sizeof(CRegistryKey)];}storage;p=new(storage.bytes) CRegistryKey;p->Handle=(HKEY)expected;expectedAllocation=storage.bytes;p->~CRegistryKey();ok=ok&&closed==2&&deleted==2&&!errors;puts(ok?"00c739c0_TEST PASS":"00c739c0_TEST FAIL");return ok?0:1;}