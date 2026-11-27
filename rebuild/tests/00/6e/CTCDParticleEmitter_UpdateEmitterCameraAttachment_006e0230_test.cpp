
#include <stdio.h>
struct C3DVector {
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator*(float t)const{return C3DVector(X*t,Y*t,Z*t);}
};
struct CRightHandedSet {C3DVector up,forward;void Init(const C3DVector& a,const C3DVector& b){up=a;forward=b;}};
struct CCamera {C3DVector pos;CRightHandedSet rhs;const C3DVector& PeekWorldPos()const{return pos;}const C3DVector& PeekViewDirection()const{return rhs.forward;}const C3DVector& PeekUpDirection()const{return rhs.up;}};
struct C3DPositionInterpolator {C3DVector start,end;C3DPositionInterpolator(){}C3DPositionInterpolator(const C3DVector& a,const C3DVector& b):start(a),end(b){}};
struct C3DOrientationInterpolator {CRightHandedSet start,end;C3DOrientationInterpolator(){}C3DOrientationInterpolator(const CRightHandedSet& a,const CRightHandedSet& b):start(a),end(b){}};
namespace NParticleEngine {struct CParticleEmitter{
 CRightHandedSet current,last;C3DVector pos;
 C3DPositionInterpolator pi;C3DOrientationInterpolator oi;int pc,oc,queries;
 CParticleEmitter():pc(0),oc(0),queries(0){}
 CRightHandedSet GetCurrentOrientation(){++queries;return current;}
 const C3DVector& PeekEndPos()const{return pos;} const CRightHandedSet& PeekEndOrientation()const{return last;}
 void SetPosition(const C3DPositionInterpolator& v){pi=v;++pc;}void SetOrientation(const C3DOrientationInterpolator& v){oi=v;++oc;}
};}
struct CameraManager {CCamera c;int count;bool bad;CCamera GetRenderCamera(float t){++count;if(t!=1)bad=true;return c;}};
struct CWorld{CameraManager cm;CameraManager* GetGameCamera(){return &cm;}};
struct Physics{int count;C3DVector p;void SetPosition(const C3DVector& v){++count;p=v;}};
struct Thing{Physics phy;Physics* GetPhysicsTC(){return &phy;}};
struct CTCDParticleEmitter {
 enum EAttachmentFlags{AF_USE_THING_ORIENTATION=32,AF_USE_OFFSET=128};
 struct Params{CRightHandedSet RHSet;}params,*PCreationParameters;
 NParticleEngine::CParticleEmitter* PEmitter;bool WaitingForParticleToLoad,ResetEmitterPosition,ResetEmitterOrientation;unsigned long AttachmentFlags;float FloatOffset;CWorld w;Thing parent;
 CWorld& GetWorld(){return w;}Thing& GetParentThing(){return parent;}
 void UpdateEmitterCameraAttachment(bool reset);
};
void CTCDParticleEmitter::UpdateEmitterCameraAttachment(bool reset)
{
    if (PEmitter != 0 || WaitingForParticleToLoad) {
        CCamera current_camera = GetWorld().GetGameCamera()->GetRenderCamera(1.0f);
        C3DVector new_pos = current_camera.PeekWorldPos() + current_camera.PeekViewDirection() * 4.0f;
        CRightHandedSet new_rhs;
        if (AttachmentFlags & AF_USE_THING_ORIENTATION) {
            new_rhs.Init(current_camera.PeekUpDirection(), current_camera.PeekViewDirection());
        } else if (PEmitter != 0) {
            new_rhs = PEmitter->GetCurrentOrientation();
        } else {
            new_rhs = PCreationParameters->RHSet;
        }

        if (ResetEmitterPosition || ResetEmitterOrientation) {
            if (AttachmentFlags & AF_USE_OFFSET)
                new_pos.Z += FloatOffset;
            if (ResetEmitterPosition) {
                GetParentThing().GetPhysicsTC()->SetPosition(new_pos);
                if (PEmitter != 0)
                    PEmitter->SetPosition(C3DPositionInterpolator(reset ? new_pos : PEmitter->PeekEndPos(), new_pos));
                ResetEmitterPosition = false;
            }
            if (ResetEmitterOrientation) {
                if (PEmitter != 0)
                    PEmitter->SetOrientation(C3DOrientationInterpolator(reset ? new_rhs : PEmitter->PeekEndOrientation(), new_rhs));
                else
                    PCreationParameters->RHSet = new_rhs;
                ResetEmitterOrientation = false;
            }
        }
    }
}

static bool eq(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static bool eq(const CRightHandedSet& a,const CRightHandedSet& b){return eq(a.up,b.up)&&eq(a.forward,b.forward);}
static CRightHandedSet basis(int k){CRightHandedSet r;r.up=C3DVector(1+k,4-k,3*k);r.forward=C3DVector(-2*k,1+k,6-k);return r;}
int main(){
 for(int state=0;state<128;++state)for(int k=-3;k<=3;++k)for(int z=-2;z<=2;++z){
  CTCDParticleEmitter c;NParticleEngine::CParticleEmitter e;
  bool live=(state&1)!=0,waiting=(state&2)!=0,rp=(state&4)!=0,ro=(state&8)!=0,orient=(state&16)!=0,offset=(state&32)!=0,reset=(state&64)!=0;
  c.PEmitter=live?&e:0;c.WaitingForParticleToLoad=waiting;c.ResetEmitterPosition=rp;c.ResetEmitterOrientation=ro;c.AttachmentFlags=(orient?32:0)|(offset?128:0);c.FloatOffset=(float)z;c.w.cm.count=0;c.w.cm.bad=false;c.parent.phy.count=0;c.PCreationParameters=&c.params;
  c.params.RHSet=basis(k+8);e.current=basis(k-2);e.last=basis(k+1);e.pos=C3DVector(-9,5,7);c.w.cm.c.rhs=basis(k);c.w.cm.c.pos=C3DVector(1-k,3+k,2*k);
  CRightHandedSet expected=orient?c.w.cm.c.rhs:(live?e.current:c.params.RHSet);CRightHandedSet original=c.params.RHSet;
  C3DVector p=C3DVector(1-k+4*(-2*k),3+k+4*(1+k),2*k+4*(6-k)+(offset?z:0));
  const bool active=live||waiting;c.UpdateEmitterCameraAttachment(reset);
  if(c.w.cm.count!=int(active)||c.w.cm.bad||c.WaitingForParticleToLoad!=waiting||c.ResetEmitterPosition!=(rp&&!active)||c.ResetEmitterOrientation!=(ro&&!active))return 1;
  if(c.parent.phy.count!=int(active&&rp)||(active&&rp&&!eq(c.parent.phy.p,p)))return 2;
  if(e.pc!=int(live&&rp)||e.oc!=int(live&&ro)||e.queries!=int(live&&!orient))return 3;
  if(e.pc&&(!eq(e.pi.start,reset?p:e.pos)||!eq(e.pi.end,p)))return 4;
  if(e.oc&&(!eq(e.oi.start,reset?expected:e.last)||!eq(e.oi.end,expected)))return 5;
  if(!eq(c.params.RHSet,!live&&active&&ro?expected:original))return 6;
 }
 puts("006e0230_TEST PASS");return 0;}