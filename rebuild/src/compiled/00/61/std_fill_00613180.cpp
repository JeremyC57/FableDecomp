// std::fill(CUIMiniMapMarker* first, CUIMiniMapMarker* last, const CUIMiniMapMarker& val) -- assign val to every mini-map marker of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x00613180, 96 bytes, `ret 4`: __fastcall (first ecx, last edx -- spilled to the `push ecx`
// frame word -- and the value reference on the stack).
// Types (Ego_r.pdb / the landed constructor 0x005FA530): CTCInventoryMap::CUIMiniMapMarker (28 bytes) = Region +0,
//   UIDefName CCharString +4, Position C2DVector +8, DefNameLength +0x10, GraphicIndex +0x14, Angle +0x18.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Shape: the marker's implicit copy assignment is memberwise because CCharString declares operator=: Region, the
// string assignment call (its argument &val.UIDefName hoisted into ebp), then the five words after it. VC7.1
// strength-reduces the destination into a second induction register at the Position member (`lea esi,[ebx+8]`; the
// string's `this` is `lea ecx,[esi-4]`) and re-reads `last` from the frame word for the loop test.
class CCharString { public:
    void* m_pStringData;                        // +0x00
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C2DVector { public:
    float X;                                    // +0x00
    float Y;                                    // +0x04
};
class CTCInventoryMap { public:
    class CUIMiniMapMarker { public:                   // 28 bytes
        long        Region;                     // +0x00
        CCharString UIDefName;                  // +0x04
        C2DVector   Position;                   // +0x08
        long        DefNameLength;              // +0x10
        long        GraphicIndex;               // +0x14
        float       Angle;                      // +0x18
    };
};
typedef CTCInventoryMap::CUIMiniMapMarker CUIMiniMapMarker;

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CUIMiniMapMarker*, CUIMiniMapMarker*, const CUIMiniMapMarker&);
}