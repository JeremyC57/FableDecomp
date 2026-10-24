// _global::CopyBackward_UIMiniMapMarker_00613390(CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, CTCInventoryMap::CUIMiniMapMarker*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the inventory map's mini-map markers
// Leaf: CopyBackward_UIMiniMapMarker_00613390
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00613390, length 130 bytes (+ int3 padding).
// SGI-STL random-access `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the moved-back result is returned.
// Element (Ego_r.pdb): CTCInventoryMap::CUIMiniMapMarker, 0x1C bytes = long Region +0, CCharString UIDefName +4,
// C2DVector Position +8, long DefNameLength +0x10, long GraphicIndex +0x14, float Angle +0x18. The implicit operator=
// copies Region, calls CCharString::operator= @ 0x0099EFB0 for UIDefName, then the five plain words.
struct CCharString {                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct C2DVector {
    float X, Y;
};

struct CUIMiniMapMarker {                    // CTCInventoryMap::CUIMiniMapMarker, 0x1C bytes
    long        Region;                      // +0x00
    CCharString UIDefName;                   // +0x04
    C2DVector   Position;                    // +0x08
    long        DefNameLength;               // +0x10
    long        GraphicIndex;                // +0x14
    float       Angle;                       // +0x18
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CUIMiniMapMarker* __fastcall CopyBackward_UIMiniMapMarker_00613390(CUIMiniMapMarker* first, CUIMiniMapMarker* last, CUIMiniMapMarker* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}