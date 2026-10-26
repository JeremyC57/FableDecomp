// std::__copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*) -- (CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, int) STLport random-access copy: n = last - first assignments through the element's out-of-line operator=, return the result's end
// Flags: /Gr
// Leaf: ??$__copy@PAVCEmbeddedHighlight@CSubtitleRenderer@@PAV12@H@std@@YIPAVCEmbeddedHighlight@CSubtitleRenderer@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493A5A, 50 bytes (size-optimised TU), __fastcall (/Gr: first ecx, last edx;
//   result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, int>; evidence: its operator= callee 0x004936EC assigns exactly that layout (CWideString::operator= 0x0099B7D0 at +0, dwords +4 / +8 / +0x0C, the byte +0x10) every 0x14 bytes; its callers are agent951's landed vector<CSubtitleRenderer::CEmbeddedHighlight>::erase 0x004940AC (CSubtitleRenderer::ProcessEmbeddedHighlightTags' vector +0xC4) and the out-of-line __copy_ptrs forwarder 0x00493C58 (tag homed at [ebp+0Bh], null distance)
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*,CArray<class_CRandomAppearanceMo" is a propagated / behavioural label (family J070: five 50-byte copy loops, each its own instantiation; retail has no identical-code folding).
// Callees: CSubtitleRenderer::CEmbeddedHighlight::operator=(const CEmbeddedHighlight&) 0x004936EC (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy verbatim; the count is the exact pointer difference (`idiv 0x14`, the
//   divisor by push/pop under /Os) and the result pointer is advanced in its own stack slot.
// Element: CSubtitleRenderer::CEmbeddedHighlight (Ego_r.pdb 0x14 bytes: CWideString Text, long CharIndex, C2DVector Offset, bool Split)
#pragma optimize("s", on)
class CWideString {                                      // Ego_r.pdb 4 bytes
public:
    void* PStringData;                                   // +0x0 CWideStringData*
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
};
class C2DVector {                                        // Ego_r.pdb 8 bytes
public:
    float X;
    float Y;
};
class CSubtitleRenderer {
public:
    class CEmbeddedHighlight {                           // Ego_r.pdb 0x14 bytes
    public:
        CWideString Text;                                // +0x00
        long        CharIndex;                           // +0x04
        C2DVector   Offset;                              // +0x08
        bool        Split;                               // +0x10
        CEmbeddedHighlight& operator=(const CEmbeddedHighlight& other);   // 0x004936EC (implicit; emitted out of line in retail)
    };
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CSubtitleRenderer::CEmbeddedHighlight* __copy<CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, int>(CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, const random_access_iterator_tag&, int*);
}