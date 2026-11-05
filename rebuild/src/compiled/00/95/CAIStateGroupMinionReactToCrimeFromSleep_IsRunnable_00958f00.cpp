// CAIStateGroup_MinionReactToCrimeFromSleep::IsRunnable() -- never runnable itself: when the creature is ready to sleep and the awake crime reaction would run, it clears the sleep flag so that reaction takes over
// Leaf: ?IsRunnable@CAIStateGroup_MinionReactToCrimeFromSleep@@UAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958F00, 91 bytes (+ int3 padding), thiscall virtual (slot 11), `ret`.
// Identity (proven): Ego_r.pdb ?IsRunnable@CAIStateGroup_MinionReactToCrimeFromSleep@@UAE_NXZ (0x0097DBC0); RTTI
//   CAIStateGroup_MinionReactToCrimeFromSleep : CAIStateGroup_MinionReactToCrime; labels_rtti_port.tsv agrees.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (inlined GetInterface, interface 0xD4 = CTCSleep,
//   bitset word 6 bit 20), CAIStateGroup_MinionReactToCrime::IsRunnable() 0x00958CB0 (the base, called non-virtually).
// Layout: Ego_r.pdb CTCSleep IsReadyToSleep +0x0C (bool).
// Shape: the landed sibling CAIStateGroup_DeedReactionRootFromSleep::IsRunnable (0x00960F00): the flag copied into a bool
//   local that starts false and gets its own `if (!ready) return false;`; the component kept in esi for the final store.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"

class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CWorld;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
class CTCSleep {                                        // interface 0xD4
public:
    unsigned char _pad00[0x0C];
    bool IsReadyToSleep;                                // +0x0C
    bool FastTrack;                                     // +0x0D
};
class CAIStateGroup_MinionReactToCrime : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();                          // 0x00958CB0
};
class CAIStateGroup_MinionReactToCrimeFromSleep : public CAIStateGroup_MinionReactToCrime {
public:
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToCrimeFromSleep::IsRunnable()
{
    CTCSleep* sleep;
    bool readyToSleep = false;
    if (PBrain->Creature->GetTC(TCI_SLEEP, &sleep))
        readyToSleep = sleep->IsReadyToSleep;
    if (!readyToSleep)
        return false;
    if (CAIStateGroup_MinionReactToCrime::IsRunnable())
        sleep->IsReadyToSleep = false;
    return false;
}