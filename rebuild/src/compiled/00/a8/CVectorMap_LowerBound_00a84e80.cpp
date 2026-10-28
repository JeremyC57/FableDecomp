// CVectorMap::LowerBound(const RetailUID& key) const -- Return the first entry not less than an unsigned 64-bit key.
// Leaf: ?LowerBound@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QBEPBU?$pair@_K_N@std@@AB_K@Z
// Identity: Own CNavSwitchableLeafNode::IsSwitchableOn00a84e40 is original PDB const virtual API; UniqueIDsOpen+32 original map<UID,bool>. Use const LowerBound/const iterator, not mutable placeholder.
// Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.
#ifdef _MSC_VER
typedef unsigned __int64 RetailUID;
#else
typedef unsigned long RetailUID;
#endif
class CANavQuadTreeNode;class CActiveEntityScriptBase;class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
namespace std{template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& item,const K& key)const{return item.first<key;}};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap{public:typedef std::pair<K,V> value_type;typedef value_type* iterator;typedef const value_type* const_iterator;CArray<value_type> Data;C Compare;bool Dirty;const_iterator LowerBound(const K& key) const{const_iterator first=Data._M_start;int count=Data._M_finish-first;C comp;while(count>0){int half=count/2;const_iterator middle=first+half;if(comp(*middle,key)){first=middle+1;count-=half+1;}else count=half;}return first;}};
typedef CVectorMap<RetailUID,bool > Map;
template Map::const_iterator Map::LowerBound(const RetailUID&) const;