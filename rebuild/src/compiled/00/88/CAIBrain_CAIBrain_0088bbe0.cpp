// CAIBrain::~CAIBrain() -- AI brain dtor: run OnKill unless it already ran, delete every state group (popping them off the back of the state-group vector map), then the member dtors in reverse order (update zones, brain def, the output's look-at thing and action, the state-group storage) and the tail jump to ~CSuspendableProcessMicrothread
// Leaf: ??1CAIBrain@@UAE@XZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088BBE0, 199 bytes, tail jump to the base destructor.
// Identity: the one vptr store 0x012780C4 carries RTTI `.?AVCAIBrain@@` (rtti_bases: CSuspendableProcessMicrothread at +0);
//   both PDBs have `??1CAIBrain@@UAE@XZ`. The catalog label is right.
// Callees: CAIBrain::OnKill 0x0088B6F0 (landed under that name; both PDBs `?OnKill@CAIBrain@@QAEXXZ`, the OnKillCalled guard
//   here; reloc_bindings also carries an invented `?OnLevelUnload@CAIBrain@@QAEXXZ` for it), each state group's scalar
//   deleting dtor (slot 0, `push 1`), free 0x00BFEA14 (STLport deallocate), CBaseIntelligentPointer::~CBaseIntelligentPointer
//   0x00A01C10 (Output.LookAtThing: CIntelligentPointer<T> has no dtor of its own), operator delete 0x00BFE9BC (the action's
//   count block), CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread 0x00A44620 (both PDBs: protected
//   `??1CSuspendableProcessMicrothread@@MAE@XZ`; reloc_bindings carries UAE). The brain def's last release is its slot 1.
// Layout: Ego_r.pdb CAIBrain with the retail 0x10-byte CVectorMap and 12-byte vector (as engine/CAIBrain.h's retail column
//   and the landed OnKill 0x0088B6F0): StateGroupList +0x28 (8-byte {key, group} pairs), Output +0x44 (CBrainOutput 0x48:
//   PAction +0x20, LookAtThing +0x40), PBrainDef +0x90, UpdateZones +0x94, OnKillCalled +0xa4. The engine header flattens
//   the vptr and the base into data members, so a local polymorphic model is used (NOTES.md Header debt).
// Shape: `while (!StateGroupList.empty()) { delete StateGroupList.back().second; StateGroupList.pop_back(); }`; the def
//   pointer release reads the pointer once (`dec [ecx+4] ; jne`), the counted pointer re-reads Info after each step.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#include "engine/CArray.h"

template <class K, class V> class CKeyPairCompareGreater { public:
};
#include "engine/CVectorMap.h"

class CBaseObjectPointer;
class CBaseIntelligentPointer { public:                     // 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResource : public CIVCountedPointeeBase { public:
};
class CDefPointeeBase : public CResource { public:
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CDefPointer() { Release(); }
};

class C3DVector { public:
    float X, Y, Z;
};
class CThing;
class CCreatureActionBase;
class CNavigationInput { public:                            // 0x18 bytes
    unsigned char _pad_0x00[0x18];
};
class CBrainOutput { public:                                // 0x48 bytes
    unsigned long                       Types;              // +0x00
    long                                LookFlags;          // +0x04
    CNavigationInput                    NavigationInput;    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;           // +0x20
    C3DVector                           MoveToPos;          // +0x28
    C3DVector                           LookAtPos;          // +0x34
    CIntelligentPointer<const CThing>   LookAtThing;        // +0x40
};
class CBrainUpdateZone { public:
    long Distance;                                          // +0x00
    long UpdateFrequency;                                   // +0x04
    long InterruptionFrequency;                             // +0x08
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CASuspendableProcess : public CBaseClassNonCopyable { public:
    bool  Running;                                          // +0x04
    bool  StopRunning;                                      // +0x05
    bool  TerminateUsingExceptions;                         // +0x06
    float TimeProcessSliceStarted;                          // +0x08
    float TimesliceLength;                                  // +0x0c
};
struct MicroThreadInfo;
class CSuspendableProcessMicrothread : public CASuspendableProcess { public:
    MicroThreadInfo* PMicrothread;                          // +0x10
protected:
    virtual ~CSuspendableProcessMicrothread();              // 0x00A44620
};

class CMainGameComponent;
class CThingAICreature;
class CThingManager;
class CPlayerManager;
class CWorld;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();                           // slot 0 (scalar deleting dtor)
};
class CBrainDef;

class CAIBrain : public CSuspendableProcessMicrothread { public:   // vtable 0x012780C4
    CMainGameComponent*                 Component;              // +0x14
    CThingAICreature*                   Creature;               // +0x18
    CThingManager*                      ThingManager;           // +0x1c
    CPlayerManager*                     PlayerManager;          // +0x20
    CWorld*                             World;                  // +0x24
    CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater<long, CAIStateGroupBase*> > StateGroupList;   // +0x28
    CAIStateGroupBase*                  PActiveStateGroup;      // +0x38
    CAIStateGroupBase*                  PLastStateGroup;        // +0x3c
    CAIStateGroupBase*                  PPendingStateGroup;     // +0x40
    CBrainOutput                        Output;                 // +0x44
    bool                                OutputValid;            // +0x8c
    bool                                UpdatingStateGroup;     // +0x8d
    bool                                StateGroupUpdated;      // +0x8e
    bool                                InitialisedCreaturePosition;    // +0x8f
    CDefPointer<const CBrainDef>        PBrainDef;              // +0x90
    std::vector<CBrainUpdateZone>       UpdateZones;            // +0x94
    CBrainUpdateZone*                   PCurrentUpdateZone;     // +0xa0
    bool                                OnKillCalled;           // +0xa4
    void OnKill();                                              // 0x0088B6F0
    virtual ~CAIBrain();
};

CAIBrain::~CAIBrain()
{
    if (!OnKillCalled)
        OnKill();
    while (!StateGroupList.empty()) {
        delete StateGroupList.back().second;
        StateGroupList.pop_back();
    }
}