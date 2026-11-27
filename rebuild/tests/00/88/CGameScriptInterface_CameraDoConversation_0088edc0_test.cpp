#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_CONVERSATION_CAMERA_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#include "engine/fable_nscript_forward.h"
#include "engine/C3DVector.h"
static int test_case;
#define check(value) do { if (!(value)) { printf("case %d line %d\n",test_case,__LINE__); exit(1); } } while(0)
unsigned long GFastOneOverSqrtTable[128];
static float angle; static int rotates;
void C3DVector::Rotate(float xy, float yz) {
    check(yz==0.0f); angle=xy; ++rotates;
    float saved=X;
    if(xy==0.25f) { X=-Y; Y=saved; }
    else { check(xy==0.75f); X=Y; Y=-saved; }
}
struct Focal {
    C3DVector p;
    C3DVector GetFocalPos(float t) { check(t==1.0f);return p; }
};
struct Physics {
    C3DVector v; C3DVector GetFacingVector() { return v; }
};
class CThingCreatureBase {
public:
    unsigned long flags; Physics physics; Focal focal;
    unsigned long GetTypeFlags() { return flags; }
    Physics* GetPhysicsTC() { return &physics; }
    Focal* GetTCGraphicAppearance() { return &focal; }
};
class CScriptThing { public:
    bool alive; CThingCreatureBase* p;
    bool IsAlive() const { return alive; }
    CThingCreatureBase* GetPThing() const { check(alive); return p; }
};
struct Camera {
    C3DVector pos[2], view[2]; int step;
    void ScriptMoveToPosAndLookAtPos(const C3DVector& p,const C3DVector& target,float t) {
        check(step==0 || step==2); int i=step/2;check(t==(i?6.0f:0.0f));
        pos[i]=p;view[i]=target;++step;
    }
    void ScriptStart() { check(step==1 || step==4);++step; }
    void ScriptSetRotationSpeed(float s) { check(step==3 && s==0);++step; }
};
struct WorldStub { Camera camera;Camera* GetGameCamera() { return &camera; } };
class CGameScriptInterface { public:
    WorldStub* World;
    void CameraDoConversation(const CScriptThing&,const CScriptThing&,ECameraOp,bool) const;
};
void CGameScriptInterface::CameraDoConversation(const CScriptThing& script_speaker, const CScriptThing& script_speakee, ECameraOp camera_op, bool wait) const
{
    if (!script_speaker.IsAlive())
        return;
    if (!script_speakee.IsAlive())
        return;
    if (!(script_speaker.GetPThing()->GetTypeFlags() & 8))
        return;
    if (!(script_speakee.GetPThing()->GetTypeFlags() & 8))
        return;
    CThingCreatureBase* tspeaker = static_cast<CThingCreatureBase*>(script_speaker.GetPThing());
    CThingCreatureBase* tspeakee = static_cast<CThingCreatureBase*>(script_speakee.GetPThing());
    float message_length_secs = 6.0f;
    switch (camera_op) {
    case CAM_OP_SPEAKER_LEFT_ZOOM_ON_SPEAKEE: {
        C3DVector start_track_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeaker->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.75f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_SPEAKER_RIGHT_ZOOM_ON_SPEAKEE: {
        C3DVector start_track_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeaker->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.25f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_SPEAKEE_LEFT_ZOOM_ON_SPEAKER: {
        C3DVector start_track_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeakee->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.75f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_SPEAKEE_RIGHT_ZOOM_ON_SPEAKER: {
        C3DVector start_track_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeakee->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos -= facing_vect * 0.5f;
        facing_vect.Rotate(0.25f, 0.0f);
        start_track_pos += facing_vect * 0.5f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_ZOOM_ON_SPEAKER_FACE: {
        C3DVector start_track_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeaker->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeaker->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos += facing_vect * 0.75f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    case CAM_OP_ZOOM_ON_SPEAKEE_FACE: {
        C3DVector start_track_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector view_pos = tspeakee->GetTCGraphicAppearance()->GetFocalPos(1.0f);
        C3DVector facing_vect = tspeakee->GetPhysicsTC()->GetFacingVector();
        facing_vect.Z = 0.0f;
        facing_vect.Normalise();
        start_track_pos += facing_vect * 0.75f;
        C3DVector end_track_pos = start_track_pos + (view_pos - start_track_pos) * 0.5f;
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(start_track_pos, view_pos, 0.0f);
        World->GetGameCamera()->ScriptStart();
        World->GetGameCamera()->ScriptMoveToPosAndLookAtPos(end_track_pos, view_pos, message_length_secs);
        World->GetGameCamera()->ScriptSetRotationSpeed(0.0f);
        World->GetGameCamera()->ScriptStart();
        break;
    }
    }
}

static void vectorIs(const C3DVector& v,float x,float y,float z)
{
    if(fabs(v.X-x)>0.0001 || fabs(v.Y-y)>0.0001 || fabs(v.Z-z)>0.0001) printf("got %f %f %f expected %f %f %f\n",v.X,v.Y,v.Z,x,y,z);
    check(fabs(v.X-x)<0.0001 && fabs(v.Y-y)<0.0001 && fabs(v.Z-z)<0.0001);
}
int main()
{
    GFastOneOverSqrtTable[64]=256; // Unit horizontal facing vectors have this seed index.
    CThingCreatureBase first,second;
    first.focal.p=C3DVector(4,6,8); second.focal.p=C3DVector(10,12,14);
    first.physics.v=C3DVector(1,0,9); second.physics.v=C3DVector(0,1,-11);
    CScriptThing speaker,speakee;speaker.p=&first;speakee.p=&second;
    WorldStub world;CGameScriptInterface game;game.World=&world;
    for(int mask=0;mask<16;++mask) for(int op=-1;op<=6;++op) for(int wait=0;wait<2;++wait) {
        test_case=mask*100+op*2+wait;
        speaker.alive=(mask&1)!=0;speakee.alive=(mask&2)!=0;
        first.flags=(mask&4)?8:4;second.flags=(mask&8)?8:4;
        world.camera.step=0;rotates=0;angle=-1;
        game.CameraDoConversation(speaker,speakee,(ECameraOp)op,wait!=0);
        if(mask!=15 || op<0 || op>5) { check(world.camera.step==0 && rotates==0); continue; }
        check(world.camera.step==5);
        bool sourceIsSpeaker=op==2 || op==3 || op==4;
        C3DVector p=sourceIsSpeaker?first.focal.p:second.focal.p;
        C3DVector v=(op==0 || op==1 || op==4)?first.focal.p:second.focal.p;
        if(op<4) {
            check(rotates==1 && angle==((op&1)?0.25f:0.75f));
            if(sourceIsSpeaker) {p.X-=0.5f;p.Y+=(op&1)?0.5f:-0.5f;}
            else {p.Y-=0.5f;p.X+=(op&1)?-0.5f:0.5f;}
        } else {check(rotates==0); if(sourceIsSpeaker) p.X+=0.75f; else p.Y+=0.75f;}
        vectorIs(world.camera.pos[0],p.X,p.Y,p.Z);
        vectorIs(world.camera.pos[1],(p.X+v.X)/2,(p.Y+v.Y)/2,(p.Z+v.Z)/2);
        for(int i=0;i<2;++i) vectorIs(world.camera.view[i],v.X,v.Y,v.Z);
    }
    puts("0088edc0_TEST PASS");return 0;
}