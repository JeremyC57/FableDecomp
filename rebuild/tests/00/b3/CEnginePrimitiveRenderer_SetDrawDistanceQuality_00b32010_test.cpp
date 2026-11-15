// Full row body; portable scene fixtures check mutation and call ordering.
#include <vector>
#include <assert.h>
#include <stdio.h>
enum EEngineVideoOptionQuality { ENGINE_VIDEO_OPTION_QUALITY_LOW, ENGINE_VIDEO_OPTION_QUALITY_NORMAL, ENGINE_VIDEO_OPTION_QUALITY_HIGH, ENGINE_VIDEO_OPTION_QUALITY_HIGHEST, ENGINE_VIDEO_OPTION_QUALITY_MAX_COUNT };
template<class T>class CArray:public std::vector<T>{};
template<class T>class CCountedPointer {public:T* Data;T* operator->()const{return Data;}};
class CEngineVideoOptionsDef {public:
 float DrawDistanceMultiplier,DrawDistanceMinimum,DrawDistanceMaximum,RepeatedMeshDrawDistanceFactor;
 float MinimumZSpriteAsMeshDistance,MaximumZSpriteAsMeshDistance,ZSpriteDrawDistanceMultiplier;
 bool EnableRepeatedMeshes;
};
class CEnginePrimitiveManagerRepeatedStaticMeshes {public:bool Enable;int Changes;void SetEnable(bool v){Enable=v;++Changes;}};
class CEnginePrimitiveRenderer;
class CEngineScene {public:int Id;void InvalidateBoundingBoxesAndDrawDistances();};
class CEnginePrimitiveRenderer {public:
 float DrawDistanceFactor,DrawDistanceMinimum,DrawDistanceMaximum,RepeatedMeshDrawDistanceFactor,ZSpriteDrawDistanceFactor;
 float MinimumZSpriteAsMeshDistance,MaximumZSpriteAsMeshDistance;bool DrawDistanceInvalid;
 CArray<CEngineScene*> SceneList;CCountedPointer<CEnginePrimitiveManagerRepeatedStaticMeshes> RepeatedStaticMeshManager;
 void SetDrawDistanceQuality(EEngineVideoOptionQuality,const CEngineVideoOptionsDef&);
};
static CEnginePrimitiveRenderer* Active;
static CEngineVideoOptionsDef* Options;
static CEngineScene* AppendScene;
static int Calls,Order[10];static bool PreviousEnable,Mutate;
void CEngineScene::InvalidateBoundingBoxesAndDrawDistances(){
 assert(Active->DrawDistanceInvalid);
 assert(Active->DrawDistanceFactor==Options->DrawDistanceMultiplier);
 assert(Active->RepeatedStaticMeshManager->Enable==PreviousEnable);
 assert(Active->RepeatedStaticMeshManager->Changes==0);
 Order[Calls++]=Id;
 if(Mutate && Calls==1){Active->SceneList.push_back(AppendScene);Options->EnableRepeatedMeshes=!Options->EnableRepeatedMeshes;}
}
void CEnginePrimitiveRenderer::SetDrawDistanceQuality(EEngineVideoOptionQuality, const CEngineVideoOptionsDef& def)
{
    DrawDistanceFactor = def.DrawDistanceMultiplier;
    DrawDistanceMinimum = def.DrawDistanceMinimum;
    DrawDistanceMaximum = def.DrawDistanceMaximum;
    RepeatedMeshDrawDistanceFactor = def.RepeatedMeshDrawDistanceFactor;
    ZSpriteDrawDistanceFactor = def.ZSpriteDrawDistanceMultiplier;
    MinimumZSpriteAsMeshDistance = def.MinimumZSpriteAsMeshDistance;
    MaximumZSpriteAsMeshDistance = def.MaximumZSpriteAsMeshDistance;
    DrawDistanceInvalid = true;
    for (CArray<CEngineScene*>::iterator scene = SceneList.begin(); scene != SceneList.end(); ++scene)
        (*scene)->InvalidateBoundingBoxesAndDrawDistances();
    RepeatedStaticMeshManager->SetEnable(def.EnableRepeatedMeshes);
}
int main(){
 for(int quality=0;quality!=4;++quality)for(int count=0;count<=4;++count)for(int mutate=0;mutate!=2;++mutate){
  CEnginePrimitiveRenderer renderer;CEngineVideoOptionsDef def;CEnginePrimitiveManagerRepeatedStaticMeshes manager;CEngineScene scenes[5];
  for(int i=0;i<5;++i)scenes[i].Id=i;
  def.DrawDistanceMultiplier=-0.25f;def.DrawDistanceMinimum=0.5f;def.DrawDistanceMaximum=128.0f;
  def.RepeatedMeshDrawDistanceFactor=2.0f;def.MinimumZSpriteAsMeshDistance=4.0f;def.MaximumZSpriteAsMeshDistance=16.0f;def.ZSpriteDrawDistanceMultiplier=0.125f;
  def.EnableRepeatedMeshes=(quality&1)!=0;manager.Enable=!def.EnableRepeatedMeshes;manager.Changes=0;
  renderer.RepeatedStaticMeshManager.Data=&manager;renderer.DrawDistanceInvalid=false;renderer.SceneList.reserve(5);
  for(int j=0;j<count;++j)renderer.SceneList.push_back(&scenes[j]);
  Active=&renderer;Options=&def;PreviousEnable=manager.Enable;Mutate=mutate!=0;AppendScene=&scenes[count];Calls=0;
  renderer.SetDrawDistanceQuality(static_cast<EEngineVideoOptionQuality>(quality),def);
  int expected=count+(mutate && count?1:0);assert(Calls==expected);
  for(int k=0;k<expected;++k)assert(Order[k]==k);
  assert(renderer.DrawDistanceFactor==def.DrawDistanceMultiplier && renderer.DrawDistanceMinimum==def.DrawDistanceMinimum && renderer.DrawDistanceMaximum==def.DrawDistanceMaximum);
  assert(renderer.RepeatedMeshDrawDistanceFactor==def.RepeatedMeshDrawDistanceFactor && renderer.ZSpriteDrawDistanceFactor==def.ZSpriteDrawDistanceMultiplier);
  assert(renderer.MinimumZSpriteAsMeshDistance==def.MinimumZSpriteAsMeshDistance && renderer.MaximumZSpriteAsMeshDistance==def.MaximumZSpriteAsMeshDistance);
  assert(renderer.DrawDistanceInvalid && manager.Enable==def.EnableRepeatedMeshes && manager.Changes==1);
 }
 puts("00b32010_TEST PASS");return 0;
}