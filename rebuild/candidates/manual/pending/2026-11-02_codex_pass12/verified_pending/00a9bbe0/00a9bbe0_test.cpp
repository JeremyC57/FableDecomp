// Self-contained active production headers and exact target/support bodies.
// External constructors, virtuals, strings and storage have fixture implementations.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define long int
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public
#ifdef _MSC_VER
#define FABLE_TEST_EXTENSION
#else
#define FABLE_TEST_EXTENSION __extension__
#endif



#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_MESH_FILE_LOADER
#define FABLE_RETAIL_CARRAY_GEOMETRY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_ENGINE_C3DMESHFILELOADER_H

#define FABLE_ENGINE_CCHUNKEDFILELOADER_H

class CChunkedFileLoader {
public:CChunkedFileLoader(const char*);virtual ~CChunkedFileLoader();
private:int ReadUnknownChunkTypes;char SupportedFileType[5];
};
#define FABLE_ENGINE_C3DMESHFILEROOTCHUNK_H

#define FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H

#define FABLE_ENGINE_CCHUNKEDFILECHUNK_H

#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H



#define FABLE_ENGINE_CBASECLASS_H








class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#define FABLE_ENGINE_CCHARSTRING_H


class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};



class CChunkedFileSuperChunk;class CChunkedFileLoader;class CChunkedFileSaver;class CDataInputStream;class CDataOutputStream;
class CChunkedFileChunk:public CBaseClassNonCopyable {
public:
 virtual ~CChunkedFileChunk();
 virtual const CCharString GetChunkDescription()const;
 virtual const CCharString GetChunkInfo()const;
 virtual int IsSuperChunk()const;
 virtual int IsUnknownType()const;
 virtual CChunkedFileSuperChunk*GetAsSuperChunk();
 virtual const CChunkedFileSuperChunk*GetAsSuperChunk()const;
 virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&)=0;
 virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&)const=0;
 virtual CChunkedFileChunk*CreateNewInstance()const=0;
protected:
 CChunkedFileChunk(const char*);
 virtual CChunkedFileChunk*InitNewInstance(CChunkedFileChunk*)const;
private:char ChunkType[5];CChunkedFileSuperChunk*Parent;
};
#define FABLE_ENGINE_CCOUNTEDPOINTER_H










































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

#define FABLE_ENGINE_CARRAY_H



#define FABLE_STLPORT46_GEOMETRY_H

#include <stddef.h>
#include <stdlib.h>
inline void* __cdecl operator new(size_t,void* p){return p;}
namespace std {
struct __false_type {};
template<class T>class allocator {
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base {
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
protected:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 template<class I>T* _M_allocate_and_copy(size_type n,I first,I last){T* result=this->_M_end_of_storage.allocate(n);T* out=result;for(;first!=last;++first,++out)_Construct(out,*first);return result;}
};
template<class T,class A>
__declspec(noinline) void vector<T,A>::reserve(size_type n){
 if(capacity()<n){const size_type old_size=size();T* temp;if(this->_M_start){temp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_M_clear();}else temp=this->_M_end_of_storage.allocate(n);_M_set(temp,temp+old_size,temp+n);}
}
template<class T,class A>
__declspec(noinline) void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 for(T* first=this->_M_start;first!=position;++first,++new_finish)_Construct(new_finish,*first);
 if(fill_len==1){_Construct(new_finish,value);++new_finish;}else for(size_type count=0;count<fill_len;++count,++new_finish)_Construct(new_finish,value);
 if(!at_end)for(T* first=position;first!=this->_M_finish;++first,++new_finish)_Construct(new_finish,*first);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
struct _Rb_tree_node_base {
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>class _Rb_global{
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
protected:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
protected:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
private:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}

template<class T>class CArray:public std::vector<T>{
public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};









class CChunkedFileSuperChunk:public CChunkedFileChunk {
public:
 virtual ~CChunkedFileSuperChunk();
 void RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>&);
 virtual int IsSuperChunk()const;
 virtual CChunkedFileSuperChunk*GetAsSuperChunk();
 virtual const CChunkedFileSuperChunk*GetAsSuperChunk()const;
 virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
 virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&)const;
 virtual CChunkedFileChunk*CreateNewInstance()const=0;
protected:
 CChunkedFileSuperChunk(const char*);
 virtual CChunkedFileChunk*InitNewInstance(CChunkedFileChunk*)const;
private:
 CArray<CCountedPointer<CChunkedFileChunk> >SubChunks;
 CArray<CCountedPointer<CChunkedFileChunk> >RegisteredSubChunkTypes;
 unsigned long CurrentChunkIndex;
};









#define FABLE_ENGINE_CMATRIX3X4_H

class CMatrix3x4 {public:
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 CMatrix3x4(){}
};

#define FABLE_ENGINE_C3DVECTOR_H


#define FABLE_VECTOR_INLINE inline
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE



#define FABLE_ENGINE_CPLANE_H





class CPlane {public:C3DVector Normal;float Distance;};
#define FABLE_ENGINE_C3DVERTEX2_H





class C3DVertex2 {
public:
    C3DVector Position,Normal;
    float U,V;
    const C3DVector GetPosition() const { return Position; }
    const C3DVector GetNormal() const { return Normal; }
    float GetU() const { return U; }
    float GetV() const { return V; }
};
#define FABLE_ENGINE_C3DVERTEXBLEND2_H

class C3DVertexBlend2 {private:unsigned long VertexIndex;float BlendValue;};
#define FABLE_ENGINE_C3DTRIANGLE2_H


class C3DTriangle2 {
public:
 unsigned long GetVertexIndex(unsigned long index)const{return VertexIndex[index];}
private:
 unsigned short VertexIndex[3];
};
#define FABLE_ENGINE_CRGBCOLOUR_H

class CRGBColour {
public:
    union {
        FABLE_TEST_EXTENSION struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};
#define FABLE_ENGINE_CIVCOUNTEDPOINTER_H


#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H















#include <stddef.h>
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }                                 
    virtual void Delete() const;                                         
    void AddRef() const { ++IVRefCount; }                                
    
    long Release() const { if(--IVRefCount==0) { Delete(); return 0; } return IVRefCount; }
    long GetRefCount() const { return IVRefCount; }                      
    mutable unsigned long IVRefCount;                                    
};



template<class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Reset(T* object);
    T* operator->() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release() {
        if (Object) { Object->Release(); Object = 0; }
    }
private:
    CIVCountedPointeeBase* Object;
};

class C3DMeshFileRootChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileRootChunk():CChunkedFileSuperChunk("3DRT"){}
public: virtual const CCharString GetChunkDescription() const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
};

class C3DMeshFileLoader:public CChunkedFileLoader {
public:C3DMeshFileLoader();virtual ~C3DMeshFileLoader();
 CCountedPointer<CChunkedFileChunk> RegisterRootSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,RootChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterSubMeshSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,SubMeshChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterPrimitiveSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,PrimitiveChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterTrianglesSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,TrianglesChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterGroupSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,GroupChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterMaterialListSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,MaterialListChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterMaterialSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,MaterialChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterMapSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,MapChunkType->GetAsSuperChunk());}
 CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk&chunk){return AddChunkType(chunk,HelpersChunkType->GetAsSuperChunk());}
 void RegisterClothSubChunkType(const CChunkedFileChunk&);
private:
 CCountedPointer<CChunkedFileChunk>AddChunkType(const CChunkedFileChunk&,CChunkedFileSuperChunk*);
 CCountedPointer<C3DMeshFileRootChunk> RootChunkType;
 CCountedPointer<CChunkedFileChunk> SubMeshChunkType;
 CCountedPointer<CChunkedFileChunk> TransformationChunkType;
 CCountedPointer<CChunkedFileChunk> BoneChunkType;
 CCountedPointer<CChunkedFileChunk> PrimitiveChunkType;
 CCountedPointer<CChunkedFileChunk> TrianglesChunkType;
 CCountedPointer<CChunkedFileChunk> SmoothingGroupsChunkType;
 CCountedPointer<CChunkedFileChunk> VerticesChunkType;
 CCountedPointer<CChunkedFileChunk> UniqueVerticesChunkType;
 CCountedPointer<CChunkedFileChunk> GroupChunkType;
 CCountedPointer<CChunkedFileChunk> PrimitiveClothChunkType;
 CCountedPointer<CChunkedFileChunk> SubMeshClothChunkType;
 CCountedPointer<CChunkedFileChunk> MaterialListChunkType;
 CCountedPointer<CChunkedFileChunk> MaterialChunkType;
 CCountedPointer<CChunkedFileChunk> MaterialExtChunkType;
 CCountedPointer<CChunkedFileChunk> MapChunkType;
 CCountedPointer<CChunkedFileChunk> EngineMaterialChunkType;
 CCountedPointer<CChunkedFileChunk> HelpersChunkType;
 CCountedPointer<CChunkedFileChunk> HelperPointChunkType;
 CCountedPointer<CChunkedFileChunk> DummyObjectChunkType;
 CCountedPointer<CChunkedFileChunk> ConvexVolumeChunkType;
 CCountedPointer<CChunkedFileChunk> MirroredTextureChunkType;
};

#define FABLE_ENGINE_C3DMESHFILESUBMESHCHUNK_H






















class C3DMeshFileSubMeshChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileSubMeshChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CCharString Name;
private: long SubMeshIndex;
private: long ParentIndex;
private: long FirstChildIndex;
private: long NextSiblingIndex;
};

#define FABLE_ENGINE_C3DMESHFILEMATERIALLISTCHUNK_H






















class C3DMeshFileMaterialListChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileMaterialListChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
};

#define FABLE_ENGINE_C3DMESHFILEHELPERSCHUNK_H






















class C3DMeshFileHelpersChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileHelpersChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
};

#define FABLE_ENGINE_C3DMESHFILETRANSFORMATIONCHUNK_H






















class C3DMeshFileTransformationChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileTransformationChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CMatrix3x4 Transform;
};

#define FABLE_ENGINE_C3DMESHFILEBONECHUNK_H






















class C3DMeshFileBoneChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileBoneChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: long BoneIndex;
private: long ParentIndex;
private: long FirstChildIndex;
private: long NextSiblingIndex;
private: CMatrix3x4 LocalizationMatrix;
private: CCharString Name;
};

#define FABLE_ENGINE_C3DMESHFILEPRIMITIVECHUNK_H






















class C3DMeshFilePrimitiveChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFilePrimitiveChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CArray<long> MaterialIndices;
};

#define FABLE_ENGINE_C3DMESHFILECLOTHCHUNK_H






















class C3DMeshFileClothChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileClothChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CCharString Name;
private: long MaterialIndex;
private: CCharString ParticleCode;
private: CArray<char> CompiledCode;
};

#define FABLE_ENGINE_C3DMESHFILETRIANGLESCHUNK_H






















class C3DMeshFileTrianglesChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileTrianglesChunk();
 virtual ~C3DMeshFileTrianglesChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CArray<C3DTriangle2> TriangleList;
};

#define FABLE_ENGINE_C3DMESHFILESMOOTHINGGROUPSCHUNK_H






















class C3DMeshFileSmoothingGroupsChunk:public CChunkedFileChunk {
public:
 C3DMeshFileSmoothingGroupsChunk();
 virtual ~C3DMeshFileSmoothingGroupsChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CArray<unsigned long> FaceGroupsList;
};

#define FABLE_ENGINE_C3DMESHFILEVERTICESCHUNK_H






















class C3DMeshFileVerticesChunk:public CChunkedFileChunk {
public:
 C3DMeshFileVerticesChunk();
 virtual ~C3DMeshFileVerticesChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CArray<C3DVertex2> VertexList;
};

#define FABLE_ENGINE_C3DMESHFILEUNIQUEVERTICESCHUNK_H






















class C3DMeshFileUniqueVerticesChunk:public CChunkedFileChunk {
public:
 C3DMeshFileUniqueVerticesChunk();
 virtual ~C3DMeshFileUniqueVerticesChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CArray<unsigned long> UniqueVerticesList;
};

#define FABLE_ENGINE_C3DMESHFILEGROUPCHUNK_H






















class C3DMeshFileGroupChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileGroupChunk();
 virtual ~C3DMeshFileGroupChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: long BoneIndex;
private: CArray<C3DVertexBlend2> VertexList;
};

#define FABLE_ENGINE_C3DMESHFILEMATERIALCHUNK_H






















class C3DMeshFileMaterialChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileMaterialChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: long MaterialIndex;
private: bool TwoSided;
private: CRGBColour Ambient;
private: CRGBColour Diffuse;
private: CRGBColour Specular;
private: float Shiny;
private: float ShinyStrength;
private: float Transparency;
private: CCharString Name;
};

#define FABLE_ENGINE_C3DMESHFILEENGINEMATERIALCHUNK_H






















class C3DMeshFileEngineMaterialChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileEngineMaterialChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private:class CMapFilenames;
private: long MaterialIndex;
private: CCharString Name;
private: CIVCountedPointer<C3DMeshFileEngineMaterialChunk::CMapFilenames> MapFilenames;
private: unsigned long DiffuseMapBankIndex;
private: unsigned long BumpMapBankIndex;
private: unsigned long ReflectionMapBankIndex;
private: unsigned long IlluminationMapBankIndex;
private: unsigned long MapFlags;
private: unsigned long SelfIllumination;
private: bool TwoSided;
private: bool Transparent;
private: bool BooleanAlpha;
private: bool DegenerateTriangles;
};

#define FABLE_ENGINE_C3DMESHFILEMATERIALEXTCHUNK_H






















class C3DMeshFileMaterialExtChunk:public CChunkedFileChunk {
public:
 C3DMeshFileMaterialExtChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: unsigned long SelfIllumination;
};

#define FABLE_ENGINE_C3DMESHFILEMAPCHUNK_H






















enum EMaterialMapType { MM_TEST_ZERO=0 };
class C3DMeshFileMapChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileMapChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: EMaterialMapType MapType;
private: CCharString MapDescription;
private: CCharString Filename;
private: unsigned long CRC;
};

#define FABLE_ENGINE_C3DMESHFILEHELPERPOINTCHUNK_H






















class C3DMeshFileHelperPointChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileHelperPointChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: C3DVector Position;
private: long SubMeshIndex;
private: long BoneIndex;
private: CCharString Name;
};

#define FABLE_ENGINE_C3DMESHFILEDUMMYOBJECTCHUNK_H






















class C3DMeshFileDummyObjectChunk:public C3DMeshFileHelperPointChunk {
public:
 C3DMeshFileDummyObjectChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: C3DVector Direction;
private: CMatrix3x4 Transform;
private: bool UseLocalOrigin;
};

#define FABLE_ENGINE_C3DMESHFILECONVEXVOLUMECHUNK_H






















class C3DMeshFileConvexVolumeChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileConvexVolumeChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CCharString Name;
private: CArray<CPlane> Planes;
};

#define FABLE_ENGINE_CMIRROREDTEXTUREHIGHLIGHTSCHUNK_H






















class CMirroredTextureHighlightsChunk:public CChunkedFileChunk {
public:
 CMirroredTextureHighlightsChunk():CChunkedFileChunk("MTEH"){}
 virtual ~CMirroredTextureHighlightsChunk(){}
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: CArray<unsigned long> TriangleVertexIndexList;
};


#pragma auto_inline(off)
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk&chunkType,CChunkedFileSuperChunk*parent)
{
 CCountedPointer<CChunkedFileChunk> chunk;
 chunk.Reset(chunkType.CreateNewInstance());
 parent->RegisterSubChunkType(chunk);
 return chunk;
}
#pragma auto_inline(on)

C3DMeshFileLoader::C3DMeshFileLoader():CChunkedFileLoader("3DMF")
{
 RootChunkType.Reset(new C3DMeshFileRootChunk);
 SubMeshChunkType = RegisterRootSubChunkType(C3DMeshFileSubMeshChunk());
 MaterialListChunkType = RegisterRootSubChunkType(C3DMeshFileMaterialListChunk());
 HelpersChunkType = RegisterRootSubChunkType(C3DMeshFileHelpersChunk());
 TransformationChunkType = RegisterSubMeshSubChunkType(C3DMeshFileTransformationChunk());
 BoneChunkType = RegisterSubMeshSubChunkType(C3DMeshFileBoneChunk());
 PrimitiveChunkType = RegisterSubMeshSubChunkType(C3DMeshFilePrimitiveChunk());
 SubMeshClothChunkType = RegisterSubMeshSubChunkType(C3DMeshFileClothChunk());
 TrianglesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileTrianglesChunk());
 SmoothingGroupsChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileSmoothingGroupsChunk());
 VerticesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileVerticesChunk());
 UniqueVerticesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileUniqueVerticesChunk());
 GroupChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileGroupChunk());
 PrimitiveClothChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileClothChunk());
 RegisterClothSubChunkType(C3DMeshFileGroupChunk());
 MaterialChunkType = RegisterMaterialListSubChunkType(C3DMeshFileMaterialChunk());
 EngineMaterialChunkType = RegisterMaterialListSubChunkType(C3DMeshFileEngineMaterialChunk());
 MaterialExtChunkType = RegisterMaterialSubChunkType(C3DMeshFileMaterialExtChunk());
 MapChunkType = RegisterMaterialSubChunkType(C3DMeshFileMapChunk());
 HelperPointChunkType = RegisterHelpersSubChunkType(C3DMeshFileHelperPointChunk());
 DummyObjectChunkType = RegisterHelpersSubChunkType(C3DMeshFileDummyObjectChunk());
 ConvexVolumeChunkType = RegisterHelpersSubChunkType(C3DMeshFileConvexVolumeChunk());
 MirroredTextureChunkType = RegisterPrimitiveSubChunkType(CMirroredTextureHighlightsChunk());
}

static unsigned live_chunks=0,live_strings=0,constructed=0,destroyed=0,registered=0,run_mode=0;
static char registration_types[24][5],registration_parents[24][5];
CBaseClass::CBaseClass(){++live_chunks;++constructed;} CBaseClass::~CBaseClass(){assert(live_chunks);--live_chunks;++destroyed;}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():PStringData(0){++live_strings;} CCharString::~CCharString(){assert(live_strings);--live_strings;}
CChunkedFileLoader::CChunkedFileLoader(const char*type):ReadUnknownChunkTypes(0){strcpy(SupportedFileType,type);} CChunkedFileLoader::~CChunkedFileLoader(){}
CChunkedFileChunk::CChunkedFileChunk(const char*type):Parent(0){strcpy(ChunkType,type);} CChunkedFileChunk::~CChunkedFileChunk(){}
const CCharString CChunkedFileChunk::GetChunkDescription()const{return CCharString();} const CCharString CChunkedFileChunk::GetChunkInfo()const{return CCharString();}
int CChunkedFileChunk::IsSuperChunk()const{return 0;}int CChunkedFileChunk::IsUnknownType()const{return 0;}
CChunkedFileSuperChunk*CChunkedFileChunk::GetAsSuperChunk(){assert(false);return 0;}const CChunkedFileSuperChunk*CChunkedFileChunk::GetAsSuperChunk()const{assert(false);return 0;}
CChunkedFileChunk*CChunkedFileChunk::InitNewInstance(CChunkedFileChunk*p)const{return p;}
CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*type):CChunkedFileChunk(type),CurrentChunkIndex(0){}
CChunkedFileSuperChunk::~CChunkedFileSuperChunk(){}
int CChunkedFileSuperChunk::IsSuperChunk()const{return 1;}
CChunkedFileSuperChunk*CChunkedFileSuperChunk::GetAsSuperChunk(){return this;}const CChunkedFileSuperChunk*CChunkedFileSuperChunk::GetAsSuperChunk()const{return this;}
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&)const{assert(false);}
CChunkedFileChunk*CChunkedFileSuperChunk::InitNewInstance(CChunkedFileChunk*p)const{return p;}
void CChunkedFileSuperChunk::RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>&chunk){assert(chunk.Data&&chunk.Info);assert(registered<24);strcpy(registration_types[registered],chunk->ChunkType);strcpy(registration_parents[registered],ChunkType);++registered;RegisteredSubChunkTypes.push_back(chunk);}
void C3DMeshFileLoader::RegisterClothSubChunkType(const CChunkedFileChunk&chunk){AddChunkType(chunk,SubMeshClothChunkType->GetAsSuperChunk());AddChunkType(chunk,PrimitiveClothChunkType->GetAsSuperChunk());}
C3DMeshFileLoader::~C3DMeshFileLoader(){}
class C3DMeshFileEngineMaterialChunk::CMapFilenames:public CIVCountedPointeeBase {public:CCharString DiffuseMap,BumpMap,ReflectionMap,IlluminationMap;};
void CIVCountedPointeeBase::Delete()const{delete this;}
template<class T>void CIVCountedPointer<T>::Reset(T*object){Release();Object=object;if(Object)Object->AddRef();}
const CCharString C3DMeshFileRootChunk::GetChunkDescription() const{return CCharString();}
CChunkedFileChunk* C3DMeshFileRootChunk::CreateNewInstance() const{return new C3DMeshFileRootChunk;}

C3DMeshFileSubMeshChunk::C3DMeshFileSubMeshChunk():CChunkedFileSuperChunk("SUBM"){SubMeshIndex=(long)(run_mode&1);ParentIndex=(long)(run_mode&1);FirstChildIndex=(long)(run_mode&1);NextSiblingIndex=(long)(run_mode&1);}
const CCharString C3DMeshFileSubMeshChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileSubMeshChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileSubMeshChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileSubMeshChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileSubMeshChunk::CreateNewInstance() const{return new C3DMeshFileSubMeshChunk;}

C3DMeshFileMaterialListChunk::C3DMeshFileMaterialListChunk():CChunkedFileSuperChunk("MATL"){}
const CCharString C3DMeshFileMaterialListChunk::GetChunkDescription() const{return CCharString();}
CChunkedFileChunk* C3DMeshFileMaterialListChunk::CreateNewInstance() const{return new C3DMeshFileMaterialListChunk;}

C3DMeshFileHelpersChunk::C3DMeshFileHelpersChunk():CChunkedFileSuperChunk("HELP"){}
const CCharString C3DMeshFileHelpersChunk::GetChunkDescription() const{return CCharString();}
CChunkedFileChunk* C3DMeshFileHelpersChunk::CreateNewInstance() const{return new C3DMeshFileHelpersChunk;}

C3DMeshFileTransformationChunk::C3DMeshFileTransformationChunk():CChunkedFileSuperChunk("TRAN"){}
const CCharString C3DMeshFileTransformationChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileTransformationChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileTransformationChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileTransformationChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileTransformationChunk::CreateNewInstance() const{return new C3DMeshFileTransformationChunk;}

C3DMeshFileBoneChunk::C3DMeshFileBoneChunk():CChunkedFileSuperChunk("BONE"){BoneIndex=(long)(run_mode&1);ParentIndex=(long)(run_mode&1);FirstChildIndex=(long)(run_mode&1);NextSiblingIndex=(long)(run_mode&1);}
const CCharString C3DMeshFileBoneChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileBoneChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileBoneChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileBoneChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileBoneChunk::CreateNewInstance() const{return new C3DMeshFileBoneChunk;}

C3DMeshFilePrimitiveChunk::C3DMeshFilePrimitiveChunk():CChunkedFileSuperChunk("PRIM"){if(run_mode&2){MaterialIndices._M_start=(long*)malloc(2*sizeof(long));MaterialIndices._M_finish=MaterialIndices._M_start;MaterialIndices._M_end_of_storage._M_data=MaterialIndices._M_start+2;}}
const CCharString C3DMeshFilePrimitiveChunk::GetChunkDescription() const{return CCharString();}
void C3DMeshFilePrimitiveChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFilePrimitiveChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFilePrimitiveChunk::CreateNewInstance() const{return new C3DMeshFilePrimitiveChunk;}

C3DMeshFileClothChunk::C3DMeshFileClothChunk():CChunkedFileSuperChunk("CLTH"){MaterialIndex=(long)(run_mode&1);if(run_mode&2){CompiledCode._M_start=(char*)malloc(2*sizeof(char));CompiledCode._M_finish=CompiledCode._M_start;CompiledCode._M_end_of_storage._M_data=CompiledCode._M_start+2;}}
const CCharString C3DMeshFileClothChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileClothChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileClothChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileClothChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileClothChunk::CreateNewInstance() const{return new C3DMeshFileClothChunk;}

C3DMeshFileTrianglesChunk::C3DMeshFileTrianglesChunk():CChunkedFileSuperChunk("TRIS"){if(run_mode&2){TriangleList._M_start=(C3DTriangle2*)malloc(2*sizeof(C3DTriangle2));TriangleList._M_finish=TriangleList._M_start;TriangleList._M_end_of_storage._M_data=TriangleList._M_start+2;}}
C3DMeshFileTrianglesChunk::~C3DMeshFileTrianglesChunk(){}
const CCharString C3DMeshFileTrianglesChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileTrianglesChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileTrianglesChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileTrianglesChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileTrianglesChunk::CreateNewInstance() const{return new C3DMeshFileTrianglesChunk;}

C3DMeshFileSmoothingGroupsChunk::C3DMeshFileSmoothingGroupsChunk():CChunkedFileChunk("SMTH"){if(run_mode&2){FaceGroupsList._M_start=(unsigned long*)malloc(2*sizeof(unsigned long));FaceGroupsList._M_finish=FaceGroupsList._M_start;FaceGroupsList._M_end_of_storage._M_data=FaceGroupsList._M_start+2;}}
C3DMeshFileSmoothingGroupsChunk::~C3DMeshFileSmoothingGroupsChunk(){}
const CCharString C3DMeshFileSmoothingGroupsChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileSmoothingGroupsChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileSmoothingGroupsChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileSmoothingGroupsChunk::CreateNewInstance() const{return new C3DMeshFileSmoothingGroupsChunk;}

C3DMeshFileVerticesChunk::C3DMeshFileVerticesChunk():CChunkedFileChunk("VERT"){if(run_mode&2){VertexList._M_start=(C3DVertex2*)malloc(2*sizeof(C3DVertex2));VertexList._M_finish=VertexList._M_start;VertexList._M_end_of_storage._M_data=VertexList._M_start+2;}}
C3DMeshFileVerticesChunk::~C3DMeshFileVerticesChunk(){}
const CCharString C3DMeshFileVerticesChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileVerticesChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileVerticesChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileVerticesChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileVerticesChunk::CreateNewInstance() const{return new C3DMeshFileVerticesChunk;}

C3DMeshFileUniqueVerticesChunk::C3DMeshFileUniqueVerticesChunk():CChunkedFileChunk("UNIQ"){if(run_mode&2){UniqueVerticesList._M_start=(unsigned long*)malloc(2*sizeof(unsigned long));UniqueVerticesList._M_finish=UniqueVerticesList._M_start;UniqueVerticesList._M_end_of_storage._M_data=UniqueVerticesList._M_start+2;}}
C3DMeshFileUniqueVerticesChunk::~C3DMeshFileUniqueVerticesChunk(){}
const CCharString C3DMeshFileUniqueVerticesChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileUniqueVerticesChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileUniqueVerticesChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileUniqueVerticesChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileUniqueVerticesChunk::CreateNewInstance() const{return new C3DMeshFileUniqueVerticesChunk;}

C3DMeshFileGroupChunk::C3DMeshFileGroupChunk():CChunkedFileSuperChunk("GRUP"){BoneIndex=(long)(run_mode&1);if(run_mode&2){VertexList._M_start=(C3DVertexBlend2*)malloc(2*sizeof(C3DVertexBlend2));VertexList._M_finish=VertexList._M_start;VertexList._M_end_of_storage._M_data=VertexList._M_start+2;}}
C3DMeshFileGroupChunk::~C3DMeshFileGroupChunk(){}
const CCharString C3DMeshFileGroupChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileGroupChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileGroupChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileGroupChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileGroupChunk::CreateNewInstance() const{return new C3DMeshFileGroupChunk;}

C3DMeshFileMaterialChunk::C3DMeshFileMaterialChunk():CChunkedFileSuperChunk("MAT_"){MaterialIndex=(long)(run_mode&1);TwoSided=(bool)(run_mode&1);Shiny=(float)(run_mode&1);ShinyStrength=(float)(run_mode&1);Transparency=(float)(run_mode&1);}
const CCharString C3DMeshFileMaterialChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileMaterialChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileMaterialChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileMaterialChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileMaterialChunk::CreateNewInstance() const{return new C3DMeshFileMaterialChunk;}

C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk():CChunkedFileSuperChunk("ENGM"){MaterialIndex=(long)(run_mode&1);if(run_mode&4){CMapFilenames*p=new CMapFilenames;p->IVRefCount=0;MapFilenames.Reset(p);}DiffuseMapBankIndex=(unsigned long)(run_mode&1);BumpMapBankIndex=(unsigned long)(run_mode&1);ReflectionMapBankIndex=(unsigned long)(run_mode&1);IlluminationMapBankIndex=(unsigned long)(run_mode&1);MapFlags=(unsigned long)(run_mode&1);SelfIllumination=(unsigned long)(run_mode&1);TwoSided=(bool)(run_mode&1);Transparent=(bool)(run_mode&1);BooleanAlpha=(bool)(run_mode&1);DegenerateTriangles=(bool)(run_mode&1);}
const CCharString C3DMeshFileEngineMaterialChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileEngineMaterialChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileEngineMaterialChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileEngineMaterialChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileEngineMaterialChunk::CreateNewInstance() const{return new C3DMeshFileEngineMaterialChunk;}

C3DMeshFileMaterialExtChunk::C3DMeshFileMaterialExtChunk():CChunkedFileChunk("MATX"){SelfIllumination=(unsigned long)(run_mode&1);}
const CCharString C3DMeshFileMaterialExtChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileMaterialExtChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileMaterialExtChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileMaterialExtChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileMaterialExtChunk::CreateNewInstance() const{return new C3DMeshFileMaterialExtChunk;}

C3DMeshFileMapChunk::C3DMeshFileMapChunk():CChunkedFileSuperChunk("MAP_"){MapType=MM_TEST_ZERO;CRC=(unsigned long)(run_mode&1);}
const CCharString C3DMeshFileMapChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileMapChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileMapChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileMapChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileMapChunk::CreateNewInstance() const{return new C3DMeshFileMapChunk;}

C3DMeshFileHelperPointChunk::C3DMeshFileHelperPointChunk():CChunkedFileSuperChunk("HPNT"){SubMeshIndex=(long)(run_mode&1);BoneIndex=(long)(run_mode&1);}
const CCharString C3DMeshFileHelperPointChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileHelperPointChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileHelperPointChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileHelperPointChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileHelperPointChunk::CreateNewInstance() const{return new C3DMeshFileHelperPointChunk;}

C3DMeshFileDummyObjectChunk::C3DMeshFileDummyObjectChunk():C3DMeshFileHelperPointChunk(){strcpy(ChunkType,"DUMY");UseLocalOrigin=(bool)(run_mode&1);}
const CCharString C3DMeshFileDummyObjectChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileDummyObjectChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileDummyObjectChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileDummyObjectChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileDummyObjectChunk::CreateNewInstance() const{return new C3DMeshFileDummyObjectChunk;}

C3DMeshFileConvexVolumeChunk::C3DMeshFileConvexVolumeChunk():CChunkedFileSuperChunk("CVOL"){if(run_mode&2){Planes._M_start=(CPlane*)malloc(2*sizeof(CPlane));Planes._M_finish=Planes._M_start;Planes._M_end_of_storage._M_data=Planes._M_start+2;}}
const CCharString C3DMeshFileConvexVolumeChunk::GetChunkDescription() const{return CCharString();}
const CCharString C3DMeshFileConvexVolumeChunk::GetChunkInfo() const{return CCharString();}
void C3DMeshFileConvexVolumeChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void C3DMeshFileConvexVolumeChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* C3DMeshFileConvexVolumeChunk::CreateNewInstance() const{return new C3DMeshFileConvexVolumeChunk;}
const CCharString CMirroredTextureHighlightsChunk::GetChunkDescription() const{return CCharString();}
const CCharString CMirroredTextureHighlightsChunk::GetChunkInfo() const{return CCharString();}
void CMirroredTextureHighlightsChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){assert(false);}
void CMirroredTextureHighlightsChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{assert(false);}
CChunkedFileChunk* CMirroredTextureHighlightsChunk::CreateNewInstance() const{return new CMirroredTextureHighlightsChunk;}

static void check_pointer(const CCountedPointer<CChunkedFileChunk>&p,const char*tag){assert(p.Data&&p.Info&&p.Info->RefCount==2);assert(p.Info->Data==p.Data);assert(strcmp(p->ChunkType,tag)==0);}
static void run(unsigned mode){
 assert(live_chunks==0&&live_strings==0);run_mode=mode;constructed=destroyed=registered=0;
 {C3DMeshFileLoader loader;
 assert(strcmp(loader.SupportedFileType,"3DMF")==0);assert(loader.RootChunkType.Data&&loader.RootChunkType.Info->RefCount==1);assert(strcmp(loader.RootChunkType->ChunkType,"3DRT")==0);
check_pointer(loader.SubMeshChunkType,"SUBM");
check_pointer(loader.TransformationChunkType,"TRAN");
check_pointer(loader.BoneChunkType,"BONE");
check_pointer(loader.PrimitiveChunkType,"PRIM");
check_pointer(loader.TrianglesChunkType,"TRIS");
check_pointer(loader.SmoothingGroupsChunkType,"SMTH");
check_pointer(loader.VerticesChunkType,"VERT");
check_pointer(loader.UniqueVerticesChunkType,"UNIQ");
check_pointer(loader.GroupChunkType,"GRUP");
check_pointer(loader.PrimitiveClothChunkType,"CLTH");
check_pointer(loader.SubMeshClothChunkType,"CLTH");
check_pointer(loader.MaterialListChunkType,"MATL");
check_pointer(loader.MaterialChunkType,"MAT_");
check_pointer(loader.MaterialExtChunkType,"MATX");
check_pointer(loader.MapChunkType,"MAP_");
check_pointer(loader.EngineMaterialChunkType,"ENGM");
check_pointer(loader.HelpersChunkType,"HELP");
check_pointer(loader.HelperPointChunkType,"HPNT");
check_pointer(loader.DummyObjectChunkType,"DUMY");
check_pointer(loader.ConvexVolumeChunkType,"CVOL");
check_pointer(loader.MirroredTextureChunkType,"MTEH");

 const char*expected_types[]={"SUBM","MATL","HELP","TRAN","BONE","PRIM","CLTH","TRIS","SMTH","VERT","UNIQ","GRUP","CLTH","GRUP","GRUP","MAT_","ENGM","MATX","MAP_","HPNT","DUMY","CVOL","MTEH"};
 const char*expected_parents[]={"3DRT","3DRT","3DRT","SUBM","SUBM","SUBM","SUBM","PRIM","PRIM","PRIM","PRIM","PRIM","PRIM","CLTH","CLTH","MATL","MATL","MAT_","MAT_","HELP","HELP","HELP","PRIM"};
 assert(registered==23);for(unsigned i=0;i<registered;++i){assert(strcmp(registration_types[i],expected_types[i])==0);assert(strcmp(registration_parents[i],expected_parents[i])==0);}
 CChunkedFileSuperChunk*a=loader.SubMeshClothChunkType->GetAsSuperChunk();CChunkedFileSuperChunk*b=loader.PrimitiveClothChunkType->GetAsSuperChunk();assert(a!=b);assert(a->RegisteredSubChunkTypes.size()==1&&b->RegisteredSubChunkTypes.size()==1);assert(a->RegisteredSubChunkTypes[0].Data!=b->RegisteredSubChunkTypes[0].Data);assert(a->RegisteredSubChunkTypes[0].Info->RefCount==1&&b->RegisteredSubChunkTypes[0].Info->RefCount==1);
 assert(loader.RootChunkType->RegisteredSubChunkTypes.size()==3);assert(loader.SubMeshChunkType->GetAsSuperChunk()->RegisteredSubChunkTypes.size()==4);assert(loader.PrimitiveChunkType->GetAsSuperChunk()->RegisteredSubChunkTypes.size()==7);
 assert(live_chunks==24&&constructed==46&&destroyed==22);
 }
 assert(live_chunks==0&&live_strings==0&&constructed==destroyed);
}
int main(){
#ifdef _MSC_VER
 assert(sizeof(C3DMeshFileLoader)==192&&sizeof(CChunkedFileSuperChunk)==44&&sizeof(C3DMeshFileBoneChunk)==112&&sizeof(C3DMeshFileEngineMaterialChunk)==84&&sizeof(CMirroredTextureHighlightsChunk)==28);
#endif
 for(unsigned repeat=0;repeat<3;++repeat)for(unsigned mode=0;mode<8;++mode)run(mode);
 puts("00a9bbe0_TEST PASS");return 0;
}
