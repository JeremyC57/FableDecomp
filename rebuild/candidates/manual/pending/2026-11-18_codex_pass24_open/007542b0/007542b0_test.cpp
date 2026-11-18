#include <math.h>
#include <stdio.h>
#include <list>
#include <string>

struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator*(float k) const { return C3DVector(X*k,Y*k,Z*k); }
    C3DVector& operator+=(const C3DVector& v) { *this=*this+v;return *this; }
    C3DVector Cross(const C3DVector& o) const { return C3DVector(Y*o.Z-Z*o.Y,Z*o.X-X*o.Z,X*o.Y-Y*o.X); }
    float Dot(const C3DVector& o) const { return X*o.X+Y*o.Y+Z*o.Z; }
    void AccurateNormalise() { *this=*this*(1.0f/(float)sqrt(Dot(*this))); }
    C3DVector Rotated(const C3DVector& axis,float radians) const
    {
        float c=(float)cos(radians),s=(float)sin(radians);
        return *this*c+axis.Cross(*this)*s+axis*(axis.Dot(*this)*(1-c));
    }
    void Rotate(const C3DVector& axis,float turns) { *this=Rotated(axis,turns*6.2831853f); }
};
static bool Near(float a,float b) { return fabs(a-b)<0.0001f; }

typedef std::string CCharString;
const CCharString EYE_LEFT_DUMMY_OBJECT_NAME("left"),EYE_RIGHT_DUMMY_OBJECT_NAME("right");
struct CRightHandedSet {
    C3DVector Up,Forward;
    void Init(const C3DVector& up,const C3DVector& fwd) { Up=up;Forward=fwd; }
    const C3DVector& GetUp() const { return Up; }
    const C3DVector& GetForward() const { return Forward; }
    C3DVector GetRight() const { return Forward.Cross(Up); }
    void SetUp(const C3DVector& v) { Up=v; }
    void SetForward(const C3DVector& v) { Forward=v; }
};
struct CMatrix3x4 {
    float E11,E12,E13,E21,E22,E23,E31,E32,E33;
    void SetFromVectorAndAngle(const C3DVector& axis,float angle)
    {
        C3DVector row=C3DVector(1,0,0).Rotated(axis,angle);E11=row.X;E12=row.Y;E13=row.Z;
        row=C3DVector(0,1,0).Rotated(axis,angle);E21=row.X;E22=row.Y;E23=row.Z;
        row=C3DVector(0,0,1).Rotated(axis,angle);E31=row.X;E32=row.Y;E33=row.Z;
    }
};
enum { GAPT_STATIC_MESH = 1 };
namespace NGraphicAppearance {
struct CPrimitive {
    int Type,Index;
    CPrimitive(int type=0,int index=0):Type(type),Index(index) {}
};
struct CEngineGraphic { int BankIndex;float RenderSizeX; };
struct CGraphicAppearanceDrawInfo { unsigned long ReasonsForDraw;bool TextureMorphsUsed;const int* PTextureMorphs; };
struct CEnginePrimitiveStaticMesh {
    int MeshIndex,TintColour,MeshEffect,ThemeChannelDesc;
    C3DVector WorldPos;
    float Size,FadeStart,Alpha;
    bool Lit;
    CRightHandedSet RightHandedSet;
};
struct CEnginePrimitiveMorphedStaticMesh : CEnginePrimitiveStaticMesh { int TextureMorphs; };
struct CEnginePrimitiveAttachmentInfo {
    int Flags,ParentDummyIndex,ChildDummyIndex;
    CCharString ParentDummyName,ChildDummyName;
};
struct CTCGraphicAppearance {
    bool Left,Right,Invalid;
    int LightingChannel;
    unsigned char Alpha;
    unsigned Adds,Attaches;
    CPrimitive Dummy,Primitives[2];
    CRightHandedSet LastSet;
    CTCGraphicAppearance():Left(true),Right(true),Invalid(false),LightingChannel(5),Alpha(127),Adds(0),Attaches(0) {}
    const CPrimitive* GetFirstPrimitiveWithDummyObject(const CCharString& name) const
    { return (name=="left"?Left:Right)?&Dummy:0; }
    unsigned char GetAlpha() const { return Alpha; }
    int PeekLightingChannel() const { return LightingChannel; }
    float GetFadeDistance() const { return 40; }
    int GetTintColour() const { return 6; }
    int GetPMeshEffect() const { return 7; }
    CPrimitive* AddPersistentPrimitive(const CPrimitive& primitive)
    {
        Primitives[Adds]=primitive;
        return &Primitives[Adds++];
    }
    void SetAttachedPrimitiveGraphics(CPrimitive* primitive,const CEnginePrimitiveStaticMesh& mesh,
        const CEnginePrimitiveAttachmentInfo& attachment,const CPrimitive* dummy)
    {
        ++Attaches;
        LastSet=mesh.RightHandedSet;
        Invalid=Invalid||primitive->Index!=17||primitive->Type!=1||mesh.MeshIndex!=17||mesh.Size!=6.0f||
            mesh.WorldPos.X||mesh.WorldPos.Y||mesh.WorldPos.Z||mesh.FadeStart!=40||!mesh.Lit||mesh.Alpha!=127||
            mesh.TintColour!=6||mesh.MeshEffect!=7||mesh.ThemeChannelDesc!=5||attachment.Flags!=4||
            attachment.ParentDummyIndex||attachment.ChildDummyIndex||!attachment.ChildDummyName.empty()||
            (attachment.ParentDummyName!="left"&&attachment.ParentDummyName!="right")||dummy!=&Dummy;
    }
};
}
using namespace NGraphicAppearance;
struct CThingCreatureBase {
    CEngineGraphic Graphic;
    bool Dead;
    float Scale;
    CThingCreatureBase():Dead(false),Scale(2) { Graphic.BankIndex=17;Graphic.RenderSizeX=3; }
    float GetScale() const { return Scale; }
    bool IsDead() const { return Dead; }
    const CEngineGraphic& GetEyeGraphic() const { return Graphic; }
};
struct CLookDef { bool DrawEyePrimitives;CLookDef():DrawEyePrimitives(true) {} };
struct CTCLook {
    CThingCreatureBase* PParentCreature;
    CLookDef* PDef;
    float EyeAnglesRelToHeadXY,EyeAnglesRelToHeadYZ;
    bool EyeAnglesChanged;
    CPrimitive* LeftEyePrimitive;
    CPrimitive* RightEyePrimitive;
    CTCLook(CThingCreatureBase& creature,CLookDef& def):PParentCreature(&creature),PDef(&def),
        EyeAnglesRelToHeadXY(0),EyeAnglesRelToHeadYZ(0),EyeAnglesChanged(false),LeftEyePrimitive(0),RightEyePrimitive(0) {}
    bool OnAppearanceDraw(CTCGraphicAppearance&,const CGraphicAppearanceDrawInfo&);
};

bool CTCLook::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& info)
{
    if (!PDef->DrawEyePrimitives)
        return true;
    if (!((info.ReasonsForDraw | 8) || (info.ReasonsForDraw | 4) || !LeftEyePrimitive || !RightEyePrimitive || EyeAnglesChanged))
        return true;
    if (PParentCreature && !PParentCreature->IsDead()) {
        const CEngineGraphic& eyeGraphic = PParentCreature->GetEyeGraphic();
        if (eyeGraphic.BankIndex) {
        CEnginePrimitiveStaticMesh* mesh = 0;
        CEnginePrimitiveStaticMesh staticMesh;
        CEnginePrimitiveMorphedStaticMesh morphedMesh;
        const CPrimitive* leftDummy = appearance.GetFirstPrimitiveWithDummyObject(EYE_LEFT_DUMMY_OBJECT_NAME);
        if (leftDummy) {
            if (info.TextureMorphsUsed && info.PTextureMorphs) {
                morphedMesh.TextureMorphs = *info.PTextureMorphs;
                mesh = &morphedMesh;
            } else
                mesh = &staticMesh;
            mesh->MeshIndex = eyeGraphic.BankIndex;
            mesh->Size = PParentCreature->GetScale() * eyeGraphic.RenderSizeX;
            mesh->WorldPos.X = mesh->WorldPos.Y = mesh->WorldPos.Z = 0.0f;
            mesh->FadeStart = appearance.GetFadeDistance();
            mesh->Lit = true;
            mesh->Alpha = appearance.GetAlpha();
            mesh->TintColour = appearance.GetTintColour();
            mesh->MeshEffect = appearance.GetPMeshEffect();
            mesh->ThemeChannelDesc = appearance.PeekLightingChannel();
            if (!LeftEyePrimitive)
                LeftEyePrimitive = appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH, eyeGraphic.BankIndex));
            CRightHandedSet set;
            CMatrix3x4 rotationYZ;
            CMatrix3x4 rotationXY;
            set.Init(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f));
            rotationXY.SetFromVectorAndAngle(set.GetUp(), EyeAnglesRelToHeadXY);
            set.SetUp(C3DVector(rotationXY.E11 * set.GetUp().X + rotationXY.E21 * set.GetUp().Y + rotationXY.E31 * set.GetUp().Z,
                               rotationXY.E12 * set.GetUp().X + rotationXY.E22 * set.GetUp().Y + rotationXY.E32 * set.GetUp().Z,
                               rotationXY.E13 * set.GetUp().X + rotationXY.E23 * set.GetUp().Y + rotationXY.E33 * set.GetUp().Z));
            set.SetForward(C3DVector(set.GetForward().X * rotationXY.E11 + set.GetForward().Y * rotationXY.E21 + set.GetForward().Z * rotationXY.E31,
                                    set.GetForward().X * rotationXY.E12 + set.GetForward().Y * rotationXY.E22 + set.GetForward().Z * rotationXY.E32,
                                    set.GetForward().X * rotationXY.E13 + set.GetForward().Y * rotationXY.E23 + set.GetForward().Z * rotationXY.E33));
            rotationYZ.SetFromVectorAndAngle(set.GetRight(), EyeAnglesRelToHeadYZ);
            set.SetUp(C3DVector(rotationYZ.E11 * set.GetUp().X + rotationYZ.E21 * set.GetUp().Y + rotationYZ.E31 * set.GetUp().Z,
                               rotationYZ.E12 * set.GetUp().X + rotationYZ.E22 * set.GetUp().Y + rotationYZ.E32 * set.GetUp().Z,
                               rotationYZ.E13 * set.GetUp().X + rotationYZ.E23 * set.GetUp().Y + rotationYZ.E33 * set.GetUp().Z));
            set.SetForward(C3DVector(set.GetForward().X * rotationYZ.E11 + set.GetForward().Y * rotationYZ.E21 + set.GetForward().Z * rotationYZ.E31,
                                    set.GetForward().X * rotationYZ.E12 + set.GetForward().Y * rotationYZ.E22 + set.GetForward().Z * rotationYZ.E32,
                                    set.GetForward().X * rotationYZ.E13 + set.GetForward().Y * rotationYZ.E23 + set.GetForward().Z * rotationYZ.E33));
            CEnginePrimitiveAttachmentInfo attach;
            attach.Flags = 4;
            attach.ParentDummyName = EYE_LEFT_DUMMY_OBJECT_NAME;
            attach.ParentDummyIndex = 0;
            attach.ChildDummyName = "";
            mesh->RightHandedSet = set;
            attach.ChildDummyIndex = 0;
            appearance.SetAttachedPrimitiveGraphics(LeftEyePrimitive, *mesh, attach, leftDummy);
        } else
            LeftEyePrimitive = 0;
        const CPrimitive* rightDummy = appearance.GetFirstPrimitiveWithDummyObject(EYE_RIGHT_DUMMY_OBJECT_NAME);
        if (rightDummy) {
            if (!RightEyePrimitive)
                RightEyePrimitive = appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH, eyeGraphic.BankIndex));
            CRightHandedSet set;
            CMatrix3x4 rotationYZ;
            CMatrix3x4 rotationXY;
            set.Init(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f));
            rotationXY.SetFromVectorAndAngle(set.GetUp(), EyeAnglesRelToHeadXY);
            set.SetUp(C3DVector(rotationXY.E11 * set.GetUp().X + rotationXY.E21 * set.GetUp().Y + rotationXY.E31 * set.GetUp().Z,
                               rotationXY.E12 * set.GetUp().X + rotationXY.E22 * set.GetUp().Y + rotationXY.E32 * set.GetUp().Z,
                               rotationXY.E13 * set.GetUp().X + rotationXY.E23 * set.GetUp().Y + rotationXY.E33 * set.GetUp().Z));
            set.SetForward(C3DVector(set.GetForward().X * rotationXY.E11 + set.GetForward().Y * rotationXY.E21 + set.GetForward().Z * rotationXY.E31,
                                    set.GetForward().X * rotationXY.E12 + set.GetForward().Y * rotationXY.E22 + set.GetForward().Z * rotationXY.E32,
                                    set.GetForward().X * rotationXY.E13 + set.GetForward().Y * rotationXY.E23 + set.GetForward().Z * rotationXY.E33));
            rotationYZ.SetFromVectorAndAngle(set.GetRight(), EyeAnglesRelToHeadYZ);
            set.SetUp(C3DVector(rotationYZ.E11 * set.GetUp().X + rotationYZ.E21 * set.GetUp().Y + rotationYZ.E31 * set.GetUp().Z,
                               rotationYZ.E12 * set.GetUp().X + rotationYZ.E22 * set.GetUp().Y + rotationYZ.E32 * set.GetUp().Z,
                               rotationYZ.E13 * set.GetUp().X + rotationYZ.E23 * set.GetUp().Y + rotationYZ.E33 * set.GetUp().Z));
            set.SetForward(C3DVector(set.GetForward().X * rotationYZ.E11 + set.GetForward().Y * rotationYZ.E21 + set.GetForward().Z * rotationYZ.E31,
                                    set.GetForward().X * rotationYZ.E12 + set.GetForward().Y * rotationYZ.E22 + set.GetForward().Z * rotationYZ.E32,
                                    set.GetForward().X * rotationYZ.E13 + set.GetForward().Y * rotationYZ.E23 + set.GetForward().Z * rotationYZ.E33));
            CEnginePrimitiveAttachmentInfo attach;
            attach.Flags = 4;
            attach.ParentDummyName = EYE_RIGHT_DUMMY_OBJECT_NAME;
            attach.ParentDummyIndex = 0;
            attach.ChildDummyName = "";
            mesh->RightHandedSet = set;
            attach.ChildDummyIndex = 0;
            appearance.SetAttachedPrimitiveGraphics(RightEyePrimitive, *mesh, attach, rightDummy);
        } else
            RightEyePrimitive = 0;
        }
    }
    if (LeftEyePrimitive == 0 && RightEyePrimitive == 0)
        return false;
    return true;
}

int main()
{
    CThingCreatureBase creature;
    CLookDef def;
    CTCLook eyes(creature,def);
    CTCGraphicAppearance appearance;
    CGraphicAppearanceDrawInfo info={0,false,0};
    def.DrawEyePrimitives=false;
    if(!eyes.OnAppearanceDraw(appearance,info)||appearance.Adds)
        return 1;
    def.DrawEyePrimitives=true;creature.Dead=true;
    if(eyes.OnAppearanceDraw(appearance,info)||appearance.Adds)
        return 2;
    creature.Dead=false;
    eyes.EyeAnglesRelToHeadXY=1.5707963f;
    if(!eyes.OnAppearanceDraw(appearance,info)||appearance.Adds!=2||appearance.Attaches!=2||
        appearance.Invalid||!Near(appearance.LastSet.Forward.X,-1)||!Near(appearance.LastSet.Up.Z,1))
        return 3;
    int morph=41;info.TextureMorphsUsed=true;info.PTextureMorphs=&morph;
    appearance.Right=false;
    if(!eyes.OnAppearanceDraw(appearance,info)||appearance.Adds!=2||appearance.Attaches!=3||
        eyes.RightEyePrimitive||!eyes.LeftEyePrimitive||appearance.Invalid)
        return 4;
    appearance.Left=false;
    if(eyes.OnAppearanceDraw(appearance,info)||eyes.LeftEyePrimitive||appearance.Attaches!=3)
        return 5;
    puts("007542b0_TEST PASS");
    return 0;
}
