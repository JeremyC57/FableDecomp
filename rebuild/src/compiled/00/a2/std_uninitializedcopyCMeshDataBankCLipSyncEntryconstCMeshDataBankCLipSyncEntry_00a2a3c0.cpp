// std::__uninitialized_copy(CMeshDataBank::CLipSyncEntry const* first, CMeshDataBank::CLipSyncEntry const* last, CMeshDataBank::CLipSyncEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCLipSyncEntry@CMeshDataBank@@PAV12@@std@@YIPAVCLipSyncEntry@CMeshDataBank@@PBV12@0PAV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2A3C0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a2b970 std::vector<CMeshDataBank::CLipSyncEntry, std::allocator<CMeshDataBank::CLipSyncEntry>>::vector; binder ??$__uninitialized_copy@PBVCLipSyncEntry@CMeshDataBank@@PAV12@@std@@YIPAVCLipSyncEntry@CMeshDataBank@@PBV12@0PAV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMeshDataBank::CLipSyncEntry.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

class CMeshDataBank {
public:
class CLipSyncEntry {
public:
    CCharString first;
    long second;
};
};
template CMeshDataBank::CLipSyncEntry* std::__uninitialized_copy<CMeshDataBank::CLipSyncEntry const*,CMeshDataBank::CLipSyncEntry* >(CMeshDataBank::CLipSyncEntry const*, CMeshDataBank::CLipSyncEntry const*, CMeshDataBank::CLipSyncEntry*, std::__false_type const&);
