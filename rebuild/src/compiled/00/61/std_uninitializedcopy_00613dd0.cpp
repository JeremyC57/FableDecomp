// std::__uninitialized_copy(const CTCInventoryMap::CUIMiniMapMarker* first, const CTCInventoryMap::CUIMiniMapMarker* last, CTCInventoryMap::CUIMiniMapMarker* result, const __false_type&) -- STLport placement copy of mini-map markers (region, UI def-name string, position, name length, graphic index, angle); returns the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00613DD0, 84 bytes (+12 int3), `ret 8` (/Gr TU: first in ecx -> esi, last in
//   edx -> ebx; result and the empty tag reference on the stack).
// Callers: the CUIMiniMapMarker vector growth / insert helpers 0x006182F0, 0x00618860 (four call sites) of
//   CTCInventoryMap::MiniMapMarkers.
// Callee: CCharString::CCharString(const CCharString&) 0x0099EC30 (the UIDefName member).
// Label correction: the catalog `_Copy_backward<CExplosionRing*,CExplosionRing*>` is propagated; the `ret 8`, the
//   placement null test and the forward stride name STLport `__uninitialized_copy(first, last, result, __false_type)`,
//   and the 0x1C-byte element with a string at +4 is PDB CTCInventoryMap::CUIMiniMapMarker.
// Shape: speed TU (the loop head is aligned with a `nop`); the marker's implicit copy constructor is inlined: Region,
//   then the out-of-line string copy, then the five plain words addressed off the two cursors ([esi+N] -> [edi+N]).
//   C2DVector needs its user copy constructor (`X(other.X), Y(other.Y)`): with a POD C2DVector VC7.1 merges +8..+0x1C
//   into one block copy and strength-reduces two extra cursors (`lea esi,[ebx+8] ; add edi,8`, 111 bytes v 84).
// Model: CCharString is modelled locally (fable_string.h inlines its copy constructor; retail calls 0x0099EC30).
#include <new>

class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};

class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

class CTCInventoryMap { public:
    class CUIMiniMapMarker { public:                      // 0x1C bytes (Ego_r.pdb)
        long        Region;                        // +0x00
        CCharString UIDefName;                     // +0x04
        C2DVector   Position;                      // +0x08
        long        DefNameLength;                 // +0x10
        long        GraphicIndex;                  // +0x14
        float       Angle;                         // +0x18
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

typedef CTCInventoryMap::CUIMiniMapMarker TMarker;
template TMarker* __fastcall __uninitialized_copy<const TMarker*, TMarker*>(const TMarker*, const TMarker*, TMarker*,
                                                                           const __false_type&);
}