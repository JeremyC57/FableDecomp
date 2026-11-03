// Standalone fixture: active production headers and exact target body are expanded below.
// Host-only long/int mapping preserves native 32-bit stream words; pointers remain 64-bit.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <math.h>
#else
extern "C" double sqrt(double);
#endif
#include <assert.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define long int
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public


#define FABLE_ENGINE_CPARTICLEPROGRAM_H



#define FABLE_ENGINE_CPARTICLESIMULATION_H


#define FABLE_ENGINE_CMOVABLERESOURCE_H









class CMovableResource {
public:
    CMovableResource() { }                                               
    virtual ~CMovableResource() { }                                      
    virtual void RelocateData(long, void*, void*, long) = 0;             
    virtual bool CanRelocateData(long) const;                            
    virtual unsigned long Verify() const;                                
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

#define FABLE_ENGINE_C4DVECTOR_H

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
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
class C4DVector { public:float X,Y,Z,W; C4DVector(){}
 C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}
 C4DVector(const C3DVector& v,float w):X(v.X),Y(v.Y),Z(v.Z),W(w){}
 
 void Initialise(const C3DVector& vect,float w){X=vect.X;Y=vect.Y;Z=vect.Z;W=w;}
 C3DVector ToC3DVector()const{return C3DVector(X,Y,Z);}
};
#define FABLE_ENGINE_C3DBOUNDINGBOX_H



class C3DBoundingBox {
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
class CParticleSimulationRenderData;
class CMovableResourceMemoryPool;
class CParticleSimulation:public CMovableResource {
public:
 CParticleSimulation();
 CParticleSimulation(const CParticleSimulation&);
 virtual ~CParticleSimulation();
 virtual void RelocateData(long,void*,void*,long);
 unsigned long GetParticleCount()const{return Size;}
 C3DVector GetParticlePosition(unsigned long index)const{return Positions[index].ToC3DVector();}
 const C4DVector* PeekParticlePositions()const{return Positions;}
 void SetParticlePosition(unsigned long index,const C3DVector& vect){Positions[index].Initialise(vect,1.0f);}
 CCountedPointer<CParticleSimulationRenderData> RenderData;
protected:
 float Timestep;
 bool TimestepChanged;
 float TimestepMultiplier;
 unsigned long Size;
 C4DVector* Positions;
 C4DVector* LastPositions;
 C4DVector* SavedPositions;
 C4DVector* Accelerations;
 float* SimulationAlphas;
 float MaximumAdjustment;
 float GravityStrength;
 float WindStrength;
 bool DraggingEnable;
 bool DraggingRotational;
 float DraggingStrength;
 bool AccelerationEnable;
 float GlobalDamping;
private:
 double CurrentTime;
 C3DBoundingBox BoundingBox;
 bool BoundingBoxValid;
 long MemoryHandle;
 CMovableResourceMemoryPool* MemoryPool;
};
#define FABLE_ENGINE_CMEMORYDATAOUTPUTSTREAM_H


#define FABLE_ENGINE_CDATAOUTPUTSTREAM_H

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

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
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
class CWideString;
class CDataOutputStream : public CDataStream {
public:
    virtual ~CDataOutputStream();
    virtual void Write(const void*,long)=0;
    void WriteSLONG(long);
    void WriteULONG(unsigned long);
private:
    CCharString NewLine;
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
};
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





class CMemoryDataOutputStream : public CDataOutputStream {
private:
    CArray<char> Data;
    unsigned long CurrentPosition;
public:
    CMemoryDataOutputStream();
    virtual ~CMemoryDataOutputStream();
    virtual void SetPosition(unsigned long);
    virtual unsigned long GetPosition() const;
    virtual void Close();
    virtual void Write(const void*,long);
    unsigned long GetLength() const;
    const void* PeekData() const;
};





#define FABLE_ENGINE_C3DTRIANGLE2_H


class C3DTriangle2 {
public:
 unsigned long GetVertexIndex(unsigned long index)const{return VertexIndex[index];}
private:
 unsigned short VertexIndex[3];
};
#define FABLE_ENGINE_C3DQUAD2_H


class C3DQuad2 {
public:
 unsigned long GetVertexIndex(unsigned long index)const{return VertexIndex[index];}
private:
 unsigned short VertexIndex[4];
};
class CMatrix3x4;
class CParticleProgram:public CMovableResource {
public:
 CParticleProgram();
 virtual ~CParticleProgram();
 virtual void RelocateData(long,void*,void*,long);
 const CParticleSimulation& PeekInitialSimulation()const{return InitialSimulation;}
 CParticleSimulation& GetInitialSimulation(){return InitialSimulation;}
 void ApplyTransform(const CMatrix3x4&);
private:
 void CalculateAveragePatchSize(const CParticleSimulation&);
 CParticleSimulation InitialSimulation;
 unsigned char ExportParticles[16]; 
 CArray<C3DTriangle2> RenderTris; 
 CArray<C3DQuad2> RenderQuads; 
 unsigned char RenderVertices[12]; 
 unsigned long NonSimCount;
 C4DVector* NonSimPositions;
 unsigned char IndexedTextureCoords[12]; 
 unsigned char TempVertexPositions[12]; 
 unsigned char ParticleIndices[12]; 
 unsigned char VertexIndices[12]; 
 bool BezierEnable;
 float AveragePatchSize;
 unsigned char SimGroups[12]; 
 unsigned char NonSimGroups[12]; 
 CMemoryDataOutputStream Constraints;
 unsigned char LoopStack[12]; 
 long MemoryHandle;
 CMovableResourceMemoryPool* MemoryPool;
};

#pragma intrinsic(sqrt)
void CParticleProgram::CalculateAveragePatchSize(const CParticleSimulation& sim)
{
 float total_squared_length=0.0f;
 unsigned long total_sides=0;
 unsigned long no_tris=RenderTris.size();
 for(unsigned long c_tri=0;c_tri<no_tris;++c_tri) {
  const C3DTriangle2& tri=RenderTris[c_tri];
  C3DVector a=sim.GetParticlePosition(tri.GetVertexIndex(0));
  C3DVector b=sim.GetParticlePosition(tri.GetVertexIndex(1));
  C3DVector c=sim.GetParticlePosition(tri.GetVertexIndex(2));
  total_squared_length+=(b-a).GetSquaredMagnitude();
  total_squared_length+=(c-b).GetSquaredMagnitude();
  total_squared_length+=(a-c).GetSquaredMagnitude();
  total_sides+=3;
 }
 unsigned long no_quads=RenderQuads.size();
 for(unsigned long c_quad=0;c_quad<no_quads;++c_quad) {
  const C3DQuad2& quad=RenderQuads[c_quad];
  C3DVector a=sim.GetParticlePosition(quad.GetVertexIndex(0));
  C3DVector b=sim.GetParticlePosition(quad.GetVertexIndex(1));
  C3DVector c=sim.GetParticlePosition(quad.GetVertexIndex(2));
  C3DVector d=sim.GetParticlePosition(quad.GetVertexIndex(3));
  total_squared_length+=(b-a).GetSquaredMagnitude();
  total_squared_length+=(c-b).GetSquaredMagnitude();
  total_squared_length+=(d-c).GetSquaredMagnitude();
  total_squared_length+=(a-d).GetSquaredMagnitude();
  total_sides+=4;
 }
 if(total_sides)AveragePatchSize=(float)sqrt(total_squared_length/total_sides);
}

CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():PStringData(0){} CCharString::~CCharString(){}
CDataStream::~CDataStream(){} CDataOutputStream::~CDataOutputStream(){}
CMemoryDataOutputStream::CMemoryDataOutputStream():CurrentPosition(0){}
CMemoryDataOutputStream::~CMemoryDataOutputStream(){}
void CMemoryDataOutputStream::SetPosition(unsigned long n){CurrentPosition=n;}
unsigned long CMemoryDataOutputStream::GetPosition()const{return CurrentPosition;}
void CMemoryDataOutputStream::Close(){}
void CMemoryDataOutputStream::Write(const void*,long){assert(false);}
unsigned long CMemoryDataOutputStream::GetLength()const{return (unsigned long)Data.size();}
const void* CMemoryDataOutputStream::PeekData()const{return Data._M_start;}
bool CMovableResource::CanRelocateData(long)const{return true;}
unsigned long CMovableResource::Verify()const{return 123;}
CParticleSimulation::CParticleSimulation():Size(0),Positions(0){}
CParticleSimulation::~CParticleSimulation(){}
void CParticleSimulation::RelocateData(long,void*,void*,long){assert(false);}
CParticleProgram::CParticleProgram(){}
CParticleProgram::~CParticleProgram(){}
void CParticleProgram::RelocateData(long,void*,void*,long){assert(false);}

static double edge_squared(const C4DVector& a,const C4DVector& b){double x=a.X-b.X,y=a.Y-b.Y,z=a.Z-b.Z;return x*x+y*y+z*z;}
static void run(unsigned nt,unsigned nq,float scale,int shift){
 CParticleProgram p; C4DVector positions[5];
 const int coords[5][3]={{0,0,0},{2,0,0},{2,2,0},{0,2,0},{1,1,4}};
 for(unsigned k=0;k<5;++k)positions[k]=C4DVector(coords[k][0]*scale+shift,coords[k][1]*scale-shift,coords[k][2]*scale+shift,7.0f);
 p.InitialSimulation.Size=5;p.InitialSimulation.Positions=positions;p.AveragePatchSize=9.25f;
 C3DTriangle2* tris=nt?(C3DTriangle2*)malloc(nt*sizeof(C3DTriangle2)):0;
 C3DQuad2* quads=nq?(C3DQuad2*)malloc(nq*sizeof(C3DQuad2)):0;
 p.RenderTris._M_start=tris;p.RenderTris._M_finish=tris+nt;p.RenderTris._M_end_of_storage._M_data=tris+nt;
 p.RenderQuads._M_start=quads;p.RenderQuads._M_finish=quads+nq;p.RenderQuads._M_end_of_storage._M_data=quads+nq;
 double expected=0.0;
 for(unsigned i=0;i<nt;++i){for(unsigned j=0;j<3;++j)tris[i].VertexIndex[j]=(unsigned short)((i+j)%5);for(unsigned j=0;j<3;++j)expected+=edge_squared(positions[tris[i].VertexIndex[j]],positions[tris[i].VertexIndex[(j+1)%3]]);}
 for(unsigned i=0;i<nq;++i){for(unsigned j=0;j<4;++j)quads[i].VertexIndex[j]=(unsigned short)((i+j)%5);for(unsigned j=0;j<4;++j)expected+=edge_squared(positions[quads[i].VertexIndex[j]],positions[quads[i].VertexIndex[(j+1)%4]]);}
 C4DVector before[5];memcpy(before,positions,sizeof(before));
 p.CalculateAveragePatchSize(p.InitialSimulation);
 if(nt||nq){float want=(float)sqrt(expected/(nt*3+nq*4));float delta=p.AveragePatchSize-want;if(delta<0)delta=-delta;assert(delta<=0.00001f);}
 else assert(p.AveragePatchSize==9.25f);
 assert(memcmp(before,positions,sizeof(before))==0);
 assert(p.InitialSimulation.Size==5&&p.RenderTris.size()==nt&&p.RenderQuads.size()==nq);
}
int main(){for(unsigned nt=0;nt<=9;++nt)for(unsigned nq=0;nq<=5;++nq){run(nt,nq,0.0f,0);run(nt,nq,0.5f,0);run(nt,nq,2.0f,8);run(nt,nq,-2.0f,-4);}puts("00ae0710_TEST PASS");return 0;}