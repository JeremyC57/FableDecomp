extern "C" int puts(const char*);
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

static int fixture_resize_error=0;
namespace std {
template<class T,class A> void vector<T,A>::resize(unsigned int count,const T& value) {
    if(count>(unsigned int)(this->_M_end_of_storage._M_data-this->_M_start)) { fixture_resize_error=1;return; }
    T* p=this->_M_finish;
    for(;p<this->_M_start+count;++p) *p=value;
    this->_M_finish=this->_M_start+count;
}
}
template<class K,class V,class C> typename CVectorMap<K,V,C>::const_iterator CVectorMap<K,V,C>::LowerBound(const K& k)const {
    const std::pair<K,V>* p=this->_M_start;
    for(;p!=this->_M_finish && p->first<k;++p) { }
    return p;
}
template<class K,class V,class C> typename CVectorMap<K,V,C>::const_iterator CVectorMap<K,V,C>::UpperBound(const K& k)const {
    const std::pair<K,V>* p=this->_M_start;
    for(;p!=this->_M_finish && !(k<p->first);++p) { }
    return p;
}
static bool fixture_equal(const CTextureMorphEntry& a,const CTextureMorphEntry& b) {
    return a.MorphType==b.MorphType && a.TextureLayer==b.TextureLayer && a.ReplacingTextureIndex==b.ReplacingTextureIndex && a.BankIndex==b.BankIndex && a.Blend==b.Blend;
}
int main() {
    if(sizeof(CTextureMorphEntry)!=20 || sizeof(std::pair<EHeroMorphType,CTextureMorphEntry>)!=24 || sizeof(std::vector<CTextureMorphEntry>)!=12) return 1;
    typedef std::pair<EHeroMorphType,CTextureMorphEntry> Entry;
    Entry entries[5];int keys[5]={1,2,2,2,4};
    for(int i=0;i<5;++i) {
        entries[i].first=(EHeroMorphType)keys[i];
        entries[i].second.MorphType=(EHeroMorphType)(10+i);
        entries[i].second.TextureLayer=20+i;
        entries[i].second.ReplacingTextureIndex=30+i;
        entries[i].second.BankIndex=40+i;
        entries[i].second.Blend=(ECompositeBlendType)(50+i);
    }
    CTextureMorphEntry storage[8];
    for(int i=0;i<8;++i) storage[i]=CTextureMorphEntry();
    std::vector<CTextureMorphEntry> out;
    out._M_start=storage;out._M_finish=storage;out._M_end_of_storage._M_data=storage+8;
    CTextureMorphs owner;
    owner.Morphs._M_start=entries;owner.Morphs._M_finish=entries+5;owner.Morphs._M_end_of_storage._M_data=entries+5;
    if(!owner.GetAllMorphsOfType((EHeroMorphType)2,out) || out._M_finish!=storage+3 || fixture_resize_error) return 2;
    for(int i=0;i<3;++i) if(!fixture_equal(storage[i],entries[i+1].second)) return 3;
    CTextureMorphEntry saved=storage[0];
    if(owner.GetAllMorphsOfType((EHeroMorphType)3,out) || out._M_finish!=storage+3 || !fixture_equal(storage[0],saved)) return 4;
    if(!owner.GetAllMorphsOfType((EHeroMorphType)4,out) || out._M_finish!=storage+1 || !fixture_equal(storage[0],entries[4].second)) return 5;
    owner.Morphs._M_finish=entries;
    if(owner.GetAllMorphsOfType((EHeroMorphType)1,out) || out._M_finish!=storage+1) return 6;
    puts("0081b0a0_TEST PASS");return 0;
}