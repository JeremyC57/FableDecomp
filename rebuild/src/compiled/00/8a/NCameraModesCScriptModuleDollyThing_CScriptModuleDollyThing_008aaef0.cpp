// NCameraModes::CScriptModuleDollyThing::CScriptModuleDollyThing(void* thing, const C3DVector& offset, const SHandle& target) -- CScriptModuleBase, the thing at +0x4c, this class's vtable, the offset copied to +0x50..+0x58, the target's word at +0x5c
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAEF0, length 55 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070. Vtable 0x01278188 = NCameraModes::CScriptModuleDollyThing (rtti-vtables.tsv). Shape: the thing store
// precedes the vtable store, the three words of the offset go through ecx/edx/eax, the target word last; `this` is returned, `ret 0Ch`. The chunk label
// "CEngineScenePrimitiveStatsEnumerator" is propagated.
#include "engine/C3DVector.h"
struct SHandle_008aaef0 { long m_id; };

struct CScriptModuleBase {
    virtual void v00();
    unsigned char _pad[0x48];                      // +0x04 .. +0x4c
    CScriptModuleBase();                           // 0x005ED070
};

namespace NCameraModes {
struct CScriptModuleDollyThing : CScriptModuleBase {   // vtable 0x01278188
    void*            m_pThing;                     // +0x4c
    float            m_offsetX;                     // +0x50 (three floats initialised one by one: no inline copy constructor)
    float            m_offsetY;                     // +0x54
    float            m_offsetZ;                     // +0x58
    SHandle_008aaef0 m_target;                     // +0x5c
    virtual void v00();
    CScriptModuleDollyThing(void* thing, const C3DVector& offset, const SHandle_008aaef0& target);
};
}

NCameraModes::CScriptModuleDollyThing::CScriptModuleDollyThing(void* thing, const C3DVector& offset, const SHandle_008aaef0& target)
    : CScriptModuleBase()
    , m_pThing(thing)
    , m_offsetX(offset.X), m_offsetY(offset.Y), m_offsetZ(offset.Z)
    , m_target(target)
{
}