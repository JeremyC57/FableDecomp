// Row dual-build test: CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSectionsSIMD 0x00B98E10 (retail_asm: the original
// hand-SSE kernel is the shared macro FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_CLIPSUBSECTIONSSIMD in
// engine/CEnginePrimitiveManagerRepeatedStaticMeshesClipSubSectionsSIMD.h; built as-is = the asm path, /DFABLE_PORTABLE = its C++ twin).
// tools/decomp_pipeline/portable_twins_check.py builds it both ways and requires identical output: the full per-case trace of
// 7680 recursive clip cases (finite and raw-IEEE inputs, three tree shapes, ten flag sets). Derived from the row's behaviour test
// (Codex pass 22), which carries the production body verbatim over a layout fixture; the trace goes to stdout here.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
// ---- test fixture (Codex pass 22 rows/00b98e10/ClipSimdFixture.h, inlined) ----
#ifndef CLIP_SIMD_FIXTURE_H
#define CLIP_SIMD_FIXTURE_H
struct C4DVector {float X,Y,Z,W;};struct CRGBFloatColour {float R,G,B,A;};
namespace NEnginePrimitiveManagerRepeatedStaticMeshes {struct CSubsectionElement {float BoundingSphereCenterX[4],BoundingSphereCenterY[4],BoundingSphereCenterZ[4],BoundingSphereRadius[4];unsigned char ObjectCount[4],StartIndex[4],SubSectionOffset[4],Pad[4];};}
class CEnginePrimitiveManagerRepeatedStaticMeshes {public:
    struct CClipListElement {unsigned char Start,Size;};
    struct CSIMDVariableBlock {
        float NormalX[32];
        float NormalY[32];
        float NormalZ[32];
        float PosX[32];
        float PosY[32];
        float PosZ[32];
        C4DVector ObjectMatricies[32];
        C4DVector ObjectOffsets[32];
        CRGBFloatColour Colour[32];
        CRGBFloatColour MainLightColour[32];
        float CameraPosX[4];
        float CameraPosY[4];
        float CameraPosZ[4];
        float CameraClipPlaneLeftX[4];
        float CameraClipPlaneLeftY[4];
        float CameraClipPlaneLeftZ[4];
        float CameraClipPlaneRightX[4];
        float CameraClipPlaneRightY[4];
        float CameraClipPlaneRightZ[4];
        float CameraClipPlaneTopX[4];
        float CameraClipPlaneTopY[4];
        float CameraClipPlaneTopZ[4];
        float CameraClipPlaneBottomX[4];
        float CameraClipPlaneBottomY[4];
        float CameraClipPlaneBottomZ[4];
        float FadeDistance[4];
    };
    unsigned char Prefix[0x90];
    CSIMDVariableBlock* SIMDVariableBlock;
    void ClipSubSectionsSIMD(const NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement*,unsigned long,CClipListElement*&);
};
#endif

// ---- end fixture ----
#define FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshesClipSubSectionsSIMD.h"
#ifdef _MSC_VER
#define TEST_ALIGN16 __declspec(align(16))
#else
#define TEST_ALIGN16 __attribute__((aligned(16)))
#endif
static unsigned int gCase;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"case%u line%d %s\n",gCase,__LINE__,#x);std::exit(1);}}while(0)

void CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSectionsSIMD(const NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement* element,unsigned long clip_flags,CClipListElement*& clip_list)
{
    unsigned long reject_mask=0;
    if(!element->ObjectCount[0])reject_mask=1;
    if(!element->ObjectCount[1])reject_mask|=2;
    if(!element->ObjectCount[2])reject_mask|=4;
    if(!element->ObjectCount[3])reject_mask|=8;
    unsigned long sub_clip_flags=clip_flags;
    FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_CLIPSUBSECTIONSSIMD

    if(reject_mask!=15) {
        for(long i=0;i<4;++i) {
            if(!(reject_mask&(1UL<<i))) {
                unsigned long pass_mask=0x111110UL<<i;
                if(element->SubSectionOffset[i] && (sub_clip_flags&pass_mask)!=pass_mask)
                    ClipSubSectionsSIMD(element+element->SubSectionOffset[i],((sub_clip_flags>>i)&0x111110UL)*15,clip_list);
                else {
                    clip_list->Start=element->StartIndex[i];
                    clip_list->Size=element->ObjectCount[i];
                    ++clip_list;
                }
            }
        }
    }
}


typedef NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement Node;
typedef CEnginePrimitiveManagerRepeatedStaticMeshes Manager;
static TEST_ALIGN16 Node gNodes[85];static TEST_ALIGN16 Manager::CSIMDVariableBlock gVars;
static Manager::CClipListElement gActual[1024],gExpected[1024];static unsigned int gSeed=0x98241236U;
static unsigned int Next(){gSeed^=gSeed<<13;gSeed^=gSeed>>17;gSeed^=gSeed<<5;return gSeed;}
static float BitsFloat(unsigned int b){float f;std::memcpy(&f,&b,4);return f;}
static float Exact(){return float(int(Next()%129)-64)*0.25f;}
static float Fuzz(){unsigned int b=Next();switch(b&31){case 0:return BitsFloat(0x7fc00001U|(b&0x80000000U));case 1:return BitsFloat(0x7f800001U|(b&0x80000000U));case 2:return BitsFloat(0x7f800000U|(b&0x80000000U));case 3:return BitsFloat(b&0x80000000U);default:return BitsFloat(b);}}
static bool Sign(float f){unsigned int b;std::memcpy(&b,&f,4);return(b>>31)!=0;}
static void Oracle(const Node*n,bool skipRange,Manager::CClipListElement*&out){
 for(int i=0;i<4;++i){if(!n->ObjectCount[i])continue;float x=n->BoundingSphereCenterX[i]-gVars.CameraPosX[i],y=n->BoundingSphereCenterY[i]-gVars.CameraPosY[i],z=n->BoundingSphereCenterZ[i]-gVars.CameraPosZ[i],r=n->BoundingSphereRadius[i];float d=(x*x+y*y)+z*z;float o=gVars.FadeDistance[i]+r;if(!skipRange&&Sign(o*o-d))continue;
  float dots[4]={(x*gVars.CameraClipPlaneLeftX[i]+y*gVars.CameraClipPlaneLeftY[i])+z*gVars.CameraClipPlaneLeftZ[i],(x*gVars.CameraClipPlaneRightX[i]+y*gVars.CameraClipPlaneRightY[i])+z*gVars.CameraClipPlaneRightZ[i],(x*gVars.CameraClipPlaneTopX[i]+y*gVars.CameraClipPlaneTopY[i])+z*gVars.CameraClipPlaneTopZ[i],(x*gVars.CameraClipPlaneBottomX[i]+y*gVars.CameraClipPlaneBottomY[i])+z*gVars.CameraClipPlaneBottomZ[i]};bool reject=false;for(int p=0;p<4;++p)if(Sign(r-dots[p]))reject=true;if(reject)continue;
  // Earlier pass nibbles are destroyed by the real SIMD block; a child never meets its all-five-groups shortcut.
  if(n->SubSectionOffset[i])Oracle(n+n->SubSectionOffset[i],false,out);else{out->Start=n->StartIndex[i];out->Size=n->ObjectCount[i];++out;}
 }
}
static void Fill(bool fuzz,int shape){
 std::memset(gNodes,0,sizeof(gNodes));std::memset(&gVars,0,sizeof(gVars));
 for(int i=0;i<4;++i){gVars.CameraPosX[i]=fuzz?Fuzz():Exact();gVars.CameraPosY[i]=fuzz?Fuzz():Exact();gVars.CameraPosZ[i]=fuzz?Fuzz():Exact();gVars.FadeDistance[i]=fuzz?Fuzz():float(Next()%65)*0.5f;
  float*planes[12]={gVars.CameraClipPlaneLeftX,gVars.CameraClipPlaneLeftY,gVars.CameraClipPlaneLeftZ,gVars.CameraClipPlaneRightX,gVars.CameraClipPlaneRightY,gVars.CameraClipPlaneRightZ,gVars.CameraClipPlaneTopX,gVars.CameraClipPlaneTopY,gVars.CameraClipPlaneTopZ,gVars.CameraClipPlaneBottomX,gVars.CameraClipPlaneBottomY,gVars.CameraClipPlaneBottomZ};for(int p=0;p<12;++p)planes[p][i]=fuzz?Fuzz():float(int(Next()%9)-4)*0.5f;
 }
 for(int k=0;k<21;++k)for(int i=0;i<4;++i){Node&n=gNodes[k];n.BoundingSphereCenterX[i]=fuzz?Fuzz():Exact();n.BoundingSphereCenterY[i]=fuzz?Fuzz():Exact();n.BoundingSphereCenterZ[i]=fuzz?Fuzz():Exact();n.BoundingSphereRadius[i]=fuzz?Fuzz():float(Next()%65)*0.5f;n.ObjectCount[i]=(unsigned char)(Next()%17);n.StartIndex[i]=(unsigned char)Next();if(k<5&&shape){int child=k==0?i+1:5+(k-1)*4+i;bool link=shape==2||(Next()&1)!=0;n.SubSectionOffset[i]=link?(unsigned char)(child-k):0;}}
}
static void One(std::FILE*f,bool fuzz,int shape,unsigned long flags){
 ++gCase;Fill(fuzz,shape);Manager m;std::memset(m.Prefix,0x5a,sizeof(m.Prefix));m.SIMDVariableBlock=&gVars;std::memset(gActual,0xa5,sizeof(gActual));std::memset(gExpected,0x6c,sizeof(gExpected));Manager::CClipListElement*out=gActual+3;Node before[85];std::memcpy(before,gNodes,sizeof(gNodes));m.ClipSubSectionsSIMD(gNodes,flags,out);CHECK(out>=gActual+3&&out<=gActual+3+64);unsigned n=unsigned(out-(gActual+3));CHECK(std::memcmp(before,gNodes,sizeof(gNodes))==0);CHECK(gActual[2].Start==0xa5&&gActual[2].Size==0xa5);CHECK(out->Start==0xa5&&out->Size==0xa5);for(unsigned i=0;i<sizeof(m.Prefix);++i)CHECK(m.Prefix[i]==0x5a);CHECK(m.SIMDVariableBlock==&gVars);
 if(!fuzz){Manager::CClipListElement*expected=gExpected;Oracle(gNodes,(flags&0xf0)!=0,expected);CHECK(unsigned(expected-gExpected)==n);CHECK(std::memcmp(gExpected,gActual+3,n*2)==0);}
 std::fprintf(f,"%u %u",gCase,n);for(unsigned i=0;i<n;++i)std::fprintf(f," %u,%u",unsigned(gActual[3+i].Start),unsigned(gActual[3+i].Size));std::fputc('\n',f);
}
int main(int argc,char**argv){(void)argc;(void)argv;std::FILE*f=stdout;CHECK(f!=0);unsigned long flags[]={0,0xf0,0xffffff,0x111110,0x888880,0xf00,0xf000,0xf0000,0xf00000,0x80000000UL};for(int fuzz=0;fuzz<2;++fuzz)for(int shape=0;shape<3;++shape)for(int which=0;which<10;++which)for(int i=0;i<128;++i)One(f,fuzz!=0,shape,flags[which]);std::fflush(f);CHECK(gCase==7680);std::puts("00b98e10_TEST PASS");return 0;}
