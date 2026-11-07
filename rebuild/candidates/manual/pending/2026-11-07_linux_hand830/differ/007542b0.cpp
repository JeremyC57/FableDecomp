// CTCLook::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- draw the creature's eyes: add each eye mesh primitive once, turn it by the eye angles (XY about Up, YZ about Right) and attach it to the eye dummy
// Grade: DIFFER(2268v2266) -- frame slot assignment only in the large regions: retail's left eye uses the matrix slots 0x64 then 0x94 (the right eye the reverse), ours 0x90 then 0x60; retail packs the CPrimitive temporary at 0x34, ours at 0x110 (frame 0x168 v 0x180); Size/WorldPos store scheduling
// Next idea: the shape of retail's per-eye block that frees the CPrimitive temporary into the rotation-result slots (a separate {} scope or a helper the FableWin.pdb inline list names), then the matrix slot order
// Leaf: ?OnAppearanceDraw@CTCLook@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANGLE
#define FABLE_RETAIL_CRIGHTHANDEDSET_INIT
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"

class CCharString { public:
    void* PStringData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CEngineTextureMorph { public: unsigned char _pad[0x1c]; };
class CEngineMeshEffect { public:
    virtual void v00();
    virtual void Delete() const;                // slot 1: destroy on the last reference
    long RefCount;                              // +0x04
};
template <class T> class CIVCountedPointer { public:
    T* P;
    CIVCountedPointer() : P(0) { }
    __forceinline ~CIVCountedPointer() { Release(); }
    __forceinline void Release() { T* object = P; if (object) { if (--object->RefCount == 0) object->Delete(); P = 0; } }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (P != other.P) {
            Release();
            P = other.P;
            if (P) ++P->RefCount;
        }
        return *this;
    }
};
class CRGBColour { public: unsigned long Value; };
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char Channel[3];
    CEnginePrimitiveThemeChannelDesc() { Channel[0] = 0; Channel[1] = 0; Channel[2] = 0; }
};
class CEnginePrimitive { public:
    long Type;                                  // +0x00
    long InterpolationSet;                      // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
protected:
    CEnginePrimitive(long type, long interpolationSet) : Type(type), InterpolationSet(interpolationSet) { }
};
class CEnginePrimitiveStaticMesh : public CEnginePrimitive { public:
    unsigned long MeshIndex;                    // +0x0C
    C3DVector WorldPos;                         // +0x10
    float Size;                                 // +0x1C
    float FadeStart;                            // +0x20
    CRightHandedSet RightHandedSet;             // +0x24
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;   // +0x3C
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc2;   // +0x40
    CRGBColour TintColour;                      // +0x44
    unsigned char Alpha;                        // +0x48
    bool Lit : 1;                               // +0x49
    CEnginePrimitiveStaticMesh() : CEnginePrimitive(8, 0) { }
};
struct CMorphMapHeader { unsigned char _pad[0x14]; };
class CEnginePrimitiveMorphedStaticMesh : public CEnginePrimitiveStaticMesh { public:
    std::vector<CEngineTextureMorph> TextureMorphs;   // +0x4C
    void* MorphMapHeader;                       // +0x58
    unsigned long MorphMapCount;                // +0x5C
    CEnginePrimitiveMorphedStaticMesh();        // 0x007554C0
    ~CEnginePrimitiveMorphedStaticMesh();       // 0x00755520
};
class CEnginePrimitiveAttachmentInfo { public:
    unsigned long Flags;                        // +0x00
    CCharString ParentDummyName;                // +0x04
    long ParentDummyIndex;                      // +0x08
    CCharString ChildDummyName;                 // +0x0C
    long ChildDummyIndex;                       // +0x10
    __forceinline CEnginePrimitiveAttachmentInfo() { }
    __forceinline ~CEnginePrimitiveAttachmentInfo() { }
};

namespace NGraphicAppearance {
class CPrimitive { public:
    unsigned char _pad[0x20];
    CPrimitive(long type, long bankIndex);      // 0x00642390
    ~CPrimitive();                              // 0x004C4160
};
class CGraphicAppearanceDrawInfo { public:
    unsigned char _pad00[0x10];
    std::vector<CEngineTextureMorph>* PTextureMorphs;   // +0x10
    unsigned char _pad14[0x1c - 0x14];
    unsigned long Flags;                        // +0x1C
    bool _b20;
    bool UseTextureMorphs;                      // +0x21
};
class CTCGraphicAppearance { public:
    unsigned char _pad00[0x40];
    CEnginePrimitiveThemeChannelDesc LightingChannel;   // +0x40
    unsigned char _pad43[0x5a - 0x43];
    unsigned char Alpha;                        // +0x5A
    const CPrimitive* GetFirstPrimitiveWithDummyObject(const CCharString& name) const;   // 0x004BC410
    float GetFadeDistance() const;              // 0x004BC180
    const CRGBColour GetTintColour() const;     // 0x004BD040
    CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect();   // 0x004BCF50
    CPrimitive* AddPersistentPrimitive(const CPrimitive& primitive);   // 0x004C0050
    void SetAttachedPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& graphics, CEnginePrimitiveAttachmentInfo& info,
                                      const CPrimitive* parent);   // 0x004BD690
};
}
using namespace NGraphicAppearance;

class CEngineGraphic { public:
    unsigned long BankIndex;                    // +0x00
    float AnimStep;                             // +0x04
    float RenderSizeX;                          // +0x08
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0: dead
    unsigned char _pad92[0x9c - 0x92];
    float Scale;                                // +0x9C
    const CEngineGraphic& GetEyeGraphic() const;   // 0x00662160
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CLookDef { public:
    unsigned char _pad[0x4f];
    bool HasEyes;                               // +0x4F
};

extern CCharString g_LeftEyeDummyName;          // 0x013BAC54
extern CCharString g_RightEyeDummyName;         // 0x013BAC58

class CTCLook { public:
    virtual ~CTCLook();
    CThingCreatureBase* PParentCreature;        // +0x04
    const CLookDef* PDef;                       // +0x08
    unsigned char _pad0c[0x28 - 0x0c];
    float EyeAnglesRelToHeadXY;                 // +0x28
    float EyeAnglesRelToHeadYZ;                 // +0x2C
    unsigned char _pad30[0x74 - 0x30];
    bool EyeAnglesChanged;                      // +0x74
    CPrimitive* LeftEyePrimitive;               // +0x78
    CPrimitive* RightEyePrimitive;              // +0x7C
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
};

bool CTCLook::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info)
{
    if (!PDef->HasEyes)
        return true;
    if (!((info.Flags | 8) || (info.Flags | 4) || !LeftEyePrimitive || !RightEyePrimitive || EyeAnglesChanged))
        return true;
    if (PParentCreature && !PParentCreature->IsDead()) {
        const CEngineGraphic& eyeGraphic = PParentCreature->GetEyeGraphic();
        if (eyeGraphic.BankIndex) {
        CEnginePrimitiveStaticMesh* mesh = 0;
        CEnginePrimitiveStaticMesh staticMesh;
        CEnginePrimitiveMorphedStaticMesh morphedMesh;
        const CPrimitive* leftDummy = appearance.GetFirstPrimitiveWithDummyObject(g_LeftEyeDummyName);
        if (leftDummy) {
            if (info.UseTextureMorphs && info.PTextureMorphs) {
                morphedMesh.TextureMorphs = *info.PTextureMorphs;
                mesh = &morphedMesh;
            } else
                mesh = &staticMesh;
            mesh->MeshIndex = eyeGraphic.BankIndex;
            mesh->Size = PParentCreature->Scale * eyeGraphic.RenderSizeX;
            mesh->WorldPos.X = mesh->WorldPos.Y = mesh->WorldPos.Z = 0.0f;
            mesh->FadeStart = appearance.GetFadeDistance();
            mesh->Lit = true;
            mesh->Alpha = appearance.Alpha;
            mesh->TintColour = appearance.GetTintColour();
            mesh->MeshEffect = appearance.GetPMeshEffect();
            mesh->ThemeChannelDesc2 = appearance.LightingChannel;
            if (!LeftEyePrimitive)
                LeftEyePrimitive = appearance.AddPersistentPrimitive(CPrimitive(1, eyeGraphic.BankIndex));
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
            attach.ParentDummyName = g_LeftEyeDummyName;
            attach.ParentDummyIndex = 0;
            attach.ChildDummyName = "";
            mesh->RightHandedSet = set;
            attach.ChildDummyIndex = 0;
            appearance.SetAttachedPrimitiveGraphics(LeftEyePrimitive, *mesh, attach, leftDummy);
        } else
            LeftEyePrimitive = 0;
        const CPrimitive* rightDummy = appearance.GetFirstPrimitiveWithDummyObject(g_RightEyeDummyName);
        if (rightDummy) {
            if (!RightEyePrimitive)
                RightEyePrimitive = appearance.AddPersistentPrimitive(CPrimitive(1, eyeGraphic.BankIndex));
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
            attach.ParentDummyName = g_RightEyeDummyName;
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
