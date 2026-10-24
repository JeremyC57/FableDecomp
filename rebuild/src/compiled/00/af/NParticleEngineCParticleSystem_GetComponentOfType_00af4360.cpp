// NParticleEngine::CParticleSystem::GetComponentOfType(EPSCType type, unsigned long index) -- the index'th component of the given type: walk the component map's equal range [LowerBound(type), UpperBound(type)) index steps and return that instance's component pointer, or 0 when the range runs out
// Leaf: ?GetComponentOfType@CParticleSystem@NParticleEngine@@QAEPAVCPSCBase@2@W4EPSCType@2@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF4360, 79 bytes (+1 int3 padding), __fastcall `ret 8`.
// Identity: FableWin.pdb name set `?GetComponentOfType@CParticleSystem@NParticleEngine@@QAEPAVCPSCBase@2@W4EPSCType@2@K@Z`;
//   it sits between CParticleSystem::ClearAllPrimitives 0x00AF4320 and ::GetParticleStats 0x00AF43B0 (both landed) and
//   reads the Components map at +0x0c. Caller 0x00B11B51.
// Callees: CVectorMap<EPSCType, CComponentInstance>::LowerBound 0x00AF60C0 and ::UpperBound 0x00AF6120 (landed as
//   "UpperBoundCVectorMap00af6120"), both `(const EPSCType&)` returning the entry pointer.
// Layout: retail CParticleSystem (vptr +0, Components +0x0c, as the landed 0x00AF4100); a map entry is
//   pair<EPSCType, CComponentInstance> (0x18 bytes): CComponentInstance {vptr +0, CCountedPointer<CPSCBase> Component +4
//   (Data, Block), Enabled +0xc, InstanceName +0x10}, so the component pointer is entry +0x08.
// Label: catalog "Map_CountRangeEntries" is descriptive.
// Shape: the bounded walk `for (i = 0; i < index; ++i) { if (it == last) return 0; ++it; }` then
//   `if (it != last) return <component>; return 0;` -- the positive test keeps the value return on the fall-through and
//   merges both `return 0`s into one block at the function end (`if (it == last) return 0;` splits them).
namespace NParticleEngine {

enum EPSCType { PSC_TYPE_NONE = 0 };

class CPSCBase;
struct CCountBlock;

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
};

class CComponentInstance {
public:
    virtual ~CComponentInstance();
    CCountedPointer<CPSCBase> Component;                    // +0x04
    bool                      Enabled;                      // +0x0c
    unsigned long             InstanceName;                 // +0x10
};

struct CComponentEntry {                                    // pair<EPSCType, CComponentInstance>, 0x18 bytes
    EPSCType           first;                               // +0x00
    CComponentInstance second;                              // +0x04
};

class CComponentMap {                                       // CVectorMap<EPSCType, CComponentInstance>, 0x10 bytes
public:
    CComponentEntry* Start;                                 // +0x0
    CComponentEntry* Finish;                                // +0x4
    CComponentEntry* EndOfStorage;                          // +0x8
    bool             Compare;                               // +0xc
    bool             Dirty;                                 // +0xd
    CComponentEntry* LowerBound(const EPSCType& key);       // 0x00AF60C0
    CComponentEntry* UpperBound(const EPSCType& key);       // 0x00AF6120
};

class CParticleSystem {
public:
    virtual ~CParticleSystem();
    void*         PParentEmitter;                           // +0x04
    void*         PEngine;                                  // +0x08
    CComponentMap Components;                               // +0x0c
    CPSCBase* GetComponentOfType(EPSCType type, unsigned long index);
};

CPSCBase* CParticleSystem::GetComponentOfType(EPSCType type, unsigned long index)
{
    CComponentEntry* it = Components.LowerBound(type);
    CComponentEntry* last = Components.UpperBound(type);
    for (unsigned long i = 0; i < index; ++i) {
        if (it == last)
            return 0;
        ++it;
    }
    if (it != last)
        return it->second.Component.Data;
    return 0;
}

}