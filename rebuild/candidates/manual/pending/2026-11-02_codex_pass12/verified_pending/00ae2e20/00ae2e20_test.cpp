// Standalone target and active production headers expanded below.
// Fixture-only implementations provide ordered-tree, sort and unrelated base lifecycles.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <new>
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



#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM
#define FABLE_RETAIL_PARTICLE_SPRINGS
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


#define FABLE_VECTOR_INLINE __forceinline
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
 friend class CParticleProgram;
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

#define FABLE_RETAIL_HEADER_CDATASTREAM
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
#define FABLE_STLPORT_PARTICLE_SPRINGS_H




#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned long>{typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<long>{typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity{const T&operator()(const T&x)const{return x;}};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T,class U>struct pair{T first;U second;pair(){}pair(const T&a,const U&b):first(a),second(b){} };
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline void swap(T&a,T&b){T x=a;a=b;b=x;}
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>__declspec(noinline) void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
template<class T>class allocator{public:allocator(){}allocator(const allocator&){}~allocator(){}T*allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class Value,class T,class A>struct _STLP_alloc_proxy:public A{Value _M_data;_STLP_alloc_proxy(const A&a,Value p):A(a),_M_data(p){} };
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>struct _Rb_global{static _Rb_tree_node_base*__fastcall _M_increment(_Rb_tree_node_base*x){if(x->_M_right!=0){x=x->_M_right;while(x->_M_left!=0)x=x->_M_left;}else{_Rb_tree_node_base*y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}return x;}};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{typedef typename Traits::reference reference;_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<T>*p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<T,_Nonconst_traits<T> >&x){_M_node=x._M_node;}reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}};
template<class T,class A>struct _Rb_tree_base{public:_Rb_tree_base():_M_header(allocator<_Rb_tree_node<T> >(),0){} protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;pair<iterator,bool>insert_unique(const V&);const_iterator begin()const{return (_Rb_tree_node<V>*)this->_M_header._M_data->_M_left;}const_iterator end()const{return this->_M_header._M_data;}};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{public:typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::const_iterator const_iterator;typedef const_iterator iterator;set();set(const set&);~set();pair<iterator,bool>insert(const K&x){pair<typename _Rep_type::iterator,bool>p=_M_t.insert_unique(x);return pair<iterator,bool>((const iterator&)p.first,p.second);}iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}};
template<>struct __type_traits<set<unsigned long> >{typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}_Vector_base(unsigned int n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}};
template<class T>inline T*__uninitialized_copy(const T*first,const T*last,T*out,const __false_type&){T*cur=out;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}
inline void*__copy_trivial(const void*first,const void*last,void*out){return last==first?out:(char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class T>inline T*__uninitialized_copy(const T*first,const T*last,T*out,const __true_type&){return (T*)__copy_trivial(first,last,out);}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __false_type&){I cur=p;for(;n>0;--n,++cur)_Construct(cur,x);return cur;}
template<class T>inline T*fill_n(T*p,unsigned int n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{typedef _Vector_base<T,A>Base;public:typedef T*iterator;typedef const T*const_iterator;typedef typename __type_traits<T>::has_trivial_assignment_operator IsPOD;explicit vector(const A&a=A()):Base(a){}explicit vector(unsigned int n):Base(n,A()){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,T(),IsPOD());}vector(unsigned int n,const T&x,const A&a=A()):Base(n,a){this->_M_finish=fill_n(this->_M_start,n,x);}vector(const vector&x):Base(x.size(),x.get_allocator()){this->_M_finish=__uninitialized_copy(x._M_start,x._M_finish,this->_M_start,IsPOD());}A get_allocator()const{return (const A&)this->_M_end_of_storage;}~vector(){_Destroy(this->_M_start,this->_M_finish);}unsigned int size()const{return this->_M_finish-this->_M_start;}bool empty()const{return this->_M_start==this->_M_finish;}iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}void swap(vector&x){std::swap(this->_M_start,x._M_start);std::swap(this->_M_finish,x._M_finish);std::swap(this->_M_end_of_storage,x._M_end_of_storage);}void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,IsPOD(),1,true);}protected:
void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
void _M_set(T*a,T*b,T*c){this->_M_start=a;this->_M_finish=b;this->_M_end_of_storage._M_data=c;}
void _M_insert_overflow(T*pos,const T&x,const __true_type&,unsigned int n,bool atEnd=false){unsigned int old=size(),len=old+(max)(old,n);T*start=this->_M_end_of_storage.allocate(len);T*finish=(T*)__copy_trivial(this->_M_start,pos,start);finish=fill_n(finish,n,x);if(!atEnd)finish=(T*)__copy_trivial(pos,this->_M_finish,finish);_M_clear();_M_set(start,finish,start+len);}
void _M_insert_overflow(T*pos,const T&x,const __false_type&,unsigned int n,bool atEnd=false){unsigned int old=size(),len=old+(max)(old,n);T*start=this->_M_end_of_storage.allocate(len);T*finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());if(n==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,n,x,__false_type());if(!atEnd)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());_M_clear();_M_set(start,finish,start+len);}
};
template<class I>void __fastcall sort(I,I);
}
template<class T>class CArray:public std::vector<T>{public:CArray(){}explicit CArray(unsigned int n):std::vector<T>(n){}CArray(unsigned int n,const T&x):std::vector<T>(n,x){}CArray(const CArray&x):std::vector<T>(x){} T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,x);++this->_M_finish;}else std::vector<T>::push_back(x);} };
template<>__declspec(noinline) CArray<std::set<unsigned long> >::CArray(unsigned int n):std::vector<std::set<unsigned long> >(n){}
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
class CMatrix3x4;
class CParticleDistanceConstraint;
class CParticleProgram:public CMovableResource {
public:
 CParticleProgram();
 virtual ~CParticleProgram();
 virtual void RelocateData(long,void*,void*,long);
 const CParticleSimulation& PeekInitialSimulation()const{return InitialSimulation;}
 CParticleSimulation& GetInitialSimulation(){return InitialSimulation;}
 void ApplyTransform(const CMatrix3x4&);
 void OptimiseSpringOrder(CArray<CParticleDistanceConstraint>&)const;
private:
 CParticleSimulation InitialSimulation;
 unsigned char ExportParticles[16]; 
 unsigned char RenderTris[12]; 
 unsigned char RenderQuads[12]; 
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
#define FABLE_CPARTICLEDISTANCECONSTRAINT_H

class CParticleDistanceConstraint{public:unsigned long Index1,Index2;float Distance,Strength;};
extern const float DELTA_NOTIONAL_ZERO;
void CParticleProgram::OptimiseSpringOrder(CArray<CParticleDistanceConstraint>&springs)const
{
 unsigned long no_particles=InitialSimulation.Size;
 CArray<std::set<unsigned long> >vertex_connections(no_particles);
 for(unsigned long c_spring=0;c_spring<springs.size();++c_spring){const CParticleDistanceConstraint&spring=springs[c_spring];vertex_connections[spring.Index1].insert(spring.Index2);vertex_connections[spring.Index2].insert(spring.Index1);}
 CArray<long>egde_distances(no_particles,-1);
 CArray<unsigned long>particle_list;
 for(unsigned long c_particle=0;c_particle<no_particles;++c_particle){if(InitialSimulation.SimulationAlphas[c_particle]<DELTA_NOTIONAL_ZERO){egde_distances[c_particle]=0;particle_list.push_back(c_particle);}}
 long current_dist=1;
 while(!particle_list.empty()){
  CArray<unsigned long>next_particle_list;
  for(unsigned long c_particle=0;c_particle<particle_list.size();++c_particle){unsigned long here=particle_list[c_particle];const std::set<unsigned long>&connection_list=vertex_connections[here];for(std::set<unsigned long>::const_iterator itr=connection_list.begin();itr!=connection_list.end();++itr){unsigned long there=*itr;if(egde_distances[there]==-1){egde_distances[there]=current_dist;next_particle_list.push_back(there);}}}
  particle_list.swap(next_particle_list);
  ++current_dist;
 }
 CArray<std::pair<long,unsigned long> >sorted_order;
 for(unsigned long c_spring=0;c_spring<springs.size();++c_spring){const CParticleDistanceConstraint&spring=springs[c_spring];long dist=egde_distances[spring.Index1]+egde_distances[spring.Index2];sorted_order.push_back(std::pair<long,unsigned long>(dist,c_spring));}
 std::sort(sorted_order.begin(),sorted_order.end());
 CArray<CParticleDistanceConstraint>springs_copy(springs);
 for(unsigned long c_spring=0;c_spring<springs.size();++c_spring)springs[c_spring]=springs_copy[sorted_order[c_spring].second];
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


extern const float DELTA_NOTIONAL_ZERO=0.0009765625f;
namespace std {
static unsigned live_nodes=0;
template<class T>static _Rb_tree_node<T>*allocate_node(){++live_nodes;return new _Rb_tree_node<T>;}
template<class T>static void release_nodes(_Rb_tree_node<T>*p){if(!p)return;release_nodes((_Rb_tree_node<T>*)p->_M_left);release_nodes((_Rb_tree_node<T>*)p->_M_right);delete p;--live_nodes;}
template<class K,class V,class Key,class Compare,class A>pair<typename _Rb_tree<K,V,Key,Compare,A>::iterator,bool> _Rb_tree<K,V,Key,Compare,A>::insert_unique(const V&value){
 _Rb_tree_node<V>*header=this->_M_header._M_data;_Rb_tree_node_base*parent=header;_Rb_tree_node_base*cur=header->_M_parent;bool left=true;
 while(cur){parent=cur;V old=((_Rb_tree_node<V>*)cur)->_M_value_field;if(_M_key_compare(value,old)){left=true;cur=cur->_M_left;}else if(_M_key_compare(old,value)){left=false;cur=cur->_M_right;}else return pair<iterator,bool>(iterator((_Rb_tree_node<V>*)cur),false);}
 _Rb_tree_node<V>*node=allocate_node<V>();node->_M_value_field=value;node->_M_parent=parent;node->_M_left=node->_M_right=0;node->_M_color=false;
 if(parent==header){header->_M_parent=node;header->_M_left=header->_M_right=node;}else if(left){parent->_M_left=node;if(header->_M_left==parent)header->_M_left=node;}else{parent->_M_right=node;if(header->_M_right==parent)header->_M_right=node;}
 ++_M_node_count;return pair<iterator,bool>(iterator(node),true);
}
template<class K,class Compare,class A>set<K,Compare,A>::set(){_Rb_tree_node<K>*h=allocate_node<K>();h->_M_parent=0;h->_M_left=h->_M_right=h;h->_M_color=true;_M_t._M_header._M_data=h;_M_t._M_node_count=0;}
template<class K,class Compare,class A>set<K,Compare,A>::set(const set&other){_Rb_tree_node<K>*h=allocate_node<K>();h->_M_parent=0;h->_M_left=h->_M_right=h;h->_M_color=true;_M_t._M_header._M_data=h;_M_t._M_node_count=0;for(const_iterator i=other.begin();i!=other.end();++i)insert(*i);}
template<class K,class Compare,class A>set<K,Compare,A>::~set(){_Rb_tree_node<K>*h=_M_t._M_header._M_data;if(h){release_nodes((_Rb_tree_node<K>*)h->_M_parent);delete h;--live_nodes;}}
template<class I>void __fastcall sort(I first,I last){for(I p=first;p!=last;++p)for(I q=p;q!=first;){I before=q-1;if(before->first<q->first||(before->first==q->first&&before->second<=q->second))break;swap(*before,*q);q=before;}}
}
static void add(CArray<CParticleDistanceConstraint>&springs,unsigned first,unsigned second){CParticleDistanceConstraint c;c.Index1=first;c.Index2=second;c.Distance=(float)springs.size()+0.5f;c.Strength=(float)springs.size()*0.25f;springs.push_back(c);}
static void run(unsigned n,unsigned graph,unsigned seed_mode,unsigned reverse){
 CParticleProgram program;float alpha[16];for(unsigned i=0;i<16;++i){bool seed=seed_mode==0?false:seed_mode==1?true:seed_mode==2?i==0:seed_mode==3?i+1==n:(i%3)==0;alpha[i]=seed?0.0f:DELTA_NOTIONAL_ZERO;}
 program.InitialSimulation.Size=n;program.InitialSimulation.SimulationAlphas=alpha;
 CArray<CParticleDistanceConstraint> springs;
 for(unsigned i=0;i<n;++i){unsigned a=reverse?n-1-i:i;if(graph==0&&i+1<n)add(springs,a,reverse?a-1:a+1);if(graph==1&&a)add(springs,0,a);if(graph==2&&n>1)add(springs,a,(a+1)%n);if(graph==3&&a%3!=2&&a+1<n)add(springs,a,a+1);if(graph==4){add(springs,a,a);if(a+1<n){add(springs,a,a+1);add(springs,a+1,a);}}}
 CParticleDistanceConstraint saved[64];unsigned order[64];unsigned total=springs.size();assert(total<=64);bool connected[16][16];memset(connected,0,sizeof(connected));long distance[16];
 for(unsigned i=0;i<n;++i)distance[i]=alpha[i]<DELTA_NOTIONAL_ZERO?0:-1;
 for(unsigned i=0;i<total;++i){saved[i]=springs[i];order[i]=i;connected[saved[i].Index1][saved[i].Index2]=connected[saved[i].Index2][saved[i].Index1]=true;}
 // Independent breadth levels with a fixed adjacency matrix, without the production tree/frontier containers.
 for(long level=0;level<(long)n;++level)for(unsigned a=0;a<n;++a)if(distance[a]==level)for(unsigned b=0;b<n;++b)if(connected[a][b]&&distance[b]==-1)distance[b]=level+1;
 for(unsigned i=0;i<total;++i)for(unsigned j=i+1;j<total;++j){long left=distance[saved[order[i]].Index1]+distance[saved[order[i]].Index2],right=distance[saved[order[j]].Index1]+distance[saved[order[j]].Index2];if(right<left||(right==left&&order[j]<order[i])){unsigned tmp=order[i];order[i]=order[j];order[j]=tmp;}}
 program.OptimiseSpringOrder(springs);assert(springs.size()==total);assert(program.InitialSimulation.Size==n&&program.InitialSimulation.SimulationAlphas==alpha);assert(std::live_nodes==0);
 for(unsigned i=0;i<total;++i){const CParticleDistanceConstraint&expected=saved[order[i]];assert(springs[i].Index1==expected.Index1&&springs[i].Index2==expected.Index2&&springs[i].Distance==expected.Distance&&springs[i].Strength==expected.Strength);}
}
int main(){
#ifdef _MSC_VER
 assert(sizeof(CParticleProgram)==0x140&&sizeof(CParticleDistanceConstraint)==16&&sizeof(std::set<unsigned long>)==12);
#endif
 const unsigned sizes[]={0,1,2,3,4,7,12,16};for(unsigned i=0;i<8;++i)for(unsigned graph=0;graph<5;++graph)for(unsigned seed=0;seed<5;++seed)for(unsigned reverse=0;reverse<2;++reverse)run(sizes[i],graph,seed,reverse);
 puts("00ae2e20_TEST PASS");return 0;
}
