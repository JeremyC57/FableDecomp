// CTCLook::OnAppearanceDraw() -- build and orient the look component's persistent eye graphics.
// Grade: DIFFER(2220v2266) -- The shared owner, render mesh and attachment classes retain the native receiver offsets and frame extent. Retained handles, index-slot evaluation and the two eye transform schedules remain different.
// Leaf: ?OnAppearanceDraw@CTCLook@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
// Retail: 0x007542B0, 2266 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_EYEGRAPHIC_PASS24
#define FABLE_RETAIL_CTHINGPHYSICAL_SCALE_ACCESS_PASS24
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INIT
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANGLE
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_EYEDRAW_PASS24
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETATTACHEDPRIMITIVEGRAPHICS
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_CPRIMITIVE_EYEDRAW_PASS24
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_EYEDRAW_PASS24
#define FABLE_RETAIL_CENGINEPRIMITIVEMORPHEDSTATICMESH_EYEDRAW_PASS24
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTINFO_EYEDRAW_PASS24
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCLook.h"
#include "engine/CEngineGraphic.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/CEnginePrimitiveMorphedStaticMesh.h"
#include "engine/CEnginePrimitiveAttachmentInfo.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern const CCharString EYE_LEFT_DUMMY_OBJECT_NAME;
extern const CCharString EYE_RIGHT_DUMMY_OBJECT_NAME;

using namespace NGraphicAppearance;

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
