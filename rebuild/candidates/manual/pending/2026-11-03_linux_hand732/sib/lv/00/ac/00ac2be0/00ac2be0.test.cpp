// Standalone production target and active headers expanded below.
// Fixture-only access visibility exposes the tree for independent structural assertions.
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
#define private public
#define protected public



#define FABLE_ENGINE_C3DMESHPARTITIONBUILDERELEMENT_H
#define FABLE_ENGINE_CARRAY_H




#define FABLE_STLPORT46_GEOMETRY_H
#define FABLE_STLPORT_CONTAINERS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<bool>{typedef __true_type is_POD_type;typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned char>{typedef __true_type is_POD_type;typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>class allocator{public:allocator(){} allocator(const allocator&){} ~allocator(){} T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};struct random_access_iterator_tag:public bidirectional_iterator_tag{};
template<class I,class O,class D>inline O __copy(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n){*d=*a;++a;++d;}return d;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __false_type&){return __copy(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O,class D>inline O __copy_backward(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n)*--d=*--b;return d;}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __false_type&){return __copy_backward(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __true_type&){ptrdiff_t n=b-a;return n?(O)memmove(d-n,a,n*sizeof(*a)):d;}
template<class I,class T>inline void fill(I a,I b,const T&x){for(;a!=b;++a)*a=x;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*copy(const T*a,const T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class T>inline T*copy(T*a,T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __false_type&){O cur=d;for(;a!=b;++a,++cur)_Construct(&*cur,*a);return cur;}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __false_type&){I cur=p;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef size_t size_type;typedef A allocator_type;typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 __declspec(noinline) void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false);
 __declspec(noinline) void _M_insert_overflow(T*position,const T&x,const __false_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=new_start;
 new_finish=__uninitialized_copy(this->_M_start,position,new_start,_IsPODType());
 if(fill_len==1){_Construct(new_finish,x);++new_finish;}else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,_IsPODType());
 if(!at_end)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,_IsPODType());
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
 template<class I>pointer _M_allocate_and_copy(size_type n,I first,I last){pointer result=this->_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,_IsPODType());return result;}

public:
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 explicit vector(size_type n):_Vector_base<T,A>(n,A()){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,T(),_IsPODType());}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 vector&operator=(const vector&);
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 __declspec(noinline) void _M_fill_insert(iterator,size_type,const T&);
 void insert(iterator p,size_type n,const T&x){_M_fill_insert(p,n,x);}
 
 
 void resize(size_type n,const T& x){if(n<size())erase(begin()+n,end());else insert(end(),n-size(),x);}
 void resize(size_type n){resize(n,T());}
};
template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector& x){
 if(&x!=this){const size_type xlen=x.size();if(xlen>capacity()){pointer tmp=_M_allocate_and_copy(xlen,(const_pointer)x._M_start+0,(const_pointer)x._M_finish+0);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;}
 else if(size()>=xlen){pointer i=__copy_ptrs((const_pointer)x._M_start+0,(const_pointer)x._M_finish+0,(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);}
 else{__copy_ptrs((const_pointer)x._M_start,(const_pointer)x._M_start+size(),(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());__uninitialized_copy((const_pointer)x._M_start+size(),(const_pointer)x._M_finish+0,this->_M_finish,_IsPODType());}this->_M_finish=this->_M_start+xlen;}return *this;
}
template<class T,class A>void vector<T,A>::_M_fill_insert(iterator position,size_type n,const T&x){
 if(n!=0){if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T x_copy=x;const size_type elems_after=this->_M_finish-position;pointer old_finish=this->_M_finish;
 if(elems_after>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());this->_M_finish+=n;__copy_backward_ptrs(position,old_finish-n,old_finish,typename __type_traits<T>::has_trivial_assignment_operator());fill(position,position+n,x_copy);}
 else{__uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy,_IsPODType());this->_M_finish+=n-elems_after;__uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());this->_M_finish+=elems_after;fill(position,old_finish,x_copy);}}
 else _M_insert_overflow(position,x,_IsPODType(),n);}
}
template<class T,class A>void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);pointer new_start=this->_M_end_of_storage.allocate(len);pointer new_finish=(pointer)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(pointer)__copy_trivial(position,this->_M_finish,new_finish);_M_clear();_M_set(new_start,new_finish,new_start+len);
}

}

template<class T>class CArray:public std::vector<T>{
public:CArray(){} void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,x);++this->_M_finish;}else std::vector<T>::push_back(x);} T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};







#define FABLE_ENGINE_C3DBOUNDINGBOX_H
#define FABLE_ENGINE_C3DVECTOR_H


#define FABLE_VECTOR_INLINE inline
#define FABLE_ENGINE_GFMATHS_H
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned long GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
 *(unsigned long*)&n &= 0x7fffffffUL;
 return n;
}
inline
float __fastcall GFFastOneOverSqrt(float n){
 typedef unsigned long word_type;
 word_type bits=*(word_type*)&n;
 word_type exponent=(0x17c-((bits>>23)&0xff))<<22;
 word_type mantissa=(word_type)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15;
 float estimate;
 *(word_type*)&estimate=(exponent&0xff800000)|mantissa;
 return (3.0f-estimate*estimate*n)*estimate*0.5f;
}
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
    C3DVector(const C3DVector& p){Initialise(p.X,p.Y,p.Z);}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE


class C3DBoundingBox {
public:
 void BuildFromPoints(const std::vector<C3DVector>&);
 const C3DVector GetCenter()const{return (MinCoord+MaxCoord)*0.5f;}
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#define FABLE_ENGINE_C3DBOUNDINGSPHERE_H


class C3DBoundingSphere {
public:
 C3DBoundingSphere():Center(0.0f,0.0f,0.0f),Radius(0.0f){}
 void AccurateBuildFromPoints(const std::vector<C3DVector>&,float);
 float GetRadius()const{return Radius;}
    C3DVector Center;
    float Radius;
};
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
#define FABLE_ENGINE_C3DTRIANGLE2_H


class C3DTriangle2 {
public:
 unsigned long GetVertexIndex(unsigned long index)const{return VertexIndex[index];}
private:
 unsigned short VertexIndex[3];
};


class C3DMeshPartitionBuilderElement { public:
 long StartTriangle,TriangleCount;C3DBoundingBox BoundingBox;
 C3DBoundingSphere BoundingSphere;CArray<C3DTriangle2>Triangles;
 C3DMeshPartitionBuilderElement*SubElements[2];
 long PartitionSpace(const C3DVertex2*,long,float);
public:
 C3DMeshPartitionBuilderElement(){SubElements[0]=0;SubElements[1]=0;}
 ~C3DMeshPartitionBuilderElement(){delete SubElements[0];delete SubElements[1];}
};
long C3DMeshPartitionBuilderElement::PartitionSpace(const C3DVertex2*verts,long start_tri_index,float parent_radius)
{
 StartTriangle=start_tri_index;
 delete SubElements[0];delete SubElements[1];SubElements[0]=0;SubElements[1]=0;
 CArray<C3DVector>section_verts;CArray<bool>added_mask;
 for(long tri=0;tri<Triangles.size();++tri)for(long id=0;id<3;++id){long vert_id=Triangles[tri].GetVertexIndex(id);if(vert_id>=added_mask.size())added_mask.resize(vert_id+1,false);if(!added_mask[vert_id]){added_mask[vert_id]=true;section_verts.push_back(verts[vert_id].GetPosition());}}
 BoundingBox.BuildFromPoints(section_verts);BoundingSphere.AccurateBuildFromPoints(section_verts,0.001f);
 if(Triangles.size()==1){TriangleCount=1;return 1;}
 float best_axis_child_size=1e30f;long best_axis=-1;
 CArray<C3DTriangle2>left_tri_list[3];CArray<C3DTriangle2>right_tri_list[3];
 for(long axis=0;axis<3;++axis){
  C3DVector left_box_min(1e30f,1e30f,1e30f),left_box_max(-1e30f,-1e30f,-1e30f),right_box_min(1e30f,1e30f,1e30f),right_box_max(-1e30f,-1e30f,-1e30f);
  for(long t=0;t<Triangles.size();++t){
   C3DTriangle2&triangle=Triangles[t];C3DVector vert0=verts[triangle.GetVertexIndex(0)].GetPosition(),vert1=verts[triangle.GetVertexIndex(1)].GetPosition(),vert2=verts[triangle.GetVertexIndex(2)].GetPosition();
   C3DVector tri_min(((vert0.X<vert1.X?vert0.X:vert1.X)<vert2.X?(vert0.X<vert1.X?vert0.X:vert1.X):vert2.X),((vert0.Y<vert1.Y?vert0.Y:vert1.Y)<vert2.Y?(vert0.Y<vert1.Y?vert0.Y:vert1.Y):vert2.Y),((vert0.Z<vert1.Z?vert0.Z:vert1.Z)<vert2.Z?(vert0.Z<vert1.Z?vert0.Z:vert1.Z):vert2.Z)),tri_max(((vert0.X>vert1.X?vert0.X:vert1.X)>vert2.X?(vert0.X>vert1.X?vert0.X:vert1.X):vert2.X),((vert0.Y>vert1.Y?vert0.Y:vert1.Y)>vert2.Y?(vert0.Y>vert1.Y?vert0.Y:vert1.Y):vert2.Y),((vert0.Z>vert1.Z?vert0.Z:vert1.Z)>vert2.Z?(vert0.Z>vert1.Z?vert0.Z:vert1.Z):vert2.Z));
   C3DVector tri_center((tri_min.X+tri_max.X)*0.5f,(tri_min.Y+tri_max.Y)*0.5f,(tri_min.Z+tri_max.Z)*0.5f);
   bool on_right=axis==0?tri_center.X>BoundingBox.GetCenter().X:axis==1?tri_center.Y>BoundingBox.GetCenter().Y:tri_center.Z>BoundingBox.GetCenter().Z;
   C3DVector*box_min;C3DVector*box_max;
   if(on_right){box_min=&right_box_min;box_max=&right_box_max;right_tri_list[axis].push_back(triangle);}else{box_min=&left_box_min;box_max=&left_box_max;left_tri_list[axis].push_back(triangle);}
   box_min->LimitUpper(tri_min);box_max->LimitLower(tri_max);
  }
  if(left_tri_list[axis].size()&&right_tri_list[axis].size()){
   float size_left=left_box_max.X-left_box_min.X+left_box_max.Y-left_box_min.Y+left_box_max.Z-left_box_min.Z;
   float size_right=right_box_max.X-right_box_min.X+right_box_max.Y-right_box_min.Y+right_box_max.Z-right_box_min.Z;
   float max_child_size=size_left>size_right?size_left:size_right;
   if(max_child_size<best_axis_child_size){best_axis_child_size=max_child_size;best_axis=axis;}
  }
 }
 if(best_axis<0){TriangleCount=Triangles.size();return TriangleCount;}
 SubElements[0]=new C3DMeshPartitionBuilderElement;SubElements[1]=new C3DMeshPartitionBuilderElement;
 for(long n=0;n<left_tri_list[best_axis].size();++n)SubElements[0]->Triangles.push_back(left_tri_list[best_axis][n]);
 for(long n=0;n<right_tri_list[best_axis].size();++n)SubElements[1]->Triangles.push_back(right_tri_list[best_axis][n]);
 Triangles.clear();
 long count0=SubElements[0]->PartitionSpace(verts,start_tri_index,BoundingSphere.GetRadius());
 long count1=SubElements[1]->PartitionSpace(verts,start_tri_index+count0,BoundingSphere.GetRadius());
 TriangleCount=count0+count1;return TriangleCount;
}

static unsigned node_builds=0,sphere_builds=0;
void C3DBoundingBox::BuildFromPoints(const std::vector<C3DVector>& points){
 ++node_builds;
 if(points.size()==0){MinCoord=C3DVector(0,0,0);MaxCoord=C3DVector(0,0,0);return;}
 MinCoord=points[0];MaxCoord=points[0];
 for(unsigned i=1;i<points.size();++i){MinCoord.LimitUpper(points[i]);MaxCoord.LimitLower(points[i]);}
}
void C3DBoundingSphere::AccurateBuildFromPoints(const std::vector<C3DVector>& points,float accuracy){
 assert(accuracy==0.001f);++sphere_builds;
 // This callee's result is passed through as parent_radius, unused by this leaf.
 Center=points.size()?points[0]:C3DVector(0,0,0);Radius=2.0f;
}
static float component(const C3DVector& v,unsigned k){return k==0?v.X:k==1?v.Y:v.Z;}
static void bounds(const C3DVertex2* vertices,const C3DTriangle2* triangles,const unsigned*ids,unsigned count,float low[3],float high[3]){
 for(unsigned k=0;k<3;++k){low[k]=count?1e30f:0;high[k]=count?-1e30f:0;}
 for(unsigned i=0;i<count;++i)for(unsigned j=0;j<3;++j)for(unsigned k=0;k<3;++k){float value=component(vertices[triangles[ids[i]].GetVertexIndex(j)].Position,k);if(value<low[k])low[k]=value;if(value>high[k])high[k]=value;}
}
static unsigned validate(const C3DMeshPartitionBuilderElement*node,const C3DVertex2*vertices,const C3DTriangle2*triangles,const unsigned*ids,unsigned count,long start){
 assert(node->StartTriangle==start&&node->TriangleCount==(long)count);
 float low[3],high[3];bounds(vertices,triangles,ids,count,low,high);
 for(unsigned k=0;k<3;++k){assert(component(node->BoundingBox.MinCoord,k)==low[k]);assert(component(node->BoundingBox.MaxCoord,k)==high[k]);}
 int best=-1;float best_size=1e30f;unsigned expected[3][2][64];unsigned counts[3][2];memset(counts,0,sizeof(counts));
 if(count!=1)for(unsigned axis=0;axis<3;++axis){
  for(unsigned i=0;i<count;++i){float tri_low[3],tri_high[3];bounds(vertices,triangles,ids+i,1,tri_low,tri_high);unsigned side=(tri_low[axis]+tri_high[axis])>(low[axis]+high[axis])?1:0;expected[axis][side][counts[axis][side]++]=ids[i];}
  if(counts[axis][0]&&counts[axis][1]){float a[3],b[3];bounds(vertices,triangles,expected[axis][0],counts[axis][0],a,b);float left=b[0]-a[0]+b[1]-a[1]+b[2]-a[2];bounds(vertices,triangles,expected[axis][1],counts[axis][1],a,b);float right=b[0]-a[0]+b[1]-a[1]+b[2]-a[2];float worst=left>right?left:right;if(worst<best_size){best_size=worst;best=(int)axis;}}
 }
 if(best<0){assert(!node->SubElements[0]&&!node->SubElements[1]);assert(node->Triangles.size()==count);for(unsigned i=0;i<count;++i)for(unsigned j=0;j<3;++j)assert(node->Triangles[i].GetVertexIndex(j)==triangles[ids[i]].GetVertexIndex(j));return 1;}
 assert(node->SubElements[0]&&node->SubElements[1]&&node->Triangles.size()==0);
 unsigned a=validate(node->SubElements[0],vertices,triangles,expected[best][0],counts[best][0],start);
 unsigned b=validate(node->SubElements[1],vertices,triangles,expected[best][1],counts[best][1],start+counts[best][0]);
 return 1+a+b;
}
static void run(unsigned count,unsigned axis,unsigned mode,long start){
 C3DVertex2 vertices[256];memset(vertices,0,sizeof(vertices));C3DTriangle2 triangles[64];unsigned ids[64];C3DMeshPartitionBuilderElement root;
 for(unsigned i=0;i<count;++i){unsigned order=mode==2?count-1-i:i;unsigned offset=mode==1?0:order*4;for(unsigned j=0;j<3;++j){unsigned v=order*9+j*2;triangles[i].VertexIndex[j]=(unsigned short)v;float xyz[3]={0,0,0};xyz[axis]=(float)offset;xyz[(axis+1)%3]=(float)(j&1);xyz[(axis+2)%3]=(float)((j>>1)&1);if(mode==3){xyz[(axis+1)%3]+=(float)((order%3)*4);xyz[(axis+2)%3]+=(float)((order%2)*8);}vertices[v].Position=C3DVector(xyz[0],xyz[1],xyz[2]);}root.Triangles.push_back(triangles[i]);ids[i]=i;}
 // Existing child trees must be discarded before rebuilding the parent's partition.
 root.SubElements[0]=new C3DMeshPartitionBuilderElement;root.SubElements[1]=new C3DMeshPartitionBuilderElement;
 if(count){root.SubElements[0]->Triangles.push_back(triangles[0]);root.SubElements[1]->Triangles.push_back(triangles[count-1]);}
 node_builds=sphere_builds=0;long result=root.PartitionSpace(vertices,start,32.0f);assert(result==(long)count);
 unsigned expected_nodes=validate(&root,vertices,triangles,ids,count,start);assert(node_builds==expected_nodes&&sphere_builds==expected_nodes);
}
int main(){
#ifdef _MSC_VER
 assert(sizeof(C3DMeshPartitionBuilderElement)==68&&sizeof(C3DTriangle2)==6&&sizeof(C3DVertex2)==32);
#endif
 const unsigned counts[]={0,1,2,3,4,7,8,15,23};
 for(unsigned i=0;i<9;++i)for(unsigned axis=0;axis<3;++axis)for(unsigned mode=0;mode<4;++mode){run(counts[i],axis,mode,0);run(counts[i],axis,mode,37);}
 puts("00ac2be0_TEST PASS");return 0;
}