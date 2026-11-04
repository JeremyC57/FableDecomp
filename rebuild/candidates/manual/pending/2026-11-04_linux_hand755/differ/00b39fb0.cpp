// CEnginePrimitiveAttachmentManager::UpdatePrimitives(const CArray<CEngineCamera*>& cameras, float time) -- walk the attached primitives: drop the ones nobody else references any more; for an animated-mesh parent, rebuild the output primitive when the parent's grown bounding sphere is visible to a camera; the output primitive is enabled exactly when it was updated
// Grade: DIFFER(284v284) -- scheduling only: retail issues `fadd [sphere.Radius]` before the camera/distance pushes and the `fstp` after the `lea` of the sphere; ours pushes the cameras and forms the lea first, then fadd+fstp (same instructions)
// Leaf: ?UpdatePrimitives@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B39FB0, length 282 bytes (`ret 8`).
// Callees: CEngineInternalAttachedPrimitive::GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere*, float) @ 0x00B395E0,
// the output primitive's virtual slot 19 (+0x4c, the bounding radius into a float&), CEnginePrimitiveAttachmentManager::IsVisible
// (const C3DBoundingSphere*, float, const CArray<CEngineCamera*>*) @ 0x00B38ED0, CEngineInternalAttachedPrimitive::UpdateOutputPrimitive
// (float) @ 0x00B399A0, the list value's virtual destructor (slot 0, flag 0), free @ 0x00BFEA14.
// Layout: as the landed CEnginePrimitiveAttachmentManager ctor 0x00B39F30 (AttachedPrimitives: a 4-byte STLport list at +0x10; the
// node value is the 12-byte CEngineInternalAttachedPrimitiveInfo of 0x00B3AAE0: vptr, then a CEnginePrimitiveHandle whose primitive
// pointer is node+0x10); PDB CEngineInternalAttachedPrimitive (reference count +4, ParentType +0x78, OutputPrimitive's data +0x90);
// the output primitive's MaxDrawDistance +0x18 and Enable bit (bit 0 of +0x34) per engine/CEngineInternalPrimitiveBase.h.
// Shape: the successor is read before the node is processed; erasing is the inline STLport unlink + destroy + free; the bounding
// sphere is a zero-constructed local grown by twice the output radius in place; `this` is parked in the frame while bl carries the
// visibility flag.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"
struct CEngineCamera;
#include "engine/CArray.h"

struct CEngineFadeDistance { float FadeBeginDistance; float MaxDrawDistance; };
class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18();
    virtual bool GetBoundingRadius(float& radius);   // slot 19 (+0x4c)
    long          RefCount;                        // +0x04
    unsigned char _pad08[0xc];
    CEngineFadeDistance FadeDistance;              // +0x14
    unsigned char _pad1c[0x18];
    unsigned char Enable : 1;                      // +0x34 bit 0
};

class CEngineInternalAttachedPrimitive {
public:
    unsigned char                 _pad00[4];
    unsigned long                 RefCount;        // +0x04
    unsigned char                 _pad08[0x70];
    long                          ParentType;      // +0x78
    unsigned char                 _pad7c[0x14];
    CEngineInternalPrimitiveBase* OutputPrimitive; // +0x90 (OutputPrimitive.Data)
    bool GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere* sphere, float time);   // 0x00B395E0
    void UpdateOutputPrimitive(float time);        // 0x00B399A0
};

struct CBaseClass {
    virtual ~CBaseClass();
};
struct CEnginePrimitiveHandle : CBaseClass {
    CEngineInternalAttachedPrimitive* m_pPrimitive;   // +0x04
    unsigned long GetRefCount() const { return m_pPrimitive ? m_pPrimitive->RefCount : 0; }
};
struct CEngineInternalAttachedPrimitiveInfo : CBaseClass {
    CEnginePrimitiveHandle Handle;                 // +0x04
};

struct SAttachedNode {                             // STLport _List_node<CEngineInternalAttachedPrimitiveInfo>, 0x14 bytes
    SAttachedNode*                       _M_next;
    SAttachedNode*                       _M_prev;
    CEngineInternalAttachedPrimitiveInfo _M_data;
};

class CEnginePrimitiveAttachmentManager {
public:
    unsigned char  _pad00[0x10];                   // root vptr, CEngineComponent (vptr, word, two flags)
    SAttachedNode* AttachedPrimitives;             // +0x10 (the list sentinel)
private:
    bool IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const;   // 0x00B38ED0
public:
    void UpdatePrimitives(const CArray<CEngineCamera*>& cameras, float time);
};

void CEnginePrimitiveAttachmentManager::UpdatePrimitives(const CArray<CEngineCamera*>& cameras, float time)
{
    CEngineInternalAttachedPrimitiveInfo* info;
    SAttachedNode* next;
    SAttachedNode* node = AttachedPrimitives->_M_next;
    while (node != AttachedPrimitives) {
        next = node->_M_next;
        info = &node->_M_data;
        if (info->Handle.GetRefCount() > 1) {
            CEngineInternalAttachedPrimitive* attached = info->Handle.m_pPrimitive;
            CEngineInternalPrimitiveBase* output = attached->OutputPrimitive;
            long parentType = attached->ParentType;
            bool updated = false;
            C3DBoundingSphere sphere;
            switch (parentType) {
            case 2:
                if (attached->GetParentWorldSpaceBoundingSphereAnimatedMesh(&sphere, time)) {
                    float radius;
                    if (output->GetBoundingRadius(radius)) {
                        attached = info->Handle.m_pPrimitive;
                        sphere.Radius = radius + radius + sphere.Radius;
                        if (IsVisible(sphere, attached->OutputPrimitive->FadeDistance.MaxDrawDistance, cameras)) {
                            attached->UpdateOutputPrimitive(time);
                            updated = true;
                        }
                    }
                }
                break;
            }
            info->Handle.m_pPrimitive->OutputPrimitive->Enable = updated;
        } else {
            SAttachedNode* prev = node->_M_prev;
            prev->_M_next = next;
            next->_M_prev = prev;
            info->~CEngineInternalAttachedPrimitiveInfo();
            free(node);
        }
        node = next;
    }
}
