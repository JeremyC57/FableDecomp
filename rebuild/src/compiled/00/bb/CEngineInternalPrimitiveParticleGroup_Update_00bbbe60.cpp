// Leaf: ?Update@CEngineInternalPrimitiveParticleGroup@@UAEXABVCEnginePrimitive@@K@Z
// Header route (NF319, 2026-11-23): defined on the real class of engine/CEngineInternalPrimitiveParticleGroup.h (was ?Update@CEngineInternalPrimitiveParticleGroup_Methods@@QAEXABVCEnginePrimitive@@K@Z); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_DECLS

#include "engine/CEngineInternalPrimitiveParticleGroup.h"
class CEnginePrimitive;

struct CParticleVector4 {
    int values[4];
};

struct CEnginePrimitiveUpdateView {
    unsigned char padding0[0xc];
    unsigned long type;
    unsigned char enabled;
    unsigned char visible;
    unsigned char padding12[2];
    unsigned long mode;
    unsigned char padding18;
    unsigned char option0;
    unsigned char option1;
    unsigned char option2;
    CParticleVector4 transform;
};


void CEngineInternalPrimitiveParticleGroup::Update(const CEnginePrimitive& primitive, unsigned long key) {
    const CEnginePrimitiveUpdateView& source =
        (const CEnginePrimitiveUpdateView&)primitive;
    unsigned char* bytes = (unsigned char*)this;
    *(unsigned long*)(bytes + 0xc) = key;
    bytes[0xa0] = source.enabled;
    bytes[0xa1] = source.visible;
    bytes[0x27] = 0xff;
    *(unsigned long*)(bytes + 0x6c) = source.type;
    *(CParticleVector4*)(bytes + 0x5c) = source.transform;
    *(unsigned long*)(bytes + 0xa4) = source.mode;
    bytes[0xa8] = source.option0;
    bytes[0xa9] = source.option1;
    bytes[0xaa] = source.option2;
    switch (*(unsigned long*)(bytes + 0xa4)) {
    case 0:
        *(unsigned long*)(bytes + 0x30) = 0x8000;
        break;
    case 1:
        *(unsigned long*)(bytes + 0x30) = 0x100000;
        break;
    case 2:
        *(unsigned long*)(bytes + 0x30) = 0x80000;
        break;
    }
}
