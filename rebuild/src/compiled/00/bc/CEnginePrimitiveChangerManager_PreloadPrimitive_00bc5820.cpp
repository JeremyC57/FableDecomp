// CEnginePrimitiveChangerManager::PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&, unsigned long) -- preload a primitive changer's children through their own type managers: every child when PreloadAll is set (true if any preload reports true), otherwise just the active child (its result)
// Leaf: PreloadPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC5820, 182 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?PreloadPrimitive@CEnginePrimitiveChangerManager@@UAE_NAAVCEngineInternalPrimitiveBase@@ABVC3DVector@@K@Z.
// Callees: the child type's manager slot 9 (+0x24) PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&,
//   unsigned long), managers from the primitive type table 0x01436E84 (engine/CEnginePrimitiveTypeInfo.h:
//   PrimitiveManagers[] at +0x10) indexed by the child's type word (+8).
// Layout: the changer (retail, as the landed RenderPrimitive 0x00BC5970): Primitives vector +0x48 {first, last, end} of
//   8-byte CEnginePrimitiveHandle {vptr, Data}, MeshEffect +0x54, ActiveIndex +0x58, PreloadAll +0x5c.
// Shape: one exit `return result` after an if/else: the PreloadAll loop ORs every child's result into a bool that lives
//   in cl with a frame home (`xor cl,cl` init, `mov cl,[m] ; or cl,al ; mov [m],cl` per child); the child count is read
//   once before the loop, the first pointer each pass through a handle reference tested on its Data; the else arm takes
//   the active child from an inline helper (null when ActiveIndex is -1 or out of range) and assigns the call's result.
//   VC returns the call's al directly and gives the null/invalid arm its own `mov al,cl` epilogue (an inline-condition
//   `else if (valid)` form cross-jumps that arm into the loop's exit and is 8 bytes short).
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"

class C3DVector { public: float X, Y, Z; };

class CEnginePrimitiveManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);   // slot 9 (+0x24)
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

class CEnginePrimitiveHandle { public:
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

static inline CEngineInternalPrimitiveBase* GetActiveChild(CEngineInternalPrimitiveChanger& changer)
{
    if (changer.ActiveIndex != (unsigned long)-1 && changer.ActiveIndex < changer.Primitives.size())
        return changer.Primitives[changer.ActiveIndex].Data;
    return 0;
}

class CEnginePrimitiveChangerManager { public:
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
    } else {
        CEngineInternalPrimitiveBase* child = GetActiveChild(changer);
        if (child)
            result = GetManager(child)->PreloadPrimitive(*child, position, flags);
    }
    return result;
}