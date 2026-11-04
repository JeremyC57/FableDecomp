#ifndef FABLE_ENGINE_NUISYSTEM_CMESH_H
#define FABLE_ENGINE_NUISYSTEM_CMESH_H
// NUISystem::CMesh (new, chunk 749): RTTI .?AVCMesh@NUISystem@@, single direct base CChangingStateComponent
// (ghidra_out/rtti_bases.tsv: CUIInterface@0, CObserver@4, CActionParam@24). Vtables 0x01249F9C (+0, 149 slots: 0 ??_G
// 0x00550C30, 1 Update 0x00550070, 2 Draw 0x005502E0, 43 Initialise 0x00550060), 0x01249F74 (+4), 0x01249F6C (+0x18).
// New virtuals (bodies read from retail): 142 GetMeshIndex 0x00550B80 (reads +0x15C), 143 SetMeshIndex 0x00550B90 (+0x15C),
// 144 SetHeight 0x00550BA0 (+0x16C), 145 SetWidth 0x00550BB0 (+0x170), 146 SetAnimationIndex 0x00550BC0 (+0x19C),
// 147 SetMeshType 0x00550BD0 (+0x160), 148 SetOrientation 0x00550BE0 (+0x174). Name set: Ego_r.pdb / FableWin.pdb
// (??0..QAE@J@Z, ??0..QAE@ABV01@@Z, ??1..UAE@XZ, ??_G, Draw, GetDataFromDef, GetMeshIndex, Initialise, SetAnimationIndex,
// SetHeight, SetMeshIndex, SetMeshType(EEngineGraphicType), SetOrientation(CRightHandedSet&), SetWidth, Update).
// Layout: Ego_r.pdb (sizeof 468) minus 0x30: MeshIndex +0x15C, MeshType +0x160, Handle +0x164, Height +0x16C, Width +0x170,
// Orientation +0x174, RotationAxis +0x18C, RotationSpeed +0x198, AnimationIndex +0x19C, NoRenderNextFrame +0x1A0 (the landed
// ctor 0x00550190, Update 0x00550070, SetOrientation 0x00550BE0 and Draw 0x005502E0); sizeof 0x1A4.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CRightHandedSet.h"
enum EEngineGraphicType;
namespace NUISystem {
class CMesh : public CChangingStateComponent {
public:
    CMesh(long);
    CMesh(const CMesh&);
    virtual ~CMesh();
    virtual void Initialise();
    virtual void Update(float);
    virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*);
    virtual unsigned long GetMeshIndex();                                // slot 142
    virtual void SetMeshIndex(unsigned long);                            // slot 143
    virtual void SetHeight(unsigned long);                               // slot 144
    virtual void SetWidth(unsigned long);                                // slot 145
    virtual void SetAnimationIndex(unsigned long);                       // slot 146
    virtual void SetMeshType(EEngineGraphicType);                        // slot 147
    virtual void SetOrientation(CRightHandedSet&);                       // slot 148
    void GetDataFromDef();
protected:
    unsigned long MeshIndex;                                             // +0x15c
    EEngineGraphicType MeshType;                                         // +0x160
    CEnginePrimitiveHandle Handle;                                       // +0x164
    unsigned long Height;                                                // +0x16c
    unsigned long Width;                                                 // +0x170
    CRightHandedSet Orientation;                                         // +0x174
    C3DVector RotationAxis;                                              // +0x18c
    float RotationSpeed;                                                 // +0x198
    unsigned long AnimationIndex;                                        // +0x19c
    unsigned char NoRenderNextFrame;                                     // +0x1a0
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CMesh) == 0x1a4);
#endif
