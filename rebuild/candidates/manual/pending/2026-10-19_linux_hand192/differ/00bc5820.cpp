// Grade: DIFFER(177v182) -- the spilled bool result: retail initialises it from `xor cl,cl`, ORs through cl (`mov cl,[m] ; or cl,al ; mov [m],cl`) and returns cl, the lane ORs into memory and reloads al; retail also keeps the vector's first pointer in ebx for the size (ours edx, pushes ebx/ebp in the other order) (tried `|=`, `result = result | x`, a named bool for the call)
// CEnginePrimitiveChangerManager::PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&, unsigned long) -- preload a primitive changer's children through their own type managers: every child when PreloadAll is set (true if any preload reports true), otherwise just the active child (its result)
// Leaf: PreloadPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC5820, 182 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?PreloadPrimitive@CEnginePrimitiveChangerManager@@UAE_NAAVCEngineInternalPrimitiveBase@@ABVC3DVector@@K@Z.
// Callees: the child type's manager slot 9 (+0x24) PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&,
//   unsigned long), managers from the primitive type table 0x01436E84 (engine/CEnginePrimitiveTypeInfo.h:
//   PrimitiveManagers[] at +0x10) indexed by the child's type word (+8).
// Layout: the changer (retail, as the landed RenderPrimitive 0x00BC5970): Primitives vector +0x48 {first, last, end} of
//   8-byte CEnginePrimitiveHandle {vptr, Data}, MeshEffect +0x54, ActiveIndex +0x58, PreloadAll +0x5c.
// Shape: the bool result is a frame byte (`or cl, al` per child); the child count is read once before the loop, the first
//   pointer each pass through a handle reference tested on its Data (`lea eax,[eax+ebp*8]`); the active-child branch returns the call's result
//   directly and falls back to the false result.
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"

struct C3DVector { float X, Y, Z; };

struct CEnginePrimitiveManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);   // slot 9 (+0x24)
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

struct CEnginePrimitiveHandle {
    virtual ~CEnginePrimitiveHandle();
    CEngineInternalPrimitiveBase* Data;
};
struct CPrimitiveArray {
    CEnginePrimitiveHandle* First;
    CEnginePrimitiveHandle* Last;
    CEnginePrimitiveHandle* Limit;
    unsigned long size() const { return Last - First; }
    CEnginePrimitiveHandle& operator[](unsigned long i) { return First[i]; }
};
struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {
    CPrimitiveArray Primitives;                        // +0x48
    void*           MeshEffect;                        // +0x54
    unsigned long   ActiveIndex;                       // +0x58
    bool            PreloadAll;                        // +0x5c
};

static inline CEnginePrimitiveManager* GetManager(const CEngineInternalPrimitiveBase* primitive)
{
    unsigned long type = *reinterpret_cast<const unsigned long*>(primitive->_pad_0x08);
    return g_pEnginePrimitiveTypeInfo->PrimitiveManagers[type];
}

struct CEnginePrimitiveChangerManager {
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);
};

bool CEnginePrimitiveChangerManager::PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags)
{
    CEngineInternalPrimitiveChanger& changer = static_cast<CEngineInternalPrimitiveChanger&>(primitive);
    bool result = false;
    if (changer.PreloadAll) {
        unsigned long count = changer.Primitives.size();
        for (unsigned long i = 0; i < count; ++i) {
            CEnginePrimitiveHandle& handle = changer.Primitives[i];
            if (handle.Data) {
                CEngineInternalPrimitiveBase* child = handle.Data;
                result = result | GetManager(child)->PreloadPrimitive(*child, position, flags);
            }
        }
        return result;
    }
    if (changer.ActiveIndex != (unsigned long)-1 && changer.ActiveIndex < changer.Primitives.size()) {
        CEngineInternalPrimitiveBase* child = changer.Primitives[changer.ActiveIndex].Data;
        if (child)
            return GetManager(child)->PreloadPrimitive(*child, position, flags);
    }
    return result;
}
