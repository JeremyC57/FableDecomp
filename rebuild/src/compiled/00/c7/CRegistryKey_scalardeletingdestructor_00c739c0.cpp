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
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(HKEY);
class CRegistryKey{public:HKEY Handle;CRegistryKey():Handle(0){}virtual ~CRegistryKey(){if(Handle){RegCloseKey(Handle);Handle=0;}}};
void* MakeHolder(){return new CRegistryKey;}