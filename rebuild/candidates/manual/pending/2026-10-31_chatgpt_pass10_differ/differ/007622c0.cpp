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
class CTCShapeManager;
namespace NShape{
// Layout: original PDB CShape protected Owner reference4, Points8; STLport12-byte
// vectors put SelectedPoints at20 rather than donor Dinkumware offset24.
// Only the virtual destructor prefix is needed by this non-virtual method.
class CShape{public:virtual ~CShape();bool UpdateSplit();
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
