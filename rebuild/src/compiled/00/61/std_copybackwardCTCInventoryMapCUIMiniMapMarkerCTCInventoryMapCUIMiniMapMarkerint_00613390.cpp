// std::__copy_backward(CTCInventoryMap::CUIMiniMapMarker* first, CTCInventoryMap::CUIMiniMapMarker* last, CTCInventoryMap::CUIMiniMapMarker* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@H@std@@YIPAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00613390, 130 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00618860 source (stdvector_Mfillinsert_00618860.cpp) names CTCInventoryMap::CUIMiniMapMarker; binder ??$__copy_backward@PAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@H@std@@YIPAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTCInventoryMap::CUIMiniMapMarker.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

#include "engine/C2DVector.h"
class CTCInventoryMap {
public:
class CUIMiniMapMarker {
public:                    // CTCInventoryMap::CUIMiniMapMarker, 0x1C bytes
    long        Region;                      // +0x00
    CCharString UIDefName;                   // +0x04
    C2DVector   Position;                    // +0x08
    long        DefNameLength;               // +0x10
    long        GraphicIndex;                // +0x14
    float       Angle;                       // +0x18
};
};
template CTCInventoryMap::CUIMiniMapMarker* std::__copy_backward<CTCInventoryMap::CUIMiniMapMarker*,CTCInventoryMap::CUIMiniMapMarker*,int >(CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, std::random_access_iterator_tag const&, int*);
