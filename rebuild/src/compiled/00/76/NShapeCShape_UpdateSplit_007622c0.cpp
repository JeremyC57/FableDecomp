// NShape::CShape::UpdateSplit() -- when two points are selected insert their midpoint into the shape point list
// Leaf: ?UpdateSplit@CShape@NShape@@QAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags:
// Shape: the insertion position is a named iterator seeded with the last selected point and replaced by the first when it
//   sits further along (`mov ebp,ecx` into the second point's register; FableWin locals list an insertion iterator), and the
//   midpoint adds the second point's copy to the first's ((b + a) * 0.5f: retail loads the first point's floats first).
//   Callee 0x007653B0 is STLport vector<C3DVector>::insert(iterator, const C3DVector&) (landed spelling C3DVectorVector::insert is a local model).
#include <stdlib.h>
extern "C" void* __cdecl memmove(void*,const void*,unsigned int);
// Layout: C3DVector original PDB0x2783, public X0 Y4 Z8; 12-byte retail stride.
#include "engine/C3DVector.h"
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
   C3DVector midpoint=(C3DVector(*b)+C3DVector(*a))*0.5f;
   C3DVector*pos=b;if(indexA>indexB)pos=a;
   Points.insert(pos,midpoint);SelectedPoints.clear();return true;
  }
 }
 return false;
}