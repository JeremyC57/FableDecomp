// std::__pop_heap(pair<pair<long,long>,CEngineTextureDecal>* first, .. last, .. result, pair<..> value, CKeyPairCompareLess<pair<long,long>,CEngineTextureDecal> comp, int*) -- STLport pop_heap core for the decal heap: *result = *first, then sift `value` down from the root
// Leaf: ??$__pop_heap@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@U12@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@H@std@@YIXPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@0@00U10@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@PAH@Z
// Retail: Fable.exe @ 0x00BEEDA0, 147 bytes, `ret 28h` (result, the 0x1C-byte value, the empty comparator and the distance pointer
//   on the stack; first/last in ecx/edx: the STLport algorithm TUs are /Gr, `__fastcall` on the template definition).
//   Callers: the sort_heap/partial_sort loops at 0x00BEEF7A, 0x00BEF16A, 0x00BEF402, 0x00BEF7C8 (the decal map's
//   vector<pair<pair<long,long>, CEngineTextureDecal> > sort). The catalog label _Cons_val<pair<CCharString,C3DVector>> is propagated.
// Callees: vector<CCompositeBlendDesc>::operator= 0x006CADE0 and copy constructor 0x005BB346 (the pair's implicit assignment and
//   copy), __adjust_heap 0x00BEEA00 (bound spelling), free 0x00BFEA14 (the by-value value's vector storage).
// Layout: Ego_r.pdb CCompositeBlendDesc (16 bytes) and CEngineTextureDecal (OriginalTextureIndex +0, DecalID +4, OverlaidBlends +8;
//   retail 12-byte STLport vector), so the pair is 0x1C bytes (`last - first` divides by 28 through the 0x92492493 multiply).
// Shape (chunk 892): STLport 4.6 (stlport/fable_containers.h FABLE_RETAIL_STLP_HEAP; the row is the explicit instantiation)
//   `*__result = *__first; __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val,
//   __comp);` over the shared stlport/fable_containers.h vector (FABLE_RETAIL_STLP_VECTOR_C: operator= out of line, as retail
//   calls it). The vector copy constructor is not expanded either, so VC7.1 compiles both bodies into this TU, as in retail's:
//   c2 then knows neither captures its argument, and the by-value copy of `value` into the argument block pipelines its loads
//   one store ahead (`mov edx,[esp+18h]` before `sub esp,1Ch`, 0x00BEEDD5). The earlier local model (declaration-only copy
//   constructor of an invented vector struct) left the 8 copy lines paired.
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "engine/CRGBColour.h"
#include "engine/CVectorMap.h"

enum ECompositeBlendType { };                            // Ego_r.pdb enum, values not needed here

class CCompositeBlendDesc {                              // Ego_r.pdb, 16 bytes
public:
    unsigned long       TextureIndex;                    // +0x00
    ECompositeBlendType BlendType;                       // +0x04
    CRGBColour          DiffuseColour;                   // +0x08
    CRGBColour          SpecularColour;                  // +0x0c
};

class CEngineTextureDecal {                              // Ego_r.pdb (retail 12-byte STLport vector: 0x14 bytes)
public:
    long                             OriginalTextureIndex;   // +0x00
    long                             DecalID;                // +0x04
    std::vector<CCompositeBlendDesc> OverlaidBlends;         // +0x08
};

template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<std::pair<long, long>, CEngineTextureDecal>*,
                                                  std::pair<std::pair<long, long>, CEngineTextureDecal>*,
                                                  std::pair<std::pair<long, long>, CEngineTextureDecal>*,
                                                  std::pair<std::pair<long, long>, CEngineTextureDecal>,
                                                  CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal>, int*);