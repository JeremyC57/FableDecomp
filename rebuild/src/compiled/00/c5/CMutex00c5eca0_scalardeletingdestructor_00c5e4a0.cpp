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
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
struct CMutex_00c5eca0{public:void* Handle;CMutex_00c5eca0():Handle(0){}virtual ~CMutex_00c5eca0(){if(Handle){CloseHandle(Handle);Handle=0;}}};
void* MakeHolder(){return new CMutex_00c5eca0;}