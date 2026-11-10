// std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >::~pair() -- destroy the value (ReleaseData of the counted component pointer), then tail-jump the key's ~CCharString
// Leaf: ??1?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429E16, 19 bytes, thiscall: `lea ecx,[esi+4] ; call 0x004291DE ; mov ecx,esi ; jmp 0x0099EAE0`.
// Identity: FableWin.pdb ??1?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAE@XZ (Ego_r has the ??_G of the
//   same pair): the value type of NUISystem::CManager's BastardChildren map<CCharString, CCountedPointer<CComponent> > (the landed
//   map operator[] 0x0042D45D and ClearBastardChild 0x0041DED2). Was landed as a descriptive CObject_00429e16::DestroyMembers (catalog
//   CMapStringToString::CPair::~CPair, propagated).
// Callees: CCountedPointer<NUISystem::CComponent>::ReleaseData 0x004291DE (the inline destructor's call; the destructor COMDAT is the
//   5-byte jmp 0x00429C53), CCharString::~CCharString 0x0099EAE0.
// Model: STLport 4.6 pair (struct, first/second); the empty user destructor stands for the implicit one, which an explicit
//   instantiation does not emit. engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: this speed TU would
//   otherwise expand the 44-byte release (as the earlier FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY did for the destructor).
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    ~pair() { }
};
}
template struct std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >;
