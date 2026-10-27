// std::fill(CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts* first, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts* last, const CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts& value) -- STLport fill: assign value to every element of [first, last)
// Leaf: ??$fill@PAVCMeshMorphSetParts@CMeshMorphSet@CTCAppearanceMorphBase@@V123@@std@@YIXPAVCMeshMorphSetParts@CMeshMorphSet@CTCAppearanceMorphBase@@0ABV123@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00728c10, 46 bytes, /Gr __fastcall (first in ecx, last in edx, &value on the stack), `ret 4`.
// Identity (probable): the element's assignment is the leading dword then vector<CEngineTextureMorph>::operator= 0x00728830 (bound; family L032) on +4, 16-byte stride = Ego_r.pdb CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts {unsigned long MeshIndex; CEngineTextureMorphSet TextureMorphs} (the only 16-byte {scalar, CArray<CEngineTextureMorph>} record there); its caller is the fill insert 0x00728DF0 of that vector (resize 0x00728F70).
// Callee: the second member's operator= 0x00728830 (thiscall).
// Shape (family L052): `for (; first != last; ++first) *first = value;` -- the element's implicit assignment copies the
//   leading dword through eax and calls the member assignment with &value + 4 kept in ebp; stride 16.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // out of line
};
}
template <class T> class CArray : public std::vector<T> { };   // retail CArray<T> = the 12-byte STLport vector
class CEngineTextureMorph;
class CEngineTextureMorphSet {                             // Ego_r.pdb, 12 bytes in retail
public:
    CArray<CEngineTextureMorph> Morphs;                   // +0x00
};
class CTCAppearanceMorphBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {                         // Ego_r.pdb, 16 bytes in retail
        public:
            unsigned long MeshIndex;                      // +0x00
            CEngineTextureMorphSet TextureMorphs;         // +0x04
        };
    };
};
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
template void __fastcall std::fill(CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, const CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts&);