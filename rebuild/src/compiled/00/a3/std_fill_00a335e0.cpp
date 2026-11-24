// std::fill(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, const pair<unsigned long, C3DMeshVolume>& val) -- assign val to every keyed mesh volume of [first, last)
// Leaf: ??$fill@PAU?$pair@KVC3DMeshVolume@@@std@@U12@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@0ABU10@@Z
// Name fix (NF383, 2026-11-24): was the pair<long,C3DMeshVolume> instance (J). Retail has no ICF; the only callers 0x00A350B1 / 0x00A3510B are in
//   0x00A35010 vector<pair<unsigned long,C3DMeshVolume> >::_M_fill_insert (landed leaf, K), reached from resize 0x00A35430 <- resize 0x00A35580 <-
//   C3DMeshStats::LoadBinary 0x00A2F2D0, whose FableWin body calls vector<pair<unsigned long,C3DMeshVolume> >::resize; FableWin lib_3d_mesh_stats.obj
//   defines this K instance (72 bytes, debug). Table binder 00a35010 already binds the K spelling here.
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe @ 0x00A335E0, 144 bytes, `ret 4`: __fastcall (first ecx -> ebp, last edx -- spilled to a frame word --
// and the value reference on the stack).
// Types (Ego_r.pdb): C3DMeshVolume (12 bytes) = Name CCharString +0, ConvexVolume CCountedPointer<CContainmentVolume>
//   (Data +4, Info +8); the pair is 16 bytes (first unsigned long +0, second +4).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, operator delete @ 0x00BFE9BC (the count block).
// Shape: the pair's implicit copy assignment is memberwise: the key (the value reference re-read from its stack slot,
// `mov eax,[esp+1Ch]`), Name's operator= call (argument = &val.second, hoisted into ebx), then the inlined
// CCountedPointer operator=: both source words read into locals before the block compare (Data spilled to the frame),
// the old block released (count decrement, re-read, destroy hook with the object in ecx, delete), the new words stored
// and the new block's count bumped. The count-block address is a second induction register (`lea esi,[ebp+0Ch]`).
#include "engine/CCountedPointer.h"


class CCharString { public:
    void* m_pStringData;                        // +0x00
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CContainmentVolume;
class C3DMeshVolume { public:                          // 12 bytes
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<unsigned long, C3DMeshVolume> CMeshVolumeEntry;
template void __fastcall fill(CMeshVolumeEntry*, CMeshVolumeEntry*, const CMeshVolumeEntry&);
}