#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERPARTICLEMESHGROUP_H
struct CEnginePrimitiveManagerParticleMeshGroup {};
#endif
#define malloc ObserveListAllocate
// CEnginePrimitiveManagerParticleMeshGroup::CreateInternalPrimitive(long) -- creates one of the two particle-mesh primitive types; retail 0x00BC3410 length 0xbd. Uses retail constructors evidenced by its type switches and vtables (0x012A61FC and 0x012A6184); shared CEngineInternalPrimitiveBase layout confirms the 0x48 prefix but has no constructor declaration, so a constructor-bearing shell repeats that prefix; RTTI confirms intermediate particle-group vtable 0x012A59D4 and final particle-mesh-group/mesh vtables 0x012A61FC/0x012A6184. The generated particle-mesh-group PDB size 0xfc is 0x14 larger than this retail allocation, so the container-bearing group uses observed retail size 0xe8.
#include "engine/CEnginePrimitiveManagerParticleMeshGroup.h"
#include <stddef.h>
void* __cdecl operator new(size_t); // 0x00BFEA1A
extern "C" void* __cdecl malloc(size_t); // 0x00BFEA0E
// The shared base has an explicit vptr data member; this equivalent virtual shell has one compiler
// vptr and the missing constructor, avoiding a second vptr from inheriting that storage header.
struct CEngineInternalPrimitiveBase {
    virtual void v00() {}; unsigned char _pad04[0x44];
    CEngineInternalPrimitiveBase(long type, unsigned long flags); // 0x00B8FF00
};
struct CEnginePrimitiveHandle { void* vptr; void* data; };
template<class T> struct CRetailList {
    struct Node { Node* next; Node* prev; T value; };
    Node* node;
    CRetailList() { node=0; Node* p=(Node*)malloc(sizeof(Node)); p->next=p; p->prev=p; node=p; }
};
struct CIterator { unsigned char used; void* node; };
struct CIteratorArray { CIterator* first; CIterator* last; CIterator* end; CIteratorArray():first(0),last(0),end(0){} };
struct C3DBoundingSphere { float m_centreX, m_centreY, m_centreZ, m_radius; C3DBoundingSphere():m_centreX(0),m_centreY(0),m_centreZ(0),m_radius(0){} };
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

#undef malloc
static void* allocations[3]; static size_t sizes[3]; static int allocationCount, newCalls, baseCalls, phase;
static long expectedType; static unsigned long expectedFlags;static bool failNew,wrong;
static void* allocate(size_t size) {
    if(allocationCount>=3){wrong=true;return 0;}
    void* p=std::malloc(size);if(p)std::memset(p,0xa5,size);
    allocations[allocationCount]=p;sizes[allocationCount++]=size;return p;
}
void* __cdecl operator new(size_t size) {
    ++newCalls;if(phase!=0)wrong=true;phase=1;
    return failNew?0:allocate(size);
}
extern "C" void* __cdecl ObserveListAllocate(size_t size) {
    if(expectedType!=0x1c || (phase!=2 && phase!=3))wrong=true;
    CEngineInternalPrimitiveParticleMeshGroup* p=(CEngineInternalPrimitiveParticleMeshGroup*)allocations[0];
    if(phase==2) {
        if(p->Primitives.node!=0 || size!=sizeof(CRetailList<CEnginePrimitiveHandle>::Node))wrong=true;
    } else {
        if(p->Primitives.node!=allocations[1] || p->Primitives.node->next!=p->Primitives.node ||
           p->Primitives.node->prev!=p->Primitives.node || p->PrimitiveIDToIterator.first ||
           p->PrimitiveIDToIterator.last || p->PrimitiveIDToIterator.end || p->FreePrimitiveIDs.node ||
           size!=sizeof(CRetailList<long>::Node))wrong=true;
    }
    ++phase;return allocate(size);
}
CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type,unsigned long flags) {
    if(this!=allocations[0] || type!=expectedType || flags!=expectedFlags || phase!=1)wrong=true;
    ++baseCalls;phase=2;
}
static bool run(long type,bool fail) {
    allocationCount=newCalls=baseCalls=phase=0;wrong=false;failNew=fail;
    expectedType=type;expectedFlags=type==0x1c?0x88000:0x8000;
    CEnginePrimitiveManagerParticleMeshGroup_Methods manager;
    CEngineInternalPrimitiveBase* result=manager.CreateInternalPrimitive(type);
    bool supported=type==0x1c || type==0x17;
    bool ok=!wrong;
    if(!supported) ok=ok && !result && !newCalls && !baseCalls && !allocationCount;
    else if(fail) ok=ok && !result && newCalls==1 && !baseCalls && !allocationCount;
    else {
        ok=ok && result==allocations[0] && newCalls==1 && baseCalls==1;
        if(type==0x1c) {
            CEngineInternalPrimitiveParticleMeshGroup* group=(CEngineInternalPrimitiveParticleMeshGroup*)result;
            ok=ok && sizes[0]==sizeof(*group) && allocationCount==3 && phase==4 &&
                group->Primitives.node==allocations[1] && group->FreePrimitiveIDs.node==allocations[2] &&
                group->FreePrimitiveIDs.node->next==group->FreePrimitiveIDs.node &&
                group->FreePrimitiveIDs.node->prev==group->FreePrimitiveIDs.node &&
                !group->PrimitiveIDToIterator.first && !group->PrimitiveIDToIterator.last && !group->PrimitiveIDToIterator.end &&
                group->BSphere.m_centreX==0.0f && group->BSphere.m_centreY==0.0f &&
                group->BSphere.m_centreZ==0.0f && group->BSphere.m_radius==0.0f;
        } else ok=ok && allocationCount==1 && sizes[0]==sizeof(CEngineInternalPrimitiveParticleMesh) && phase==2;
    }
    for(int i=0;i<allocationCount;++i)std::free(allocations[i]);
    return ok;
}
int main() {
#ifdef _MSC_VER
    if(sizeof(CEngineInternalPrimitiveParticleMeshGroup)!=0xe8 || sizeof(CEngineInternalPrimitiveParticleMesh)!=0x88) return 2;
#endif
    if(!run(0x1c,false)||!run(0x17,false)||!run(0x1c,true)||!run(0x17,true)||!run(0,false)||!run(-1,false)||!run(0x1d,false)) return 1;
    std::puts("00bc3410_TEST PASS");return 0;
}