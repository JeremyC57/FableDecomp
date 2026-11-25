// CEngineInternalPrimitiveGroup::ClearAllChildPrimitives() -- erase every child handle, then recompute the render-layer mask (now zero) and, when it changed, re-register the position with the group's primitive flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC6580, length 75 bytes.
// Callees: vector<CEnginePrimitiveHandle>::erase(first, last) @ 0x00440D70 (on the vector at +0x50), UpdatePosition @ 0x00B91840 (`__fastcall`, a sibling jump).
// Shape: the inlined UpdateRenderLayerMask (@ 0x00BC6180 as its own body) re-reads `first` after the erase (`mov esi,[esi]`), the end pointer once into edx.
// NF48 (round 7): the CRetailHandleVector stand-in is STLport 4.6.2 std::vector<CEnginePrimitiveHandle> from the shared stlport/fable_containers.h;
// FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE keeps vector<CEnginePrimitiveHandle>::erase(iterator, iterator) 0x00440D70 out of line as retail calls it.
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
class CEngineInternalPrimitiveBase;
class C3DBoundingBox { public: float Min[3]; float Max[3]; };

struct CEngineInternalPrimitiveVtable {           // the retail CEngineInternalPrimitiveBase vtable slots used by the group/changer/viewport bodies
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void PostRender();                                       // slot 14 (+0x38)
    virtual void v15(); virtual void v16();
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);      // slot 17 (+0x44)
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void SetAnimation(void* animation);                      // slot 23 (+0x5c)
    virtual void SetPosition(void* position);                        // slot 24 (+0x60)
    virtual void v25(); virtual void v26();
    virtual CEngineInternalPrimitiveVtable* GetAsOcclusionPrimitive();   // slot 27 (+0x6c)
    virtual void ResetOcclusionInfo(long layer);                     // slot 28 (+0x70)
};

class CEngineInternalPrimitiveBase : public CEngineInternalPrimitiveVtable { public:   // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes)
    unsigned char  _pad04[0x2c];                   // +0x04 .. +0x30
    unsigned long  RenderLayerMask;                // +0x30
    unsigned char  PostRenderEnable;               // +0x34 bitfield group: bit 3 = post-render enabled
    unsigned char  _pad35[0x13];                   // +0x35 .. +0x48
};

class CEnginePrimitiveHandle { public:                    // 8 bytes: the handle id and the primitive
    long                          m_id;
    CEngineInternalPrimitiveBase* m_pPrimitive;
};


// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00B91840 ?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z (was ?UpdatePosition_00b91840@@YIXPAVCEngineInternalPrimitiveBase@@K@Z)
class CEngineScene { public: static void __fastcall UpdatePosition(CEngineInternalPrimitiveBase&, unsigned long); };
struct CEngineInternalPrimitiveGroup : CEngineInternalPrimitiveBase {   // engine/CEngineInternalPrimitiveGroup.h: Primitives +0x50 (retail: three words), PrimitiveFlags +0x5c (PDB +0x60)
    unsigned char       _pad48[8];
    std::vector<CEnginePrimitiveHandle> Primitives;                // +0x50
    unsigned long       PrimitiveFlags;            // +0x5c
    void UpdateRenderLayerMask()
    {
        unsigned long mask = 0;
        for (CEnginePrimitiveHandle* h = Primitives.begin(); h != Primitives.end(); ++h) {
            if (h->m_pPrimitive)
                mask |= h->m_pPrimitive->RenderLayerMask;
        }
        if (RenderLayerMask != mask) {
            RenderLayerMask = mask;
            CEngineScene::UpdatePosition(*(this), PrimitiveFlags);
        }
    }
    virtual void ClearAllChildPrimitives();
};

void CEngineInternalPrimitiveGroup::ClearAllChildPrimitives()
{
    Primitives.clear();
    UpdateRenderLayerMask();
}