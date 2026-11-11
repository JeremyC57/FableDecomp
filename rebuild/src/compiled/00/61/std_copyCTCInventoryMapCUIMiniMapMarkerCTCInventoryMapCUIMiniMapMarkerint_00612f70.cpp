// std::__copy(CTCInventoryMap::CUIMiniMapMarker* first, CTCInventoryMap::CUIMiniMapMarker* last, CTCInventoryMap::CUIMiniMapMarker* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@H@std@@YIPAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00612F70, 133 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00616d50 std::vector<CTCInventoryMap::CUIMiniMapMarker, std::allocator<CTCInventoryMap::CUIMiniMapMarker>>::erase; binder ??$__copy@PAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@H@std@@YIPAVCUIMiniMapMarker@CTCInventoryMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTCInventoryMap::CUIMiniMapMarker.
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
public:// Ego_r.pdb, 0x1c bytes
    long        Region;                         // +0x00
    CCharString UIDefName;                      // +0x04
    C2DVector   Position;                       // +0x08
    long        DefNameLength;                  // +0x10
    long        GraphicIndex;                   // +0x14
    float       Angle;                          // +0x18
};
};
template CTCInventoryMap::CUIMiniMapMarker* std::__copy<CTCInventoryMap::CUIMiniMapMarker*,CTCInventoryMap::CUIMiniMapMarker*,int >(CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, std::random_access_iterator_tag const&, int*);
