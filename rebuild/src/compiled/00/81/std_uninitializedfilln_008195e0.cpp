// std::__uninitialized_fill_n(iterator first, iterator last_or_count, iterator_or_value result, const __false_type& tag) -- placement-construct the output range.
// Identity: proven; corrected element CTextureMorphEntry20: 0081B300 texture morph binary transfer -> pair resize0081D920, and CTextureMorphs::GetAllMorphsOfType0081B0A0 -> vector resize0081DAA0 -> fillinsert0081D0B0 -> this fill helper. hand463 and both PDBs confirm actual class and five fields. Prior CMapwhoLevelInfo model was incorrect.
// Layout: no shared CTextureMorphEntry header; PDB class key V, five POD fields.
// Leaf: ??$__uninitialized_fill_n@PAVCTextureMorphEntry@@IV1@@std@@YIPAVCTextureMorphEntry@@PAV1@IABV1@ABU__false_type@0@@Z
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
enum EHeroMorphType { MORPH_TYPE_NONE }; enum ECompositeBlendType { COMPOSITE_BLEND_NONE }; class CTextureMorphEntry { public: EHeroMorphType MorphType; long TextureLayer,ReplacingTextureIndex,BankIndex; ECompositeBlendType Blend; };
namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Iter,class Size,class T> Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { Iter current=first; for(;count>0;--count,++current) _Construct(&*current,value); return current; } }

typedef CTextureMorphEntry Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);