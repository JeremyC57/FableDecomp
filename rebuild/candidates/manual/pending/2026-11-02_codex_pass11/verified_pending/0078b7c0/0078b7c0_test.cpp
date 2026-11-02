// NMaths::GetRootsNumerically<CAnalyseRootsOfBowPullBackFunction>(float,float,const CAnalyseRootsOfBowPullBackFunction&,long,long) -- Find bow pullback roots numerically.
// Leaf: ??$GetRootsNumerically@VCAnalyseRootsOfBowPullBackFunction@?A0x0524c104@@@NMaths@@YI?AV?$vector@MV?$allocator@M@std@@@std@@MMABVCAnalyseRootsOfBowPullBackFunction@?A0x0524c104@@JJ@Z
// Grade: RELOCATION_MATCH -- complete1390-byte function; relocation names and behavior audited separately
// Flags: /Gr
#define FABLE_RETAIL_BOW_ROOTS
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

// Modified for the documented Fable STLport instantiations; unused facilities omitted.
#ifndef FABLE_STLPORT46_VECTOR_ROOTS_H
#define FABLE_STLPORT46_VECTOR_ROOTS_H
// STLport 4.6 _vector.h, _alloc.h, _construct.h and _uninitialized.h.
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
#ifdef FABLE_RETAIL_NAV_OUTPUT
template<class T>struct __type_traits<T*> {typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
#endif
template<class T> inline const T& max(const T& a,const T& b) { return a < b ? b : a; }
template<class A,class B> struct pair {
    typedef A first_type; typedef B second_type;
    A first; B second;
    pair():first(A()),second(B()){}
    pair(const A& a,const B& b):first(a),second(b){}
#if defined(FABLE_RETAIL_ELLIPSE) || defined(FABLE_RETAIL_TRAJECTORY) || defined(FABLE_RETAIL_BOW_ROOTS)
    // Original STLport _pair.h member-template configuration.
    template<class U,class V> pair(const pair<U,V>& p):first(p.first),second(p.second){}
    pair(const pair<A,B>& p):first(p.first),second(p.second){}
#endif
};
#if (defined(FABLE_RETAIL_TRAJECTORY) || defined(FABLE_RETAIL_ELLIPSE) || defined(FABLE_RETAIL_BOW_ROOTS)) && defined(_MSC_VER)
// Original _pair.h _STLP_CALL, configured as cdecl by config/stl_msvc.h.
template<class A,class B> inline pair<A,B> __cdecl make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
#else
template<class A,class B> inline pair<A,B> make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
#endif
template<class T> class allocator {
public:
    allocator() {}
#if (defined(FABLE_RETAIL_NAV_REGIONS) && !defined(FABLE_RETAIL_NAV_OUTPUT)) || defined(FABLE_RETAIL_NAV_OUTPUT_FULL_GROWTH) || defined(FABLE_RETAIL_ELLIPSE) || defined(FABLE_RETAIL_TRAJECTORY) || defined(FABLE_RETAIL_BOW_ROOTS)
    ~allocator() {}
#endif
#if defined(FABLE_RETAIL_NAV_REGIONS) || defined(FABLE_RETAIL_ELLIPSE) || defined(FABLE_RETAIL_TRAJECTORY) || defined(FABLE_RETAIL_BOW_ROOTS)
    allocator(const allocator&){}
    template<class U>allocator(const allocator<U>&){}
#endif
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
#ifdef FABLE_RETAIL_NAV_OUTPUT
 bool empty()const{return begin()==end();}
 T& back(){return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
#endif
#ifdef FABLE_STLPORT_CYLINDER
 void push_back(const T& x);
#else
 void push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
#endif
#if defined(FABLE_STLPORT_SPLINE) || defined(FABLE_RETAIL_NAV_REGIONS)
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
#if defined(FABLE_STLPORT_SPLINE) || (defined(FABLE_RETAIL_NAV_OUTPUT) && !defined(FABLE_RETAIL_NAV_OUTPUT_FULL_GROWTH))
 void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);
#else
#ifdef FABLE_RETAIL_NAV_OUTPUT_FULL_GROWTH
 // Original STLport growth body; native coordinate growth remains out of line.
 __declspec(noinline)
#endif
 void _M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
 }
#endif
#ifdef FABLE_RETAIL_NAV_OUTPUT
 __declspec(noinline)
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

#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
#ifdef _MSC_VER
#include <math.h>
#else
// GNU host adapter avoids mixing its private STL traits with original STLport.
extern "C" {double tanh(double);double fabs(double);}
#endif
#pragma intrinsic(tanh,fabs)
extern const float DELTA_NOTIONAL_ZERO;
inline double __fastcall GFTanh(double value){return tanh(value);}
namespace {
inline float GetBowPullBackAmountFromPullBackTimer(long pullback_timer){float tanh_weight=0.05f;return (float)GFTanh(pullback_timer*tanh_weight);}
// Layout: FableWin original class V size4; private DesiredPullBackAmount at+0,
// confirmed by native GetValue receiver load and caller functor construction.
class CAnalyseRootsOfBowPullBackFunction {
 float DesiredPullBackAmount;
public:
 CAnalyseRootsOfBowPullBackFunction(float desired_pullback_amount):DesiredPullBackAmount(desired_pullback_amount){}
 float GetValue(float x_axis)const{return DesiredPullBackAmount-GetBowPullBackAmountFromPullBackTimer((long)x_axis);}
 bool IsValid()const;
};
}
namespace NMaths {
template<class F> std::vector<float> GetRootsNumerically(float range_start,float range_end,const F& func,long num_roots,long num_asymptotes) {
 float mod_range_start=range_start-DELTA_NOTIONAL_ZERO;
 float mod_range_end=range_end+DELTA_NOTIONAL_ZERO;
 std::vector<std::pair<float,float> > roots_between;
 long num_steps=(num_roots+num_asymptotes)-1;
 long found_roots;
 do {
  ++num_steps;roots_between.clear();found_roots=0;
  float step=(mod_range_end-mod_range_start)/num_steps;float r_start=mod_range_start;float r_end=r_start+step;float val_a=func.GetValue(mod_range_start);
  for(long c1=0;c1!=num_steps;++c1){
   float val_b=func.GetValue(r_end);
   if((val_a>=0.0f && val_b<=0.0f)||(val_a<=0.0f && val_b>=0.0f)){
    ++found_roots;roots_between.push_back(std::make_pair(r_start,r_end));
   }
   val_a=val_b;r_start=r_end;r_end+=step;
  }
 }while(found_roots<(num_roots+num_asymptotes));

 std::vector<float> roots;
 for(std::vector<std::pair<float,float> >::iterator it=roots_between.begin();it!=roots_between.end();++it){
  const std::pair<float,float>& roots_pair=*it;
  float r_start=roots_pair.first,r_end=roots_pair.second;
  const float accuracy=0.001f;
  float val_a=func.GetValue(r_start);
  float val_b=func.GetValue(r_end);
  std::pair<bool,float> val_a_converging;
  val_a_converging.first=true;
  val_a_converging.second=val_a;
  std::pair<bool,float> val_b_converging;
  val_b_converging.first=true;
  val_b_converging.second=val_b;
  while(r_end-r_start>accuracy){
   float r_mid=(r_start+r_end)*0.5f;val_b=func.GetValue(r_mid);
   if(val_a<=0.0f){if(val_b<=0.0f){val_a=val_b;r_start=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_a_converging.second)+DELTA_NOTIONAL_ZERO))val_a_converging.first=true;else val_a_converging.first=false;val_a_converging.second=val_b;}else{r_end=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_b_converging.second)+DELTA_NOTIONAL_ZERO))val_b_converging.first=true;else val_b_converging.first=false;val_b_converging.second=val_b;}}
   else{if(val_b>=0.0f){val_a=val_b;r_start=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_a_converging.second)+DELTA_NOTIONAL_ZERO))val_a_converging.first=true;else val_a_converging.first=false;val_a_converging.second=val_b;}else{r_end=r_mid;if(!((float)fabs(val_b)>(float)fabs(val_b_converging.second)+DELTA_NOTIONAL_ZERO))val_b_converging.first=true;else val_b_converging.first=false;val_b_converging.second=val_b;}}
  }
  if(val_a_converging.first && val_b_converging.first)roots.push_back(r_start);
 }
 for(std::vector<float>::iterator it=roots.begin();it!=roots.end();){
  float root=*it;
  if(!(root>=range_start && root<range_end))it=roots.erase(it);else ++it;
 }
 return roots;
}
template std::vector<float> GetRootsNumerically<CAnalyseRootsOfBowPullBackFunction>(float,float,const CAnalyseRootsOfBowPullBackFunction&,long,long);
}

#include <stdio.h>
#include <assert.h>
// Native DELTA_NOTIONAL_ZERO, proven at 0x0129ba3c; preserve its exact float bits.
#include <string.h>
static float NativeEpsilonForTest()
{
    const unsigned int bits=0x38d1b717u;
    float value;
    memcpy(&value,&bits,sizeof(value));
    return value;
}
extern const float DELTA_NOTIONAL_ZERO=NativeEpsilonForTest();
int main(){
#ifdef _MSC_VER
 assert(sizeof(CAnalyseRootsOfBowPullBackFunction)==4);
#endif
 const float desired[5]={0.0f,0.25f,-0.25f,0.5f,-0.5f};
 const float expected[5]={0.0f,6.0f,-6.0f,11.0f,-11.0f};
 for(int index=0;index<5;++index){
  CAnalyseRootsOfBowPullBackFunction function(desired[index]);
  std::vector<float> roots=NMaths::GetRootsNumerically(-32.0f,32.0f,function,1,0);
  if(index==0){/* Native equality branch advances the zero plateau toward the upper bound. */assert(roots.size()==1);assert(*roots.begin()>31.998046875f && *roots.begin()<32.0f);}else{assert(roots.size()==1);assert((float)fabs(*roots.begin()-expected[index])<0.001953125f);}
 }
 CAnalyseRootsOfBowPullBackFunction zero(0.0f);
 std::vector<float> roots=NMaths::GetRootsNumerically(-2.0f,2.0f,zero,2,0);
 assert(roots.size()==2);
 assert((float)fabs(*roots.begin())<0.001953125f);
 assert((float)fabs(*(roots.begin()+1)-2.0f)<0.001953125f);
 const float fractions[7]={0.125f,0.25f,0.375f,0.5f,0.625f,0.75f,0.875f};
 const float jump_points[7]={3.0f,6.0f,8.0f,11.0f,15.0f,20.0f,28.0f};
 const float radii[4]={32.0f,48.0f,64.0f,128.0f};
 for(int value_index=0;value_index<7;++value_index){
  for(int range_index=0;range_index<4;++range_index){
   for(int sign_index=0;sign_index<2;++sign_index){
    float sign=sign_index?1.0f:-1.0f;
    CAnalyseRootsOfBowPullBackFunction function(sign*fractions[value_index]);
    std::vector<float> result=NMaths::GetRootsNumerically(-radii[range_index],radii[range_index],function,1,0);
    assert(result.size()==1);
    float root=*result.begin();
    assert(root>=-radii[range_index] && root<radii[range_index]);
    assert((float)fabs(root-sign*jump_points[value_index])<0.001953125f);
    float expected_x=sign*jump_points[value_index];
    float just_inside=expected_x-sign;
    float at_jump=function.GetValue(expected_x);
    float before_jump=function.GetValue(just_inside);
    assert(sign*at_jump<=0.0f && sign*before_jump>0.0f);
   }
  }
 }
 puts("0078b7c0_TEST PASS");return 0;
}
