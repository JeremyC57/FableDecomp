// CEnginePrimitiveManager2DParticleGroup::CreateInternalPrimitive(EEnginePrimitiveType type) -- allocates and constructs a 2D particle group primitive (0x108 bytes); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE1A00, length 121 bytes (128 with padding).
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long flags)
// @ 0x00B8FF00 (called with (0x20, 0x80000000)), malloc @ 0x00BFEA0E (the two list sentinels). Vtables (rtti-vtables.tsv):
// 0x012A59D4 = CEngineInternalPrimitiveParticleGroup, 0x012A7E3C = CEngineInternalPrimitive2DParticleGroup.
// Shape: `type` is not tested (`ret 4`, `this` unused). The group constructor runs the base constructor, stores the group
// vtable, then builds its members as the landed group destructor @ 0x00BBA140 lays them out: the 4-byte STLport list
// Primitives at +0x48 (sentinel cleared, 0x10-byte node malloc'd and self-linked, then published), the three-pointer CArray
// PrimitiveIDToIterator at +0x4C, the 4-byte list FreePrimitiveIDs at +0x58 (0xC-byte node); the two malloc argument pops
// merge into one `add esp,8`. The 2D group then zeroes its four words +0x5C..+0x68 and stores its own vtable last. One zero
// register (edi) serves the null test and every clear. All three constructors (2D group, group, list) must be
// `__forceinline`: VC7.1 keeps a constructor of this size out of line otherwise.
#include <stdlib.h>

void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

template <class T>
struct CRetailSTLportList {                        // STLport list: the sentinel node pointer only
    struct SNode {
        SNode* next;
        SNode* prev;
        T      value;
    };
    SNode* m_node;
    __forceinline CRetailSTLportList()
    {
        m_node = 0;
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->next = node;
        node->prev = node;
        m_node = node;
    }
};

struct CEnginePrimitiveHandle {                    // 8 bytes: the handle id and the primitive
    long                          m_id;
    CEngineInternalPrimitiveBase* m_pPrimitive;
};

struct CPrimitiveIterator { unsigned char m_used; void* m_pNode; };

template <class T>
struct CArray {                                    // retail CArray: first, last, end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase {   // vtable 0x012A59D4
    CRetailSTLportList<CEnginePrimitiveHandle> Primitives;          // +0x48
    CArray<CPrimitiveIterator>                 PrimitiveIDToIterator;   // +0x4C
    CRetailSTLportList<long>                   FreePrimitiveIDs;    // +0x58
    virtual void v00();
    __forceinline CEngineInternalPrimitiveParticleGroup() : CEngineInternalPrimitiveBase(0x20, 0x80000000) { }
};

struct CEngineInternalPrimitive2DParticleGroup : CEngineInternalPrimitiveParticleGroup {   // vtable 0x012A7E3C, 0x108 bytes
    long          m_5c;                            // +0x5C
    long          m_60;                            // +0x60
    long          m_64;                            // +0x64
    long          m_68;                            // +0x68
    unsigned char _rest6c[0x9c];                   // +0x6C .. +0x108 (not initialised here)
    virtual void v00();
    __forceinline CEngineInternalPrimitive2DParticleGroup() : m_5c(0), m_60(0), m_64(0), m_68(0) { }
};

enum EEnginePrimitiveType { ENGINE_PRIMITIVE_TYPE_2D_PARTICLE_GROUP = 0x20 };

struct CEnginePrimitiveManager2DParticleGroup {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManager2DParticleGroup::CreateInternalPrimitive(EEnginePrimitiveType type)
{
    return new CEngineInternalPrimitive2DParticleGroup;
}