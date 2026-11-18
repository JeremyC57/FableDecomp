// std::__pop_heap(pair<long, C3DMeshVolume>* first, last, result, pair<..> value, CKeyPairCompareLess<long, C3DMeshVolume>, int*) -- STLport pop step: move the top to *result, then sift value down from the root
// Leaf: ??$__pop_heap@PAU?$pair@JVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@H@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@00U10@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71B40, length 159 bytes (`ret 1Ch`: result, the 0x10-byte value, the comparator and the
// distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CGameCameraManager::PushCameraMode" is propagated and wrong; the callee 0x00A71220 carries the manifest name
// `_Adjust_heap<std::pair<long,CCountedPointer<CNavConnectorNode> >*,...,CKeyPairCompareLess<...>>`, so this is the matching __pop_heap (byte-identical twin 0x00A34420).
// Element: 0x10 bytes = the key + a 12-byte {CCharString, CCountedPointer<CNavConnectorNode>} record whose assignment is the out-of-line
// 0x00A32960 (label "CScriptGameResourceObjectScriptedThingBase::operator=": CCharString::operator= then the counted-pointer assignment;
// byte-identical to the pair<CCharString, CCountedPointer> assignment 0x00CBA700). The manifest's 12-byte pair<key, CCountedPointer> does
// not fit the 16-byte stride (`sar 4`) or the string copy, so the record is modelled from the bytes.
// Callees: the record's operator= @ 0x00A32960, CCharString::CCharString(const CCharString&) @ 0x0099EC30, __adjust_heap @ 0x00A71220,
// the count block's destroy function, operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the pair's IMPLICIT assignment (key inline, record out of line) and copy constructor (string out of line, counted pointer
// inline: copy both words, add-ref through the re-read block) are inlined; the by-value `value` is destroyed by this callee: the
// counted pointer's destructor (release with the block re-read, then object and block zero stores) and the string's.
// NF87 (2026-11-17): the STLport pair<long, C3DMeshVolume> / global CKeyPairCompareLess<long, C3DMeshVolume> instance (engine/C3DMeshVolume.h,
//   engine/CVectorMap.h) of NF85 0x00A34420 (the unsigned long twin), by explicit instantiation (FABLE_RETAIL_STLP_HEAP); its out-of-line
//   __adjust_heap 0x00A71220 is landed under exactly that spelling. The old SKeyedPair / SNamedPointer stand-ins are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>,CKeyPairCompareLess<long,C3DMeshVolume>,int*);
