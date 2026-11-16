// CEnginePrimitiveManagerParticleGroup::RenderSubPrimGroups(CEngineInternalPrimitiveBase& primitive, ERenderPassType pass, const CMatrix3x4& transform) -- render a particle group's member primitive by type: nested groups recurse over their children, sprites and sprite trails go straight to their managers (the trail restoring the vertex-shader constant layout), everything else through its manager's virtual sub-primitive render
// Leaf: ?RenderSubPrimGroups@CEnginePrimitiveManagerParticleGroup@@IAEXAAVCEngineInternalPrimitiveBase@@W4ERenderPassType@@ABVCMatrix3x4@@@Z
// Naming wave 35c: FableWin.pdb + Ego_r.pdb protected (IAE); queue land0411f.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBC130, length 480 bytes (switch tables inline), `ret 0Ch`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 / CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (the CEngineSubPrimitiveBase local, vtable 0x012A3E7C;
//   its out-of-line ctor 0x00B98A50 in the type-30 case), CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite
//   0x00BC04F0, CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail 0x00BBE090,
//   CShaderRenderManager::SetVertexShaderConstantLayout 0x0098B5E0 (singleton 0x013BC470, current layout +8),
//   virtual slot 5 of the type's primitive manager.
// Globals: the primitive manager table @ 0x01436E84 (managers by type at +0x10), the object @ 0x01436EB0 (+0x2c >= 2
//   gates type 30).
// Layout: the primitive's type at +8, a group's member list (STLport list, element primitive at node +0xc) at +0x48.
// Shape: switch over the type (7 sprite, 20 trail, 24 group, 30 gated, default); the sub-primitive is a stack
//   CEngineSubPrimitiveBase(primitive) with the implicit destructor (no vtable re-store, just ~CBase) at the end of each
//   case; VC7.1's per-function inline budget leaves the fourth constructor (type 30) out of line, as retail.
class CMatrix3x4;
enum ERenderPassType { RENDER_PASS_DEFAULT = 0 };

#include "engine/CBaseClassNonCopyable.h"

class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {     // vtable 0x012A3E7C
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
};

// NF46 (round 7): the 0x01436E84 global is NEngineInternal::GPPrimitiveTypeInfo (Ego_r.pdb / FableWin.pdb
// ?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A); the manager is its FableWin.pdb inline GetPrimitiveManager(type)
// (PrimitiveManagers +0x10, engine/CEnginePrimitiveTypeInfo.h, overlay opt-in _GETPRIMITIVEMANAGER), a CEnginePrimitiveManager
// (engine/CEnginePrimitiveManager.h: the Ego_r.pdb virtuals in retail slot order, RenderSubPrimitive slot 5). Were the
// CEnginePrimitiveManagerTable / g_pPrimitiveManagers and local CEnginePrimitiveManager slot-view stand-ins.
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveManager.h"
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }   // 0x01436E84
class CEnginePrimitiveManagerParticleSpriteGroup : public CEnginePrimitiveManager {
public:
    void RenderSingleSprite(CEngineSubPrimitiveBase* subPrimitive, ERenderPassType pass);           // 0x00BC04F0
};
class CEnginePrimitiveManagerParticleSpriteTrailGroup : public CEnginePrimitiveManager {
public:
    void RenderSingleSpriteTrail(CEngineSubPrimitiveBase* subPrimitive, ERenderPassType pass);      // 0x00BBE090
};


struct CEngineCapabilities_01436eb0 {
    unsigned char _pad00[0x2c];
    long          Level;                            // +0x2c
};
extern CEngineCapabilities_01436eb0* g_pEngineCapabilities_01436eb0;   // 0x01436EB0

class CShaderRenderManager {
public:
    unsigned char _pad00[8];
    long          CurrentVSConstantLayoutIndex;     // +0x08
    void SetVertexShaderConstantLayout(long layout);   // 0x0098B5E0
};
extern CShaderRenderManager g_ShaderRenderManager;  // 0x013BC470

struct SGroupMember {
    long                          Key;              // +0x08 of the node
    CEngineInternalPrimitiveBase* Primitive;        // +0x0c of the node
};
struct SGroupNode {
    SGroupNode*  Next;
    SGroupNode*  Prev;
    SGroupMember Value;
};

class CEngineInternalPrimitiveBase {
public:
    unsigned char _pad00[8];
    long          Type;                             // +0x08
    unsigned char _pad0c[0x3c];
    SGroupNode*   Members;                          // +0x48 (sentinel)
};

class CEnginePrimitiveManagerParticleGroup {
protected:
    void RenderSubPrimGroups(CEngineInternalPrimitiveBase& primitive, ERenderPassType pass, const CMatrix3x4& transform);
};

void CEnginePrimitiveManagerParticleGroup::RenderSubPrimGroups(CEngineInternalPrimitiveBase& primitive, ERenderPassType pass,
                                                               const CMatrix3x4& transform)
{
    switch (primitive.Type) {
    case 24: {
        for (SGroupNode* node = primitive.Members->Next; node != primitive.Members; node = node->Next)
            RenderSubPrimGroups(*node->Value.Primitive, pass, transform);
        break;
    }
    case 7: {
        CEnginePrimitiveManagerParticleSpriteGroup* manager = (CEnginePrimitiveManagerParticleSpriteGroup*)NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(27);
        CEngineSubPrimitiveBase subPrimitive(primitive);
        manager->RenderSingleSprite(&subPrimitive, pass);
        break;
    }
    case 20: {
        CEnginePrimitiveManagerParticleSpriteTrailGroup* manager = (CEnginePrimitiveManagerParticleSpriteTrailGroup*)NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(29);
        CEngineSubPrimitiveBase subPrimitive(primitive);
        manager->RenderSingleSpriteTrail(&subPrimitive, pass);
        long layout = g_ShaderRenderManager.CurrentVSConstantLayoutIndex;
        g_ShaderRenderManager.SetVertexShaderConstantLayout(5);
        g_ShaderRenderManager.SetVertexShaderConstantLayout(layout);
        break;
    }
    case 30: {
        if (g_pEngineCapabilities_01436eb0 && g_pEngineCapabilities_01436eb0->Level >= 2) {
            CEnginePrimitiveManager* manager = NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(primitive.Type);
            CEngineSubPrimitiveBase subPrimitive(primitive);
            manager->RenderSubPrimitive(subPrimitive, pass);
        }
        break;
    }
    default: {
        CEnginePrimitiveManager* manager = NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(primitive.Type);
        CEngineSubPrimitiveBase subPrimitive(primitive);
        manager->RenderSubPrimitive(subPrimitive, pass);
        break;
    }
    }
}