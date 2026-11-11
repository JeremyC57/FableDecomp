// std::__uninitialized_fill_n(CMeshDataBank::CLipSyncEntry* first, unsigned int n, CMeshDataBank::CLipSyncEntry const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCLipSyncEntry@CMeshDataBank@@IV12@@std@@YIPAVCLipSyncEntry@CMeshDataBank@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2A390, 48 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a2d3b0 std::vector<CMeshDataBank::CLipSyncEntry, std::allocator<CMeshDataBank::CLipSyncEntry>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVCLipSyncEntry@CMeshDataBank@@IV12@@std@@YIPAVCLipSyncEntry@CMeshDataBank@@PAV12@IABV12@ABU__false_type@0@@Z
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
template CMeshDataBank::CLipSyncEntry* std::__uninitialized_fill_n<CMeshDataBank::CLipSyncEntry*,unsigned int,CMeshDataBank::CLipSyncEntry >(CMeshDataBank::CLipSyncEntry*, unsigned int, CMeshDataBank::CLipSyncEntry const&, std::__false_type const&);
