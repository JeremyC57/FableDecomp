// 00a198c0 CNavigatorFlyer::GetRandomNavigablePos -- step toward a heading and query a navigable height.
// Leaf: ?GetRandomNavigablePos@CNavigatorFlyer@@UBE?AVC3DVector@@ABV2@MABVC2DVector@@M@Z
// Retail:00a198c0,289 bytes,const virtual thiscall slot6.
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_HEIGHTTHEMES
#define FABLE_RETAIL_CNAVIGATORFLYER
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#define FABLE_RETAIL_CNAVIGATORMANAGER_SAMEMAP
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C3DVECTOR_ADD2D_DIRECT
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CNavigatorFlyer.h"
#include "engine/CNavigatorManager.h"
#include "engine/CANavQuadTreeNode.h"
extern C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM;
C3DVector CNavigatorFlyer::GetRandomNavigablePos(const C3DVector& current_pos,float dist_to_keep_within,const C2DVector& direction_to_head,float accuracy) const
{
 const CNavigatorManager* nav_manager=NavigationManager;
 C2DVector direction_norm(direction_to_head);
 direction_norm.Normalise();
 C3DVector random_pos=current_pos+(direction_norm*dist_to_keep_within);
 const CANavQuadTreeNode* node=nav_manager->GetNodeAt(C2DVector(random_pos),0);
 if(node&&node->IsNavigable()&&nav_manager->IsInSameMap(C2DVector(current_pos),C2DVector(random_pos))){
  random_pos.Z=FindPosZAtIncludingThemes(random_pos+ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM,true);
  return random_pos;
 }
 return current_pos;
}