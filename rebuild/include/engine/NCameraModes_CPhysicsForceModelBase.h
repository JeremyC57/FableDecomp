#ifndef FABLE_ENGINE_NCAMERAMODES_CPHYSICSFORCEMODELBASE_H
#define FABLE_ENGINE_NCAMERAMODES_CPHYSICSFORCEMODELBASE_H
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30
#include "rebuild_abi.h"
#include "engine/GFCos.h"
#include "engine/GFSin.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#endif
#include "engine/NCameraModes_CPhysicsState.h"
#include "engine/CPhysicsCollisionPointsObject.h"
#include "engine/CGameCameraManager.h"
#include "engine/CWorld.h"
#include "engine/NCameraModes_CBase.h"
// Original Ego_r full class244 bytes; three native collision objects36 replace donor60.
extern const C3DVector UnitSet[4];
extern double FABLE_FASTCALL GFCos(float);
extern double FABLE_FASTCALL GFSin(float);
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_CALLER_PASS30
#include "engine/CPhysicsMeshObject.h"
class CThing;
namespace NCameraModes {
class CGetThingsWithCollisionMesh : public std::unary_function<const CThing*,bool> {
public:
    CGetThingsWithCollisionMesh() {}
    bool operator()(const CThing*) const;
};
class CGetMeshObject : public std::unary_function<const CThing*,const CPhysicsMeshObject> {
public:
    CGetMeshObject() {}
    const CPhysicsMeshObject operator()(const CThing*) const;
};
}
namespace std {
template<> _List_base<const CThing*,allocator<const CThing*> >::~_List_base();
template<> void _List_base<CPhysicsMeshObject,allocator<CPhysicsMeshObject> >::clear();
template<class InputIter,class OutputIter,class UnaryOperation>
OutputIter FABLE_FASTCALL transform(InputIter first, InputIter last, OutputIter result, UnaryOperation op);
}
#endif
namespace NCameraModes {
class CBase;
class CPhysicsForceModelBase {
public:
#if defined(FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30) || defined(FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_CALLER_PASS30) || defined(FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_DECL_PASS30)
 // Original FableWin 0288C330. The three references are supplied by the camera owner.
 CPhysicsForceModelBase(CBase&, const CGameCameraManager&, CPhysicsState&);
#endif
 virtual void FrameUpdate();
 virtual void Reset();
 virtual C3DVector DoThingEvasion()=0;
 virtual C3DVector DoThemeEvasion()=0;
 virtual C3DVector DoGroundEvasion()=0;
 virtual C3DVector DoCreatureEvasion()=0;
 virtual C3DVector DoAttractToPoint()=0;
 virtual C3DVector DoVelocityDamping()=0;
protected:
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_SET_ACCELERATION_PASS27
 // Original FableWin 0x02B1F4A0 and native final store in 0x008FA660.
 void SetAcceleration(const C3DVector& acceleration) { LinkedState.Acceleration = acceleration; }
#endif
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30
 CBase& GetOwner() { return Owner; }
#endif
 C3DVector GetPosition(){return LinkedState.Position;}
 const CGameCameraManager& GetCameraManager() const {return CameraManager;}
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30
 // FableWin 02B22F00 and the 108-byte 008FBCB0 owner return the first mesh intersection.
 bool GetPointIntersection(std::list<CPhysicsMeshObject>& meshes, long, const C3DVector& from,
                           const C3DVector& to, C3DVector& out)
 {
     std::list<CPhysicsMeshObject>::const_iterator it = meshes.begin();
     while (it != meshes.end()) {
         const CPhysicsMeshObject& mesh = *it++;
         CMeshIntersectionData data;
         if (mesh.GetPointIntersectionFromTo(from, to, data)) {
             out = data.IntersectionPosition;
             return true;
         }
     }
     return false;
 }
#endif
 bool GetPointIntersection(const CWorldMap&,long,const C3DVector&,const C3DVector&,C3DVector&) {return false;}
#ifdef _MSC_VER
 #define FABLE_FORCE_SCAN_NOINLINE __declspec(noinline)
#else
 #define FABLE_FORCE_SCAN_NOINLINE
#endif
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_PASS30
 template<class T> FABLE_FORCE_SCAN_NOINLINE void UpdateScan(CPhysicsCollisionPointsObject& phys_obj,T& collision_objects,const C3DVector& origin,long test_type,float scale) {
    unsigned long wf = GetCameraManager().GetWorld().GetFrame();
    long number = wf % phys_obj.ScanDensity;
    float phi = float(number) / phys_obj.ScanDensity;
    C3DVector cam_pos = GetOwner().GetPosition();
    TriggerFrame = wf;
    bool update = false;
    if (wf < TriggerFrame + phys_obj.ScanDensity)
        update = true;
    if (update) {
        long number_stored = 0;
        for (float this_theta = phys_obj.StartTheta; this_theta <= 0.5f + DELTA_NOTIONAL_ZERO;
             this_theta += phys_obj.DeltaTheta) {
            long number_of_tests = phys_obj.GetNumberOfTests(this_theta);
            long factor = phys_obj.ScanDensity / number_of_tests;
            if (wf % factor == 0) {
                std::list<std::pair<float, std::list<C3DVector> > >::iterator it = phys_obj.SmartPointList.begin();
                bool quit = false;
                while (it != phys_obj.SmartPointList.end() && !quit) {
                    float stored_theta = (it++)->first;
                    if (stored_theta > this_theta - DELTA_NOTIONAL_ZERO &&
                        stored_theta < this_theta + DELTA_NOTIONAL_ZERO)
                        quit = true;
                }
                std::list<C3DVector>& points_list = (*--it).second;
                long size_guy = points_list.size();
                if (size_guy == number_of_tests)
                    points_list.pop_front();
                // Bracket: native 008FC2CF loads origin and the mesh list together; see the expression controls.
                C3DVector test_pos = origin
                    + UnitSet[2] * scale * ((GFCos(phi))) * ((GFSin(this_theta)))
                    + UnitSet[1] * scale * ((GFSin(phi))) * ((GFSin(this_theta)))
                    + UnitSet[3] * scale * ((GFCos(this_theta)));
                C3DVector collision_pos = test_pos;
                GetPointIntersection(collision_objects, test_type, origin, test_pos, collision_pos);
                points_list.push_back(collision_pos);
            }
        }
    }
 }
#elif defined(FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_CALLER_PASS30) || defined(FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_SCAN_DECL_PASS30)
 template<class T> FABLE_FORCE_SCAN_NOINLINE void UpdateScan(CPhysicsCollisionPointsObject& phys_obj,T& collision_objects,const C3DVector& origin,long test_type,float scale);
#else
 template<class T> FABLE_FORCE_SCAN_NOINLINE void UpdateScan(CPhysicsCollisionPointsObject& phys_obj,T& collision_objects,const C3DVector& origin,long test_type,float scale) {
  unsigned long wf=GetCameraManager().GetWorld().GetFrame();
  long number=wf%phys_obj.ScanDensity;
  float phi=float(number)/phys_obj.ScanDensity;
  C3DVector cam_pos=Owner.GetPosition();
  TriggerFrame=wf;
  bool update=false;
  if(wf<TriggerFrame+phys_obj.ScanDensity) update=true;
  if(update) {
   long number_stored=0;
   for(float this_theta=phys_obj.StartTheta;this_theta<=0.5+DELTA_NOTIONAL_ZERO;this_theta+=phys_obj.DeltaTheta) {
    long number_of_tests=phys_obj.GetNumberOfTests(this_theta);
    long factor=phys_obj.ScanDensity/number_of_tests;
    if(wf%factor==0) {
     std::list<std::pair<float,std::list<C3DVector> > >::iterator it=phys_obj.SmartPointList.begin();
     bool quit=false;
     while(it!=phys_obj.SmartPointList.end()&&!quit) {
      float stored_theta=(it++)->first;
      if(stored_theta>this_theta-DELTA_NOTIONAL_ZERO&&stored_theta<this_theta+DELTA_NOTIONAL_ZERO) quit=true;
     }
     std::list<C3DVector>& points_list=(*--it).second;
     long size_guy=points_list.size();
     if(size_guy==number_of_tests) points_list.pop_front();
     C3DVector test_pos=origin+UnitSet[2]*scale*GFCos(phi)*GFSin(this_theta)
       +UnitSet[1]*scale*GFSin(phi)*GFSin(this_theta)
       +UnitSet[3]*scale*GFCos(this_theta);
     C3DVector collision_pos=test_pos;
     GetPointIntersection(collision_objects,test_type,origin,test_pos,collision_pos);
     points_list.push_back(collision_pos);
    }
   }
  }
 }
#endif
#ifdef FABLE_RETAIL_CPHYSICSFORCEMODELBASE_MESH_CALLER_PASS30
 template<class T> void GetCollisionPoints(CPhysicsCollisionPointsObject& points, T& world, const C3DVector& pos, long type, float radius) {
#else
 template<class T> void GetCollisionPoints(CPhysicsCollisionPointsObject& points,const T& world,const C3DVector& pos,long type,float radius) {
#endif
  UpdateScan<T>(points,world,pos,type,radius);points.GetPointsList();
 }
 float PotentialField(const C3DVector& pos,const C3DVector& source,float scale,float r_prime) const {
  return (scale*0.1f)/((pos-source).GetMagnitude()-r_prime);
 }
 C3DVector PotentialFieldBufferPoints(const C3DVector& field_is_at,const std::list<C3DVector>& points_list,float override_scale_factor,float r_prime) const {
  float field_at_pos[4]={0.0f,0.0f,0.0f,0.0f};
  for(std::list<C3DVector>::const_iterator it=points_list.begin();it!=points_list.end();++it) {
   for(long counter=0;counter<4;++counter) {
    C3DVector pos_to_check=field_is_at+UnitSet[counter]*0.1f;
    field_at_pos[counter]+=PotentialField(pos_to_check,*it,override_scale_factor,r_prime);
   }
  }
  return C3DVector((field_at_pos[1]-field_at_pos[0])/0.1f,(field_at_pos[2]-field_at_pos[0])/0.1f,(field_at_pos[3]-field_at_pos[0])/0.1f);
 }
private:
 CPhysicsState& LinkedState;
 CPhysicsCollisionPointsObject Things,Themes,Ground;
 C3DVector LastAcceleration,ScanCenter,AverageCamPos;
 float ScanRadSquared;
 long TriggerFrame;
 bool AllowedToCullPhysics;
 CBase& Owner;
 const CGameCameraManager& CameraManager;
};
}
#endif
