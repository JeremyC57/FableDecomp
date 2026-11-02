#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __thiscall
#define __forceinline inline
#define __declspec(x)
#endif



#ifdef _MSC_VER
#include <math.h>
#else
extern "C" {double sqrt(double);double fabs(double);double acos(double);double sin(double);double cos(double);double tanh(double); }
#endif
#pragma intrinsic(sqrt,fabs)
#define FABLE_STLPORT_CYLINDER
#define FABLE_RETAIL_VECTOR_SPLINE
#ifndef FABLE_NBBBMATHS_CCYLINDER_H
#define FABLE_NBBBMATHS_CCYLINDER_H

#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H


#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
#ifndef FABLE_ENGINE_GFMATHS_H
#define FABLE_ENGINE_GFMATHS_H
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned long GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned long*)&n &= 0x7fffffffUL;
#else
 *(unsigned int*)&n &= 0x7fffffffU;
#endif
 return n;
}
#ifdef _MSC_VER
__forceinline
#else
inline
#endif
float __fastcall GFFastOneOverSqrt(float n){
#ifdef _MSC_VER
 typedef unsigned long word_type;
#else
 typedef unsigned int word_type;
#endif
 word_type bits=*(word_type*)&n;
 word_type exponent=(0x17c-((bits>>23)&0xff))<<22;
 word_type mantissa=(word_type)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15;
 float estimate;
 *(word_type*)&estimate=(exponent&0xff800000)|mantissa;
 return (3.0f-estimate*estimate*n)*estimate*0.5f;
}
#endif

#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    C3DVector(const C3DVector& p){*this=p;}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#ifdef FABLE_RETAIL_VECTOR_SPLINE
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif




#ifndef FABLE_STLPORT46_VECTOR_ROOTS_H
#define FABLE_STLPORT46_VECTOR_ROOTS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t, void* p) { return p; }
#else
#include <new>
#endif
namespace std {
struct __true_type {};
struct __false_type {};
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<class T> inline const T& max(const T& a,const T& b) { return a < b ? b : a; }
template<class A,class B> struct pair {
    typedef A first_type; typedef B second_type;
    A first; B second;
    pair():first(A()),second(B()){}
    pair(const A& a,const B& b):first(a),second(b){}
};
template<class A,class B> inline pair<A,B> make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
template<class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n,const void* = 0) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
    void deallocate(T* p,size_t) { if(p) free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {
public: V _M_data;
    _STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class T,class U> inline void _Construct(T* p,const U& x) { new(p) T(x); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class T> inline void __destroy_aux(T* first,T* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline void __destroy_aux(T*,T*,const __true_type&) {}
template<class T> inline void _Destroy(T* first,T* last) { __destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor()); }
inline void* __copy_trivial(const void* first,const void* last,void* out) {
 return last==first ? out : (char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __false_type&) {
 O cur=out; for(;first!=last;++first,++cur) _Construct(&*cur,*first); return cur;
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __true_type&) {
 return (O)__copy_trivial(first,last,out);
}
template<class T> inline T* __uninitialized_fill_n(T* first,size_t n,const T& value,const __false_type&) { T* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,value);return cur;}
template<class T> inline T* fill_n(T* first,size_t n,const T& value) { for(;n>0;--n,++first)*first=value;return first; }
template<class T,class A> class _Vector_base {
public: typedef A allocator_type;
 _Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0) {
  _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
 }
 ~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:
 T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:
 typedef T value_type;typedef T* pointer;typedef const T* const_pointer;typedef T* iterator;typedef const T* const_iterator;typedef size_t size_type;typedef A allocator_type;
 typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
 explicit vector(const A& a=A()):_Vector_base<T,A>(a){}
 vector(const vector& x):_Vector_base<T,A>(x.size(),x.get_allocator()){
 this->_M_finish=__uninitialized_copy((const_pointer)x._M_start,(const_pointer)x._M_finish,this->_M_start,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 A get_allocator()const{return (const A&)this->_M_end_of_storage;}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
#ifdef FABLE_STLPORT_CYLINDER
 void push_back(const T& x);
#else
 void push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
#endif
#ifdef FABLE_STLPORT_SPLINE
 vector& operator=(const vector&);
 iterator insert(iterator,const T&);
 T& operator[](size_type n){return this->_M_start[n];}
 const T& operator[](size_type n)const{return this->_M_start[n];}
#endif
 void clear(){erase(begin(),end());}
 iterator erase(iterator pos){if(pos+1!=end()) __copy_trivial(pos+1,this->_M_finish,pos);--this->_M_finish;_Destroy(this->_M_finish);return pos;}
 iterator erase(iterator first,iterator last){pointer i=first;for(;last!=this->_M_finish;++last,++i)*i=*last;_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
#ifdef FABLE_STLPORT_SPLINE
 void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);
#else
 void _M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
 }
#endif
 void _M_insert_overflow(pointer pos,const T& x,const __true_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=(pointer)__copy_trivial(this->_M_start,pos,start);
 finish=fill_n(finish,fill_len,x);if(!atend)finish=(pointer)__copy_trivial(pos,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
 }
};
#ifdef FABLE_STLPORT_CYLINDER
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class I,class C>void sort(I,I,C);
#endif
}
#endif

namespace NBBBMaths {
class CCylinder {
public:
 CCylinder(const C3DVector& bottom,const C3DVector& top,float radius):Radius(radius),AxisBottom(bottom),AxisTop(top){}
 bool GetFirstIntersectionWithFiniteLine(const C3DVector&,const C3DVector&,C3DVector&,C3DVector&)const;
 bool GetFirstIntersectionWithTriangle(const std::vector<C3DVector>&,C3DVector&,C3DVector&)const;
private:
 float Radius;
 C3DVector AxisBottom;
 C3DVector AxisTop;
};
}
#endif

extern const float DELTA_NOTIONAL_ZERO;
#pragma auto_inline(off)
namespace std {
template<class T,class A>void vector<T,A>::push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
}
#pragma auto_inline(on)
bool NBBBMaths::CCylinder::GetFirstIntersectionWithFiniteLine(const C3DVector& line_start,const C3DVector& line_end,C3DVector& intersection_pos,C3DVector& outward_normal)const{
 C3DVector cyl_ax=AxisTop-AxisBottom;
 float cyl_height=cyl_ax.GetSquaredMagnitude();
 if(!(cyl_height>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO))return false;
 cyl_height=(float)sqrt(cyl_height);cyl_ax/=cyl_height;
 C3DVector axis_x;
 axis_x.Z=0.0f;
 if((float)fabs(cyl_ax.X)>(float)fabs(cyl_ax.Y)){
  axis_x.Y=1.0f;axis_x.X=0.0f;
  if((float)fabs(cyl_ax.X)>DELTA_NOTIONAL_ZERO)axis_x.X=-cyl_ax.Y/cyl_ax.X;
 }else{
  axis_x.X=1.0f;axis_x.Y=0.0f;
  if((float)fabs(cyl_ax.Y)>DELTA_NOTIONAL_ZERO)axis_x.Y=-cyl_ax.X/cyl_ax.Y;
 }
 axis_x.AccurateNormalise();
 C3DVector axis_y=cyl_ax.Cross(axis_x);axis_y.AccurateNormalise();
 C3DVector lhs=line_start-AxisBottom;
 C3DVector lhs_prime(lhs.Dot(axis_x),lhs.Dot(axis_y),0.0f);
 C3DVector test_line=line_end-line_start;
 C3DVector test_line_prime(test_line.Dot(axis_x),test_line.Dot(axis_y),0.0f);
 float coef_a=test_line_prime.X*test_line_prime.X+test_line_prime.Y*test_line_prime.Y;
 float coef_b=2.0f*(lhs_prime.X*test_line_prime.X+lhs_prime.Y*test_line_prime.Y);
 float coef_c=lhs_prime.X*lhs_prime.X+lhs_prime.Y*lhs_prime.Y-Radius*Radius;
 float desc=coef_b*coef_b-4.0f*coef_a*coef_c;
 if(desc<0.0f)return false;
 desc=(float)sqrt(desc);
 std::vector<float> valid_lambdas;
 float lambda1=(-coef_b+desc)/(2.0f*coef_a);
 float lambda2=(-coef_b-desc)/(2.0f*coef_a);
 if(lambda1>0.0f&&lambda1<1.0f){
  C3DVector int_pos=line_start+test_line*lambda1;
  C3DVector out_line=int_pos-AxisBottom;
  float height_param=out_line.Dot(cyl_ax);
  if(height_param>0.0f&&height_param<cyl_height)valid_lambdas.push_back(lambda1);
 }
 if(lambda2>0.0f&&lambda2<1.0f){
  C3DVector int_pos=line_start+test_line*lambda2;
  C3DVector out_line=int_pos-AxisBottom;
  float height_param=out_line.Dot(cyl_ax);
  if(height_param>0.0f&&height_param<cyl_height)valid_lambdas.push_back(lambda2);
 }
 if(valid_lambdas.begin()==valid_lambdas.end())return false;
 std::sort(valid_lambdas.begin(),valid_lambdas.end(),std::less<float>());
 float lambda_to_use=*valid_lambdas.begin();
 intersection_pos=line_start+test_line*lambda_to_use;
 C3DVector out_line=intersection_pos-AxisBottom;
 outward_normal=C3DVector(out_line.Dot(axis_x),out_line.Dot(axis_y),0.0f);
 outward_normal.AccurateNormalise();
 return true;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
static float native_epsilon(){unsigned int bits=0x38d1b717;float result;memcpy(&result,&bits,4);return result;}
const float DELTA_NOTIONAL_ZERO=native_epsilon();
C3DVector& C3DVector::AccurateNormalise(){float magnitude=(float)sqrt(X*X+Y*Y+Z*Z);if(magnitude!=0.0f){X/=magnitude;Y/=magnitude;Z/=magnitude;}return *this;}
namespace std {
template<class I,class C>void sort(I first,I last,C compare){for(I i=first;i!=last;++i){typename std::vector<float>::value_type value=*i;I j=i;while(j!=first&&compare(value,*(j-1))){*j=*(j-1);--j;}*j=value;}}
}
static bool close_value(float a,float b){return (float)fabs(a-b)<0.0009765625f;}
static bool same_vector(const C3DVector&a,const C3DVector&b){return close_value(a.X,b.X)&&close_value(a.Y,b.Y)&&close_value(a.Z,b.Z);}
int main(){
#ifdef _MSC_VER
 assert(sizeof(NBBBMaths::CCylinder)==28);
#endif
 NBBBMaths::CCylinder cylinder(C3DVector(0,0,-2),C3DVector(0,0,2),1);
 C3DVector pos(9,9,9),normal(8,8,8);
 assert(cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(-2,0,0),C3DVector(2,0,0),pos,normal));
 assert(same_vector(pos,C3DVector(-1,0,0)));assert(same_vector(normal,C3DVector(-1,0,0)));
 assert(cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(0,0,0),C3DVector(2,0,0),pos,normal));
 assert(same_vector(pos,C3DVector(1,0,0)));assert(same_vector(normal,C3DVector(1,0,0)));
 assert(cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(-2,1,0),C3DVector(2,1,0),pos,normal));
 assert(same_vector(pos,C3DVector(0,1,0)));assert(same_vector(normal,C3DVector(0,1,0)));
 assert(!cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(-2,2,0),C3DVector(2,2,0),pos,normal));
 assert(!cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(-2,0,2),C3DVector(2,0,2),pos,normal));
 assert(!cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(1,0,0),C3DVector(2,0,0),pos,normal));
 assert(!cylinder.GetFirstIntersectionWithFiniteLine(C3DVector(0,0,-3),C3DVector(0,0,3),pos,normal));
 NBBBMaths::CCylinder flat(C3DVector(0,0,0),C3DVector(0,0,0),1);
 assert(!flat.GetFirstIntersectionWithFiniteLine(C3DVector(-2,0,0),C3DVector(2,0,0),pos,normal));
 NBBBMaths::CCylinder horizontal(C3DVector(-2,0,0),C3DVector(2,0,0),1);
 assert(horizontal.GetFirstIntersectionWithFiniteLine(C3DVector(0,-2,0),C3DVector(0,2,0),pos,normal));
 assert(same_vector(pos,C3DVector(0,-1,0)));assert(same_vector(normal,C3DVector(-1,0,0)));
 for(unsigned boundary=0;boundary<2;++boundary){
  float height=boundary?0.0001220703125f:0.00006103515625f;
  NBBBMaths::CCylinder tiny(C3DVector(0,0,0),C3DVector(0,0,height),1);
  pos=C3DVector(9,9,9);normal=C3DVector(8,8,8);
  bool hit=tiny.GetFirstIntersectionWithFiniteLine(C3DVector(-2,0,height*0.5f),C3DVector(2,0,height*0.5f),pos,normal);
  assert(hit==(boundary!=0));
  if(hit){assert(same_vector(pos,C3DVector(-1,0,height*0.5f)));assert(same_vector(normal,C3DVector(-1,0,0)));}
  else {assert(same_vector(pos,C3DVector(9,9,9)));assert(same_vector(normal,C3DVector(8,8,8)));}
 }
 puts("00a59fd0_TEST PASS");return 0;
}
