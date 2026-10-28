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
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
struct CEvent_00c5eca0{public:void* Handle;CEvent_00c5eca0():Handle(0){}virtual ~CEvent_00c5eca0(){if(Handle){CloseHandle(Handle);Handle=0;}}};
void* MakeHolder(){return new CEvent_00c5eca0;}