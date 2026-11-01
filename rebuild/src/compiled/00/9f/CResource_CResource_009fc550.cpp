// CResource::~CResource() -- the virtual destructor of the resource base: unlink from the owning resource list when there is one (CResourceList::RemoveResource), then the inline ~CIVCountedPointeeBase
// Leaf: ??1CResource@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC550, 31 bytes, thiscall, plain `ret`.
// Identity (naming wave 20, chunk 688): FableWin.pdb ??1CResource@@UAE@XZ; engine/CResource.h (RTTI CResource : CIVCountedPointeeBase; vtable
//   0x012354A4 stored at entry, the base's 0x012354D0 at the end by the inline base destructor); the +8 member is ResourceList (Ego_r.pdb)
//   and the thiscall it makes with `this` pushed is ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z 0x009FC020 (landed; both name sets).
//   Every def destructor tail-jumps here after its CDefPointeeBase vtable store (218 landed rows bind this spelling). Was the ~CMemoryBlock
//   stand-in `CMemoryBlock_dtor_009fc550` (a __fastcall free function over raw vtable stores).
// Callees: CResourceList::RemoveResource @ 0x009FC020.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"                        // : CIVCountedPointeeBase (inline virtual destructor)

class CResourceList {
public:
    void RemoveResource(CResource& resource);        // 0x009FC020
};

CResource::~CResource()
{
    if (ResourceList != 0)
        ResourceList->RemoveResource(*this);
}
