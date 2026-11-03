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



#pragma intrinsic(sqrt)
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
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
    C3DVector(const C3DVector& p){*this=p;}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return (Z*Z+Y*Y+X*X);}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
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
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#include <stdlib.h>
namespace std {
struct __true_type { };
template<class _Tp> class allocator {
public:
    void deallocate(_Tp* p, size_t) {
        free(p);
    }
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
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
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
    void push_back(const _Tp&);
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
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


template<class T> class CArray : public std::vector<T> {
public:
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
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
#define FABLE_ENGINE_CMATRIX3X4_H

class CMatrix3x4 {public:
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 CMatrix3x4(){}
 
 void TransformPoint(C3DVector& point) const {
  const float xx=point.X;
  const float yy=point.Y;
  const float zz=point.Z;
  point.X=E11*xx+(E21*yy+E31*zz)+E41;
  point.Y=E12*xx;
  point.Y+=E32*zz;
  point.Y+=E22*yy;
  point.Y+=E42;
  point.Z=E13*xx;
  point.Z+=E33*zz;
  point.Z+=E23*yy;
  point.Z+=E43;
 }
 void TransformPoint(const C3DVector& pSource,C3DVector& destination)const {
  destination.X=E11*pSource.X+(E21*pSource.Y+E31*pSource.Z)+E41;
  destination.Y=E12*pSource.X;
  destination.Y+=E32*pSource.Z;
  destination.Y+=E22*pSource.Y;
  destination.Y+=E42;
  destination.Z=E13*pSource.X;
  destination.Z+=E33*pSource.Z;
  destination.Z+=E23*pSource.Y;
  destination.Z+=E43;
 }
};
#define FABLE_ENGINE_CPARTICLEDISTANCECONSTRAINT_H

class CParticleDistanceConstraint {public: unsigned long Index1; unsigned long Index2; float Distance; float Strength;};
#define FABLE_ENGINE_CPARTICLEMINMAXDISTANCECONSTRAINT_H

class CParticleMinMaxDistanceConstraint {public: unsigned long Index1; unsigned long Index2; float MinDistance; float MaxDistance;};
#define FABLE_ENGINE_TCONSTRAINTTYPE_H

enum TConstraintType {CONSTRAINT_REPEAT=0,CONSTRAINT_END_REPEAT=1,
 CONSTRAINT_DISTANCE=2,CONSTRAINT_MIN_MAX_DISTANCE=3,CONSTRAINT_UNBEND=4,
 CONSTRAINT_SPHERE_COLLISION=5,CONSTRAINT_CYLINDER_COLLISION=6};

void CParticleProgram::ApplyTransform(const CMatrix3x4& transform)
{
    const CParticleSimulation& sim=PeekInitialSimulation();
    char* data=(char*)Constraints.PeekData();
    unsigned long total_length=Constraints.GetLength();
    unsigned long pos=0;
    while(pos<total_length) {
        const unsigned long* header=(const unsigned long*)(data+pos);
        unsigned long type=header[0];
        unsigned long count=header[1];
        unsigned long length=header[2];
        pos+=3*sizeof(unsigned long);
        if(type==CONSTRAINT_DISTANCE) {
            CParticleDistanceConstraint* constraints=(CParticleDistanceConstraint*)(data+pos);
            for(unsigned long c1=0;c1<count;++c1) {
                CParticleDistanceConstraint& constraint=constraints[c1];
                C3DVector pos1=sim.GetParticlePosition(constraint.Index1);
                C3DVector pos2=sim.GetParticlePosition(constraint.Index2);
                float old_dist=(pos2-pos1).GetMagnitude();
                transform.TransformPoint(pos1);
                transform.TransformPoint(pos2);
                float new_dist=(pos2-pos1).GetMagnitude();
                if(old_dist>0.0f) {
                    float scale=new_dist/old_dist;
                    constraint.Distance*=scale;
                }
            }
        } else if(type==CONSTRAINT_MIN_MAX_DISTANCE) {
            CParticleMinMaxDistanceConstraint* constraints=(CParticleMinMaxDistanceConstraint*)(data+pos);
            for(unsigned long c1=0;c1<count;++c1) {
                CParticleMinMaxDistanceConstraint& constraint=constraints[c1];
                C3DVector pos1=sim.GetParticlePosition(constraint.Index1);
                C3DVector pos2=sim.GetParticlePosition(constraint.Index2);
                float old_dist=(pos2-pos1).GetMagnitude();
                transform.TransformPoint(pos1);
                transform.TransformPoint(pos2);
                float new_dist=(pos2-pos1).GetMagnitude();
                if(old_dist>0.0f) {
                    float scale=new_dist/old_dist;
                    constraint.MinDistance*=scale;
                    constraint.MaxDistance*=scale;
                }
            }
        }
        pos+=length;
    }
    for(unsigned long c1=0;c1<InitialSimulation.GetParticleCount();++c1) {
        C3DVector pos;
        transform.TransformPoint(InitialSimulation.GetParticlePosition(c1),pos);
        InitialSimulation.SetParticlePosition(c1,pos);
    }
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
struct RecordBlock {
 unsigned long head0[3];unsigned long ignored[2];
 unsigned long head1[3];CParticleDistanceConstraint distance[4];
 unsigned long head2[3];CParticleMinMaxDistanceConstraint ranged[4];
 unsigned long head3[3];
};
static bool close_value(double a,double b){double delta=a-b;if(delta<0)delta=-delta;double size=b<0?-b:b;return delta*65536.0<=1.0+size;}
static unsigned case_count;
static void run(unsigned n,int mode,bool withConstraints,int shift){
 CParticleProgram program;C4DVector positions[4];C3DVector original[4];
 const float coordinates[4][3]={{1,2,3},{4,6,3},{4,2,3},{1,2,7}};
 for(unsigned i=0;i<4;++i){original[i]=C3DVector(coordinates[i][0],coordinates[i][1],coordinates[i][2]);positions[i]=C4DVector(original[i],7.0f);}
 program.InitialSimulation.Size=n;program.InitialSimulation.Positions=positions;
 RecordBlock b;memset(&b,0,sizeof(b));
 b.head0[0]=CONSTRAINT_REPEAT;b.head0[1]=7;b.head0[2]=sizeof(b.ignored);b.ignored[0]=0x12345678;b.ignored[1]=0x76543210;
 b.head1[0]=CONSTRAINT_DISTANCE;b.head1[1]=4;b.head1[2]=sizeof(b.distance);
 b.head2[0]=CONSTRAINT_MIN_MAX_DISTANCE;b.head2[1]=4;b.head2[2]=sizeof(b.ranged);
 b.head3[0]=CONSTRAINT_DISTANCE;
 for(unsigned i=0;i<4;++i){b.distance[i].Index1=0;b.distance[i].Index2=i;b.distance[i].Distance=(float)(i+2);b.distance[i].Strength=0.5f;b.ranged[i].Index1=0;b.ranged[i].Index2=i;b.ranged[i].MinDistance=(float)(i+2);b.ranged[i].MaxDistance=(float)(i+4);}
 if(withConstraints){char* p=(char*)malloc(sizeof(b));assert(p);memcpy(p,&b,sizeof(b));program.Constraints.Data._M_start=p;program.Constraints.Data._M_finish=p+sizeof(b);program.Constraints.Data._M_end_of_storage._M_data=p+sizeof(b);}
 CMatrix3x4 matrix;memset(&matrix,0,sizeof(matrix));
 switch(mode){
 case 0:matrix.E11=matrix.E22=matrix.E33=1.0f;break;
 case 1:matrix.E12=2.0f;matrix.E21=-2.0f;matrix.E33=2.0f;break;
 case 2:matrix.E11=matrix.E22=matrix.E33=-2.0f;break;
 case 3:break;
 case 4:matrix.E11=2.0f;matrix.E22=3.0f;matrix.E33=4.0f;break;
 case 5:matrix.E11=1.0f;matrix.E21=1.0f;matrix.E22=2.0f;matrix.E31=-1.0f;matrix.E32=1.0f;matrix.E33=1.0f;break;
 case 6:matrix.E13=1.0f;matrix.E21=-1.0f;matrix.E32=1.0f;break;
 case 7:matrix.E11=1.0f;matrix.E12=-2.0f;matrix.E13=3.0f;matrix.E21=-1.0f;matrix.E22=4.0f;matrix.E23=2.0f;matrix.E31=2.0f;matrix.E32=1.0f;matrix.E33=-3.0f;break;
 }
 const float offsets[4][3]={{0,0,0},{8,-4,16},{-16,32,-8},{3,7,-11}};
 matrix.E41=offsets[shift][0];matrix.E42=offsets[shift][1];matrix.E43=offsets[shift][2];
 double expected[4][3];
 for(unsigned i=0;i<4;++i){
  expected[i][0]=(double)original[i].X*matrix.E11+(double)original[i].Y*matrix.E21+(double)original[i].Z*matrix.E31+matrix.E41;
  expected[i][1]=(double)original[i].X*matrix.E12+(double)original[i].Y*matrix.E22+(double)original[i].Z*matrix.E32+matrix.E42;
  expected[i][2]=(double)original[i].X*matrix.E13+(double)original[i].Y*matrix.E23+(double)original[i].Z*matrix.E33+matrix.E43;
 }
 program.ApplyTransform(matrix);
 for(unsigned i=0;i<4;++i){if(i>=n){assert(positions[i].X==original[i].X&&positions[i].Y==original[i].Y&&positions[i].Z==original[i].Z&&positions[i].W==7.0f);continue;}
  assert(positions[i].X==(float)expected[i][0]&&positions[i].Y==(float)expected[i][1]&&positions[i].Z==(float)expected[i][2]&&positions[i].W==1.0f);
 }
 if(withConstraints){const RecordBlock& after=*(const RecordBlock*)program.Constraints.Data._M_start;
  assert(memcmp(after.head0,b.head0,sizeof(b.head0))==0&&memcmp(after.ignored,b.ignored,sizeof(b.ignored))==0&&memcmp(after.head1,b.head1,sizeof(b.head1))==0&&memcmp(after.head2,b.head2,sizeof(b.head2))==0&&memcmp(after.head3,b.head3,sizeof(b.head3))==0);
  for(unsigned i=0;i<4;++i){double before_sq=0,after_sq=0;
   for(unsigned k=0;k<3;++k){double old_delta=coordinates[i][k]-coordinates[0][k];double new_delta=expected[i][k]-expected[0][k];before_sq+=old_delta*old_delta;after_sq+=new_delta*new_delta;}
   double factor=before_sq==0?1.0:sqrt(after_sq)/sqrt(before_sq);
   assert(after.distance[i].Index1==0&&after.distance[i].Index2==i&&after.distance[i].Strength==0.5f);
   assert(close_value(after.distance[i].Distance,b.distance[i].Distance*factor));
   assert(after.ranged[i].Index1==0&&after.ranged[i].Index2==i);
   assert(close_value(after.ranged[i].MinDistance,b.ranged[i].MinDistance*factor)&&close_value(after.ranged[i].MaxDistance,b.ranged[i].MaxDistance*factor));
  }
 }
 ++case_count;
}
int main(){for(unsigned n=0;n<=4;++n)for(int mode=0;mode<8;++mode)for(int shift=0;shift<4;++shift){run(n,mode,false,shift);run(n,mode,true,shift);}assert(case_count==320);puts("00ae0070_TEST PASS");return 0;}