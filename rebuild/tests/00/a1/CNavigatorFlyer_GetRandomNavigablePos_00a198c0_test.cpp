#include <cstdio>
#include <cmath>
class C3DVector;
class C2DVector{public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C3DVector&);C2DVector& Normalise(){float k=1.0f/(float)std::sqrt(X*X+Y*Y);X*=k;Y*=k;return *this;}C2DVector operator*(float s)const{return C2DVector(X*s,Y*s);}};
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C2DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z);}C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}};
C2DVector::C2DVector(const C3DVector& v):X(v.X),Y(v.Y){}
class CANavQuadTreeNode{public:bool Blocked;bool IsNavigable()const{return !Blocked;}};
static bool gPresent,gSame;static int gNodes,gMaps,gHeights,gLayer;static C2DVector gNodeAt(0,0),gMapFrom(0,0),gMapTo(0,0);static C3DVector gHeightAt;static bool gUseNavigation;static CANavQuadTreeNode gNode;
class CNavigatorManager{public:const CANavQuadTreeNode* GetNodeAt(const C2DVector& p,unsigned char layer)const{++gNodes;gNodeAt=p;gLayer=layer;return gPresent?&gNode:0;}bool IsInSameMap(const C2DVector& a,const C2DVector& b)const{++gMaps;gMapFrom=a;gMapTo=b;return gSame;}};
class CNavigatorExternalInterface{public:virtual float FindPosZAtIncludingThemes(const C3DVector& p,bool n)const{++gHeights;gHeightAt=p;gUseNavigation=n;return p.Z+8;}};
class CNavigatorBase{public:CNavigatorManager* NavigationManager;CNavigatorExternalInterface* PInterface;float FindPosZAtIncludingThemes(const C3DVector& p,bool n)const{return PInterface->FindPosZAtIncludingThemes(p,n);}};
class CNavigatorFlyer:public CNavigatorBase{public:C3DVector GetRandomNavigablePos(const C3DVector&,float,const C2DVector&,float)const;};
C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM(2,-4,16);
C3DVector CNavigatorFlyer::GetRandomNavigablePos(const C3DVector& current_pos,float dist_to_keep_within,const C2DVector& direction_to_head,float accuracy)const
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
static bool Near(double a,double b){return std::fabs(a-b)<1.0/131072.0;}
int main(){int ok=1,cases=0;CNavigatorManager manager;CNavigatorExternalInterface external;CNavigatorFlyer flyer;flyer.NavigationManager=&manager;flyer.PInterface=&external;C2DVector dirs[4]={C2DVector(1,0),C2DVector(0,-2),C2DVector(3,4),C2DVector(-4,3)};float distances[4]={0,2,-4,0.5f};C3DVector pos(8,-16,32);
for(int present=0;present<2;++present)for(int blocked=0;blocked<2;++blocked)for(int same=0;same<2;++same)for(int di=0;di<4;++di)for(int si=0;si<4;++si)for(int ai=0;ai<2;++ai){gPresent=present!=0;gNode.Blocked=blocked!=0;gSame=same!=0;gNodes=gMaps=gHeights=0;gLayer=-1;gUseNavigation=false;double len=std::sqrt(double(dirs[di].X)*dirs[di].X+double(dirs[di].Y)*dirs[di].Y);double x=pos.X+dirs[di].X/len*distances[si],y=pos.Y+dirs[di].Y/len*distances[si];C3DVector result=flyer.GetRandomNavigablePos(pos,distances[si],dirs[di],ai?0.5f:8.0f);bool success=present&&!blocked&&same;ok &= gNodes==1&&gLayer==0&&Near(gNodeAt.X,x)&&Near(gNodeAt.Y,y);ok &= gMaps==(present&&!blocked?1:0)&&gHeights==(success?1:0);if(present&&!blocked)ok &= gMapFrom.X==pos.X&&gMapFrom.Y==pos.Y&&Near(gMapTo.X,x)&&Near(gMapTo.Y,y);if(success)ok &= gUseNavigation&&Near(gHeightAt.X,x+2)&&Near(gHeightAt.Y,y-4)&&gHeightAt.Z==48&&Near(result.X,x)&&Near(result.Y,y)&&result.Z==56;else ok &= result.X==pos.X&&result.Y==pos.Y&&result.Z==pos.Z;++cases;}
ok &= cases==256;std::printf(ok?"00a198c0_TEST PASS\n":"00a198c0_TEST FAIL\n");return ok?0:1;}