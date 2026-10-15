// CEnginePrimitiveManagerParticleSpriteTrailGroup::CreateInternalPrimitive(long) -- allocate and construct a particle sprite-trail group primitive (0xbc bytes: the particle-group part with base type 0x1d and flags 0x88000, then the derived vtable); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBEA70, length 121 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @
// 0x00B8FF00 (0x1d, 0x88000), malloc @ 0x00BFEA0E (each list's sentinel node). Vtables 0x012A59D4 =
// CEngineInternalPrimitiveParticleGroup and 0x012A5BFC = CEngineInternalPrimitiveParticleSpriteTrailGroup
// (rtti-vtables.tsv). The type argument is unused (`ret 4`).
// Layout: the retail particle group (see 0x00BBCEB0: lists at +0x48 / +0x58, id array +0x4c, sphere +0x5c, 0xac
// bytes) plus the trail group's own 0x10 bytes, which this constructor leaves alone.
// Shape: the inlined (`__forceinline`) particle-group constructor exactly as in 0x00BBCEB0 with the trail group's
// type and flags, then the derived class's inline constructor stores its own vtable last (after `pop edi`).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

template<class T> struct CRetailList {             // STLport list: the node pointer only
    struct SNode { SNode* m_pNext; SNode* m_pPrev; T m_value; };
    SNode* m_pNode;
    CRetailList() : m_pNode(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pNode = node;
    }
};

struct CEnginePrimitiveHandle { void* m_vptr; void* m_pData; };   // 8 bytes (vptr + counted pointer)
struct CPrimitiveIterator { unsigned char m_used; void* m_pNode; };
struct CIteratorArray {                            // retail CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    CIteratorArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};
struct C3DBoundingSphere {                         // centre + radius, zeroed by its inline constructor (as dwords)
    long m_centreX, m_centreY, m_centreZ, m_radius;
    C3DBoundingSphere() : m_centreX(0), m_centreY(0), m_centreZ(0), m_radius(0) { }
};

struct CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase {   // vtable 0x012A59D4; 0xac bytes (retail)
    CRetailList<CEnginePrimitiveHandle> Primitives;            // +0x48 (16-byte nodes)
    CIteratorArray                      PrimitiveIDToIterator; // +0x4c
    CRetailList<long>                   FreePrimitiveIDs;      // +0x58 (12-byte nodes)
    C3DBoundingSphere                   BSphere;               // +0x5c
    unsigned char                       _rest6c[0xac - 0x6c];  // DistanceFromCam, WorldMat, flags (not initialised here)
    virtual void v00();
    __forceinline CEngineInternalPrimitiveParticleGroup(long type, unsigned long flags)
        : CEngineInternalPrimitiveBase(type, flags) { }
};

struct CEngineInternalPrimitiveParticleSpriteTrailGroup : CEngineInternalPrimitiveParticleGroup {   // vtable 0x012A5BFC; 0xbc bytes
    unsigned char _trail[0x10];                    // +0xac .. +0xbc (not initialised here)
    virtual void v00();
    __forceinline CEngineInternalPrimitiveParticleSpriteTrailGroup()
        : CEngineInternalPrimitiveParticleGroup(0x1d, 0x88000) { }
};

#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"

struct CEnginePrimitiveManagerParticleSpriteTrailGroup_Methods : CEnginePrimitiveManagerParticleSpriteTrailGroup {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerParticleSpriteTrailGroup_Methods::CreateInternalPrimitive(long type)
{
    return new CEngineInternalPrimitiveParticleSpriteTrailGroup;
}