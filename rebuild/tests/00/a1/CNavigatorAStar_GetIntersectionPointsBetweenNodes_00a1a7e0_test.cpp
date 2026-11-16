// Actual shared classes and target body; test doubles replace only external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CNAVIGATORASTAR_CTOR_PASS23
#define private public
#define protected public
// CNavigatorAStar::GetIntersectionPointsBetweenNodes -- cross the shared edge of two navigation cells.
// Leaf: ?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z
// Retail: 0x00A1A7E0,862bytes,thiscall,hidden result and ret16.
// Flags: none
#define FABLE_RETAIL_CNAVIGATORASTAR_INTERSECTIONS_PASS23
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVIGATORBASE
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorAStar.h"
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#endif
std::pair<C2DVector, C2DVector> CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float distance) const
{
    C2DBoxF fromBox(from.Position, from.GetSize() * 0.5f);
    C2DBoxF toBox(to.Position, to.GetSize() * 0.5f);
    if ((float)fabs(fromBox.BRX - toBox.TLX) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.Y - to.Position.Y) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(fromBox.BRX - distance, from.Position.Y);
            C2DVector second(fromBox.BRX + distance, from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.Y - from.Position.Y) / (to.Position.X - from.Position.X);
            float offset1 = from.GetSize() * 0.5f - distance;
            C2DVector first(fromBox.BRX - distance, offset1 * gradient + from.Position.Y);
            float offset2 = from.GetSize() * 0.5f + distance;
            C2DVector second(fromBox.BRX + distance, offset2 * gradient + from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else if ((float)fabs(fromBox.TLX - toBox.BRX) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.Y - to.Position.Y) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(fromBox.TLX + distance, from.Position.Y);
            C2DVector second(fromBox.TLX - distance, from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.Y - from.Position.Y) / (to.Position.X - from.Position.X);
            float offset1 = distance - from.GetSize() * 0.5f;
            C2DVector first(fromBox.TLX + distance, offset1 * gradient + from.Position.Y);
            float offset2 = -(from.GetSize() * 0.5f) - distance;
            C2DVector second(fromBox.TLX - distance, offset2 * gradient + from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else if ((float)fabs(fromBox.BRY - toBox.TLY) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.X - to.Position.X) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(from.Position.X, fromBox.BRY - distance);
            C2DVector second(from.Position.X, fromBox.BRY + distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.X - from.Position.X) / (to.Position.Y - from.Position.Y);
            float offset1 = from.GetSize() * 0.5f - distance;
            C2DVector first(offset1 * gradient + from.Position.X, fromBox.BRY - distance);
            float offset2 = from.GetSize() * 0.5f + distance;
            C2DVector second(offset2 * gradient + from.Position.X, fromBox.BRY + distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else {
        if ((float)fabs(from.Position.X - to.Position.X) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(from.Position.X, fromBox.TLY + distance);
            C2DVector second(from.Position.X, fromBox.TLY - distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.X - from.Position.X) / (to.Position.Y - from.Position.Y);
            float offset1 = distance - from.GetSize() * 0.5f;
            C2DVector first(offset1 * gradient + from.Position.X, fromBox.TLY + distance);
            float offset2 = -(from.GetSize() * 0.5f) - distance;
            C2DVector second(offset2 * gradient + from.Position.X, fromBox.TLY - distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    }
}

#undef private
#undef protected
const float DELTA_NOTIONAL_ZERO=0.0000999999974737875163555145263671875f;
static int SizeCalls;
static float NodeSize(unsigned int level){assert(level<=6);static const float sizes[]={32.0f,16.0f,8.0f,4.0f,2.0f,1.0f,0.5f};return sizes[level];}
float CANavQuadTreeNode::GetSize()const{++SizeCalls;return NodeSize(QuadTreeLevel);}
class FixtureInterface:public CNavigatorExternalInterface { public:
bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const {abort();}
float GetTerrainDesirabilityAt(const C3DVector&) const {abort();}
float GetTerrainHeightAt(const C3DVector&) const {abort();}
float GetThemeHeightAt(const C3DVector&) const {abort();}
float FindPosZAt(const C3DVector&,bool) const {abort();}
float FindPosZAtIncludingThemes(const C3DVector&,bool) const {abort();}
float PeekNavigatorRadius(bool) const {abort();}
C2DVector PeekNavigatorOrientation() const {abort();}
C2DVector PeekNavigatorPosition2D() const {abort();}
C3DVector PeekNavigatorPosition3D() const {abort();}
float PeekNavigatorMaxSpeed(const C3DVector&) const {abort();}
const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const {abort();}
std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() {abort();}
std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles() {abort();}
const CSteeringAgent* GetPThingToNotAvoid() const {abort();}
float GetProximityForDirectApproach() const {abort();}
long GetNavigationLevelAt(const C3DVector&) const {abort();}
long GetConstantFPS() const {abort();}
float GetMinPathCost() const {abort();}
float GetMaxPathCost() const {abort();}
bool IsPlayer() const {abort();}
bool IsDead() const {abort();}
float GetForwardNavigationWidthExtension() const {abort();}
CNavigatorExternalInterface* Clone() const {abort();}
};
class FixtureNavigator:public CNavigatorAStar {public:
FixtureNavigator(CNavigatorManager& m,const CNavigatorExternalInterface& i):CNavigatorAStar(m,i){}
ENavigatorReturn MoveToPos(const C3DVector&, const C3DVector&, float, C3DVector&, bool, bool, bool, bool, bool, bool, float) {abort();}
ENavigatorReturn MoveToPosProximity(const C3DVector&, const C3DVector&, float, float, C3DVector&, bool, bool, bool, bool, bool, bool, float) {abort();}
ENavigatorReturn MoveToNavLineOfSightProximity(const C3DVector&, const C3DVector&, float, float, C3DVector&, bool, bool, bool, bool, bool, bool, float) {abort();}
C3DVector GetRandomNavigablePos(const C3DVector&, float, bool) const {abort();}
C3DVector GetRandomNavigablePos(const C3DVector&, float, const C2DVector&, float) const {abort();}
bool GetNearestNavigablePosTo(const C3DVector&, float, C3DVector&) const {abort();}
bool CanNavigateToPos(const C3DVector&, const C3DVector&, float, bool) const {abort();}
bool IsNavLineOfSightClear(const C3DVector&, const C3DVector&) const {abort();}
bool IsNewDestinationGoingToCauseRestart(const C3DVector&, float) const {abort();}
const C3DVector& GetCurrentDestination() const {abort();}
void UpdateLastPosition(const C3DVector&) {abort();}
void CalculateRouteFromTo(const C3DVector&, const C3DVector&, std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >&) {abort();}
void InitialiseFromPreCalculatedRoute(const C3DVector&, const std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >&, const C3DVector&) {abort();}
};
CANavQuadTreeNode::~CANavQuadTreeNode(){}
const CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() const{abort();}
CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours(){abort();}
bool CANavQuadTreeNode::HasNeighbours() const{abort();}
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const{abort();}
long CANavQuadTreeNode::GetRegionID() const{abort();}
ENavQuadTreeNodeType CANavQuadTreeNode::GetNodeType() const{abort();}
const CNavRegion* CANavQuadTreeNode::GetRegion() const{abort();}
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const{abort();}
bool CANavQuadTreeNode::IsSwitchable() const{abort();}
bool CANavQuadTreeNode::IsSwitchableOn(unsigned __int64) const{abort();}
const CANavQuadTreeNode* CANavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const{abort();}
bool CANavQuadTreeNode::MakeRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&){abort();}
bool CANavQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&){abort();}
bool CANavQuadTreeNode::RemoveRegionZero(){abort();}
unsigned long CANavQuadTreeNode::GetMemoryUsage(unsigned long&, unsigned long&, unsigned long&, unsigned long&) const{abort();}

CNavigatorExternalInterface::~CNavigatorExternalInterface(){}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject(){}
CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{abort();}
void CClass::Save(CCharString&){abort();}
CNavigatorBase::CNavigatorBase(CNavigatorManager& m,unsigned long,const CNavigatorExternalInterface& i):PInterface(const_cast<CNavigatorExternalInterface*>(&i)),OnRoute(false),Interrupted(false),NavigationManager(&m),SteeringAgent(0){}
CNavigatorBase::~CNavigatorBase(){}
const CANavQuadTreeNode* CNavigatorBase::GetNextNeighbour(const CANavQuadTreeNode&,const CANavQuadTreeNode*,const C2DVector&){abort();}
void CNavigatorBase::CreateLineList(std::vector<C3DVector>&){abort();}
void CNavigatorBase::Interrupt(){abort();}
void CNavigatorBase::CalculateNavigableWidthOfLine(const C3DVector&,const C3DVector&,float&,float&){abort();}
CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(unsigned char level,unsigned char layer){Position.X=Position.Y=0;QuadTreeLevel=level;NavLayer=layer;LeafNode=true;Blocked=true;}
CNavigatorAStar::CNavigatorAStar(CNavigatorManager& m,const CNavigatorExternalInterface& i):CNavigatorBase(m,0,i),BlockedStartNode(0,0),BlockedEndNode(0,0),BlockedNodeAskedToGetTo(0,0){}
void CNavigatorAStar::UpdateLastPosition(const C3DVector&){abort();}
static void InitNode(CANavQuadTreeNode& n,float x,float y,unsigned int level){n.Position.X=x;n.Position.Y=y;n.QuadTreeLevel=(unsigned char)level;n.NavLayer=0;n.LeafNode=true;n.Blocked=false;}
static void CheckPoint(const C2DVector& actual,double x,double y){assert(fabs((double)actual.X-x)<0.00001);assert(fabs((double)actual.Y-y)<0.00001);}
int main(){
 CNavigatorManager manager;FixtureInterface external;FixtureNavigator nav(manager,external);
 unsigned int cases=0;
 for(unsigned int level=0;level<7;++level)for(unsigned int other=0;other<7;++other)
 for(unsigned int side=0;side<4;++side)for(int off=-2;off<=2;++off)for(unsigned int margin=0;margin<5;++margin){
  CANavQuadTreeNode a,b;float x=3.25f,y=-5.5f,half=NodeSize(level)*0.5f;
  float sep=half+NodeSize(other)*0.5f,shift=(float)off*0.125f,distance=(float)margin*0.125f;
  float dx=side==0?sep:side==1?-sep:shift;
  float dy=side==2?sep:side==3?-sep:shift;
  InitNode(a,x,y,level);InitNode(b,x+dx,y+dy,other);SizeCalls=0;
  std::pair<C2DVector,C2DVector> result=nav.GetIntersectionPointsBetweenNodes(a,b,distance);
  assert(SizeCalls==(off==0?2:4));
  // Independent line parameter through each side of the boundary strip.
  double ax,ay,bx,by;
  if(side<2){double edge=x+(side==0?half:-half),step=side==0?distance:-distance;ax=edge-step;bx=edge+step;ay=y+(ax-x)*dy/dx;by=y+(bx-x)*dy/dx;}
  else{double edge=y+(side==2?half:-half),step=side==2?distance:-distance;ay=edge-step;by=edge+step;ax=x+(ay-y)*dx/dy;bx=x+(by-y)*dx/dy;}
  CheckPoint(result.first,ax,ay);CheckPoint(result.second,bx,by);++cases;
 }
 assert(cases==4900);puts("00a1a7e0_TEST PASS");return 0;
}