// 00bd6810 RenderDebugBoneHierarchy behavior test. The submitted function body is reproduced below.
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_DEBUG_BONE_PASS29
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CMatrix3x4.h"

namespace Fixture {
static void check(bool ok) { if (!ok) abort(); }
enum { limit = 6 };
static int phase, draws, restores;
struct CRenderStateScopedCaptureBlock {
    CRenderStateScopedCaptureBlock() { check(phase==0); phase=1; }
    ~CRenderStateScopedCaptureBlock() { check(phase==2); phase=0; ++restores; }
};
struct C3DBone2 {
    long parent, children;
    long GetParent() const { return parent; }
    long GetOriginalNoChildren() const { return children; }
};
struct C3DMesh2 {
    C3DBone2 bones[limit];
    unsigned long count;
    unsigned long GetBoneCount() const { return count; }
    const C3DBone2* GetBone(unsigned long i) const { check(i<count); return &bones[i]; }
};
struct CEngineRenderInfoAnimatedMesh {
    C3DMesh2* Mesh;
    CMatrix3x4 ObjectTransform;
};
template<class T> class CArray {
    T contents[limit]; unsigned long count;
public:
    explicit CArray(unsigned long n) : count(n) { check(n<=limit); }
    T& operator[](unsigned long index) { check(index<count); return contents[index]; }
    T* GetAsCArray() { return count ? contents : 0; }
};
struct ColourValue {
    int r,g,b,a;
    void Initialise(int R,int G,int B,int A) { r=R; g=G; b=B; a=A; }
};
struct CEnginePrimitiveLine {
    C3DVector Start, End;
    ColourValue Colour;
    float Width; bool UseZBuffer; int BankIndex;
    CEnginePrimitiveLine() : Width(-1), UseZBuffer(true), BankIndex(-1) { Colour.Initialise(0,0,0,0); }
};
struct Renderer {
    CEnginePrimitiveLine lines[limit];
    void AddPrimitive(const CEnginePrimitiveLine& line) { check(phase==2 && draws<limit); lines[draws++]=line; }
};
namespace NEngineInternal { static Renderer* GPPrimitiveRenderer; }
struct Blend { int id; };
struct AnimationManager {
    const Blend *start,*end;
    const C3DMesh2* expectedMesh;
    const C3DVector* expectedScale;
    float expectedBlend;
    CMatrix3x4 source[limit];
    void CalcBlendedMeshTransformations(const Blend& a,const Blend& b,float v,const C3DVector* scales,
                                        const C3DMesh2& mesh,CMatrix3x4* dest,unsigned long n) {
        check(phase==1 && &a==start && &b==end && expectedBlend==v);
        check(&mesh==expectedMesh && scales==expectedScale && n==mesh.count);
        check((dest!=0)==(n!=0)); for(unsigned long i=0;i<n;++i) dest[i]=source[i]; phase=2;
    }
};
struct MeshInformation {
    AnimationManager manager;
    AnimationManager& GetAnimationManager() { return manager; }
};
struct CEngineInternalPrimitiveAnimatedMesh {
    CEngineRenderInfoAnimatedMesh info;
    MeshInformation* MeshInfo;
    struct { Blend StartState, EndState; } Anim;
    const C3DVector* scale;
    float blend;
    const CEngineRenderInfoAnimatedMesh* PeekRenderInfo() const { return &info; }
    float GetCurrentInterpolate() const { return blend; }
    const C3DVector* GetBoneScalings(const C3DMesh2& m) const { check(&m==info.Mesh); return scale; }
};
class CEnginePrimitiveManagerAnimatedMeshesBase {
public: void RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh&);
};

void CEnginePrimitiveManagerAnimatedMeshesBase::RenderDebugBoneHierarchy(CEngineInternalPrimitiveAnimatedMesh& primitive)
{
    CRenderStateScopedCaptureBlock capture_block;
    const CEngineRenderInfoAnimatedMesh* render_info=primitive.PeekRenderInfo();
    const C3DMesh2& mesh=*render_info->Mesh;
    unsigned long no_bones=mesh.GetBoneCount();
    CArray<CMatrix3x4> bone_transforms(no_bones);
    float interpolate=primitive.GetCurrentInterpolate();
    const C3DVector* bone_morphs=primitive.GetBoneScalings(mesh);
    primitive.MeshInfo->GetAnimationManager().CalcBlendedMeshTransformations(primitive.Anim.StartState,primitive.Anim.EndState,interpolate,bone_morphs,mesh,bone_transforms.GetAsCArray(),no_bones);
    bool visible=false;
    for(unsigned long c_bone=0;c_bone<no_bones;++c_bone) {
        const C3DBone2* bone=mesh.GetBone(c_bone);
        long parent_index=bone->GetParent();
        if(parent_index!=-1 && visible) {
            CMatrix3x4 parent_transform=bone_transforms[parent_index];
            CMatrix3x4 child_transform=bone_transforms[c_bone];
            parent_transform.PostMultiply(render_info->ObjectTransform);
            child_transform.PostMultiply(render_info->ObjectTransform);
            CEnginePrimitiveLine line;
            line.Start=parent_transform.GetTranslation();
            line.End=child_transform.GetTranslation();
            line.Colour.Initialise(255,255,255,255);
            line.Width=0.02f;
            line.UseZBuffer=false;
            line.BankIndex=0;
            NEngineInternal::GPPrimitiveRenderer->AddPrimitive(line);
        }
        if(bone->GetOriginalNoChildren()>1) visible=true;
    }
}


static void fill(CMatrix3x4& m,int k) {
    m.E11=1; m.E12=2; m.E13=-1;
    m.E21=-2; m.E22=1; m.E23=3;
    m.E31=3; m.E32=-1; m.E33=2;
    m.E41=float(k-2); m.E42=float(3*k+1); m.E43=float(2-k*k);
}
static void point(const C3DVector& v,int k,const CMatrix3x4& world) {
    int x=k-2,y=3*k+1,z=2-k*k;
    check(v.X==x-2*y+3*z+world.E41);
    check(v.Y==2*x+y-z+world.E42);
    check(v.Z==-x+3*y+2*z+world.E43);
}
static void run(unsigned long count,int branchMask,int parentMask,int mode) {
    C3DMesh2 mesh; mesh.count=count;
    Renderer renderer; NEngineInternal::GPPrimitiveRenderer=&renderer;
    MeshInformation info; AnimationManager& manager=info.manager;
    CEngineInternalPrimitiveAnimatedMesh primitive; C3DVector scale(1,2,3);
    primitive.info.Mesh=&mesh; primitive.MeshInfo=&info; primitive.scale=(mode&1)?&scale:0;
    primitive.blend=float(mode)*0.25f; fill(primitive.info.ObjectTransform,mode+2);
    manager.start=&primitive.Anim.StartState; manager.end=&primitive.Anim.EndState;
    manager.expectedScale=primitive.scale; manager.expectedMesh=&mesh; manager.expectedBlend=primitive.blend;
    int firstBranch=limit;
    for(unsigned long i=0;i<count;++i) {
        mesh.bones[i].children=(branchMask&(1<<i))?2+(i%3):(i%2);
        mesh.bones[i].parent=(i==0 || (parentMask&(1<<i)))?-1:long((i-1)/(1+(mode&1)));
        if(mesh.bones[i].children>1 && firstBranch==limit) firstBranch=int(i);
        fill(manager.source[i],int(i)+mode);
    }
    draws=0; restores=0;
    CEnginePrimitiveManagerAnimatedMeshesBase subject; subject.RenderDebugBoneHierarchy(primitive);
    check(phase==0 && restores==1);
    int wanted=0;
    for(int child=firstBranch+1;child<int(count);++child) {
        long parent=mesh.bones[child].parent; if(parent<0) continue;
        check(wanted<draws); const CEnginePrimitiveLine& line=renderer.lines[wanted++];
        point(line.Start,int(parent)+mode,primitive.info.ObjectTransform);
        point(line.End,child+mode,primitive.info.ObjectTransform);
        check(line.Width==0.02f && line.BankIndex==0 && !line.UseZBuffer);
        check(line.Colour.r==255 && line.Colour.g==255 && line.Colour.b==255 && line.Colour.a==255);
    }
    check(draws==wanted);
}
}
int main() {
    for(int n=0;n<=Fixture::limit;++n)
        for(int branch=0;branch<(1<<n);++branch)
            for(int roots=0;roots<(1<<n);roots+=2)
                for(int mode=0;mode<4;++mode) Fixture::run(n,branch,roots,mode);
    puts("00bd6810_TEST PASS"); return 0;
}