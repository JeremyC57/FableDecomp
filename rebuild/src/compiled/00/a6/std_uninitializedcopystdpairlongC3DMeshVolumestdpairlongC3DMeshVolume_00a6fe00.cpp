// std::__uninitialized_copy(std::pair<long,C3DMeshVolume>* first, std::pair<long,C3DMeshVolume>* last, std::pair<long,C3DMeshVolume>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@JVC3DMeshVolume@@@std@@PAU12@@std@@YIPAU?$pair@JVC3DMeshVolume@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6FE00, 99 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a72cc0 std::vector<std::pair<long, C3DMeshVolume>, std::allocator<std::pair<long, C3DMeshVolume>>>::_M_insert_overflow; caller 00a72cc0 std::vector<std::pair<long, C3DMeshVolume>, std::allocator<std::pair<long, C3DMeshVolume>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAU?$pair@JVC3DMeshVolume@@@std@@PAU12@@std@@YIPAU?$pair@JVC3DMeshVolume@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,C3DMeshVolume>.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

#include "engine/CCountedPointer.h"

class CContainmentVolume;

class C3DMeshVolume { public:                         // Ego_r.pdb, 12 bytes
    CCharString Name;                          // +0x0
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x4
};
template std::pair<long,C3DMeshVolume>* std::__uninitialized_copy<std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>* >(std::pair<long,C3DMeshVolume>*, std::pair<long,C3DMeshVolume>*, std::pair<long,C3DMeshVolume>*, std::__false_type const&);
