#include <stdio.h>
#include <vector>
#include <math.h>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}float Dot(const C3DVector& o)const{return X*o.X+Y*o.Y+Z*o.Z;}};
namespace QuadricOptimiserInternals {
class CQuadric{public:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;void operator+=(const CQuadric&);double EvaluateError(const C3DVector&)const;};
class COptimisedVertex{public:C3DVector Normal;CQuadric PositionError;float Cost,Group;float GetGroupDifference(const COptimisedVertex& o)const{return(float)fabs(Group-o.Group);}};
class CSubContraction{public:unsigned long Index1,Index2;};
class COptimisedPrimitive{public:std::vector<COptimisedVertex> Vertices;struct Params{float GroupWeight,HighPenalty;}Parameters;float ComputeSubContractionCost(const CSubContraction&,const C3DVector&);};
static CQuadric expectedQuadric;static int called,badQuadric;static const C3DVector* expectedPosition;
double CQuadric::EvaluateError(const C3DVector& p)const{
 ++called;if(&p!=expectedPosition||AA!=expectedQuadric.AA||AB!=expectedQuadric.AB||AC!=expectedQuadric.AC||AD!=expectedQuadric.AD||BB!=expectedQuadric.BB||BC!=expectedQuadric.BC||BD!=expectedQuadric.BD||CC!=expectedQuadric.CC||CD!=expectedQuadric.CD||DD!=expectedQuadric.DD||Weight!=expectedQuadric.Weight)badQuadric=1;
 return AA+BB+CC+DD;
}
}
namespace QuadricOptimiserInternals {
void CQuadric::operator+=(const CQuadric& other){AA+=other.AA;AB+=other.AB;AC+=other.AC;AD+=other.AD;BB+=other.BB;BC+=other.BC;BD+=other.BD;CC+=other.CC;CD+=other.CD;DD+=other.DD;Weight+=other.Weight;}

float COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction& sub_contraction,const C3DVector& position){
 CQuadric quadric;
 float cost;
 const COptimisedVertex& vertex1=Vertices[sub_contraction.Index1];
 const COptimisedVertex& vertex2=Vertices[sub_contraction.Index2];
 quadric=vertex1.PositionError;
 quadric+=vertex2.PositionError;
 cost=(float)(quadric.EvaluateError(position)*0.5);
 cost=vertex1.GetGroupDifference(vertex2)*Parameters.GroupWeight+cost;
 cost+=(vertex1.Cost>vertex2.Cost)?vertex1.Cost:vertex2.Cost;
 if(vertex1.Normal.Dot(vertex2.Normal)<0.0f)cost+=Parameters.HighPenalty;
 return cost;
}
}

using namespace QuadricOptimiserInternals;
static void fill(CQuadric& q,double n){q.AA=n;q.AB=n+1;q.AC=n+2;q.AD=n+3;q.BB=n+4;q.BC=n+5;q.BD=n+6;q.CC=n+7;q.CD=n+8;q.DD=n+9;q.Weight=n+10;}
static CQuadric sum(const CQuadric&a,const CQuadric&b){CQuadric q;q.AA=a.AA+b.AA;q.AB=a.AB+b.AB;q.AC=a.AC+b.AC;q.AD=a.AD+b.AD;q.BB=a.BB+b.BB;q.BC=a.BC+b.BC;q.BD=a.BD+b.BD;q.CC=a.CC+b.CC;q.CD=a.CD+b.CD;q.DD=a.DD+b.DD;q.Weight=a.Weight+b.Weight;return q;}
int main(){
 const float costs[]={-4,-1,0,2};int cases=0;
 for(int pair=0;pair<3;++pair)for(int weight=0;weight<3;++weight)for(int penalty=0;penalty<3;++penalty)for(int a=0;a<4;++a)for(int b=0;b<4;++b)for(int normal=-1;normal<=1;++normal){
  COptimisedPrimitive row;row.Vertices.resize(2);fill(row.Vertices[0].PositionError,0.5);fill(row.Vertices[1].PositionError,1.5);row.Vertices[0].Cost=costs[a];row.Vertices[1].Cost=costs[b];row.Vertices[0].Group=0.25f;row.Vertices[1].Group=0.75f;row.Vertices[0].Normal=C3DVector(1,0,0);row.Vertices[1].Normal=C3DVector((float)normal,0,0);row.Parameters.GroupWeight=weight*0.25f;row.Parameters.HighPenalty=penalty*4.0f;
  CSubContraction sub;sub.Index1=pair==1?1:0;sub.Index2=pair==2?0:1;const COptimisedVertex& x=row.Vertices[sub.Index1];const COptimisedVertex& y=row.Vertices[sub.Index2];expectedQuadric=sum(x.PositionError,y.PositionError);C3DVector position(2,3,4);expectedPosition=&position;called=badQuadric=0;
  float got=row.ComputeSubContractionCost(sub,position);float expected=(float)((expectedQuadric.AA+expectedQuadric.BB+expectedQuadric.CC+expectedQuadric.DD)*0.5);expected+=(float)fabs(x.Group-y.Group)*row.Parameters.GroupWeight;expected+=(x.Cost>y.Cost)?x.Cost:y.Cost;if(x.Normal.Dot(y.Normal)<0)expected+=row.Parameters.HighPenalty;
  if(got!=expected||called!=1||badQuadric)return 1;++cases;
 }
 if(cases!=1296)return 2;puts("00ac82a0_TEST PASS");return 0;
}