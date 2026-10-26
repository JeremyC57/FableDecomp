// CEnginePrimitiveManagerParticleMeshGroup::CreateInternalPrimitive(long) -- creates one of the two particle-mesh primitive types; retail 0x00BC3410 length 0xbd. Uses retail constructors evidenced by its type switches and vtables (0x012A61FC and 0x012A6184); shared CEngineInternalPrimitiveBase layout confirms the 0x48 prefix but has no constructor declaration, so a constructor-bearing shell repeats that prefix; RTTI confirms intermediate particle-group vtable 0x012A59D4 and final particle-mesh-group/mesh vtables 0x012A61FC/0x012A6184. The generated particle-mesh-group PDB size 0xfc is 0x14 larger than this retail allocation, so the container-bearing group uses observed retail size 0xe8.
#include "engine/CEnginePrimitiveManagerParticleMeshGroup.h"
#include <stddef.h>
void* __cdecl operator new(size_t); // 0x00BFEA1A
extern "C" void* __cdecl malloc(size_t); // 0x00BFEA0E
// The shared base has an explicit vptr data member; this equivalent virtual shell has one compiler
// vptr and the missing constructor, avoiding a second vptr from inheriting that storage header.
class CEngineInternalPrimitiveBase { public:
    virtual void v00() {}; unsigned char _pad04[0x44];
    CEngineInternalPrimitiveBase(long type, unsigned long flags); // 0x00B8FF00
};
class CEnginePrimitiveHandle { public: void* vptr; void* data; };
template<class T> struct CRetailList {
    struct Node { Node* next; Node* prev; T value; };
    Node* node;
    CRetailList() { node=0; Node* p=(Node*)malloc(sizeof(Node)); p->next=p; p->prev=p; node=p; }
};
struct CIterator { unsigned char used; void* node; };
struct CIteratorArray { CIterator* first; CIterator* last; CIterator* end; CIteratorArray():first(0),last(0),end(0){} };
class C3DBoundingSphere { public: float m_centreX, m_centreY, m_centreZ, m_radius; C3DBoundingSphere():m_centreX(0),m_centreY(0),m_centreZ(0),m_radius(0){} };
struct CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase {
    CRetailList<CEnginePrimitiveHandle> Primitives;
    CIteratorArray PrimitiveIDToIterator;
    CRetailList<long> FreePrimitiveIDs;
    C3DBoundingSphere BSphere;
    unsigned char rest[0xac-0x6c];
    virtual void v00() {};
    __forceinline CEngineInternalPrimitiveParticleGroup(long t,unsigned long f):CEngineInternalPrimitiveBase(t,f) { /* BSphere inline constructor initializes four float components (PDB C3DVector Center + float Radius). */ }
};
struct CEngineInternalPrimitiveParticleMeshGroup : CEngineInternalPrimitiveParticleGroup {
    unsigned char derivedRetailTail[0x3c];
    virtual void v00() {};
    __forceinline CEngineInternalPrimitiveParticleMeshGroup():CEngineInternalPrimitiveParticleGroup(0x1c,0x88000){}
};
struct CEngineInternalPrimitiveParticleMesh : CEngineInternalPrimitiveBase {
    unsigned char meshTail[0x40];
    virtual void v00() {};
    __forceinline CEngineInternalPrimitiveParticleMesh():CEngineInternalPrimitiveBase(0x17,0x8000){}
};
struct CEnginePrimitiveManagerParticleMeshGroup_Methods : CEnginePrimitiveManagerParticleMeshGroup { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long); };
CEngineInternalPrimitiveBase* CEnginePrimitiveManagerParticleMeshGroup_Methods::CreateInternalPrimitive(long type)
{
    if (type == 0x1c) return new CEngineInternalPrimitiveParticleMeshGroup;
    if (type == 0x17) return new CEngineInternalPrimitiveParticleMesh;
    return 0;
}