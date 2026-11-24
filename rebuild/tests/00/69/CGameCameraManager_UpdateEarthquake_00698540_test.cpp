#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector(){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector&p){X=p.X;Y=p.Y;Z=p.Z;}
 const C3DVector operator+(const C3DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}
 const C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
 const C3DVector operator/(float s)const{float r=1.0f/s;return C3DVector(X*r,Y*r,Z*r);}
 const C3DVector&operator+=(const C3DVector&o){X+=o.X;Y+=o.Y;Z+=o.Z;return *this;}
 float GetAccurateMagnitude()const{float magnitude=X*X+Y*Y+Z*Z;return(float)sqrt(magnitude);}
};
struct CWorld{static long frame;static long GetFrame(){return frame;}};long CWorld::frame=0;
struct CMainGameComponent{static long fps;static long GetConstantFPS(){return fps;}};long CMainGameComponent::fps=10;
struct CameraDouble{C3DVector pos;int moves;CameraDouble():pos(0,0,0),moves(0){}const C3DVector&PeekWorldPos()const{return pos;}void MoveTo(const C3DVector&p){pos=p;++moves;}};
namespace NRumble{static int calls;static float attenuation;static C3DVector gotSource,gotCamera;float GetRumbleDistanceAttenuationAtPos(const C3DVector&a,const C3DVector&b){++calls;gotSource=a;gotCamera=b;return attenuation;}}
inline double GFExp(double x){return exp(x);}inline double GFSin(float a){return sin(a*(double)6.2831855f);}
const float DELTA_NOTIONAL_ZERO=0.0001f;
class CGameCameraManager{public:
 struct CQuake{C3DVector Position,Axis;long Frame;float Amplitude,Duration,Frequency,Phase;bool Global;};
 CameraDouble Camera;CWorld World;std::vector<CQuake> EarthQuakes;
 const CWorld&GetWorld()const{return World;}void UpdateEarthquake();
};

void CGameCameraManager::UpdateEarthquake()
{
    C3DVector pos=Camera.PeekWorldPos();
    long frame=GetWorld().GetFrame();
    long const_fps=CMainGameComponent::GetConstantFPS();
    const float MAX_DISPLACEMENT=1.0f;
    C3DVector offset_vector(0.0f,0.0f,0.0f);
    for(std::vector<CQuake>::iterator it=EarthQuakes.begin();it!=EarthQuakes.end();) {
        CQuake& quake=*it;
        long frames=frame-quake.Frame;
        float seconds=(float)frames/const_fps;
        if(seconds>quake.Duration)it=EarthQuakes.erase(it);
        else {
            ++it;
            float distance_attenuator=1.0f;
            if(!quake.Global)distance_attenuator=NRumble::GetRumbleDistanceAttenuationAtPos(quake.Position,pos);
            float amplitude=quake.Amplitude*distance_attenuator;
            float decay_param=3.0f/quake.Duration;
            float displacement=(float)(amplitude*GFExp(-seconds*decay_param)*GFSin(seconds*quake.Frequency+quake.Phase*0.25f));
            offset_vector+=quake.Axis*displacement;
        }
    }
    float mag=offset_vector.GetAccurateMagnitude();
    if(mag>DELTA_NOTIONAL_ZERO) {
        float disp=mag<MAX_DISPLACEMENT?mag:MAX_DISPLACEMENT;
        Camera.MoveTo(pos+offset_vector*disp/mag);
    }
}
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
static bool close(float a,float b){return fabs(a-b)<0.00003;}
static CGameCameraManager::CQuake quake(float amp,float dur,float freq,float phase,bool global){CGameCameraManager::CQuake q;q.Position=C3DVector(9,8,7);q.Axis=C3DVector(1,0,0);q.Frame=10;q.Amplitude=amp;q.Duration=dur;q.Frequency=freq;q.Phase=phase;q.Global=global;return q;}
static void reset(){CWorld::frame=10;CMainGameComponent::fps=10;NRumble::calls=0;NRumble::attenuation=0.25f;}
int main(){
 reset();{CGameCameraManager m;m.Camera.pos=C3DVector(1,2,3);m.UpdateEarthquake();check(m.Camera.moves==0&&m.EarthQuakes.empty(),"empty");}
 reset();{CGameCameraManager m;m.EarthQuakes.push_back(quake(0.5f,2,0,1,true));m.UpdateEarthquake();check(m.Camera.moves==1&&close(m.Camera.pos.X,0.5f)&&NRumble::calls==0,"global phase and no attenuation");}
 reset();{CGameCameraManager m;m.Camera.pos=C3DVector(1,2,3);m.EarthQuakes.push_back(quake(0.8f,2,0,1,false));m.UpdateEarthquake();check(close(m.Camera.pos.X,1.2f)&&close(m.Camera.pos.Y,2)&&NRumble::calls==1,"local attenuation");check(close(NRumble::gotSource.X,9)&&close(NRumble::gotCamera.X,1),"attenuation arguments");}
 reset();{CGameCameraManager m;m.EarthQuakes.push_back(quake(4,2,0,1,true));m.EarthQuakes[0].Axis=C3DVector(0,3,4);m.UpdateEarthquake();check(close(m.Camera.pos.X,0)&&close(m.Camera.pos.Y,0.6f)&&close(m.Camera.pos.Z,0.8f),"direction-preserving unit clamp");}
 reset();{CGameCameraManager m;m.EarthQuakes.push_back(quake(0.5f,2,0,1,true));m.EarthQuakes.push_back(quake(0.5f,2,0,1,true));m.EarthQuakes[1].Axis=C3DVector(-1,0,0);m.UpdateEarthquake();check(m.Camera.moves==0,"cancellation");}
 reset();{CGameCameraManager m;m.EarthQuakes.push_back(quake(0.00001f,2,0,1,true));m.UpdateEarthquake();check(m.Camera.moves==0,"notional zero");}
 reset();{CGameCameraManager m;CWorld::frame=31;m.EarthQuakes.push_back(quake(1,2,0,1,false));m.EarthQuakes.push_back(quake(1,0,0,1,false));m.EarthQuakes.push_back(quake(0.5f,4,0,1,true));m.UpdateEarthquake();float expected=(float)(0.5*exp(-2.1*0.75));check(m.EarthQuakes.size()==1&&close(m.Camera.pos.X,expected)&&NRumble::calls==0,"erase consecutive expired quakes and preserve survivor");}
 reset();{CGameCameraManager m;CWorld::frame=30;m.EarthQuakes.push_back(quake(0.5f,2,0,1,true));m.UpdateEarthquake();check(m.EarthQuakes.size()==1&&close(m.Camera.pos.X,(float)(0.5*exp(-3.0))),"duration boundary survives");}
 reset();{CGameCameraManager m;CWorld::frame=15;m.EarthQuakes.push_back(quake(0.5f,2,0.25f,0,true));m.UpdateEarthquake();float expected=(float)(0.5*exp(-0.75)*sin(0.125*(double)6.2831855f));check(close(m.Camera.pos.X,expected),"frame to seconds, frequency, exponential decay");}
 puts("00698540_TEST PASS");return 0;
}