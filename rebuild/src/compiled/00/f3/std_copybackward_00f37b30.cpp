// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (CGPUScratchBufferTextureManager::CResourceEntry*, CGPUScratchBufferTextureManager::CResourceEntry*, int) STLport random-access backward copy of trivially-assigned records: n = last - first block copies `*--result = *--last`, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@H@std@@YIPAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37B30, 68 bytes (+ int3 padding), __fastcall (/Gr: first ecx, last edx; result,
//   the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (probable): std::__copy_backward<CGPUScratchBufferTextureManager::CResourceEntry*, CGPUScratchBufferTextureManager::CResourceEntry*, int>; evidence: unreferenced; it sits in CGPUScratchBufferTextureManager's object code among the 0x2C-stride helpers of its Resources (Ego_r.pdb CArray<CGPUScratchBufferTextureManager::CResourceEntry>, 44-byte entries): the erase 0x00F37EE0 and the _M_fill_insert 0x00F381F0 under the resize 0x00F385A0 that CGPUScratchBufferTextureManager::AllocResourceEntry 0x00F372B0 calls; both PDB name sets hold vector<CGPUScratchBufferTextureManager::CResourceEntry>::resize.
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is a propagated Dinkumware spelling (family J101: five backward copies of 0x24- to
//   0x34-byte records; retail has no identical-code folding).
// Callees: none (the 0x2C-byte assignment is one `rep movsd` of 11 dwords).
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim (J078's model over a trivially-assigned element); the
//   count is a division by 0x2C (multiply by 0x2E8BA2E9).
enum EResourceType { };                                   // Ego_r.pdb (enumerators not needed)
enum ESurfaceMultisampleType { };
enum ESurfacePool { };
class CTexture;
class CSurface;
class C2DExtentsI {                                        // 8 bytes (Ego_r.pdb)
public:
    long X;                                                // +0x00
    long Y;                                                // +0x04
};
class CPixelFormat {                                       // 4 bytes (Ego_r.pdb)
public:
    long FormatIndex;                                      // +0x00
};
class CGPUScratchBufferTextureManager {                    // partial view: only the nested type is used here
public:
    struct CResourceEntry {                                // 44 bytes (Ego_r.pdb; struct key U from both PDB name sets)
        EResourceType ResourceType;                        // +0x00
        CTexture* Texture;                                 // +0x04
        CSurface* Surface;                                 // +0x08
        long PageIndex;                                    // +0x0C
        C2DExtentsI Dimensions;                            // +0x10
        long NoLevels;                                     // +0x18
        CPixelFormat PixelFormat;                          // +0x1C
        ESurfaceMultisampleType MultiSampleType;           // +0x20
        unsigned long UseageFlags;                         // +0x24
        ESurfacePool MemoryPool;                           // +0x28
    };
};
namespace std {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef CGPUScratchBufferTextureManager::CResourceEntry TElement_00f37b30;
template TElement_00f37b30* __copy_backward<TElement_00f37b30*, TElement_00f37b30*, int>(TElement_00f37b30*, TElement_00f37b30*, TElement_00f37b30*,
                                                                         const random_access_iterator_tag&, int*);
}