// Self-contained fixture: exact selected algorithms and predicates; controlled pointer/FOV boundaries.
#include <stdio.h>
#include <assert.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
class CBaseObject{};
class CThing : public CBaseObject{};
class CBaseObjectPointer {public:CBaseObject* Item;};
static int get_count=0,sets=0,fov_calls=0;
class CBaseIntelligentPointer {
public:
 CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();
 CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);
private:CBaseObjectPointer* PData;
};
CBaseIntelligentPointer::CBaseIntelligentPointer(){PData=new CBaseObjectPointer;PData->Item=0;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){delete PData;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{++get_count;return PData->Item;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){++sets;PData->Item=p;}
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:
 void Set(T* pointer){SetPItem(pointer);}
 CIntelligentPointer& operator=(const CIntelligentPointer& other){CBaseObject* item=other.GetPItem();SetPItem(item);return *this;}
};
template<class T>inline bool FABLE_FASTCALL operator==(const CIntelligentPointer<T>& pointer,const CBaseObject* item){return pointer.GetPItem()==item;}
class C3DVector {
public:float X,Y,Z;
 C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
 const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
 float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
};
namespace std {template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};}
#ifndef FABLE_ENGINE_NPLAYERTARGETING_H
#define FABLE_ENGINE_NPLAYERTARGETING_H
namespace NPlayerTargeting {
// Original PDB class, size28: intelligent pointer0, index8, vector12, frame24.
class CTargetPoint {
public:
 CTargetPoint();
 bool operator==(const CTargetPoint&);
 CIntelligentPointer<CThing> PThing;
 long Index;
 C3DVector TargetingPosition;
 long FrameRejected;
};
}
#endif
#ifndef FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
#define FABLE_ENGINE_CTHINGFILTER_ISINFOV_H
class CThingFilter_IsInFOV : public std::unary_function<CThing,bool> {
public:
 bool operator()(const NPlayerTargeting::CTargetPoint&)const;
 bool operator()(const C3DVector&)const;
 bool operator()(const CThing&)const;
 bool operator()(const CThing*)const;
private:
 C3DVector FromPos,Direction;
 float CosFOV,FOV;
 bool AngleTest3D;
};
#endif
#ifndef FABLE_ENGINE_CTARGETPOINTFILTERS_H
#define FABLE_ENGINE_CTARGETPOINTFILTERS_H
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
namespace {
class CTargetPointFilter_FurtherFromPosThan : public std::unary_function<NPlayerTargeting::CTargetPoint,bool> {
public:
 CTargetPointFilter_FurtherFromPosThan(const C3DVector& pos,float range):Pos(pos),Range(range){}
 bool operator()(const NPlayerTargeting::CTargetPoint& point)const {
  if((point.TargetingPosition-Pos).GetSquaredMagnitude()>GFSqr(Range))return true;
  return false;
 }
private:
 const C3DVector& Pos;
 float Range;
};
class CTargetPointFilter_IsLockedTarget : public std::unary_function<NPlayerTargeting::CTargetPoint,bool> {
public:
 CTargetPointFilter_IsLockedTarget(const CThing* target):PLockedTarget(target){}
 bool operator()(const NPlayerTargeting::CTargetPoint& point)const{return point.PThing==PLockedTarget;}
private:
 const CThing* PLockedTarget;
};
}
#endif
#ifndef FABLE_ENGINE_CFILTER_BIND3_H
#define FABLE_ENGINE_CFILTER_BIND3_H
// Original PDB fields are protected const references, with no functor copies.
template<class TF1,class TF2,class TF3> class CFilter_Bind3 {
protected:
 const TF1& F1;
 const TF2& F2;
 const TF3& F3;
public:
 CFilter_Bind3(const TF1& f1,const TF2& f2,const TF3& f3):F1(f1),F2(f2),F3(f3){}
 template<class T> bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t);}
};
#endif
namespace std {
template<class _Predicate> class unary_negate : public unary_function<typename _Predicate::argument_type,bool> {
protected:
 _Predicate _M_pred;
public:
 explicit unary_negate(const _Predicate& pred):_M_pred(pred){}
 bool operator()(const typename _Predicate::argument_type& x)const{return !_M_pred(x);}
};
template<class _InputIter,class _OutputIter,class _Predicate>
_OutputIter FABLE_FASTCALL remove_copy_if(_InputIter first,_InputIter last,_OutputIter result,_Predicate pred) {
 for(;first!=last;++first)if(!pred(*first)){*result=*first;++result;}
 return result;
}

}

NPlayerTargeting::CTargetPoint::CTargetPoint():Index(0),TargetingPosition(0,0,0),FrameRejected(0){}
bool CThingFilter_IsInFOV::operator()(const NPlayerTargeting::CTargetPoint& p)const{++fov_calls;return (p.Index&1)!=0;}
int main(){
 typedef NPlayerTargeting::CTargetPoint Point;
 typedef CFilter_Bind3<CThingFilter_IsInFOV,CTargetPointFilter_FurtherFromPosThan,std::unary_negate<CTargetPointFilter_IsLockedTarget> > Predicate;
 CThing things[16];Point source[16],dest[16];CThingFilter_IsInFOV fov;
 int cases=0;
 for(int n=0;n<=16;++n)for(int lock=-1;lock<16;++lock)for(int radius=0;radius<=5;++radius) {
  C3DVector origin(1,2,3);CTargetPointFilter_FurtherFromPosThan far(origin,(float)radius);
  CTargetPointFilter_IsLockedTarget locked(lock<0?0:&things[lock]);std::unary_negate<CTargetPointFilter_IsLockedTarget> unlocked(locked);
  Predicate pred(fov,far,unlocked);int ids[16],count=0,expected_gets=0;
  for(int i=0;i<n;++i){
   source[i].Index=i;source[i].PThing.Set(i==0?0:&things[i]);
   source[i].TargetingPosition=C3DVector(1+(float)(i%4),2+(float)(i/4),3);
   source[i].FrameRejected=100+i;
   bool first=(i&1)!=0;bool second=(i%4)*(i%4)+(i/4)*(i/4)>radius*radius;
   bool third=i!=lock;if(i==0&&lock==-1)third=false;
   if(first&&second)++expected_gets;
   if(!(first&&second&&third)){ids[count++]=i;++expected_gets;}
  }
  get_count=sets=fov_calls=0;
  Point* end=std::remove_copy_if(source,source+n,dest,pred);
  assert(end==dest+count&&get_count==expected_gets&&sets==count&&fov_calls==n);
  for(int i=0;i<count;++i){int id=ids[i];assert(dest[i].Index==id&&dest[i].FrameRejected==100+id);
   assert(dest[i].PThing.GetPItem()==(id==0?0:&things[id]));
   assert(dest[i].TargetingPosition.X==source[id].TargetingPosition.X&&dest[i].TargetingPosition.Y==source[id].TargetingPosition.Y&&dest[i].TargetingPosition.Z==3);
  }
  // A result beginning at first is also valid: stable in-place compaction.
  end=std::remove_copy_if(source,source+n,source,pred);assert(end==source+count);
  for(int i=0;i<count;++i)assert(source[i].Index==ids[i]);
  ++cases;
 }
 assert(cases==1734);puts("00807a90_TEST PASS");return 0;
}