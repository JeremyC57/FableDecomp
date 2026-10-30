// CVolume::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129C840), the volume released (0x00A67460 with 1), then CBaseClass::~CBaseClass (0x0099A2E0) and free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FCD10, length 48 bytes.
// Leaf: ??_GCVolume@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x0129C840) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CVolume : public CBaseClass { public:              // vtable 0x0129C840
    void ReleaseVolume(long flags);                // 0x00A67460
    __forceinline virtual ~CVolume()
    {
        ReleaseVolume(1);
    }
};

void* MakeOne_009fcd10()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CVolume;
}