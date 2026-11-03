// Self-contained production header expansion, active branches only.
// Existing CDefClassBase/CParentDefClassBase packing is preserved verbatim.
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct ScriptDefFixture;
static void* trackedAlloc(size_t);
static void trackedFree(void*);
class CCharStringData;
class CCharString {friend struct ScriptDefFixture;
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
class CCharString;
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
private:
 long TablePos;
};
#include <stddef.h>
#include <stdlib.h>
#include <new>
#include <string.h>
namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
};
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)trackedAlloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) trackedFree(p); }
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {friend struct ::ScriptDefFixture;
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
    void resize(size_type n) { resize(n,_Tp()); }
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
};
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
template<class T> class CArray : public std::vector<T,std::allocator<T> > {
};
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
#include <stddef.h>
#include <stddef.h>
#include <stddef.h>
#include <stddef.h>
#include <stddef.h>
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    mutable unsigned long IVRefCount;
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();
    virtual ~CResource();
    virtual bool UnloadResource();
    virtual unsigned long GetUnloadableMemory() const;
    virtual bool IsResourceLoaded() const;
    virtual void OnResourceLoaded();
    virtual bool CanAbortResourceLoad(long) const;
    virtual void OnResourceLoadAborted();
    virtual unsigned long GetDebugMemoryInUse() const;
    virtual void ResetDebugMemoryInUse();
private:
    CResourceList* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned long ResourceSize;
    unsigned long LastUsedFrame;
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }
};
class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
private:
    CDefClassBase();
    friend class CParentDefClassBase;
    friend class CSubDefClassBase;
public:
    class CSubDefInfo {
    public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefClassBase(const CDefClassBase&);
    CDefClassBase& operator=(const CDefClassBase&);
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }
    virtual ~CDefClassBase() { }
    virtual bool UnloadResource();
    virtual CDefString GetInstantiationName() const;
    virtual void SetInstantiationName(CDefString);
    virtual void AddSubDef(const CCharString&, long, long);
    virtual void RemoveSubDef(const CCharString&);
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const;
    virtual bool IsParentDef() const;
    virtual void PostTransfer();
    virtual void Transfer(CPersistContext&) = 0;
    virtual void Copy(const CDefClassBase*) = 0;
    virtual unsigned long GetSizeofClass() = 0;
    virtual void InternalSaveBinary(CDataOutputStream&);
    virtual void InternalLoadBinary(CDataInputStream&);
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();
    CParentDefClassBase(const CParentDefClassBase&);
    CParentDefClassBase& operator=(const CParentDefClassBase&);
    virtual CDefString GetInstantiationName() const;
    virtual void SetInstantiationName(CDefString);
    virtual void AddSubDef(const CCharString&, long, long);
    virtual void RemoveSubDef(const CCharString&);
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const;
    virtual bool IsParentDef() const;
    virtual void InternalSaveBinary(CDataOutputStream&);
    virtual void InternalLoadBinary(CDataInputStream&);
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();
    CDefString InstantiationName;
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase {
public:
    CSubDefClassBase();
    virtual ~CSubDefClassBase() { }
};
enum EFeatAttackType { FAT_ATTACK_ANY = 0, FAT_ATTACK_SWORD = 1, FAT_ATTACK_BOW = 2, FAT_ATTACK_HANDS = 3, FAT_ATTACK_WILL = 4 };
class CFeatDef : public CSubDefClassBase {
public:
    CFeatDef();
    CFeatDef(const CFeatDef&);
    CFeatDef& operator=(const CFeatDef&);
    virtual ~CFeatDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    CDefString FeatName;
    CDefString Verb;
    float TimeLimit;
    long TargetNumber;
    long GoldReward;
    long XPReward;
    CDefString ItemReward;
    bool NoBlocking;
    EFeatAttackType KN_AttackType;
    bool KN_Perfect;
    long KN_CreatureType;
    CDefString GF_FromRegion;
    CDefString GF_ToRegion;
    bool GF_NoTeleporting;
    CDefString CO_ItemName;
};
class CFireHeartPatternDef : public CSubDefClassBase {
public:
    CFireHeartPatternDef();
    CFireHeartPatternDef(const CFireHeartPatternDef&);
    CFireHeartPatternDef& operator=(const CFireHeartPatternDef&);
    virtual ~CFireHeartPatternDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    std::vector<bool,std::allocator<bool> > PadActive;
};
class CConversation : public CSubDefClassBase {
public:
    CConversation();
    CConversation(const CConversation&);
    CConversation& operator=(const CConversation&);
    virtual ~CConversation();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long Lines;
    std::vector<CCharString,std::allocator<CCharString> > Speaker;
    std::vector<CCharString,std::allocator<CCharString> > Dialogue;
    std::vector<CCharString,std::allocator<CCharString> > Animation;
    std::vector<CCharString,std::allocator<CCharString> > AnimLoop;
};
class CHeroSoulsWaveDef;
class CHeroSoulsRoundDef : public CSubDefClassBase {
public:
    CHeroSoulsRoundDef();
    CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);
    CHeroSoulsRoundDef& operator=(const CHeroSoulsRoundDef&);
    virtual ~CHeroSoulsRoundDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long NumWaves;
    std::vector<CHeroSoulsWaveDef,std::allocator<CHeroSoulsWaveDef> > Waves;
};
class CArenaWaveDef;
class CArenaRoundDef : public CSubDefClassBase {
public:
    CArenaRoundDef();
    CArenaRoundDef(const CArenaRoundDef&);
    CArenaRoundDef& operator=(const CArenaRoundDef&);
    virtual ~CArenaRoundDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long NumWaves;
    std::vector<CArenaWaveDef,std::allocator<CArenaWaveDef> > Waves;
};
class CScriptDef : public CParentDefClassBase {
public:
    CScriptDef();
    CScriptDef(const CScriptDef&);
    CScriptDef& operator=(const CScriptDef&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long EXPERIENCE_GRANT_SMALL;
    long EXPERIENCE_GRANT_MEDIUM;
    long EXPERIENCE_GRANT_LARGE;
    long EXPERIENCE_GRANT_XLARGE;
    long EXPERIENCE_GRANT_HEROIC;
    long NPC_COMMENT_FREQUENCY;
    long NPC_COMMENT_DISTANCE;
    long AmbushTradersNakedBoastCost;
    long AmbushTradersNakedBoastReward;
    long AmbushTradersNoDamageBoastCost;
    long AmbushTradersNoDamageBoastReward;
    long AmbushTradersNoWeaponsBoastCost;
    long AmbushTradersNoWeaponsBoastReward;
    long ArenaNakedBoastCost;
    long ArenaNakedBoastReward;
    long ArenaNoDamageBoastCost;
    long ArenaNoDamageBoastReward;
    long ArenaNoWeaponsBoastCost;
    long ArenaNoWeaponsBoastReward;
    long BanditCampNoDamageBoastCost;
    long BanditCampNoDamageBoastReward;
    long BanditCampNoWeaponsBoastCost;
    long BanditCampNoWeaponsBoastReward;
    long BountyHuntNakedBoastCost;
    long BountyHuntNakedBoastReward;
    long BountyHuntNoDamageBoastCost;
    long BountyHuntNoDamageBoastReward;
    long BreakSiegeNakedBoastCost;
    long BreakSiegeNakedBoastReward;
    long BreakSiegeNoDamageBoastCost;
    long BreakSiegeNoDamageBoastReward;
    long BreakSiegeNoWeaponsBoastCost;
    long BreakSiegeNoWeaponsBoastReward;
    long GraveyardNakedBoastCost;
    long GraveyardNakedBoastReward;
    long GraveyardNoDamageBoastCost;
    long GraveyardNoDamageBoastReward;
    long GraveyardNoWeaponsBoastCost;
    long GraveyardNoWeaponsBoastReward;
    long HobbeCaveNakedBoastCost;
    long HobbeCaveNakedBoastReward;
    long HobbeCaveNoDamageBoastCost;
    long HobbeCaveNoDamageBoastReward;
    long HobbeCaveNoWeaponsBoastCost;
    long HobbeCaveNoWeaponsBoastReward;
    long HTContestNakedBoastCost;
    long HTContestNakedBoastReward;
    long HTContestNoDamageBoastCost;
    long HTContestNoDamageBoastReward;
    long HTContestNoWeaponsBoastCost;
    long HTContestNoWeaponsBoastReward;
    long HTEvilNakedBoastCost;
    long HTEvilNakedBoastReward;
    long HTEvilNoDamageBoastCost;
    long HTEvilNoDamageBoastReward;
    long HTEvilNoWeaponsBoastCost;
    long HTEvilNoWeaponsBoastReward;
    long HTGoodNakedBoastCost;
    long HTGoodNakedBoastReward;
    long HTGoodNoDamageBoastCost;
    long HTGoodNoDamageBoastReward;
    long HTGoodNoWeaponsBoastCost;
    long HTGoodNoWeaponsBoastReward;
    long MCCNakedBoastCost;
    long MCCNakedBoastReward;
    long MCCNoDamageBoastCost;
    long MCCNoDamageBoastReward;
    long MCCNoWeaponsBoastCost;
    long MCCNoWeaponsBoastReward;
    long MinionCampNakedBoastCost;
    long MinionCampNakedBoastReward;
    long MinionCampNoDamageBoastCost;
    long MinionCampNoDamageBoastReward;
    long MinionCampNoWeaponsBoastCost;
    long MinionCampNoWeaponsBoastReward;
    long OFEvilNakedBoastCost;
    long OFEvilNakedBoastReward;
    long OFEvilNoDamageBoastCost;
    long OFEvilNoDamageBoastReward;
    long OFEvilNoWeaponsBoastCost;
    long OFEvilNoWeaponsBoastReward;
    long OFGoodNakedBoastCost;
    long OFGoodNakedBoastReward;
    long OFGoodNoDamageBoastCost;
    long OFGoodNoDamageBoastReward;
    long OFGoodNoWeaponsBoastCost;
    long OFGoodNoWeaponsBoastReward;
    long PrisonNakedBoastCost;
    long PrisonNakedBoastReward;
    long PrisonNoDamageBoastCost;
    long PrisonNoDamageBoastReward;
    long PrisonNoWeaponsBoastCost;
    long PrisonNoWeaponsBoastReward;
    long RansomVictimNakedBoastCost;
    long RansomVictimNakedBoastReward;
    long RansomVictimNoDamageBoastCost;
    long RansomVictimNoDamageBoastReward;
    long RansomVictimNoWeaponsBoastCost;
    long RansomVictimNoWeaponsBoastReward;
    long TraderConflictEvilNakedBoastCost;
    long TraderConflictEvilNakedBoastReward;
    long TraderConflictEvilNoDamageBoastCost;
    long TraderConflictEvilNoDamageBoastReward;
    long TraderConflictEvilNoWeaponsBoastCost;
    long TraderConflictEvilNoWeaponsBoastReward;
    long TraderConflictGoodNakedBoastCost;
    long TraderConflictGoodNakedBoastReward;
    long TraderConflictGoodNoDamageBoastCost;
    long TraderConflictGoodNoDamageBoastReward;
    long TraderConflictGoodNoWeaponsBoastCost;
    long TraderConflictGoodNoWeaponsBoastReward;
    long TraderEscortNakedBoastCost;
    long TraderEscortNakedBoastReward;
    long TraderEscortNoDamageBoastCost;
    long TraderEscortNoDamageBoastReward;
    long TraderEscortNoWeaponsBoastCost;
    long TraderEscortNoWeaponsBoastReward;
    long WhiteBalvNakedBoastCost;
    long WhiteBalvNakedBoastReward;
    long WhiteBalvNoDamageBoastCost;
    long WhiteBalvNoDamageBoastReward;
    long WhiteBalvNoWeaponsBoastCost;
    long WhiteBalvNoWeaponsBoastReward;
    long SummoningShipNakedBoastCost;
    long SummoningShipNakedBoastReward;
    long SummoningShipNoDamageBoastCost;
    long SummoningShipNoDamageBoastReward;
    long TCGKillNoBanditsCost;
    long TCGKillNoBanditsReward;
    long TCGMadeTimeLimitCost;
    long TCGMadeTimeLimitReward;
    long TCEKeepBanditFollowerAliveCost;
    long TCEKeepBanditFollowerAliveReward;
    long TCEMadeTimeLimitCost;
    long TCEMadeTimeLimitReward;
    long RansomVictimVictimNoDamageCost;
    long RansomVictimVictimNoDamageReward;
    long RansomVictimKillKidnapperCost;
    long RansomVictimKillKidnapperReward;
    long RansomVictimSaveVictimCost;
    long RansomVictimSaveVictimReward;
    long RansomVictimHaveVictimKilledCost;
    long RansomVictimHaveVictimKilledReward;
    long MinionCampBriarNoDamageCost;
    long MinionCampBriarNoDamageReward;
    float AmbushScamTricksterApproachProximityLow;
    float AmbushScamTricksterApproachProximityHigh;
    float AmbushScamTricksterHeroProximity;
    float AmbushScamTricksterRunAwayProximity;
    float AmbushScamAmbushTriggerProximity;
    float AmbushScamApproachTriggerProximity;
    float AmbushScamBuddyWanderMax;
    float AmbushScamTricksterWanderMax;
    long AmbushScamGoldReward;
    CDefString AmbushScamBonusObject;
    long AmbushScamExperience;
    long AmbushScamRenown;
    float AmbushScamMoralityEvil;
    float AmbushScamMoralityGood;
    long AmbushScamRandomSpeechTimerMax;
    long AmbushScamRandomSpeechTimerVar;
    std::vector<CDefString,std::allocator<CDefString> > AmbushScamScreamSoundIds;
    float AmbushScamScreamProximity;
    long ArcheryCompetitionCost;
    long ArcheryGameTimeLimit;
    float ArcheryTimesUpDuration;
    long ArcheryInitialHighScoreMax;
    long ArcheryInitialHighScoreVar;
    float ArcheryScoreForPrize;
    float ArcheryScoreForPrizeHigh;
    float ArcheryScoreForPrizeMedium;
    float ArcheryScoreForPrizeLow;
    float ArcheryMaxOutsideRing;
    long ArcheryDummySpeedFast;
    long ArcheryDummySpeedMedium;
    long ArcheryDummySpeedSlow;
    long ArcheryNumberOfMoves;
    CDefString ArcheryRewardObject;
    std::vector<CDefString,std::allocator<CDefString> > ArcheryNormalPrizes;
    std::vector<CDefString,std::allocator<CDefString> > ArcheryNormalPrizesHigh;
    std::vector<CDefString,std::allocator<CDefString> > ArcheryNormalPrizesMedium;
    std::vector<CDefString,std::allocator<CDefString> > ArcheryNormalPrizesLow;
    std::vector<CDefString,std::allocator<CDefString> > ArcheryMatchHighScorePrizes;
    std::vector<CDefString,std::allocator<CDefString> > ArcheryNewHighScorePrizes;
    std::vector<float,std::allocator<float> > ArcheryScorePowerLevels;
    CDefString ArcheryReallyHighScorePrize;
    long ArcheryReallyHighScore;
    long ArcheryExperience;
    long ArcheryRenown;
    float ArcheryMoralityCheatUsedMagic;
    float ArcheryMoralityCheatSteppedOut;
    float ArcheryMoralityShotOwner;
    long ArcheryScoreDummyStatic;
    long ArcheryScoreDummyFront;
    long ArcheryScoreDummyCentre;
    long ArcheryScoreDummyLeft;
    long ArcheryScoreDummyRight;
    long ArcheryRandomSpeechTimerMax;
    long ArcheryRandomSpeechTimerVar;
    long AssassinActivateRenownLevel;
    long AssassinTriggerProximity;
    long AssassinScreamProximity;
    long AssassinChestProximity;
    long AssassinCameraOffScreenTime;
    std::vector<CDefString,std::allocator<CDefString> > AssassinScreamSoundIds;
    long AssassinReward;
    float AssassinRewardDelay;
    std::vector<CCharString,std::allocator<CCharString> > AssassinObjects;
    std::vector<CCharString,std::allocator<CCharString> > AssassinLocationName;
    std::vector<long,std::allocator<long> > AssassinRegionKillCount;
    long AssassinExperience;
    long AssassinRenown;
    float AssassinMorality;
    long BanditCampPathEavesDropProximity;
    long BanditCampKillNoBanditsBoastCost;
    long BanditCampKillNoBanditsBoastReward;
    long BanditCampKillManyBanditsBoastCost;
    long BanditCampKillManyBanditsBoastReward;
    long KillTraderTriggerDistance;
    long TollBanditCallHeroDistance;
    long TollBanditWarningOverDistance;
    long TollBanditHeroLeftDistance;
    long TollBanditPayDistance;
    long TollBanditCallHeroTimer;
    long TollBanditWarnHeroTimer;
    long TollBanditTauntTimer;
    long BanditTollSneakDistance;
    long BanditTollBanditWanderDistance;
    long BanditTollTraderWarnDistance;
    long TollBanditFee;
    long TollBanditFeeWithFollower;
    CDefString TollBanditCreatureType;
    float TollBanditCloseToExit;
    float TollBanditTimeToFade;
    long BanditCountForToll;
    float BeardyBaldyWanderDistanceMin;
    float BeardyBaldyWanderDistanceMax;
    CDefString BeardyBaldyHaircutStyle1;
    CDefString BeardyBaldyHaircutCard1;
    CDefString BeardyBaldyHaircutStyle2;
    CDefString BeardyBaldyHaircutCard2;
    CDefString BeardyBaldyBeardStyle1;
    CDefString BeardyBaldyBeardCard1;
    CDefString BeardyBaldyBeardStyle2;
    CDefString BeardyBaldyBeardCard2;
    CDefString BeardyBaldyTashStyle1;
    CDefString BeardyBaldyTashCard1;
    CDefString BeardyBaldyTashStyle2;
    CDefString BeardyBaldyTashCard2;
    CDefString BeardyBaldyRewardObjectFullComplete;
    CDefString BeardyBaldyRewardObjectHalfComplete;
    long BeardyBaldyExperience;
    long BeardyBaldyRenown;
    float BeardyBaldyMorality;
    long BeardyBaldyRandomSpeechTimerMax;
    long BeardyBaldyRandomSpeechTimerVar;
    long HenchmanHealthLow;
    long HenchmanHealthCritical;
    long HenchmanPayTimeDelayInSeconds;
    long HenchmanCheapGoldCost;
    long HenchmanExpensiveGoldCost;
    std::vector<CCharString,std::allocator<CCharString> > BookBoyMarker;
    std::vector<CCharString,std::allocator<CCharString> > BookGirlMarker;
    std::vector<CConversation,std::allocator<CConversation> > BookReactions;
    std::vector<CCharString,std::allocator<CCharString> > BookDefNames;
    long BooksNumberWanted;
    long BooksNumberAccepted;
    long BooksNumberComment;
    long BooksNumberTotal;
    long BadBooksForHat;
    long GoodBooksForHat;
    float BooksMoralityReward;
    float BordelloHeroTavernPriceMultiplier;
    long ChapelActivationAge;
    float ChapelAltarProximity;
    float ChapelAcolyteWanderDistance;
    long ChapelAcolytePrayerTimer;
    long ChapelAcolytePrayerTimerVar;
    std::vector<float,std::allocator<float> > ChapelDonationLevel;
    std::vector<float,std::allocator<float> > ChapelMoralityRewardFraction;
    CCharString ChapelDonationRewardObject;
    float ChapelWeaponBrandishProximity;
    float ChapelDonationLevelCoolThing1;
    float ChapelDonationLevelCoolThing2;
    float ChapelMoralityChangeCoolThing1;
    float ChapelMoralityChangeCoolThing2;
    long ChapelDonationRewardAgeMin;
    long ChapelDonationRewardAgeVar;
    CCharString ChapelDonationRewardTitle;
    float ChapelDonationLevelFiddleSuper;
    float ChapelDonationLevelFiddleVery;
    float ChapelDonationLevelFiddle;
    long ChapelExperience;
    long ChapelRenown;
    float ChapelMorality;
    long DealerAttentionDistance;
    long DealerAttentionTimer;
    long TraderLeavingDelay;
    long TraderAttentionDistance;
    long TraderConversationDelay;
    long TraderStopWalkingTimer;
    long TraderStandStillTimer;
    long EasyPickingsReward;
    CCharString ExposeMayorEvidenceDef;
    float EMMaxFlashDistFromStables;
    long F_FishermanHelpDelay;
    long F_FishermanIdleChatDelay;
    CDefString F_WaspType;
    float FishermanSaveHimMorality;
    long FishTypesInGame;
    long FishingRodsInGame;
    long FishingPrizes;
    std::vector<CCharString,std::allocator<CCharString> > FishPrizeDefNames;
    std::vector<CCharString,std::allocator<CCharString> > FishRodDefNames;
    std::vector<CCharString,std::allocator<CCharString> > FishDefNames;
    std::vector<float,std::allocator<float> > FishPrizeWeights;
    long FC_FighterDelay;
    long FC_MinimumFighterHealth;
    float FC_OutsideRingRange;
    float FC_RingTimeOut;
    long FC_BowerstoneAllowedHits;
    long FC_OakvaleAllowedHits;
    long FC_BanditCampAllowedHits;
    long FC_KnotholeGladeAllowedHits;
    long FC_HitsDepletionOverSession;
    long FC_BowerstoneFightCount;
    long FC_OakvaleFightCount;
    long FC_BanditCampFightCount;
    long FC_KnotholeGladeFightCount;
    long FC_StartTimeOfDay;
    long FC_EndTimeOfDay;
    long FC_MoneyAddedPerFight;
    float FC_RingRadius;
    long FC_InsideRingMessageDelay;
    long FC_AttackWarnings;
    CDefString FC_BowerstoneRewardItem;
    CDefString FC_OakvaleRewardItem;
    CDefString FC_BanditCampRewardItem;
    CDefString FC_KnotholeGladeRewardItem;
    float FC_DistanceAwayToTriggerFC;
    long FC_MinimumFee;
    float FC_ShoutOutDistance;
    long FC_LostRenownLoss;
    long FC_WinRenownGain;
    long FC_WinExperienceGain;
    long GGN_GhostSpawnNearby;
    long GGN_GhostSpawnDistance;
    long GGN_ReturnNecklaceReward;
    long GGN_GhostClueTime;
    long GGN_GhostTalkDistance;
    float GGN_ReturnNecklaceMoralityGain;
    long GGN_ReturnNecklaceExperienceGain;
    float HH_UpperBarLimit;
    float HH_BarDecrementAmount;
    float HH_BarDecrementInterval;
    long HH_ClearedHHRenown;
    float HeroDuelDefeatThunderMorality;
    long HeroDuelDefeatThunderXP;
    CDefString HiddenBootyTreasureFake;
    CDefString HiddenBootyTreasureReal;
    long HiddenBootyExperience;
    long HiddenBootyRenownReal;
    long HiddenBootyRenownFake;
    float HiddenBootyMorality;
    long TrophiesOpinionDeedTimer;
    CDefString TrophiesBriarRoseTrophyLeft;
    CDefString TrophiesBriarRoseTrophyRight;
    float TrophiesBriarRoseProximityNear;
    float TrophiesBriarRoseProximityFar;
    float TrophiesStandStillTimer;
    float TrophiesFacingWatcherTimer;
    float TrophiesBriarRoseWalkingSpeed;
    float TrophiesCrowdWanderDistanceMin;
    float TrophiesCrowdWanderDistanceMax;
    long TrophiesBriarRoseAnimTime;
    long TrophiesBriarRoseAnimTimeVar;
    long TrophiesInsideTavernTimer;
    long TrophiesOutsideTavernTimer;
    long TrophiesHitCountForBriarAttack;
    float OuterTraderDistance;
    float MiddleTraderDistance;
    float InnerTraderDistance;
    long TraderYellForHelpRandomValue;
    long TraderYellAtHeroRandomValue;
    long TraderCommentRandomValue;
    float BrotherStartDistance;
    long BrotherScreamForHelpRandomValue;
    long BrotherWaitCommentRandomValue;
    float HeroEvilMoralityValue;
    float HeroGoodMoralityValue;
    float HeroKillsAssassinMorality;
    float HeroKillsSheriffMorality;
    float HeroTakesAssassinGoldMorality;
    float HeroTakesAssassinGoldMoralityKillsAssassin;
    long HeroKillsAssassinRenown;
    CDefString HeroKillsAssassinProofOfKill;
    long HeroKillsSheriffRenown;
    float AssassinThreatDistSq;
    long RandomPopulationTraderGoldMinimum;
    long RandomPopulationTraderGoldVariation;
    float RockTrollTriggerProximity;
    CDefString RockTrollReward1;
    CDefString RockTrollReward2;
    CDefString IngredientObject;
    CDefString ObjectToGiveToMansLover;
    CDefString ObjectFromMansLover;
    CDefString EndPotion;
    CDefString SickChildSleepingAnim;
    std::vector<CDefString,std::allocator<CDefString> > SickChildsSisterAttentionAnims;
    float TellingTheTruth;
    float LyingToWomanMorality;
    float LyingToManAboutWomanNotLikingHer;
    float LyingToManAboutWomanLikingHer;
    float KilledSomeoneForMushroom;
    float StoleMushroomInBarrowFields;
    float BoughtMushroomInBarrowFields;
    float MadeWomanLaugh;
    float SingingStonesDoorManWanderMax;
    float SwordInTheStoneWanderDistanceMin;
    float SwordInTheStoneWanderDistanceMax;
    long SwordInTheStoneSuccessExperience;
    long SwordInTheStoneSuccessRenown;
    float SwordInTheStoneSuccessMorality;
    long SwordInTheStonePhysiqueChange;
    long SwordInTheStoneToughnessChange;
    long SwordInTheStoneHealthChange;
    long BardSingingFee;
    long BardNumVersesPerVisit;
    float BardConsiderEvil;
    float BardConsiderGood;
    long BardConsiderRich;
    long BardConsiderBoaster;
    long BardConsiderFailure;
    long BardSpecial;
    long BardGoAwayCost;
    float BardWanderDistanceMin;
    float BardWanderDistanceMax;
    long BardWanderResetTime;
    float BardFollowDistance;
    long BardRandomSpeechTimerMax;
    long BardRandomSpeechTimerVar;
    long BardHeroAgeHigh;
    long BardHeroAgeMedium;
    float BardHeroHealthHigh;
    float BardHeroHealthLow;
    float BardHeroWillHigh;
    float BardHeroWillLow;
    long TempleActivationAge;
    float TempleAltarProximity;
    float TempleAcolyteWanderDistance;
    long TempleAcolytePrayerTimer;
    long TempleAcolytePrayerTimerVar;
    std::vector<float,std::allocator<float> > TempleDonationLevel;
    std::vector<float,std::allocator<float> > TempleMoralityRewardFraction;
    CCharString TempleDonationRewardObject;
    float TempleDonationLevelCoolThing1;
    float TempleDonationLevelCoolThing2;
    float TempleMoralityChangeCoolThing1;
    float TempleMoralityChangeCoolThing2;
    long TempleDonationRewardAgeMin;
    long TempleDonationRewardAgeVar;
    CCharString TempleDonationRewardTitle;
    float TempleDonationLevelFiddleSuper;
    float TempleDonationLevelFiddleVery;
    float TempleDonationLevelFiddle;
    long TempleExperience;
    long TempleRenown;
    float TempleMorality;
    float TemplePeasantWalkSpeed;
    long TemplePrayerCounterActivation;
    long TempleDonationBoxRequirement;
    float TempleWeaponBrandishProximity;
    float TempleTeleportEnableProximity;
    long TemplePrayerFactorLowest;
    long TemplePrayerFactorLow;
    long TemplePrayerFactorMiddle;
    long TemplePrayerFactorHigh;
    long TemplePrayerFactorHighest;
    std::vector<long,std::allocator<long> > TemplePrayerFactorBurnChance;
    std::vector<float,std::allocator<float> > TemplePrayerWorshipRewardHealth;
    std::vector<float,std::allocator<float> > TemplePrayerWorshipRewardMorality;
    std::vector<long,std::allocator<long> > TemplePrayerWorshipRewardGold;
    std::vector<float,std::allocator<float> > TemplePrayerWorshipRewardDonationModifier;
    std::vector<float,std::allocator<float> > TemplePrayerWorshipBurnHealth;
    std::vector<long,std::allocator<long> > TemplePrayerWorshipBurnGold;
    float TourGuideFollowerDistance;
    float TourGuideOverhearDistance;
    float TourGuideTalkDistance;
    long TourGuideStandStillTimer;
    long TourGuideTalkToHeroTimer;
    long TourGuideFollowerStandStillTimer;
    long TourGuideOpeningTime;
    long TourGuideClosingTime;
    long SlumsBeggarGoldAmount;
    long OakValeBeggarGoldAmount;
    long PickUpLitterTimeInSeconds;
    long NumPiecesOfLitter;
    float TBRMoralityGoodSmall;
    float TBRMoralityGoodMedium;
    float TBRMoralityGoodLarge;
    float TBYMoralityBadSmall;
    float TBYMoralityBadMedium;
    float TBYMoralityBadLarge;
    long OakShopKeepGoldAmount;
    long ArsonGoldAmount;
    long KGExtortAmount;
    CDefString BULLY_WEAPON;
    long TH_ThunderConv_Length;
    std::vector<CCharString,std::allocator<CCharString> > TH_ThunderConv_Speaker;
    std::vector<CCharString,std::allocator<CCharString> > TH_ThunderConv_Dialogue;
    std::vector<CCharString,std::allocator<CCharString> > TH_ThunderConv_Actor;
    std::vector<CCharString,std::allocator<CCharString> > TH_ThunderConv_Animation;
    std::vector<float,std::allocator<float> > TH_ThunderConv_AnimDelay;
    long EscortTraderRewardDownpayment;
    long EscortTraderAttackComplaintTime;
    std::vector<CDefString,std::allocator<CDefString> > EscortTraderRewardObjectPerfect;
    float EscortTraderRegionExitWait;
    float EscortTraderRegionExitStop;
    float EscortTraderRegionExitDist;
    long EscortTraderHitCountHigh;
    long EscortTraderHitCountMedium;
    float EscortTraderHealthHigh;
    float EscortTraderHealthMedium;
    float EscortTraderFollowProximity1;
    float EscortTraderFollowProximity2;
    long EscortTraderProximityTimer;
    long EscortTraderExpressionDetectTimer;
    long EscortTraderHitDetectTimer;
    std::vector<CCharString,std::allocator<CCharString> > EscortTraderSpawnedBanditType;
    long EscortTraderExperience;
    long EscortTraderRenown;
    float EscortTraderMorality;
    long EscortTraderFailureRenown;
    long TraderRandomSpeechTimerMax;
    long TraderRandomSpeechTimerVar;
    float EscortTraderWanderDistance;
    long EscortTraderQuestDayInterval;
    long EscortTraderQuestStartTime;
    long AmbushTradersBanditHireCost;
    float AmbushTradersConvoyExitProximity;
    float AmbushTradersPhase1GuardWanderDistance;
    float AmbushTradersContactWanderDistance;
    float AmbushTradersConvoyTriggerProximity;
    float AmbushTradersSentryWanderDistance;
    float AmbushTradersSpySentryProximity;
    float AmbushTradersSpyHeroProximity;
    float AmbushTradersQuestSuccessReward;
    CDefString AmbushTradersQuestSuccessBonus;
    float AmbushTradersBanditWanderMax;
    long AmbushTradersExperience;
    long AmbushTradersRenown;
    float AmbushTradersMorality;
    long AmbushTradersBoastCostKillAllGuards;
    long AmbushTradersBoastRewardKillAllGuards;
    long AmbushTradersBoastCostKillAllTraders;
    long AmbushTradersBoastRewardKillAllTraders;
    long AmbushTradersBoastCostKillNone;
    long AmbushTradersBoastRewardKillNone;
    long AmbushTradersBoastCostUnassisted;
    long AmbushTradersBoastRewardUnassisted;
    float AmbushTradersBanditFollowDistance;
    float AmbushTradersPhase2GuardWanderDistance;
    float AmbushTradersSpeechBanditProximity;
    float AmbushTradersSpeechSpyProximity;
    long AmbushTradersBanditSpeechTimerLong;
    long AmbushTradersBanditSpeechTimerShort;
    long AmbushTradersSpySpeechTimerProximity;
    long AmbushTradersSpySpeechTimerLong;
    long AmbushTradersSpySpeechTimerShort;
    long AmbushTradersSpySpeechTimerSentry;
    long AmbushTradersInvalidRegionTimerMax;
    long AmbushTradersInvalidRegionTimerVar;
    long ArenaCrowdShoutDelay;
    long ArenaCrowdShoutDelayRandom;
    std::vector<CDefString,std::allocator<CDefString> > RewardObjects;
    long ArenaCountdownDuration;
    CCharString ArenaCreatureRound1;
    long ArenaNumberOrCreaturesRound1;
    long ArenaNumberOfWavesRound1;
    CCharString ArenaCreatureRound2;
    long ArenaNumberOrCreaturesRound2;
    long ArenaNumberOfWavesRound2;
    CCharString ArenaCreatureRound3;
    long ArenaNumberOrCreaturesRound3;
    long ArenaNumberOfWavesRound3;
    CCharString ArenaCreatureRound4;
    long ArenaNumberOrCreaturesRound4;
    long ArenaNumberOfWavesRound4;
    CCharString ArenaCreatureRound5;
    long ArenaNumberOrCreaturesRound5;
    long ArenaNumberOfWavesRound5;
    CCharString ArenaCreatureRound6;
    long ArenaNumberOrCreaturesRound6;
    long ArenaNumberOfWavesRound6;
    CCharString ArenaCreatureRound7;
    long ArenaNumberOrCreaturesRound7;
    long ArenaNumberOfWavesRound7;
    CCharString ArenaCreatureRound8;
    long ArenaNumberOrCreaturesRound8;
    long ArenaNumberOfWavesRound8;
    long ArenaShowNoMercyBoastAmount;
    long ArenaShowMercyBoastAmount;
    long ArenaShowNoMercyBoastReward;
    long ArenaShowMercyBoastReward;
    float ArenaSpareWhisperMorality;
    float ArenaKillWhisperMorality;
    std::vector<CDefString,std::allocator<CDefString> > ATO_DiggingRewards;
    long ATO_MaxSpawnedCreatures;
    long BountyHuntGoldReward;
    float BountyHuntVillagerIntroProximity;
    float BountyHuntVillagerWanderDistanceMin;
    float BountyHuntVillagerWanderDistanceMax;
    float BountyHuntVillagerHeroProximity;
    float BountyHuntVillagerHeroStartConversation;
    float BountyHuntCutsceneTriggerProximity;
    float BountyHuntBanditGruntWanderDistance;
    float BountyHuntBanditLeaderWanderDistance;
    float BountyHuntHostage2WanderDistance;
    float BountyHuntBanditDeputy1WanderDistance;
    float BountyHuntBanditDeputy1AttackProximity;
    float BountyHuntBanditDeputy2WanderDistance;
    float BountyHuntBanditDeputy2AttackProximity;
    float BountyHuntLeaderHealthDiffStage1;
    float BountyHuntLeaderHealthDiffStage2;
    float BountyHuntLeaderHealthDiffStage3;
    float BountyHuntLeaderHealthDiffStage4;
    long BountyHuntExperience;
    long BountyHuntRenownHostage1;
    long BountyHuntRenownHostage2;
    float BountyHuntMorality;
    float BountyHuntCompletionMorality;
    long BountyHuntBoastTimeLimit;
    long BountyHuntBoastCostTimeLimit;
    long BountyHuntBoastRewardTimeLimit;
    long BountyHuntBoastCostDecapitation;
    long BountyHuntBoastRewardDecapitation;
    long BountyHuntRansomGold;
    CDefString BountyHuntRewardObject;
    float BowerstoneIntroGuardProximity;
    float BowerstoneIntroPoshGuardProximity;
    float BowerstoneIntroCrowdWanderDistanceMin;
    float BowerstoneIntroCrowdWanderDistanceMax;
    float BS_BanditAwarenessRange;
    long BS_RewardGold;
    long BS_NoAssistanceBoastAmount;
    long BS_KilledLeaderBoastAmount;
    long BS_NoAssistanceBoastReward;
    long BS_KilledLeaderBoastReward;
    long BS_NoOfWavesInFirstAttack;
    long BS_NoOfBanditsInWave;
    CDefString BS_BanditTypeInFirstAttack;
    CDefString BS_EnvTheme;
    float DBDragonMediumHealth;
    float DBDragonLowHealth;
    float DBDragonVeryLowHealth;
    long DBNumMinionsHighHealth;
    long DBNumMinionsMediumHealth;
    long DBNumMinionsLowHealth;
    long DBNumMinionsVeryLowHealth;
    long DBNumSummonersHighHealth;
    long DBNumSummonersMediumHealth;
    long DBNumSummonersLowHealth;
    long DBNumSummonersVeryLowHealth;
    long DBNumFlyBysBetweenSummonerSpawns;
    float EndGame_UpperBarLimit;
    float EndGame_BarDecrementAmount;
    float EndGame_BarDecrementInterval;
    CDefString EndGame_TeleportEffect;
    long EndGame_TeleportHeroDistance;
    float EndGame_Stage1FX;
    float EndGame_Stage2FX;
    float EndGame_Stage3FX;
    CCharString FHSunPadFX;
    CCharString FHMoonPadFX;
    float FHPadFXHeight;
    float FHPadRadius;
    long FHNumberOfPads;
    long FHNumberOfProphets;
    long FHNumberOfPatterns;
    long FHNumberOfRounds;
    long FHNumberOfSunsEvil;
    long FHNumberOfSunsGood;
    std::vector<CFireHeartPatternDef,std::allocator<CFireHeartPatternDef> > FHPatterns;
    std::vector<long,std::allocator<long> > FHTimeLimit;
    long FHWarnTime;
    long FHNumberOfVillagers;
    float FHHealthChangeWhenZapped;
    float FHMoralityChangeOnKilled;
    float FHMoralityChangeOnFreed;
    float FHFreeAllPrisonersMoralityChange;
    long FHFreeAllPrisonersRenownBoost;
    float FHKillAllPrisonersMoralityChange;
    long FHKillAllPrisonersRenownBoost;
    long GSI_MazeCallsOverDistance;
    long GSI_MazeRegionExitDistance;
    float HS_ScreamerCount;
    long HS_ScreamersAtOnce;
    float HS_ScreamerInc;
    long HS_SecondsBeforeSuperScreamerDamage;
    float HS_SuperScreamerDamagePerSecond;
    long HS_SecondsBeforeNewSuperScreamer;
    long HS_TimePerUndeadKilled;
    long HS_MaxTimeNostroExposed;
    long HS_GMShortDistance;
    long HS_GMLongDistance;
    long HS_GMGuardDistance;
    float HS_GoodSoulTaken;
    long HC_SpawnCountPostReleaseGSon;
    long HC_ReduceCountPostSideRoom;
    float HC_SpawnGenerationRadius;
    long HC_BanditTimeBeforeComment;
    long HC_BanditCallsForHelpDelay;
    long HC_BanditGoldDemand;
    float HC_BanditFadeOut;
    long HC_HobbeSpawnDelay;
    long HC_BanditCommentTime;
    long HC_BanditCloseToMarkerDistance;
    long HC_HobbeGruntDelay;
    long HC_HobbeSpawnCount;
    float HC_RescueBanditMoralityGain;
    long HC_RescueBanditExperienceGain;
    long HC_SacrificeToNymphExperienceGain;
    float HC_SacrificeBanditMoralityLoss;
    float HC_SacrificeToNymphMoralityLoss;
    float HC_LetBanditGoAndPayMoralityGain;
    long HC_LetBanditGoRenown;
    float HC_DontPayBanditMoralityLoss;
    long HC_KillNymphBoastCost;
    long HC_KillNymphBoastReward;
    long HC_SacrificeBoastCost;
    long HC_SacrificeBoastReward;
    long HC_BoyNoHarmBoastCost;
    long HC_BoyNoHarmBoastReward;
    long HC_HobbeMaxNumberInRegion;
    long JackFightDefeatJackXP;
    long MB_MadBomberStartDistance;
    long MB_MadBomberProximityToBomb;
    long MB_MadBomberHeroCloseDelay;
    long MB_MadBomberRunToWalkDistance;
    long MB_BlacksmithCallHeroDistance;
    long MB_BombInitialTimeToBoom;
    long MB_FailMissionBombCount;
    long MB_BombGoesOffRenownLoss;
    long MB_BombDefusedExperience;
    float MB_BombDefusedMorality;
    long MCNumHitsBetweenForcePushes;
    float MCMinMinionSpawnPeriod;
    float MCMaxMinionSpawnPeriod;
    long MCC_GuardTalkingDistance;
    long MCC_MinionRunDistance;
    float MCC_MinionBreakoffDistance;
    float MCC_SavedGuardMoralityGain;
    long MCC_Timer;
    long MCC_MinionCounter;
    float OVBRBanditSightRadius;
    float OVBRBanditSoundRadius;
    float OVI_MoralityChangePerDeed;
    long ExperienceForDefeatingWhisperInOrchardFarm;
    long GuardReinforcementsYourTeam;
    long GuardReinforcementsEnemyTeam;
    long BanditReinforcementDelay;
    long OFE_NoHealthPotionBoastCost;
    long OFE_NoHealthPotionBoastReward;
    long OFE_NoBanditsDieBoastCost;
    long OFE_NoBanditsDieBoastReward;
    long OFG_NoCratesStolenBoastCost;
    long OFG_NoCratesStolenBoastReward;
    long OFG_NoGuardsDieBoastCost;
    long OFG_NoGuardsDieBoastReward;
    float OFEvilCompletionMorality;
    float OFGoodCompletionMorality;
    long PrisonExerciseStartingTimeLimit;
    long PrisonWayPointHitRange;
    long PrisonKeyRackDangerRange;
    long PrisonClothesChestDangerRange;
    long PrisonGuardProximityToAskQuestion;
    long PrisonTrainingPartnerHintDistance;
    long PrisonTrainingPartnerHintDistance2;
    long PrisonDistanceForGuardsToJeer;
    long PrisonRadiusGuardWillAttackHeroWithin;
    long PrisonGuardMumbleAudioDistance;
    float PrisonRacerRunSpeed;
    float PrisonRacerSprintSpeed;
    long PrisonRacerSprintMaxTimeSeconds;
    long PrisonRacerSprintRestTimeSeconds;
    float PrisonWardenGameSneakModifier;
    long PrisonPoetryMaxLines;
    long PrisonExperienceForKillingKraken;
    long PrisonEscapeExperienceFromMother;
    float PrisonFreePrisonerMorality;
    long MotherFollowingHeroKrakenSafetyDistance;
    long TE_TraderCommentDelay;
    long TE_TraderToBalverineDelay;
    long TE_TraderLowInfectionTime;
    long TE_TraderMediumInfectionTime;
    long TE_TraderHighInfectionTime;
    long TE_AllTradersAliveBoastCost;
    long TE_AllTradersAliveBoastReward;
    long TE_BalverineTimeToGoDistance;
    long TE_TraderCommentDistance;
    long TE_AssassinSpawnDistance;
    CDefString TE_AssassinName;
    long TE_EarthTrollTriggerDistance;
    CDefString TE_TrollName;
    long TE_TraderGoldReward;
    float TE_LetTraderFollowMorality;
    float TraderEscortLeaveTraderMorality;
    float WB_MoralityGain;
    long WB_LowHealth;
    long WB_GoldReward;
    CDefString WB_CreaturesSpawned;
    CDefString WB_WaspBossName;
    long WB_ScreamingVillagerDistance;
    long WB_ScreamingVillagerScreamsDistance;
    float WB_ScreamingVillagerFadeOutTime;
    long WB_WaspHelperCallsOutDistance;
    long WB_WaspChaseWomanPanicTime;
    long WB_ExperienceReward;
    float BC_EvilMoralityLoss;
    float BC_GoodMoralityGain;
    float BAC_RenownAward;
    float BAC_HostageMoralityGain;
    float BAC_SingleHostageMoralityLoss;
    float BAC_DoubleHostageMoralityLoss;
    float BAC_AgeAmount;
    float BAC_EnemiesToKillInFirstArea;
    float BAC_EnemiesToKillInSecondArea;
    float BAC_AssassinGoldAmount;
    float BAC_ForgerGoldAmount;
    float BAC_BoastBanditKill;
    float BAC_TheresaExperienceGift;
    float BAC_TwinBladeKilledMorality;
    float BAC_TwinBladeSavedMorality;
    float ENF_TotalTime;
    float ENF_TimeAddition;
    float ENF_TimeSubtraction;
    float EGJ_MoralityLoss;
    float EGJ_MoralityGain;
    long OGSP_KillAllBoastCost;
    long OGSP_KillAllBoastReward;
    std::vector<float,std::allocator<float> > GUI_MeleeGrades;
    std::vector<float,std::allocator<float> > GUI_SkillGrades;
    std::vector<float,std::allocator<float> > GUI_WillGrades;
    float GUI_MinHealth;
    float GUI_RaceTime;
    float GUI_RaceGold;
    float GUI_RearDummySegements;
    float GUI_MiddleDummySegements;
    float GUI_FrontDummySegements;
    float GUI_RearDummyWorth;
    float GUI_MiddleDummyWorth;
    float GUI_FrontDummyWorth;
    float GUI_GoldPerBird;
    float GUI_BirdGoldBonus;
    float GUI_SkillTimer;
    float GUI_WillTimer;
    float GUI_DepartureBeetles;
    float GUI_MeleeBeetles;
    float GUI_LampCost;
    float GUI_WillDummySpinTimer;
    float GUI_EndGuildXP;
    float HOB_TotalTime;
    float HOB_BoastHobbeMany;
    float HOB_RenownAwardSuccess;
    float HOB_RenownAwardDraw;
    float HOB_RenownAwardLost;
    long HOB_KillMoreHobbesBoastCost;
    long HOB_KillMoreHobbesBoastReward;
    long HangingTreeProtectGuardsBoastReward;
    long HangingTreeProtectGuardsBoastAmount;
    float HTGoodCompletionMorality;
    long HTGoodHeroKilledPrisonerGoldReward;
    long HTGoodHeroKilledPrisonerRenownReward;
    long HangingTreeProtectBanditsBoastReward;
    long HangingTreeProtectBanditsBoastAmount;
    long HangingTreeEvilMissionTimer;
    float HTEvilCompletionMorality;
    float HOH_MoralityGain;
    float HOH_MoralityLoss;
    long RV_AcceptedGuardLoweredGoldReward;
    long RV_AcceptedGuardLoweredRenownReward;
    long RV_LowerGoldRewardFromKillingChiefsGuard;
    float RV_HeroEvilMoralityValue;
    float RV_HeroGoodMoralityValue;
    float RV_HeroTellMoralityValue;
    long TCE_TimeLimit;
    long TCG_TimeLimit;
    long TCE_GuardRangeHighest;
    long TCE_GuardRangeHigh;
    long TCE_GuardRangeLow;
    float WBK_NumberHits;
    float WBK_RenownAwardForGates;
    float WBK_RenownAwardForFirstDefence;
    float WBK_RenownAwardForSecondDefence;
    long WizardBattleDefeatMazeXP;
    float CHK_LowPrize;
    float CHK_MidPrize;
    float CHK_HighPrize;
    long TeleporterActivationDist;
    float TimeAdvancePointFadeOutTime;
    float TimeAdvancePointFadeInTime;
    float TimeAdvancePointFadedOutPauseTime;
    float TimeAdvancePointTimeToAdvance;
    long CoreQuestReminderIntervalSeconds;
    long FlourishReminderFrequency;
    float PercentageFlourishesTakenLowerBound;
    long StopTeachingFlourishesAfterDoneThisMany;
    long CombatMultTooLongUnchanged;
    long CombatMultStopBotheringAfter;
    long LowHealthWarning;
    long VeryLowHealthWarning;
    long LowHealthMessageInterval;
    long VeryLowHealthMessageInterval;
    long HealthMessageLimit;
    float LowWillEnergyWarningLevel;
    long LowWillEnergyMessageInterval;
    long WillEnergyMessageLimit;
    long SaveMessageLimit;
    long SaveMessageInterval;
    float FishingMaxDistanceFromSpot;
    float DiggingMaxDistanceFromSpot;
    float PickpocketDurationSeconds;
    float PickpocketSpottedDurationSeconds;
    long PickpocketSpottedChancePerSecond;
    float PicklockDurationSeconds;
    float PicklockSpottedDurationSeconds;
    long PicklockSpottedChancePerSecond;
    float StealDurationSeconds;
    float StealSpottedDurationSeconds;
    long StealSpottedChancePerSecond;
    long NumberOfConcurrentlyAvailableFeats;
    std::vector<CFeatDef,std::allocator<CFeatDef> > Feats;
    std::vector<CArenaRoundDef,std::allocator<CArenaRoundDef> > ArenaRounds;
    std::vector<CHeroSoulsRoundDef,std::allocator<CHeroSoulsRoundDef> > HeroSoulsRounds;
    long DemonDoor_CombatMult_TriggerLevel;
    float DemonDoor_HighRenown_TriggerLevel;
    float DemonDoor_ShineLight_TriggerDistance;
    long DemonDoor_SummonCreature_NumCreatureTypes;
    std::vector<CCharString,std::allocator<CCharString> > DemonDoor_SummonCreature_CreatureType;
    long DemonDoor_BeEvil_TriggerDistance;
    float DemonDoor_BeEvil_EvilnessNeeded;
    float DemonDoor_DoDamage_DamageLimit;
    float DemonDoor_EatPies_FatnessLevel;
};
CScriptDef::CScriptDef() {}
class CArenaCreatureDef;
class CArenaWaveDef : public CSubDefClassBase { public:
 virtual ~CArenaWaveDef();
 virtual void Copy(const CDefClassBase*);
 virtual unsigned long GetSizeofClass();
 virtual void Transfer(CPersistContext&);
 long NumWaveCreatures;
 std::vector<CArenaCreatureDef,std::allocator<CArenaCreatureDef> > Creatures;
 bool ShortWave;
};
class CHeroSoulsCreatureDef;
class CHeroSoulsWaveDef : public CSubDefClassBase { public:
 virtual ~CHeroSoulsWaveDef();
 virtual void Copy(const CDefClassBase*);
 virtual unsigned long GetSizeofClass();
 virtual void Transfer(CPersistContext&);
 long NumWaveCreatures;
 std::vector<CHeroSoulsCreatureDef,std::allocator<CHeroSoulsCreatureDef> > Creatures;
};
static unsigned int allocations,freed,visits,expectedCount;
static void* activePointers[1024];
static void* expectedObjects[4096];
static int expectedKinds[4096];
static bool checking=false;
static void fail(int n){fprintf(stderr,"script definition destruction check %d at %u\n",n,visits);exit(n);}
static void expect(int kind,void* p){if(expectedCount>=4096)fail(90);expectedKinds[expectedCount]=kind;expectedObjects[expectedCount++]=p;}
static void note(int kind,void* p){if(checking){if(visits>=expectedCount||expectedKinds[visits]!=kind||expectedObjects[visits]!=p)fail(91);++visits;}}
static void* trackedAlloc(size_t n){void* p=malloc(n?n:1);if(!p||allocations>=1024)fail(92);activePointers[allocations++]=p;return p;}
static void trackedFree(void* p){if(!p)return;note(2,p);unsigned int i;for(i=0;i<allocations;++i)if(activePointers[i]==p){activePointers[i]=0;++freed;free(p);return;}fail(93);}
void CIVCountedPointeeBase::Delete() const { throw 1; }
CResource::CResource() : ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0) { IVRefCount=0; }
CResource::~CResource() {note(4,this);}
bool CResource::UnloadResource(){throw 1;}
unsigned long CResource::GetUnloadableMemory()const{throw 1;}
bool CResource::IsResourceLoaded()const{throw 1;}
void CResource::OnResourceLoaded(){throw 1;}
bool CResource::CanAbortResourceLoad(long)const{throw 1;}
void CResource::OnResourceLoadAborted(){throw 1;}
unsigned long CResource::GetDebugMemoryInUse()const{throw 1;}
void CResource::ResetDebugMemoryInUse(){throw 1;}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(0),Template(0),DefaultValsApplied(0){}
bool CDefClassBase::UnloadResource(){throw 1;}
CDefString CDefClassBase::GetInstantiationName()const{throw 1;}
void CDefClassBase::SetInstantiationName(CDefString){throw 1;}
void CDefClassBase::AddSubDef(const CCharString&,long,long){throw 1;}
void CDefClassBase::RemoveSubDef(const CCharString&){throw 1;}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){throw 1;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{throw 1;}
bool CDefClassBase::IsParentDef()const{throw 1;}
void CDefClassBase::PostTransfer(){throw 1;}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){throw 1;}
void CDefClassBase::InternalLoadBinary(CDataInputStream&){throw 1;}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&,const CCharString&)const{throw 1;}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){throw 1;}
CParentDefClassBase::CParentDefClassBase(){}
CDefString CParentDefClassBase::GetInstantiationName()const{throw 1;}
void CParentDefClassBase::SetInstantiationName(CDefString){throw 1;}
void CParentDefClassBase::AddSubDef(const CCharString&,long,long){throw 1;}
void CParentDefClassBase::RemoveSubDef(const CCharString&){throw 1;}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){throw 1;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&)const{throw 1;}
bool CParentDefClassBase::IsParentDef()const{throw 1;}
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&){throw 1;}
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&){throw 1;}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap(){throw 1;}
CSubDefClassBase::CSubDefClassBase(){}
CCharString::CCharString():PStringData((CCharStringData*)trackedAlloc(4)){}
CCharString::~CCharString(){note(1,this);trackedFree(PStringData);}
void CScriptDef::Copy(const CDefClassBase*){throw 1;}
unsigned long CScriptDef::GetSizeofClass(){return sizeof(*this);}
void CScriptDef::Transfer(CPersistContext&){throw 1;}
CFeatDef::CFeatDef(){}
CFeatDef::~CFeatDef(){note(3,this);}
void CFeatDef::Copy(const CDefClassBase*){throw 1;}
unsigned long CFeatDef::GetSizeofClass(){return sizeof(*this);}
void CFeatDef::Transfer(CPersistContext&){throw 1;}
CFireHeartPatternDef::CFireHeartPatternDef(){}
CFireHeartPatternDef::~CFireHeartPatternDef(){note(3,this);}
void CFireHeartPatternDef::Copy(const CDefClassBase*){throw 1;}
unsigned long CFireHeartPatternDef::GetSizeofClass(){return sizeof(*this);}
void CFireHeartPatternDef::Transfer(CPersistContext&){throw 1;}
CConversation::CConversation(){}
CConversation::~CConversation(){note(3,this);}
void CConversation::Copy(const CDefClassBase*){throw 1;}
unsigned long CConversation::GetSizeofClass(){return sizeof(*this);}
void CConversation::Transfer(CPersistContext&){throw 1;}
CHeroSoulsRoundDef::CHeroSoulsRoundDef(){}
CHeroSoulsRoundDef::~CHeroSoulsRoundDef(){note(3,this);}
void CHeroSoulsRoundDef::Copy(const CDefClassBase*){throw 1;}
unsigned long CHeroSoulsRoundDef::GetSizeofClass(){return sizeof(*this);}
void CHeroSoulsRoundDef::Transfer(CPersistContext&){throw 1;}
CArenaRoundDef::CArenaRoundDef(){}
CArenaRoundDef::~CArenaRoundDef(){note(3,this);}
void CArenaRoundDef::Copy(const CDefClassBase*){throw 1;}
unsigned long CArenaRoundDef::GetSizeofClass(){return sizeof(*this);}
void CArenaRoundDef::Transfer(CPersistContext&){throw 1;}
struct ScriptDefFixture {
 template<class T>static void fill(std::vector<T>& v,unsigned int n,unsigned int extra=0){
  if(!n&&!extra)return;
  T* p=(T*)trackedAlloc((n+extra)*sizeof(T));for(unsigned int i=0;i<n;++i)new(p+i)T;
  v._M_start=p;v._M_finish=p+n;v._M_end_of_storage._M_data=p+n+extra;
 }
 static void stringOrder(CCharString& s){expect(1,&s);expect(2,s.PStringData);}
 template<class T>static void order(std::vector<T>& v,int kind){
  for(T* p=v.begin();p!=v.end();++p){if(kind==1)stringOrder(*(CCharString*)p);else if(kind==3){expect(3,p);expect(4,p);}}
  if(v.begin())expect(2,v.begin());
 }
};
static void run(unsigned int n,unsigned int capacityOnly,unsigned int parentSize){
 allocations=freed=visits=expectedCount=0;
 CScriptDef* p=new CScriptDef;
 ScriptDefFixture::fill(p->AmbushScamScreamSoundIds,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryNormalPrizes,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryNormalPrizesHigh,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryNormalPrizesMedium,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryNormalPrizesLow,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryMatchHighScorePrizes,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryNewHighScorePrizes,n,capacityOnly);
 ScriptDefFixture::fill(p->ArcheryScorePowerLevels,n,capacityOnly);
 ScriptDefFixture::fill(p->AssassinScreamSoundIds,n,capacityOnly);
 ScriptDefFixture::fill(p->AssassinObjects,n,capacityOnly);
 ScriptDefFixture::fill(p->AssassinLocationName,n,capacityOnly);
 ScriptDefFixture::fill(p->AssassinRegionKillCount,n,capacityOnly);
 ScriptDefFixture::fill(p->BookBoyMarker,n,capacityOnly);
 ScriptDefFixture::fill(p->BookGirlMarker,n,capacityOnly);
 ScriptDefFixture::fill(p->BookReactions,n,capacityOnly);
 ScriptDefFixture::fill(p->BookDefNames,n,capacityOnly);
 ScriptDefFixture::fill(p->ChapelDonationLevel,n,capacityOnly);
 ScriptDefFixture::fill(p->ChapelMoralityRewardFraction,n,capacityOnly);
 ScriptDefFixture::fill(p->FishPrizeDefNames,n,capacityOnly);
 ScriptDefFixture::fill(p->FishRodDefNames,n,capacityOnly);
 ScriptDefFixture::fill(p->FishDefNames,n,capacityOnly);
 ScriptDefFixture::fill(p->FishPrizeWeights,n,capacityOnly);
 ScriptDefFixture::fill(p->SickChildsSisterAttentionAnims,n,capacityOnly);
 ScriptDefFixture::fill(p->TempleDonationLevel,n,capacityOnly);
 ScriptDefFixture::fill(p->TempleMoralityRewardFraction,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerFactorBurnChance,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerWorshipRewardHealth,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerWorshipRewardMorality,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerWorshipRewardGold,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerWorshipRewardDonationModifier,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerWorshipBurnHealth,n,capacityOnly);
 ScriptDefFixture::fill(p->TemplePrayerWorshipBurnGold,n,capacityOnly);
 ScriptDefFixture::fill(p->TH_ThunderConv_Speaker,n,capacityOnly);
 ScriptDefFixture::fill(p->TH_ThunderConv_Dialogue,n,capacityOnly);
 ScriptDefFixture::fill(p->TH_ThunderConv_Actor,n,capacityOnly);
 ScriptDefFixture::fill(p->TH_ThunderConv_Animation,n,capacityOnly);
 ScriptDefFixture::fill(p->TH_ThunderConv_AnimDelay,n,capacityOnly);
 ScriptDefFixture::fill(p->EscortTraderRewardObjectPerfect,n,capacityOnly);
 ScriptDefFixture::fill(p->EscortTraderSpawnedBanditType,n,capacityOnly);
 ScriptDefFixture::fill(p->RewardObjects,n,capacityOnly);
 ScriptDefFixture::fill(p->ATO_DiggingRewards,n,capacityOnly);
 ScriptDefFixture::fill(p->FHPatterns,n,capacityOnly);
 ScriptDefFixture::fill(p->FHTimeLimit,n,capacityOnly);
 ScriptDefFixture::fill(p->GUI_MeleeGrades,n,capacityOnly);
 ScriptDefFixture::fill(p->GUI_SkillGrades,n,capacityOnly);
 ScriptDefFixture::fill(p->GUI_WillGrades,n,capacityOnly);
 ScriptDefFixture::fill(p->Feats,n,capacityOnly);
 ScriptDefFixture::fill(p->ArenaRounds,n,capacityOnly);
 ScriptDefFixture::fill(p->HeroSoulsRounds,n,capacityOnly);
 ScriptDefFixture::fill(p->DemonDoor_SummonCreature_CreatureType,n,capacityOnly);
 (void)capacityOnly;
 ScriptDefFixture::fill(p->SubDefInfoMap,parentSize);
 ScriptDefFixture::order(p->DemonDoor_SummonCreature_CreatureType,1);
 ScriptDefFixture::order(p->HeroSoulsRounds,3);
 ScriptDefFixture::order(p->ArenaRounds,3);
 ScriptDefFixture::order(p->Feats,3);
 ScriptDefFixture::order(p->GUI_WillGrades,0);
 ScriptDefFixture::order(p->GUI_SkillGrades,0);
 ScriptDefFixture::order(p->GUI_MeleeGrades,0);
 ScriptDefFixture::order(p->FHTimeLimit,0);
 ScriptDefFixture::order(p->FHPatterns,3);
 ScriptDefFixture::stringOrder(p->FHMoonPadFX);
 ScriptDefFixture::stringOrder(p->FHSunPadFX);
 ScriptDefFixture::order(p->ATO_DiggingRewards,0);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound8);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound7);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound6);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound5);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound4);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound3);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound2);
 ScriptDefFixture::stringOrder(p->ArenaCreatureRound1);
 ScriptDefFixture::order(p->RewardObjects,0);
 ScriptDefFixture::order(p->EscortTraderSpawnedBanditType,1);
 ScriptDefFixture::order(p->EscortTraderRewardObjectPerfect,0);
 ScriptDefFixture::order(p->TH_ThunderConv_AnimDelay,0);
 ScriptDefFixture::order(p->TH_ThunderConv_Animation,1);
 ScriptDefFixture::order(p->TH_ThunderConv_Actor,1);
 ScriptDefFixture::order(p->TH_ThunderConv_Dialogue,1);
 ScriptDefFixture::order(p->TH_ThunderConv_Speaker,1);
 ScriptDefFixture::order(p->TemplePrayerWorshipBurnGold,0);
 ScriptDefFixture::order(p->TemplePrayerWorshipBurnHealth,0);
 ScriptDefFixture::order(p->TemplePrayerWorshipRewardDonationModifier,0);
 ScriptDefFixture::order(p->TemplePrayerWorshipRewardGold,0);
 ScriptDefFixture::order(p->TemplePrayerWorshipRewardMorality,0);
 ScriptDefFixture::order(p->TemplePrayerWorshipRewardHealth,0);
 ScriptDefFixture::order(p->TemplePrayerFactorBurnChance,0);
 ScriptDefFixture::stringOrder(p->TempleDonationRewardTitle);
 ScriptDefFixture::stringOrder(p->TempleDonationRewardObject);
 ScriptDefFixture::order(p->TempleMoralityRewardFraction,0);
 ScriptDefFixture::order(p->TempleDonationLevel,0);
 ScriptDefFixture::order(p->SickChildsSisterAttentionAnims,0);
 ScriptDefFixture::order(p->FishPrizeWeights,0);
 ScriptDefFixture::order(p->FishDefNames,1);
 ScriptDefFixture::order(p->FishRodDefNames,1);
 ScriptDefFixture::order(p->FishPrizeDefNames,1);
 ScriptDefFixture::stringOrder(p->ExposeMayorEvidenceDef);
 ScriptDefFixture::stringOrder(p->ChapelDonationRewardTitle);
 ScriptDefFixture::stringOrder(p->ChapelDonationRewardObject);
 ScriptDefFixture::order(p->ChapelMoralityRewardFraction,0);
 ScriptDefFixture::order(p->ChapelDonationLevel,0);
 ScriptDefFixture::order(p->BookDefNames,1);
 ScriptDefFixture::order(p->BookReactions,3);
 ScriptDefFixture::order(p->BookGirlMarker,1);
 ScriptDefFixture::order(p->BookBoyMarker,1);
 ScriptDefFixture::order(p->AssassinRegionKillCount,0);
 ScriptDefFixture::order(p->AssassinLocationName,1);
 ScriptDefFixture::order(p->AssassinObjects,1);
 ScriptDefFixture::order(p->AssassinScreamSoundIds,0);
 ScriptDefFixture::order(p->ArcheryScorePowerLevels,0);
 ScriptDefFixture::order(p->ArcheryNewHighScorePrizes,0);
 ScriptDefFixture::order(p->ArcheryMatchHighScorePrizes,0);
 ScriptDefFixture::order(p->ArcheryNormalPrizesLow,0);
 ScriptDefFixture::order(p->ArcheryNormalPrizesMedium,0);
 ScriptDefFixture::order(p->ArcheryNormalPrizesHigh,0);
 ScriptDefFixture::order(p->ArcheryNormalPrizes,0);
 ScriptDefFixture::order(p->AmbushScamScreamSoundIds,0);
 ScriptDefFixture::order(p->SubDefInfoMap,0);expect(4,p);
 checking=true;delete p;checking=false;
 if(visits!=expectedCount||freed!=allocations)fail(94);
}
int main(){for(unsigned int n=0;n<4;++n)for(unsigned int p=0;p<3;++p)for(unsigned int e=0;e<2;++e)run(n,e,p);puts("00f29530_TEST PASS");return 0;}