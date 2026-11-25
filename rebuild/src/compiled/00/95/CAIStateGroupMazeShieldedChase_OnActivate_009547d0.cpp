// CAIStateGroup_MazeShieldedChase::OnActivate() -- save boss home, set combat target and register a creature opponent.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail009547d0 length294. Shared group/base/brain headers; native boss Home22 is unaligned (packed layout), combat target40 and thing flags6c.
#define FABLE_RETAIL_C3DVECTOR_PACKED   // NF55: CTCBoss::PhaseStartPos at +0x22 (alignment 1, engine/CTCBoss.h)
#include "fable_compiler.h"
#ifdef _MSC_VER
#include "engine/CAIStateGroup_MazeShieldedChase.h"
#define CAIStateGroupBase CAIStateGroupBaseLayout   // engine/CAIStateGroupBase.h flattens the vptr
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#else
#define __forceinline inline
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_MazeShieldedChase:CAIStateGroupBase{void* _pad_0x14[2];char unused[4];long unusedFrame;};
#endif
struct MazeThing;
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct MazeCreaturePointerBase{virtual ~MazeCreaturePointerBase();MazeThing* item;MazeCreaturePointerBase();}; //00a01c10,00a01b10,00a01b50,00a01b90
struct MazeCreaturePointer:MazeCreaturePointerBase{FABLE_FORCEINLINE MazeCreaturePointer(MazeThing* p){((CBaseIntelligentPointer*)this)->SetPItem((CBaseObject*)p);}FABLE_FORCEINLINE MazeCreaturePointer& operator=(const MazeCreaturePointer& p){((CBaseIntelligentPointer*)this)->SetPItem((CBaseObject*)(((MazeThing*)((CBaseIntelligentPointer*)&p)->GetPItem())));return *this;}}; //derived scalar destructor004d6915,vtable0123e7d8
#include "engine/C3DVector.h"
// agentNF55 (round 7): the local boss stand-in (packed, home position +0x22) is gone; the interface-173 component is the shared
// CTCBoss: 0x007AED10 is Ego_r.pdb / FableWin.pdb ?GetBossPhase@CTCBoss@@QBEJXZ and the +0x22 store FableWin's inline
// ?SetPhaseStartPos@CTCBoss@@QAEXABVC3DVector@@@Z (PhaseStartPos +0x22; header opt-in FABLE_RETAIL_CTCBOSS_SETPHASESTARTPOS).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBOSS_SETPHASESTARTPOS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBoss.h"
struct MazeCombat{char base[0x40];MazeCreaturePointer target;};
struct MazeEntry{long key;void* value;};
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in MazeTable (?Find@MazeTable@@QAEPAUMazeEntry@@ABJ@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAE spelling at 0x0040F020.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > MazeTable;
// The stand-in's inline lookups as free helpers over the real CVectorMap (non-const LowerBound 0x0040F020 / const 0x004365B0).
template<int Key> FABLE_FORCEINLINE void* MazeLookup(MazeTable& t){ETCInterfaceType key=(ETCInterfaceType)Key;MazeTable::value_type* p=t.LowerBound(key);if(p==t.end())p=t.end();else if(p->first>Key)p=t.end();return p->second;}
template<int Key> FABLE_FORCEINLINE void* MazeLookupConst(MazeTable& t){const MazeTable& c=t;ETCInterfaceType key=(ETCInterfaceType)Key;const MazeTable::value_type* p=c.LowerBound(key);return (p==c.end() || p->first>Key ? c.end():p)->second;}
class CThing { public:
    const C3DVector& GetPos() const; //004c73d0
};
struct MazeThing : public CThing {char base[0x20];unsigned long masks[9];MazeTable table;char beforeFlags[0x18];unsigned char flags;char beforeDead[0x24];unsigned char dead;
FABLE_FORCEINLINE void GetBoss(CTCBoss** out){if(masks[5]&0x2000)*out=(CTCBoss*)MazeLookup<173>(table);}
FABLE_FORCEINLINE void GetCombat(MazeCombat** out){if(masks[0]&0x100)*out=(MazeCombat*)MazeLookup<8>(table);}
};
class CCombatManager{ public:void RegisterCombatant(MazeThing*,MazeThing*,bool);}; //006edc50
#ifndef _MSC_VER
class CWorld{ public:char prefix[0x4c];CCombatManager* PThingCombatManager_ptr;};
#endif
class CWorld;
class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
struct ShieldMethods : public CAIStateGroupBase, CAIStateGroup_MazeShieldedChase {
FABLE_FORCEINLINE MazeThing* Creature(){return (MazeThing*)((CAIStateGroupBaseLayout*)this)->PBrain->Creature;}
FABLE_FORCEINLINE MazeCreaturePointer& Target(){return *(MazeCreaturePointer*)((CAIStateGroupBaseLayout*)this+1);}
void OnActivate();};
void ShieldMethods::OnActivate(){
CTCBoss* boss;Creature()->GetBoss(&boss);
if(boss->GetBossPhase()==0)boss->SetPhaseStartPos(Creature()->GetPos());
MazeThing* target=((MazeThing*)((CBaseIntelligentPointer*)&(Target()))->GetPItem());MazeCombat* combat;Creature()->GetCombat(&combat);
combat->target=MazeCreaturePointer(target);
if(((MazeThing*)((CBaseIntelligentPointer*)&(Target()))->GetPItem())->flags&8){MazeThing* opponent=((MazeThing*)((CBaseIntelligentPointer*)&(Target()))->GetPItem());MazeThing* actor=Creature();CWorld* world=&GetWorld();world->PThingCombatManager_ptr->RegisterCombatant(actor,opponent,true);}
}