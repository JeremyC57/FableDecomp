// CEnginePrimitiveChangerManager::PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&, unsigned long) -- preload a primitive changer's children through their own type managers: every child (OR of the results) when PreloadAll is set, else only the active one
// Grade: DIFFER(171v182) -- retail keeps the result bool in cl (xor cl,cl + a frame spill, `mov al,cl` on both false returns), loads the begin into ebx, indexes with lea+load in the loop and re-materialises the two arguments through ebx; ours keeps the bool only in the frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5820, length 192 bytes (ret 0Ch; `this` unused).
// Callees: the child type's manager virtual slot 9 (+0x24) PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&,
// unsigned long) -> bool, looked up in the primitive type info's PrimitiveManagers[] (+0x10) by the child's Type (+8).
// Globals: the primitive type info @ 0x01436E84 (engine/CEnginePrimitiveTypeInfo.h).
// Layout: CEngineInternalPrimitiveChanger (PDB less 4 after the narrower retail CArray): Primitives +0x48 (CArray of 8-byte
// CEnginePrimitiveHandle, the primitive at +4), ActiveIndex +0x58, PreloadAll +0x5C.
// Shape: the child count is a named local computed once (`sar edi,3`, unsigned compares), the begin pointer is re-read
// per iteration after the virtual call; the result is a bool local OR-ed with each child's result (spilled to a frame
// byte); the active-index path tests -1 and the range before reading the child.
#include "engine/CEnginePrimitiveTypeInfo.h"

struct C3DVector { float X, Y, Z; };

class CEngineInternalPrimitiveBase {
public:
    virtual void v00();
    long m_refCount;                            // +0x04
    long Type;                                  // +0x08 EEnginePrimitiveType
};

struct CEnginePrimitiveManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position,
                                  unsigned long flags);   // slot 9 (+0x24)
};

extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

struct CEnginePrimitiveHandle {                 // 8 bytes
    void*                         m_pInfo;      // +0x00
    CEngineInternalPrimitiveBase* m_pPrimitive; // +0x04
};

class CEngineInternalPrimitiveChanger : public CEngineInternalPrimitiveBase {
public:
    unsigned char           _pad0c[0x48 - 0x0c];
    CEnginePrimitiveHandle* Primitives_first;   // +0x48 CArray<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* Primitives_last;    // +0x4c
    CEnginePrimitiveHandle* Primitives_end;     // +0x50
    void*                   MeshEffect;         // +0x54 CIVCountedPointer<CEngineMeshEffect>
    long                    ActiveIndex;        // +0x58
    bool                    PreloadAll;         // +0x5c
    unsigned long size() const { return Primitives_last - Primitives_first; }
    CEnginePrimitiveHandle& operator[](unsigned long i) { return Primitives_first[i]; }
};

class CEnginePrimitiveChangerManager {
public:
    bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);
};

static inline CEnginePrimitiveManager* GetManager(const CEngineInternalPrimitiveBase* primitive)
{
    return g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type];
}

bool CEnginePrimitiveChangerManager::PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position,
                                                      unsigned long flags)
{
    CEngineInternalPrimitiveChanger& changer = static_cast<CEngineInternalPrimitiveChanger&>(primitive);
    bool preloaded = false;
    if (changer.PreloadAll) {
        CEnginePrimitiveHandle* first = changer.Primitives_first;
        unsigned long count = changer.Primitives_last - first;
        for (unsigned long i = 0; i < count; ++i) {
            CEnginePrimitiveHandle& handle = changer[i];
            CEngineInternalPrimitiveBase* child = handle.m_pPrimitive;
            if (child)
                preloaded |= GetManager(child)->PreloadPrimitive(*child, position, flags);
        }
        return preloaded;
    }
    if (changer.ActiveIndex != -1 && (unsigned long)changer.ActiveIndex < changer.size()) {
        CEngineInternalPrimitiveBase* child = changer[changer.ActiveIndex].m_pPrimitive;
        if (child)
            return GetManager(child)->PreloadPrimitive(*child, position, flags);
    }
    return preloaded;
}
