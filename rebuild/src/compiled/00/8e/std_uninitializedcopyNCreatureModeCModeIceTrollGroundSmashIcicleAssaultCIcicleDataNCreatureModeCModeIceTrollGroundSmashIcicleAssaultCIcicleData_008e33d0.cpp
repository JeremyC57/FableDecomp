// std::__uninitialized_copy(NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData* first, NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData* last, NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCIcicleData@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@PAV123@@std@@YIPAVCIcicleData@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@PAV123@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E33D0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 008e3aa0 std::vector<CIcicleData>::_M_insert_overflow; callee copy ctor CIcicleData::CIcicleData; Ego_r/FableWin vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData.
#include "stlport/fable_containers.h"
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault {
public:
class CIcicleData {
public:
    unsigned char _body[0x24];
    CIcicleData(const CIcicleData& other);           // 0x008E2FC0
};
};
}
template NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData* std::__uninitialized_copy<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData*,NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData* >(NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData*, NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData*, NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData*, std::__false_type const&);
