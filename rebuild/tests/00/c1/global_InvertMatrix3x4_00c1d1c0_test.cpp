// Selected body tested against independent two-sided inverse identities and determinant-floor cases.
#include <math.h>
#include <cstdio>
#include <cassert>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
class CMatrix3x4 {public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;};
float g_fMatrixInverseEpsilon=0.0625f;
void FABLE_FASTCALL InvertMatrix3x4(CMatrix3x4* out, const CMatrix3x4& m)
{
    float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
    if (fabs(det) < g_fMatrixInverseEpsilon)
        det = (det < 0.0f) ? -g_fMatrixInverseEpsilon : g_fMatrixInverseEpsilon;
    float inv = 1.0f / det;
    out->E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
    out->E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
    out->E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    out->E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
    out->E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
    out->E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    const float& x=m.E41;
    out->E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
    out->E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
    out->E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    float t = m.E43 * out->E31;
    t += x * out->E11;
    t += m.E42 * out->E21;
    out->E41 = -t;
    t = m.E42 * out->E22;
    t += x * out->E12;
    t += out->E32 * m.E43;
    out->E42 = -t;
    t = m.E43 * out->E33;
    t += m.E42 * out->E23;
    t += out->E13 * x;
    out->E43 = -t;
}

static void Set(CMatrix3x4& m,const float* a){m.E11=a[0];m.E12=a[1];m.E13=a[2];m.E21=a[3];m.E22=a[4];m.E23=a[5];m.E31=a[6];m.E32=a[7];m.E33=a[8];m.E41=a[9];m.E42=a[10];m.E43=a[11];}
static void Get(const CMatrix3x4& m,float* a){a[0]=m.E11;a[1]=m.E12;a[2]=m.E13;a[3]=m.E21;a[4]=m.E22;a[5]=m.E23;a[6]=m.E31;a[7]=m.E32;a[8]=m.E33;a[9]=m.E41;a[10]=m.E42;a[11]=m.E43;}
int main(){
 int invertible=0;
 for(int pattern=0;pattern<19683;++pattern){
  float a[12],b[12],after[12];int n=pattern;
  for(int i=0;i<9;++i){a[i]=float(n%3-1);n/=3;}
  a[9]=-2.0f;a[10]=0.5f;a[11]=4.0f;
  double det=double(a[0])*(a[4]*a[8]-a[5]*a[7])-double(a[1])*(a[3]*a[8]-a[5]*a[6])+double(a[2])*(a[3]*a[7]-a[4]*a[6]);
  if(det==0.0)continue;
  ++invertible;CMatrix3x4 input,output;Set(input,a);InvertMatrix3x4(&output,input);Get(output,b);Get(input,after);
  for(int i=0;i<12;++i)assert(after[i]==a[i]);
  for(int row=0;row<3;++row)for(int col=0;col<3;++col){
   double left=0.0,right=0.0;for(int k=0;k<3;++k){left+=double(a[row*3+k])*b[k*3+col];right+=double(b[row*3+k])*a[k*3+col];}
   const double expected=row==col?1.0:0.0;assert(fabs(left-expected)<0.00001&&fabs(right-expected)<0.00001);
  }
  for(int col=0;col<3;++col){double t=b[9+col];for(int k=0;k<3;++k)t+=double(a[9+k])*b[k*3+col];assert(fabs(t)<0.00001);}
 }
 assert(invertible>10000);
 for(int sign=-1;sign<=1;++sign){
  float a[12]={0},b[12];a[0]=float(sign)*0.125f;a[4]=0.125f;a[8]=1.0f;a[9]=2.0f;a[10]=-4.0f;a[11]=0.5f;
  CMatrix3x4 input,output;Set(input,a);InvertMatrix3x4(&output,input);Get(output,b);
  if(sign){assert(b[0]==float(sign)*2.0f&&b[4]==2.0f&&b[8]==0.25f);assert(b[9]==-float(sign)*4.0f&&b[10]==8.0f&&b[11]==-0.125f);}
  else{assert(b[0]==2.0f&&b[4]==0.0f&&b[8]==0.0f&&b[9]==-4.0f&&b[10]==0.0f&&b[11]==0.0f);}
 }
 std::puts("00c1d1c0_TEST PASS");return 0;
}