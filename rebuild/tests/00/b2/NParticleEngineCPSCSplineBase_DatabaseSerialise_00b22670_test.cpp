#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
static void check(bool ok) { if (!ok) std::abort(); }
struct C3DVector {
 float x,y,z;
 C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){}
 bool equals(const C3DVector& rhs)const{return x==rhs.x && y==rhs.y && z==rhs.z;}
};
template<class T> struct CArray : std::vector<T> {};
struct CCharString {
 std::string s;
 CCharString(const char* p):s(p){}
 const char* GetAsCharArray()const{return s.c_str();}
 CCharString operator+(const CCharString& v)const{return (s+v.s).c_str();}
};
static CCharString GFIntToCharString(long i){ char b[32];std::sprintf(b,"%ld",i);return b; }
struct CCardinalSpline {
 float tension;bool ends;CArray<C3DVector> points;
 float GetTension()const{return tension;}
 void SetTension(float t){tension=t;}
 bool GetEndPointsConnected()const{return ends;}
 void SetEndPointsConnected(bool b){ends=b;}
 long GetControlPointCount()const{return static_cast<long>(points.size());}
 CArray<C3DVector> GetControlPoints()const{return points;}
 void SetControlPoints(const CArray<C3DVector>& other){points=other;}
};
template<class T> class CCountedPointer {
 T* p;
 public:
 CCountedPointer():p(0){}
 ~CCountedPointer(){delete p;}
 T* operator->()const{check(p!=0);return p;}
 bool IsNull()const{return p==0;}
 void Reset(T* q){delete p;p=q;}
 private:CCountedPointer(const CCountedPointer&);CCountedPointer& operator=(const CCountedPointer&);
};
static C3DVector point(long i) { return C3DVector(float(i+1),float(-i-2),float(i*2)); }
struct CPersistContext {
 int mode;long legacy,fixed;bool connected,offset,valid;
 std::vector<std::string> keys;std::vector<C3DVector> values;
 float tension;
 bool IsSaving()const{return mode==1 || mode==3;}
 void Transfer(const char* n,float& x){keys.push_back(n);check(keys.size()==1);if(IsSaving())tension=x;else x=tension;}
 void Transfer(const char* n,bool& x){keys.push_back(n);check(keys.size()==2);if(IsSaving()) connected=x;else x=connected;}
 void Transfer(const char* n,long& x,long def){check(def==0 && !IsSaving());keys.push_back(n);x=legacy;}
 void Transfer(const char* n,long& x){keys.push_back(n);if(IsSaving())fixed=x;else x=fixed;}
 void Transfer(const char* n,bool& x,bool def){check(!def);keys.push_back(n);if(IsSaving())offset=x;else x=offset;}
 void Transfer(const char* n,C3DVector& x){keys.push_back(n);if(!IsSaving())x=point(values.size());values.push_back(x);}
};
namespace NParticleEngine {
struct CEmitterDatabaseSerialiseInfo {};
static unsigned madeOffset,madePos,offsetCalls,posCalls,dropOffset,dropPos;
static const CCardinalSpline* receiver;static const CEmitterDatabaseSerialiseInfo* expectedInfo;
struct CPSCSplineBase {
 struct CSplineRandomOffsetData {
  CSplineRandomOffsetData(const CCardinalSpline& s){check(&s==receiver);++madeOffset;}
  ~CSplineRandomOffsetData(){++dropOffset;}
  void DatabaseSerialise(CPersistContext& c,const CEmitterDatabaseSerialiseInfo& info){check(&info==expectedInfo);c.keys.push_back("offset-data");++offsetCalls;}
 };
 struct CSplinePosParamData {
  bool valid;
  CSplinePosParamData(const CCardinalSpline& s):valid(false){check(&s==receiver);++madePos;}
  ~CSplinePosParamData(){++dropPos;}
  void DatabaseSerialise(CPersistContext& c,const CEmitterDatabaseSerialiseInfo& info){check(&info==expectedInfo);c.keys.push_back("pos-data");valid=c.valid;++posCalls;}
  bool IsParamListValid()const{return valid;}
 };
 CCardinalSpline Spline;
 CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;
 CCountedPointer<CSplinePosParamData> SplinePosParamData;
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
};
}
namespace NParticleEngine {
void CPSCSplineBase::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    float t = Spline.GetTension();
    context.Transfer("SplineTension", t);
    Spline.SetTension(t);
    bool end_points_connected = Spline.GetEndPointsConnected();
    context.Transfer("SplineConnectedEndPoints", end_points_connected);
    Spline.SetEndPointsConnected(end_points_connected);
    if (context.IsSaving()) {
        long size = Spline.GetControlPointCount();
        context.Transfer("SplineUserPointsArraySize_BugFixed", size);
        CArray<C3DVector> control_points = Spline.GetControlPoints();
        for (long ci = 0; ci < size; ++ci)
            context.Transfer((CCharString("SplineUserPointsArray_") + GFIntToCharString(ci)).GetAsCharArray(), control_points[ci]);
    } else {
        long size;
        CArray<C3DVector> control_points;
        context.Transfer("SplineUserPointsArraySize", size, 0L);
        if (size > 0) {
            if (size > 20) size = 20;
            control_points.resize(size);
            for (long ci = 0; ci < size; ++ci)
                context.Transfer("SplineUserPointsArray" + ci, control_points[ci]);
        } else {
            context.Transfer("SplineUserPointsArraySize_BugFixed", size);
            control_points.resize(size);
            for (long ci = 0; ci < size; ++ci)
                context.Transfer((CCharString("SplineUserPointsArray_") + GFIntToCharString(ci)).GetAsCharArray(), control_points[ci]);
        }
        C3DVector new_pt(0, 0, 0);
        while (control_points.size() < 2)
            control_points.push_back(new_pt);
        Spline.SetControlPoints(control_points);
    }
    bool offset_pos = !SplineRandomOffsetData.IsNull();
    context.Transfer("OffsetPos", offset_pos, false);
    if (offset_pos) {
        if (!context.IsSaving()) SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(Spline));
        SplineRandomOffsetData->DatabaseSerialise(context, info);
    }
    if (!context.IsSaving()) SplinePosParamData.Reset(new CSplinePosParamData(Spline));
    SplinePosParamData->DatabaseSerialise(context, info);
    if (!SplinePosParamData->IsParamListValid()) SplinePosParamData.Reset(0);
}
}
int main(){
 using namespace NParticleEngine;
 const long older[]={-1,0,1,2,19,20,21,77},current[]={0,1,2,6,24};
 for(int mode=0;mode<4;++mode)for(int old=0;old<8;++old)for(int modern=0;modern<5;++modern)for(unsigned flags=0;flags<64;++flags){
  CPSCSplineBase obj;receiver=&obj.Spline;CEmitterDatabaseSerialiseInfo info;expectedInfo=&info;
  CPersistContext ctx;ctx.mode=mode;ctx.legacy=older[old];ctx.fixed=current[modern];ctx.connected=(flags&1)!=0;ctx.offset=(flags&2)!=0;ctx.valid=(flags&4)!=0;ctx.tension=-0.75f;
  const bool hadOffset=(flags&8)!=0,hadEnds=(flags&16)!=0;
  const long initial=(flags&32)?3:0;obj.Spline.tension=0.25f;obj.Spline.ends=hadEnds;
  for(long j=0;j<initial;++j)obj.Spline.points.push_back(point(j+100));
  if(hadOffset)obj.SplineRandomOffsetData.Reset(new CPSCSplineBase::CSplineRandomOffsetData(obj.Spline));
  obj.SplinePosParamData.Reset(new CPSCSplineBase::CSplinePosParamData(obj.Spline));
  madeOffset=madePos=dropOffset=dropPos=posCalls=offsetCalls=0;
  obj.DatabaseSerialise(ctx,info);
  const bool save=mode==1||mode==3,legacy=!save && older[old]>0,offset=save?hadOffset:((flags&2)!=0);
  const long count=save?initial:legacy?(older[old]<20?older[old]:20):current[modern];
  check(obj.Spline.tension==(save?0.25f:-0.75f) && obj.Spline.ends==(save?hadEnds:((flags&1)!=0)));
  if(save)check(ctx.tension==0.25f && ctx.connected==hadEnds && ctx.offset==hadOffset && ctx.fixed==initial);
  std::vector<std::string> expected;expected.push_back("SplineTension");expected.push_back("SplineConnectedEndPoints");
  if(!save)expected.push_back("SplineUserPointsArraySize");
  if(!legacy)expected.push_back("SplineUserPointsArraySize_BugFixed");
  const std::string oldKey="SplineUserPointsArray";
  for(long j=0;j<count;++j){
   if(legacy)expected.push_back(oldKey.substr(j));else{char field[56];std::sprintf(field,"SplineUserPointsArray_%ld",j);expected.push_back(field);}
   check(ctx.values[j].equals(point(save?j+100:j)));
  }
  expected.push_back("OffsetPos");if(offset)expected.push_back("offset-data");expected.push_back("pos-data");
  check(expected==ctx.keys && ctx.values.size()==static_cast<unsigned long>(count));
  const long stored=save?initial:count<2?2:count;check(obj.Spline.GetControlPointCount()==stored);
  for(long j=0;j<stored;++j)check(obj.Spline.points[j].equals(j<count?point(save?j+100:j):C3DVector()));
  check(madeOffset==unsigned(!save && offset) && offsetCalls==unsigned(offset) && dropOffset==unsigned(!save && offset && hadOffset));
  check(obj.SplineRandomOffsetData.IsNull()==!(offset||hadOffset) && madePos==unsigned(!save) && posCalls==1);
  check(dropPos==unsigned(!save)+unsigned(!ctx.valid) && obj.SplinePosParamData.IsNull()==!ctx.valid);
 }
 std::puts("00b22670_TEST PASS");return 0;
}