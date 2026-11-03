#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CMESH_FACTORY_H
#define FABLE_ENGINE_CMESH_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x550190.
// Layout: complete native 0x1a4 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CMesh:public CChangingStateComponent {
public:
    CMesh(long);
    virtual ~CMesh();
protected:
    unsigned char MeshIndex[4]; // unsigned long; native+0x15c, donor+0x18c.
protected:
    unsigned char MeshType[4]; // EEngineGraphicType; native+0x160, donor+0x190.
protected:
    unsigned char Handle[8]; // CEnginePrimitiveHandle; native+0x164, donor+0x194.
protected:
    unsigned char Height[4]; // unsigned long; native+0x16c, donor+0x19c.
protected:
    unsigned char Width[4]; // unsigned long; native+0x170, donor+0x1a0.
protected:
    unsigned char Orientation[24]; // CRightHandedSet; native+0x174, donor+0x1a4.
protected:
    unsigned char RotationAxis[12]; // C3DVector; native+0x18c, donor+0x1bc.
protected:
    unsigned char RotationSpeed[4]; // float; native+0x198, donor+0x1c8.
protected:
    unsigned char AnimationIndex[4]; // unsigned long; native+0x19c, donor+0x1cc.
protected:
    unsigned char NoRenderNextFrame[1]; // unsigned char; native+0x1a0, donor+0x1d0.
    unsigned char _pad_1a1[3];
};
FABLE_STATIC_ASSERT(sizeof(CMesh)==0x1a4);
}
#endif

#else

#endif
