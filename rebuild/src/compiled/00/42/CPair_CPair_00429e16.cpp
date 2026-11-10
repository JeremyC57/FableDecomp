// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429E16, 19 bytes. Catalog name: CMapStringToString::CPair::~CPair.
// Body: destroy the member at +0x4 (CCountedPointer<NUISystem::CComponent>::~CCountedPointer @ 0x004291DE), then tail-call
// the +0x0 member CCharString::~CCharString @ 0x0099EAE0. __thiscall, this = ecx.
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
// +0x4: CCountedPointer<NUISystem::CComponent> (the NUISystem::CManager bastard-child map value; ~CCountedPointer 0x004291DE,
// called out of line here: the declaration-only destructor opt-in).
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
#pragma pack(push, 1)
struct CObject_00429e16 {
    CCharString First;                          // +0x0
    CCountedPointer<NUISystem::CComponent> Second;   // +0x4
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_00429e16::DestroyMembers()
{
    Second.~CCountedPointer();
    First.~CCharString();
}
