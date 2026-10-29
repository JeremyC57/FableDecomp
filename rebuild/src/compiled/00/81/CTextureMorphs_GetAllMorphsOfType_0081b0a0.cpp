// 0081b0a0 CTextureMorphs::GetAllMorphsOfType -- copy the selected morph type's complete entries
// Leaf: ?GetAllMorphsOfType@CTextureMorphs@@QBE_NW4EHeroMorphType@@AAV?$vector@VCTextureMorphEntry@@V?$allocator@VCTextureMorphEntry@@@std@@@std@@@Z
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
namespace std {
template<class T>class allocator { };
template<class V,class T,class A>class _STLP_alloc_proxy:public A { public:V _M_data; };
template<class T,class A>class _Vector_base { public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage; };
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:
    T* begin() { return this->_M_start; }
    const T* begin()const { return this->_M_start; }
    void resize(unsigned int,const T&);
    void resize(unsigned int n) { resize(n,T()); }
};
template<class K,class V>struct pair { K first;V second; };
}
template<class T>class CArray:public std::vector<T> { };
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