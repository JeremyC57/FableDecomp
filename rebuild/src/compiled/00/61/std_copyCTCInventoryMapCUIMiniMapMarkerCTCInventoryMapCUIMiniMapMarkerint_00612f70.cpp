// _global::Copy_UIMiniMapMarker_00612f70(const CUIMiniMapMarker* first, const CUIMiniMapMarker* last, CUIMiniMapMarker* result, const random_access_iterator_tag&, int*) -- assign [first, last) of inventory mini-map markers onto result (region, name string, position, three words), return the advanced result
// Leaf: Copy_UIMiniMapMarker_00612f70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00612F70, length 133 bytes (`ret 0Ch`).
// Identity: `__copy<CTCInventoryMap::CUIMiniMapMarker*>` -- the element matches Ego_r.pdb exactly (0x1c bytes, the string
//   assignment 0x0099EFB0 at +4); the catalog `_Move` is the Dinkumware spelling. Callers: CTCInventoryMap::CreateMiniMapMarkers
//   0x00603000 and the marker vector helpers.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The count lives in the dead `result` argument slot; result in ebp, the string pair addressed as [esi-4]/[edi-4].
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

class CCharString { public:                            // 4-byte counted string handle
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C2DVector { public: float X, Y; };

namespace CTCInventoryMap {
class CUIMiniMapMarker { public:                       // Ego_r.pdb, 0x1c bytes
    long        Region;                         // +0x00
    CCharString UIDefName;                      // +0x04
    C2DVector   Position;                       // +0x08
    long        DefNameLength;                  // +0x10
    long        GraphicIndex;                   // +0x14
    float       Angle;                          // +0x18
};
}
using CTCInventoryMap::CUIMiniMapMarker;

CUIMiniMapMarker* __fastcall Copy_UIMiniMapMarker_00612f70(const CUIMiniMapMarker* first, const CUIMiniMapMarker* last, CUIMiniMapMarker* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}