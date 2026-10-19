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