// CAIStateGroup_MazeTutorialBattleRespondToLightning::IsRunnable() -- choose a living lightning target outside boss phase2 and save hit time.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail00954e30 length262. Shared group/base/brain headers; native component map44,dead91.
#include "fable_compiler.h"
#ifdef _MSC_VER
#include "engine/CAIStateGroup_MazeTutorialBattleRespondToLightning.h"
#define CAIStateGroupBase CAIStateGroupBaseLayout   // engine/CAIStateGroupBase.h flattens the vptr
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#include "engine/CAIBrain.h"
#else
#define __forceinline inline
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_MazeTutorialBattleRespondToLightning:CAIStateGroupBase{void* _pad_0x14[2];char unused[4];long FrameOfLastHit;};
#endif
struct MazeThing;
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct MazePointerBase{virtual ~MazePointerBase();MazeThing* item;MazePointerBase();}; //00a01c10,00a01b10,00a01b50,00a01b90
struct MazePointer:MazePointerBase{FABLE_FORCEINLINE MazePointer(MazeThing* p){((CBaseIntelligentPointer*)this)->SetPItem((CBaseObject*)p);}FABLE_FORCEINLINE MazePointer& operator=(const MazePointer& p){((CBaseIntelligentPointer*)this)->SetPItem((CBaseObject*)(((MazeThing*)((CBaseIntelligentPointer*)&p)->GetPItem())));return *this;}}; //derived scalar destructor004a9710,vtable01238c6c
// agentNF55 (round 7): the local boss stand-in is gone; the interface-173 component is the shared CTCBoss and 0x007AED10 its
// Ego_r.pdb / FableWin.pdb ?GetBossPhase@CTCBoss@@QBEJXZ (engine/CTCBoss.h over the retail CTCBase).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCBoss.h"
struct MazeCombat{virtual ~MazeCombat();
virtual void slot1();
virtual void slot2();
virtual void slot3();
virtual void slot4();
virtual void slot5();
virtual void slot6();
virtual void slot7();
virtual void slot8();
virtual void slot9();
virtual void slot10();
virtual void slot11();
virtual void slot12();
virtual void slot13();
virtual void slot14();
virtual void slot15();
virtual void slot16();
virtual void slot17();
virtual void slot18();
virtual void slot19();
virtual void slot20();
virtual void slot21();
virtual void slot22();
virtual void slot23();
virtual void slot24();
virtual void slot25();
virtual void slot26();
virtual void slot27();
virtual void slot28();
virtual MazeThing* GetTarget(); //slot29
};
struct MazeEntry{long key;void* value;};
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in MazeTable (?Find@MazeTable@@QAEPAUMazeEntry@@ABJ@Z / ?FindConst@MazeTable@@QAEPAUMazeEntry@@ABJ@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAE spelling at 0x0040F020.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > MazeTable;
// The stand-in's inline lookups as free helpers over the real CVectorMap (non-const LowerBound 0x0040F020 / const 0x004365B0).
template<int Key> FABLE_FORCEINLINE void* MazeLookup(MazeTable& t){ETCInterfaceType key=(ETCInterfaceType)Key;MazeTable::value_type* p=t.LowerBound(key);if(p==t.end())p=t.end();else if(p->first>Key)p=t.end();return p->second;}
template<int Key> FABLE_FORCEINLINE void* MazeLookupConst(MazeTable& t){const MazeTable& c=t;ETCInterfaceType key=(ETCInterfaceType)Key;const MazeTable::value_type* p=c.LowerBound(key);return (p==c.end() || p->first>Key ? c.end():p)->second;}
struct MazeThing{char base[0x20];unsigned long masks[9];MazeTable table;char beforeDead[0x3d];unsigned char dead;
FABLE_FORCEINLINE void GetBoss(CTCBoss** out){if(masks[5]&0x2000)*out=(CTCBoss*)MazeLookup<173>(table);}
FABLE_FORCEINLINE void GetCombat(MazeCombat** out){if(masks[0]&0x100)*out=(MazeCombat*)MazeLookupConst<8>(table);}
};
class CWorld { public: static long __fastcall GetFrame(); }; //0049d870
class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
struct LightningMethods : public CAIStateGroupBase, CAIStateGroup_MazeTutorialBattleRespondToLightning {
FABLE_FORCEINLINE MazeThing* Creature(){return (MazeThing*)((CAIStateGroupBaseLayout*)this)->PBrain->Creature;}
bool HasBeenHitByLightning(); //00954c60
bool IsRunnable();};
bool LightningMethods::IsRunnable(){
CTCBoss* boss;Creature()->GetBoss(&boss);if(boss->GetBossPhase()==2)return false;
MazeCombat* combat;Creature()->GetCombat(&combat);MazeThing* target=combat->GetTarget();
if(target&&!(target->dead&1)&&HasBeenHitByLightning()){
FrameOfLastHit=GetWorld().GetFrame();
*(MazePointer*)_pad_0x14=MazePointer(target);
return true;}
return false;
}