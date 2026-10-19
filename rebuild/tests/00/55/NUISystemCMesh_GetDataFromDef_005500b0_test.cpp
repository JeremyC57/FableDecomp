#include <cstdio>
// NUISystem::CMesh::GetDataFromDef() -- copy mesh rendering properties from its referenced UI definition
// Leaf: GetDataFromDef
// Retail005500B0,210 bytes ending ret00550181;14 following int3 bytes excluded.
// Propagated CScrollingViewport owner is wrong: callers00550190/005501E0 install CMesh
// RTTI primary01249F9C, then call this. No CMesh/CUIDef/vector shared headers exist.
// Donor CMesh fields+18c shift48 bytes to retail+15c; CUIDef vectors shrink from16 to12.
// GetDef virtual108 returns one-pointer CDefPointer; final refcount+4 invokes Destroy slot1.
namespace NUISystem {
struct C3DVector { float X,Y,Z; C3DVector() {} C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z) {} };
struct CRightHandedSet { C3DVector Up,Forward; CRightHandedSet() {} void Set(const C3DVector& up,const C3DVector& forward) {Up=up;Forward=forward;} };
struct CUIDefView {
    virtual ~CUIDefView() {}
    virtual void Destroy();
    long References;
    unsigned char BeforeMesh[0x4c-8];
    unsigned long MeshIndex;
    unsigned char FontAndText[8];
    float Height,Width;
    unsigned char BeforeMeshType[0xa0-0x60];
    unsigned long MeshType;
    unsigned char BeforeOrientation[0x104-0xa4];
    C3DVector Up,Forward,RotationAxis;
    float RotationSpeed;
    unsigned long AnimationIndex;
    long RemoveReference() { return --References; }
    void ReleaseTemporary() { if(RemoveReference()==0) Destroy(); }
};
struct CUIDefPointer {
    CUIDefView* Object;
    CUIDefPointer() {}
    CUIDefPointer(const CUIDefPointer& p):Object(p.Object) {if(Object) ++Object->References;}
    ~CUIDefPointer() {if(Object) Object->ReleaseTemporary();}
    CUIDefView* operator->() const {return Object;}
};
struct CMesh {
    virtual ~CMesh() {}
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual CUIDefPointer GetDef() const; //slot108
    unsigned char BaseState[0x15c-4];
    unsigned long MeshIndex,MeshType;
    unsigned char Handle[8];
    unsigned long Height,Width;
    CRightHandedSet Orientation;
    C3DVector RotationAxis;
    float RotationSpeed;
    unsigned long AnimationIndex;
    unsigned char NoRenderNextFrame;
    void GetDataFromDef();
};
void CMesh::GetDataFromDef() {
    CUIDefPointer def=GetDef();
    MeshIndex=def->MeshIndex;
    MeshType=def->MeshType;
    Height=(unsigned long)def->Height;
    Width=(unsigned long)def->Width;
    Orientation.Set(def->Up,def->Forward);
    RotationAxis=def->RotationAxis;
    RotationSpeed=def->RotationSpeed;
    AnimationIndex=def->AnimationIndex;
}
}

namespace NUISystem {
static CMesh* active;
static CUIDefView* definition;
static int bad,gets,destroys;
static long startingReferences;
static unsigned long wantedHeight,wantedWidth;
static int copied() {
    return active->MeshIndex==definition->MeshIndex && active->MeshType==definition->MeshType &&
      active->Height==wantedHeight && active->Width==wantedWidth &&
      active->Orientation.Up.X==definition->Up.X && active->Orientation.Up.Y==definition->Up.Y && active->Orientation.Up.Z==definition->Up.Z &&
      active->Orientation.Forward.X==definition->Forward.X && active->Orientation.Forward.Y==definition->Forward.Y && active->Orientation.Forward.Z==definition->Forward.Z &&
      active->RotationAxis.X==definition->RotationAxis.X && active->RotationAxis.Y==definition->RotationAxis.Y && active->RotationAxis.Z==definition->RotationAxis.Z &&
      active->RotationSpeed==definition->RotationSpeed && active->AnimationIndex==definition->AnimationIndex;
}
void CUIDefView::Destroy() {
    ++destroys;if(this!=definition || References || gets!=1 || !copied()) bad=1;
}
void CMesh::v001() {}
void CMesh::v002() {}
void CMesh::v003() {}
void CMesh::v004() {}
void CMesh::v005() {}
void CMesh::v006() {}
void CMesh::v007() {}
void CMesh::v008() {}
void CMesh::v009() {}
void CMesh::v010() {}
void CMesh::v011() {}
void CMesh::v012() {}
void CMesh::v013() {}
void CMesh::v014() {}
void CMesh::v015() {}
void CMesh::v016() {}
void CMesh::v017() {}
void CMesh::v018() {}
void CMesh::v019() {}
void CMesh::v020() {}
void CMesh::v021() {}
void CMesh::v022() {}
void CMesh::v023() {}
void CMesh::v024() {}
void CMesh::v025() {}
void CMesh::v026() {}
void CMesh::v027() {}
void CMesh::v028() {}
void CMesh::v029() {}
void CMesh::v030() {}
void CMesh::v031() {}
void CMesh::v032() {}
void CMesh::v033() {}
void CMesh::v034() {}
void CMesh::v035() {}
void CMesh::v036() {}
void CMesh::v037() {}
void CMesh::v038() {}
void CMesh::v039() {}
void CMesh::v040() {}
void CMesh::v041() {}
void CMesh::v042() {}
void CMesh::v043() {}
void CMesh::v044() {}
void CMesh::v045() {}
void CMesh::v046() {}
void CMesh::v047() {}
void CMesh::v048() {}
void CMesh::v049() {}
void CMesh::v050() {}
void CMesh::v051() {}
void CMesh::v052() {}
void CMesh::v053() {}
void CMesh::v054() {}
void CMesh::v055() {}
void CMesh::v056() {}
void CMesh::v057() {}
void CMesh::v058() {}
void CMesh::v059() {}
void CMesh::v060() {}
void CMesh::v061() {}
void CMesh::v062() {}
void CMesh::v063() {}
void CMesh::v064() {}
void CMesh::v065() {}
void CMesh::v066() {}
void CMesh::v067() {}
void CMesh::v068() {}
void CMesh::v069() {}
void CMesh::v070() {}
void CMesh::v071() {}
void CMesh::v072() {}
void CMesh::v073() {}
void CMesh::v074() {}
void CMesh::v075() {}
void CMesh::v076() {}
void CMesh::v077() {}
void CMesh::v078() {}
void CMesh::v079() {}
void CMesh::v080() {}
void CMesh::v081() {}
void CMesh::v082() {}
void CMesh::v083() {}
void CMesh::v084() {}
void CMesh::v085() {}
void CMesh::v086() {}
void CMesh::v087() {}
void CMesh::v088() {}
void CMesh::v089() {}
void CMesh::v090() {}
void CMesh::v091() {}
void CMesh::v092() {}
void CMesh::v093() {}
void CMesh::v094() {}
void CMesh::v095() {}
void CMesh::v096() {}
void CMesh::v097() {}
void CMesh::v098() {}
void CMesh::v099() {}
void CMesh::v100() {}
void CMesh::v101() {}
void CMesh::v102() {}
void CMesh::v103() {}
void CMesh::v104() {}
void CMesh::v105() {}
void CMesh::v106() {}
void CMesh::v107() {}

CUIDefPointer CMesh::GetDef() const {
    if(this!=active || gets || definition->References!=startingReferences) bad=1;
    ++gets;CUIDefPointer p;p.Object=definition;return p;
}
static int run(long references,float height,float width,unsigned long h,unsigned long w) {
    CMesh mesh;CUIDefView def;active=&mesh;definition=&def;
    startingReferences=references;def.References=references;bad=gets=destroys=0;wantedHeight=h;wantedWidth=w;
    def.MeshIndex=0x1234;def.MeshType=7;def.Height=height;def.Width=width;
    def.Up.X=1.25f;def.Up.Y=-2.5f;def.Up.Z=3.75f;
    def.Forward.X=-4.5f;def.Forward.Y=5.25f;def.Forward.Z=6.125f;
    def.RotationAxis.X=-7.5f;def.RotationAxis.Y=8.25f;def.RotationAxis.Z=-9.125f;
    def.RotationSpeed=-0.75f;def.AnimationIndex=0x8765;
    for(unsigned int i=0;i<sizeof(mesh.BaseState);++i) mesh.BaseState[i]=73;
    for(unsigned int j=0;j<sizeof(mesh.Handle);++j) mesh.Handle[j]=91;
    mesh.NoRenderNextFrame=23;
    mesh.GetDataFromDef();
    if(bad || gets!=1 || destroys!=(references==1?1:0) || def.References!=references-1 || !copied() || mesh.NoRenderNextFrame!=23) return 0;
    for(unsigned int a=0;a<sizeof(mesh.BaseState);++a) if(mesh.BaseState[a]!=73) return 0;
    for(unsigned int b=0;b<sizeof(mesh.Handle);++b) if(mesh.Handle[b]!=91) return 0;
    return def.Height==height && def.Width==width;
}
}
int main() {
    using namespace NUISystem;
    if(!run(1,17.75f,28.25f,17,28) || !run(4,0.0f,0.999f,0,0) || !run(2,256.9f,1024.75f,256,1024) ||
      !run(3,1.5f,2.5f,1,2)) {printf("005500b0_TEST FAIL\n");return 1;}
    printf("005500b0_TEST PASS\n");return 0;
}