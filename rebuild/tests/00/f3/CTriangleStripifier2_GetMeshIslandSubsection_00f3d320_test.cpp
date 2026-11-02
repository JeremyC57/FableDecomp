// CTriangleStripifier2::GetMeshIslandSubsection(const CArray<unsigned short>&, const CArray<bool>&, const CArray<CArray<long> >&, CArray<unsigned short>&, CArray<long>&) -- traverse one enabled connected triangle island.
// Leaf: ?GetMeshIslandSubsection@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@_N@@ABV?$CArray@V?$CArray@J@@@@AAV2@AAV?$CArray@J@@@Z
#define FABLE_RETAIL_CARRAY_STRIPIFIER
#ifndef FABLE_CTRIANGLESTRIPIFIER2_H
#define FABLE_CTRIANGLESTRIPIFIER2_H
#if defined(FABLE_RETAIL_CARRAY_STRIPIFIER)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
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
#ifndef FABLE_STLPORT46_STRIPIFIER_H
#define FABLE_STLPORT46_STRIPIFIER_H
// Modified STLport 4.6 vector/allocator excerpts for the retail triangle TU.
// Original inheritance and POD dispatch; exception configuration disabled.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<long>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned short>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<bool>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<class T>class allocator{public:allocator(){};T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
template<class T>inline T*__copy_ptrs(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*__uninitialized_copy(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class T>inline T*__uninitialized_fill_n(T*p,size_t n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class T>inline T*__uninitialized_fill_n(T*p,size_t n,const T&x,const __false_type&){for(;n>0;--n,++p)_Construct(p,x);return p;}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 T&back(){return *(end()-1);}const T&back()const{return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,_IsPODType());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void resize(size_type,const T&);
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=(T*)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(T*)__copy_trivial(position,this->_M_finish,new_finish);
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
};
}
#endif

// Original CArray public vector base, constructors and index accessors (PDB).
// Retail uses STLport's 12-byte base; the donor's later vector base is 16 bytes.
template<class T>class CArray:public std::vector<T>{
public:
 CArray():std::vector<T>(){}
 explicit CArray(unsigned int n):std::vector<T>(n,T()){}
 CArray(unsigned int n,const T&x):std::vector<T>(n,x){}
 T&operator[](unsigned int n){return this->_M_start[n];}
 const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif

#endif

class C3DTriangle2;
// Layout: original PDB base and derived records each have size four, no data.
class CTriangleStripifierBase{
public:
 virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&)=0;
 virtual ~CTriangleStripifierBase(){}
};
class CTriangleStripifier2:public CTriangleStripifierBase{
public:
 virtual void GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&);
 virtual ~CTriangleStripifier2(){}
private:
 friend int main();
 void FillInPolygons(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<unsigned short>&,const CArray<bool>&,CArray<long>&,CArray<unsigned short>&,bool);
 void GetMeshIslandSubsection(const CArray<unsigned short>&,const CArray<bool>&,const CArray<CArray<long> >&,CArray<unsigned short>&,CArray<long>&);
};
#endif

void CTriangleStripifier2::GetMeshIslandSubsection(const CArray<unsigned short>& poly_buffer,const CArray<bool>& poly_mask,const CArray<CArray<long> >& vertex_to_poly_map,CArray<unsigned short>& subsection,CArray<long>& sub_section_to_global_poly_map)
{
 long num_polys=poly_buffer.size()/3;
 subsection.clear();
 sub_section_to_global_poly_map.resize(num_polys,0);
 long start_poly=0;
 for(;start_poly<num_polys;++start_poly)
  if(poly_mask[start_poly])break;
 CArray<long> poly_stack;
 CArray<unsigned short> vertex_stack;
 CArray<bool> polys_processed(num_polys,false);
 CArray<bool> verts_processed(vertex_to_poly_map.size(),false);
 poly_stack.push_back(start_poly);
 polys_processed[start_poly]=true;
 while(poly_stack.size()||vertex_stack.size()){
  while(poly_stack.size()){
   long poly=poly_stack.back();
   poly_stack.pop_back();
   sub_section_to_global_poly_map[subsection.size()/3]=poly;
   for(long i=0;i<3;++i){
    long vert=poly_buffer[poly*3+i];
    subsection.push_back(vert);
    if(!verts_processed[vert]){
     verts_processed[vert]=true;
     vertex_stack.push_back(vert);
    }
   }
  }
  while(vertex_stack.size()){
   unsigned short vert=vertex_stack.back();
   vertex_stack.pop_back();
   for(long pl=0;pl<vertex_to_poly_map[vert].size();++pl){
    long poly=vertex_to_poly_map[vert][pl];
    if(!polys_processed[poly]&&poly_mask[poly]){
     poly_stack.push_back(poly);
     polys_processed[poly]=true;
    }
   }
  }
 }
}

#include <stdio.h>
#include <assert.h>
// Test-side genuine STLport resize/end-fill implementation. The selected target
// calls the separately compiled resize; this fixture supplies that dependency.
namespace std {
template<class T,class A>void vector<T,A>::resize(size_type n,const T&x){
 if(n<size())erase(begin()+n,end());
 else if(n!=size()){
  size_type count=n-size();
  if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=count){
   T x_copy=x;
   this->_M_finish=__uninitialized_fill_n(this->_M_finish,count,x_copy,_IsPODType());
  }else _M_insert_overflow(end(),x,_IsPODType(),count,false);
 }
}
}
void CTriangleStripifier2::GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&){}
int main(){
 const unsigned short triangles[21]={0,1,2,2,3,4,4,5,0,5,6,7,8,9,10,10,11,8,3,6,12};
 CTriangleStripifier2 stripifier;
 // Every nonempty mask exercises start selection, disconnected islands,
 // triangles sharing only one vertex, cycles, exclusions and LIFO traversal.
 for(unsigned int bits=1;bits<128;++bits){
  CArray<unsigned short> polygons;
  for(unsigned int i=0;i<21;++i)polygons.push_back(triangles[i]);
  CArray<bool> mask(7,false);
  CArray<CArray<long> > adjacency(13);
  for(unsigned int p=0;p<7;++p){
   mask[p]=(bits&(1u<<p))!=0;
   for(unsigned int v=0;v<3;++v)adjacency[triangles[p*3+v]].push_back(p);
  }
  CArray<unsigned short> out;
  CArray<long> map;
  // Reused output buffers include both growth and retained-capacity cases.
  for(unsigned int repetition=0;repetition<3;++repetition){
   if(repetition==0){out.push_back(99);map.push_back(99);}
   stripifier.GetMeshIslandSubsection(polygons,mask,adjacency,out,map);
   bool expected[7];memset(expected,0,sizeof(expected));
   unsigned int first=0;while(!mask[first])++first;expected[first]=true;
   bool changed=true;
   while(changed){changed=false;for(unsigned int p=0;p<7;++p)if(mask[p]&&!expected[p]){
    bool shares=false;
    for(unsigned int q=0;q<7;++q)if(expected[q])for(unsigned int v=0;v<3;++v)for(unsigned int w=0;w<3;++w)
     if(triangles[p*3+v]==triangles[q*3+w])shares=true;
    if(shares){expected[p]=true;changed=true;}
   }}
   unsigned int count=0;for(unsigned int p=0;p<7;++p)if(expected[p])++count;
   assert(out.size()==count*3&&map.size()==7&&map[0]==first);
   bool seen[7];memset(seen,0,sizeof(seen));
   for(unsigned int i=0;i<count;++i){long p=map[i];assert(p>=0&&p<7&&expected[p]&&!seen[p]);seen[p]=true;
    for(unsigned int v=0;v<3;++v)assert(out[i*3+v]==triangles[p*3+v]);
   }
   for(unsigned int p=0;p<7;++p)assert(seen[p]==expected[p]);
   for(unsigned int i=0;i<21;++i)assert(polygons[i]==triangles[i]);
   for(unsigned int p=0;p<7;++p)assert(mask[p]==((bits&(1u<<p))!=0));
  }
 }
 puts("00f3d320_TEST PASS");return 0;
}