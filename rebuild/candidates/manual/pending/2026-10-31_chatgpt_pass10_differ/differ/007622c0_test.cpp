// Fixture: selected target unchanged. Original-signature shape constructor, insertion and manager are fixture boundaries.
// Real objects and reference initialization; no fabricated vtable storage.
#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#define protected public
// 007622c0 NShape::CShape::UpdateSplit
// Leaf: ?UpdateSplit@CShape@NShape@@QAE_NXZ
// Flags:
#include <stdlib.h>
extern "C" void* __cdecl memmove(void*,const void*,unsigned int);
// Layout: C3DVector original PDB0x2783, public X0 Y4 Z8; 12-byte retail stride.
class C3DVector{public:float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
 const C3DVector&operator+=(const C3DVector&v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
 const C3DVector&operator*=(float f){X*=f;Y*=f;Z*=f;return *this;}
 const C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 const C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}
};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
inline void*__copy_trivial(const void*first,const void*last,void*result){return first==last?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
 T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}
 unsigned int size()const{return this->_M_finish-this->_M_start;}
 T&operator[](unsigned int n){return *(begin()+n);}T&back(){return *(end()-1);}
 T*insert(T*,const T&);
 T*erase(T*first,T*last){T*i=(T*)__copy_trivial(last,this->_M_finish,first);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
};
}
class CTCShapeManager {};
namespace NShape{
// Layout: original PDB CShape protected Owner reference4, Points8; STLport12-byte
// vectors put SelectedPoints at20 rather than donor Dinkumware offset24.
// Only the virtual destructor prefix is needed by this non-virtual method.
class CShape{public:CShape(CTCShapeManager&owner,const std::vector<C3DVector>&points):Owner(owner),Points(points){}virtual ~CShape();bool UpdateSplit();
protected:CTCShapeManager&Owner;std::vector<C3DVector>Points;std::vector<C3DVector*>SelectedPoints;
};
}
bool NShape::CShape::UpdateSplit(){
 if(SelectedPoints.size()==2){
  C3DVector*a=SelectedPoints[0];long indexA=a-Points.begin();
  C3DVector*b=SelectedPoints.back();long indexB=b-Points.begin();
  if(abs(indexA-indexB)==1){
   C3DVector midpoint=(C3DVector(*a)+C3DVector(*b))*0.5f;
   Points.insert(indexA>indexB?a:b,midpoint);SelectedPoints.clear();return true;
  }
 }
 return false;
}

#undef protected
#define CHECK(x)do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
#ifdef _MSC_VER
typedef char CheckVectorSize[sizeof(std::vector<C3DVector>)==12?1:-1];
typedef char CheckShapeSize[sizeof(NShape::CShape)==32?1:-1];
#endif
NShape::CShape::~CShape(){}
static int insertCalls,insertIndex,relocateMode;
static C3DVector*relocationStorage;
static C3DVector insertedValue;
template<class T,class A>T*std::vector<T,A>::insert(T*where,const T&value){
 ++insertCalls;insertIndex=(int)(where-this->_M_start);insertedValue=value;
 int count=(int)(this->_M_finish-this->_M_start);CHECK(insertIndex>=0&&insertIndex<=count);
 T copy(value);T*target=relocateMode?relocationStorage:this->_M_start;
 if(relocateMode)for(int j=0;j<count;++j)target[j]=this->_M_start[j];
 for(int k=count;k>insertIndex;--k)target[k]=target[k-1];target[insertIndex]=copy;
 this->_M_start=target;this->_M_finish=target+count+1;this->_M_end_of_storage._M_data=target+10;return target+insertIndex;
}
static bool Equal(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static int cases=0;
static void Run(int n,int selected,int first,int last,int origin,int relocate){
 CTCShapeManager owner;C3DVector data[10],relocated[10],before[10];C3DVector*selectedData[4];std::vector<C3DVector>points;
 for(int k=0;k<10;++k){data[k]=C3DVector((float)(origin+2*k),(float)(origin-4*k),(float)(origin+8*k));before[k]=data[k];relocated[k]=C3DVector(500,600,700);}
 for(int j=0;j<4;++j)selectedData[j]=n?&data[first]:0;if(selected==2)selectedData[1]=&data[last];
 points._M_start=data;points._M_finish=data+n;points._M_end_of_storage._M_data=data+10;
 NShape::CShape shape(owner,points);shape.SelectedPoints._M_start=selectedData;shape.SelectedPoints._M_finish=selectedData+selected;shape.SelectedPoints._M_end_of_storage._M_data=selectedData+4;
 bool expected=selected==2&&(first+1==last||last+1==first);int destination=first>last?first:last;
 C3DVector midpoint;if(expected)midpoint=C3DVector((before[first].X+before[last].X)/2.0f,(before[first].Y+before[last].Y)/2.0f,(before[first].Z+before[last].Z)/2.0f);
 insertCalls=0;insertIndex=-1;relocateMode=relocate;relocationStorage=relocated;
 bool result=shape.UpdateSplit();CHECK(result==expected);CHECK(&shape.Owner==&owner);CHECK(insertCalls==(expected?1:0));
 CHECK(shape.SelectedPoints._M_start==selectedData&&shape.SelectedPoints._M_end_of_storage._M_data==selectedData+4);
 CHECK(shape.SelectedPoints._M_finish==selectedData+(expected?0:selected));
 if(expected){CHECK(insertIndex==destination&&Equal(insertedValue,midpoint));CHECK(shape.Points._M_start==(relocate?relocated:data));CHECK(shape.Points._M_finish==shape.Points._M_start+n+1);
  for(int bit=0;bit<n+1;++bit){const C3DVector&want=bit==destination?midpoint:before[bit<destination?bit:bit-1];CHECK(Equal(shape.Points._M_start[bit],want));}
 }else{CHECK(shape.Points._M_start==data&&shape.Points._M_finish==data+n);for(int bit=0;bit<n;++bit)CHECK(Equal(data[bit],before[bit]));}
 if(!expected||relocate)for(int bit=0;bit<n;++bit)CHECK(Equal(data[bit],before[bit]));
 ++cases;
}
int main(){
 for(int relocate=0;relocate<2;++relocate)for(int origin=0;origin<4;++origin){
  for(int n=1;n<=8;++n)for(int a=0;a<n;++a)for(int b=0;b<n;++b)Run(n,2,a,b,origin*8-16,relocate);
  for(int n=0;n<=8;++n)for(int count=0;count<=4;++count)if(count!=2)Run(n,count,0,0,origin*8-16,relocate);
 }
 CHECK(cases==1920);puts("007622c0_TEST PASS");return 0;
}
