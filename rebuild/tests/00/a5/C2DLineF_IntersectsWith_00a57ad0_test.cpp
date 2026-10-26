// C2DLineF::IntersectsWith(C2DBoxI*) -- convert integer box corners to float edges and test each using the shared segment intersection helper
class C2DVector { public: float x; float y; };
class C2DBoxI { public: long minX; long minY; long maxX; long maxY; };
class C2DLineF { public:
    float x1; float y1; float x2; float y2;
    C2DLineF(float ax,float ay,float bx,float by):x1(ax),y1(ay),x2(bx),y2(by) {}
    bool IntersectsWith(const C2DLineF* other, C2DVector* point, float tolerance); // 0x00A56C80
    int IntersectsWith(const C2DBoxI* box);
};
extern float g_C2DIntersectionTolerance; // 0x0129BA3C
int C2DLineF::IntersectsWith(const C2DBoxI* box)
{
    C2DVector point;
    C2DLineF bottom((float)box->minX, (float)box->minY, (float)box->maxX, (float)box->minY);
    C2DLineF right((float)box->maxX, (float)box->minY, (float)box->maxX, (float)box->maxY);
    C2DLineF top((float)box->minX, (float)box->maxY, (float)box->maxX, (float)box->maxY);
    C2DLineF left((float)box->minX, (float)box->minY, (float)box->minX, (float)box->maxY);
    if (IntersectsWith(&bottom, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&right, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&top, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&left, &point, g_C2DIntersectionTolerance)) return true;
    return false;
}

#include <cstdio>
static int errors,calls,hitAt;
static C2DBoxI* inputBox;
static C2DLineF* inputLine;
static C2DVector* outputPoint;
static float expectedEdges[4][4];
float g_C2DIntersectionTolerance=0.25f;
bool C2DLineF::IntersectsWith(const C2DLineF* other,C2DVector* point,float tolerance){
 if(this!=inputLine||calls>=4)++errors;
 if(calls<4){
  if(other->x1!=expectedEdges[calls][0]||other->y1!=expectedEdges[calls][1]||other->x2!=expectedEdges[calls][2]||other->y2!=expectedEdges[calls][3])++errors;
  if(tolerance!=0.25f*(calls+1))++errors;
 }
 if(calls==0)outputPoint=point;else if(point!=outputPoint)++errors;
 point->x=42;point->y=-37;
 // All four edges must already be converted before the first dependency call.
 inputBox->minX=700;inputBox->minY=701;inputBox->maxX=702;inputBox->maxY=703;
 ++calls;g_C2DIntersectionTolerance+=0.25f;
 return calls==hitAt;
}
int main(){
 C2DLineF line(-10,-5,20,30);inputLine=&line;
 const long boxes[3][4]={{-7,-11,13,17},{16777217,-16777217,16777220,-16777220},{4,4,4,4}};
 const float corners[3][4]={{-7,-11,13,17},{16777216,-16777216,16777220,-16777220},{4,4,4,4}};
 for(int which=0;which<3;++which){
  float x0=corners[which][0],y0=corners[which][1],x1=corners[which][2],y1=corners[which][3];
  float want[4][4]={{x0,y0,x1,y0},{x1,y0,x1,y1},{x0,y1,x1,y1},{x0,y0,x0,y1}};
  for(int row=0;row<4;++row)for(int col=0;col<4;++col)expectedEdges[row][col]=want[row][col];
  for(hitAt=0;hitAt<=4;++hitAt){
   C2DBoxI box={boxes[which][0],boxes[which][1],boxes[which][2],boxes[which][3]};inputBox=&box;
   calls=0;outputPoint=0;g_C2DIntersectionTolerance=0.25f;
   int result=line.IntersectsWith(&box);
   if(result!=(hitAt!=0)||calls!=(hitAt?hitAt:4)||line.x1!=-10||line.y1!=-5||line.x2!=20||line.y2!=30)++errors;
  }
 }
 if(errors){std::printf("00a57ad0_TEST FAIL %d\n",errors);return 1;}
 std::printf("00a57ad0_TEST PASS\n");return 0;
}