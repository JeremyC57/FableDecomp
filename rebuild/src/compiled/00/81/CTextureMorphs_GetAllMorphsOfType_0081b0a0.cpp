// 0081b0a0 CTextureMorphs::GetAllMorphsOfType -- copy the selected morph type's complete entries
// Leaf: ?GetAllMorphsOfType@CTextureMorphs@@QBE_NW4EHeroMorphType@@AAV?$vector@VCTextureMorphEntry@@V?$allocator@VCTextureMorphEntry@@@std@@@std@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
enum EHeroMorphType { };
enum ECompositeBlendType { };
class CTextureMorphEntry {
public:
    EHeroMorphType MorphType;
    long TextureLayer;
    long ReplacingTextureIndex;
    long BankIndex;
    ECompositeBlendType Blend;
};
#include "engine/CArray.h"
template<class K,class V>class CKeyPairCompareLess { };
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {
public:
    C Compare;bool Dirty;
    typedef const std::pair<K,V>* const_iterator;
    const_iterator LowerBound(const K&)const;
    const_iterator UpperBound(const K&)const;
};
class CTextureMorphs {
public:
    CVectorMap<EHeroMorphType,CTextureMorphEntry> Morphs;
    bool GetAllMorphsOfType(EHeroMorphType,std::vector<CTextureMorphEntry>&)const;
};
bool CTextureMorphs::GetAllMorphsOfType(EHeroMorphType type,std::vector<CTextureMorphEntry>& out)const {
    CVectorMap<EHeroMorphType,CTextureMorphEntry>::const_iterator first=Morphs.LowerBound(type);
    CVectorMap<EHeroMorphType,CTextureMorphEntry>::const_iterator last=Morphs.UpperBound(type);
    if(first!=last) {
        long count=last-first;
        out.resize(count);
        for(long i=0;i<count;++i) *(out.begin()+i)=first[i].second;
        return true;
    }
    return false;
}