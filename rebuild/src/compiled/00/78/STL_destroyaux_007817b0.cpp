// std::__destroy_aux(NFire::CFirePoint* first, NFire::CFirePoint* last, const __false_type&) -- destroy a run of 44-byte fire points: each point's singe-effect and effect intelligent pointers, then its helper name
// Leaf: ??$__destroy_aux@PAVCFirePoint@NFire@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007817B0, 50 bytes (+14 int3), `ret 4` (link-time __fastcall: first in ecx, last in
//   edx, the __false_type tag reference on the stack, never read).
// Identity: the element destroy of CTCFlammable::FirePoints (vector<NFire::CFirePoint>, CTCFlammable vtable 0x0126B1A4):
//   callers are CTCFlammable's fire-point setup 0x007808B0 / 0x007817F0, its vtable slot 8 0x007819F0, the vector's
//   insert COMDAT 0x00781A80 and the component destructor 0x00781CE0 (landed as ~CEngineInternalPrimitiveMeshBase_00781ce0,
//   whose vector at +0x10 is FirePoints).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (twice), CCharString::~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb NFire::CFirePoint (44 bytes): HelperName CCharString +0, HelperIndex +4, EffectIndex +8,
//   Resistance +0xC, Burning +0x10, FireDamageAfterFrame +0x14, PParentThing +0x18, PEffect CIntelligentPointer<CThing>
//   +0x1C, PSingeEffect CIntelligentPointer<CThing> +0x24.
// Shape: STLport 4.x `for (; first != last; ++first) _Destroy(&*first);` with the implicit ~CFirePoint inlined: members in
//   reverse order, each intelligent pointer's generated destructor reduced to the direct base-destructor call.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the element is NFire::CFirePoint.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();              // 0x00A01C10
    void* PData;                                     // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class CThing;

namespace NFire {
class CFirePoint {                                   // 44 bytes
public:
    CCharString                 HelperName;          // +0x00
    long                        HelperIndex;         // +0x04
    long                        EffectIndex;         // +0x08
    long                        Resistance;          // +0x0C
    bool                        Burning;             // +0x10
    long                        FireDamageAfterFrame;// +0x14
    CThing*                     PParentThing;        // +0x18
    CIntelligentPointer<CThing> PEffect;             // +0x1C
    CIntelligentPointer<CThing> PSingeEffect;        // +0x24
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __destroy_aux<NFire::CFirePoint*>(NFire::CFirePoint*, NFire::CFirePoint*, const __false_type&);
}