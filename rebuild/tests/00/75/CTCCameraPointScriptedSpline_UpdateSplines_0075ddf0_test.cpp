
#include <stdio.h>
#include <vector>
#include <math.h>
struct C3DVector {float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
template<class T>struct CArray:std::vector<T>{};
struct CCardinalSpline {
 CArray<C3DVector> points;float tension;bool connected;
 CCardinalSpline(const CArray<C3DVector>& p,float te=.5f,bool con=false):points(p),tension(te),connected(con){}
 long GetControlPointCount()const{return points.size();}
 void DeleteControlPoint(long i){points.erase(points.begin()+i);}
 long AppendControlPoint(const C3DVector& p){points.push_back(p);return points.size()-1;}
 void SetTension(float t){tension=t;}
};
struct SplineOwner {
 CCardinalSpline* data;int resets;
 SplineOwner():data(0),resets(0){}~SplineOwner(){delete data;}
 void Reset(CCardinalSpline* p){delete data;data=p;++resets;}bool IsNull()const{return !data;}
 CCardinalSpline* operator->(){return data;}
};
struct CTCCameraPointScriptedSpline {
 struct CKeyCamera {
  C3DVector pos,look;float speed,fov,roll,anim,duration;
  const C3DVector& GetPosition()const{return pos;}const C3DVector& GetLookDirection()const{return look;}
  float GetShuttleSpeed()const{return speed;}float GetFOV()const{return fov;}float GetRoll()const{return roll;}
  float GetAnimationSpeed()const{return anim;}float GetDuration()const{return duration;}
 };
 std::vector<CKeyCamera> KeyCameras;SplineOwner PTimingSpline,PPositionSpline,PViewDirectionSpline,PScalarSpline,PAnimationSpeedSpline;
 float Tension,AnimationSpeed,PauseFor;long NodeToStartOn,NodeToEndOn;int estimates,inits;bool orderOK;
 void EstimateKeyIndex(){++estimates;if(!PPositionSpline.data||!PViewDirectionSpline.data||!PScalarSpline.data||!PAnimationSpeedSpline.data||PTimingSpline.data->points.size()!=KeyCameras.size())orderOK=false;}
 void InitialiseSpline(long a,long b){++inits;if(inits!=estimates||a!=0||b!=-1)orderOK=false;AnimationSpeed=PauseFor=0;NodeToStartOn=a;NodeToEndOn=b<0?KeyCameras.size()-1:b;}
 void UpdateSplines();
};
void CTCCameraPointScriptedSpline::UpdateSplines()
{
    if (KeyCameras.size() > 1) {
        CArray<C3DVector> pts, look, scalar_pts, durs, anim_speed;
        float t = 0.0f;
        float step = 1.0f / (KeyCameras.size() - 1);
        if (PTimingSpline.IsNull()) {
            PTimingSpline.Reset(new CCardinalSpline(durs));
        } else {
            long num = PTimingSpline->GetControlPointCount();
            if (false) {} else for (long i = 0; i != num; ++i)
                PTimingSpline->DeleteControlPoint(0);
        }

        long count = 0;
        if (false) {} else for (std::vector<CKeyCamera>::iterator it = KeyCameras.begin(); it != KeyCameras.end(); ++it) {
            CKeyCamera& cam = *it;
            pts.push_back(cam.GetPosition());
            look.push_back(cam.GetLookDirection());
            scalar_pts.push_back(C3DVector(cam.GetShuttleSpeed(), cam.GetFOV(), cam.GetRoll()));
            anim_speed.push_back(C3DVector(cam.GetAnimationSpeed(), 0.0f, 0.0f));
            PTimingSpline->AppendControlPoint(C3DVector(t, cam.GetDuration(), 0.0f));
            t += step;
        }
        PTimingSpline->SetTension(Tension);
        PPositionSpline.Reset(new CCardinalSpline(pts));
        PViewDirectionSpline.Reset(new CCardinalSpline(look));
        PScalarSpline.Reset(new CCardinalSpline(scalar_pts));
        PAnimationSpeedSpline.Reset(new CCardinalSpline(anim_speed));
        EstimateKeyIndex();
        InitialiseSpline(0, -1);
    }
}

static bool eq(const C3DVector& a,const C3DVector& b){return fabs(a.X-b.X)<0.00001&&a.Y==b.Y&&a.Z==b.Z;}
int main(){
 for(int n=0;n<=12;++n)for(int old=-1;old<=7;++old)for(int variant=0;variant<8;++variant)for(int pre=0;pre<2;++pre){
  CTCCameraPointScriptedSpline s;CArray<C3DVector> previous;for(int j=0;j<old;++j)previous.push_back(C3DVector(99,88,77));
  if(old>=0)s.PTimingSpline.Reset(new CCardinalSpline(previous,4,true));if(pre){s.PPositionSpline.Reset(new CCardinalSpline(previous));s.PScalarSpline.Reset(new CCardinalSpline(previous));s.PViewDirectionSpline.Reset(new CCardinalSpline(previous));s.PAnimationSpeedSpline.Reset(new CCardinalSpline(previous));}
  CCardinalSpline* prior=s.PTimingSpline.data; s.estimates=s.inits=0;s.orderOK=true;s.Tension=variant*0.125f-0.25f;s.AnimationSpeed=7;s.PauseFor=3;s.NodeToStartOn=-5;s.NodeToEndOn=-6;
  for(int i=0;i<n;++i){CTCCameraPointScriptedSpline::CKeyCamera k;k.pos=C3DVector(i,i+variant,-i);k.look=C3DVector(variant,-i,2*i);k.speed=i+1;k.fov=variant+i;k.roll=-i;k.anim=i-variant;k.duration=3*i+variant;s.KeyCameras.push_back(k);}
  for(int pass=0;pass<2;++pass){
   s.UpdateSplines();
   if(n<=1){if(s.estimates||s.inits||s.PTimingSpline.data!=prior||s.PPositionSpline.resets!=pre||s.AnimationSpeed!=7||s.PauseFor!=3||s.NodeToStartOn!=-5||s.NodeToEndOn!=-6)return 1;continue;}
   if(!s.orderOK||s.estimates!=pass+1||s.inits!=pass+1||s.NodeToStartOn!=0||s.NodeToEndOn!=n-1||s.AnimationSpeed!=0||s.PauseFor!=0)return 2;
   if(prior&&s.PTimingSpline.data!=prior)return 3;prior=s.PTimingSpline.data;
   if(s.PTimingSpline.data->tension!=s.Tension||s.PTimingSpline.resets!=1||s.PTimingSpline.data->connected!=(old>=0))return 4;
   SplineOwner* mats[]={&s.PPositionSpline,&s.PViewDirectionSpline,&s.PScalarSpline,&s.PAnimationSpeedSpline};
   for(int q=0;q<4;++q)if(mats[q]->resets!=1+pass+pre||mats[q]->data->points.size()!=unsigned(n)||mats[q]->data->tension!=.5f||mats[q]->data->connected)return 5;
   for(int idx=0;idx<n;++idx){const CTCCameraPointScriptedSpline::CKeyCamera& k=s.KeyCameras[idx];
    if(!eq(s.PTimingSpline.data->points[idx],C3DVector(float(idx)/(n-1),k.duration,0))||!eq(mats[0]->data->points[idx],k.pos)||!eq(mats[1]->data->points[idx],k.look)||!eq(mats[2]->data->points[idx],C3DVector(k.speed,k.fov,k.roll))||!eq(mats[3]->data->points[idx],C3DVector(k.anim,0,0)))return 6;
   }s.Tension+=0.375f;s.KeyCameras[0].duration+=2.f;
  }
 }
 puts("0075ddf0_TEST PASS");return 0;
}