// std::__uninitialized_copy(CLipSyncPhonemeSample* first, CLipSyncPhonemeSample* last, CLipSyncPhonemeSample* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCLipSyncPhonemeSample@@PAV1@@std@@YIPAVCLipSyncPhonemeSample@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A49BC0, 70 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a4ac60 source (stdvector_Minsertoverflow_00a4ac60.cpp) names CLipSyncPhonemeSample; binder ??$__uninitialized_copy@PAVCLipSyncPhonemeSample@@PAV1@@std@@YIPAVCLipSyncPhonemeSample@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncPhonemeSample.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

#include "engine/CCountedPointer.h"

struct CCountedObject;
class CLipSyncPhonemeSample {
public:                    // 16 bytes
    CCharString Name;                          // +0x0
    long Value;                                // +0x4
    CCountedPointer<CCountedObject> Object;    // +0x8
};
template CLipSyncPhonemeSample* std::__uninitialized_copy<CLipSyncPhonemeSample*,CLipSyncPhonemeSample* >(CLipSyncPhonemeSample*, CLipSyncPhonemeSample*, CLipSyncPhonemeSample*, std::__false_type const&);
