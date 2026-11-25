// CAIStateGroup_MinionReactToBurglary::IsRunnable() -- during the creature's sleep hours, runnable when the live
// Leaf: ?IsRunnable@CAIStateGroup_MinionReactToBurglary@@UAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// player is visible to it and standing inside its home building; remember the player in PBurglar.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957F20, length 210 bytes (+ int3 padding).
// Callees: CThingAICreature::GetSleepTimes @ 0x00833390 (two out longs), CAIStateGroupBase::GetWorld @ 0x008FCFC0
// (twice), CGameTimeManager::IsTimeBetween @ 0x006BB390 (world +0x1c), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on the creature's home-building pointer (+0x168), the world's player-manager getter @ 0x0049D9D0
// and its player getter @ 0x00487DD0 (manifest "_Cons_val<...>" labels are propagated and wrong),
// CanCreatureSeeCreature @ 0x008645E0, the thing position getter @ 0x004C73D0, CThingBuilding::IsPointInside
// @ 0x0082D000, CIntelligentPointer<const CThing>(const CThing*) @ 0x0056C860, its operator= @ 0x005ED300,
// ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the home pointer read through an inline getter (`lea ecx,[eax+168h]`); the time test result and the home pointer are named locals computed before the combined test (bl / ebp);
// `PBurglar = CIntelligentPointer(player)` through a named out-of-line-constructed pointer + operator=; the clock is a named local (GetPWorld before the argument loads).
// No engine header for this class; PBurglar is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Name fix (agentNF6 2026-11-10): NCreatureAI::IsAwareOfThing 0x008645E0 is ?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z (Ego_r.pdb / FableWin.pdb), was a free CanCreatureSeeCreature(const CThing*, const CThing*, bool).
// Name fix (agentNF54): the stand-ins CWorldClock, CIntelligentPointerOut, CIntelligentPointerThing and CBaseIntelligentPointerCtor are
//   gone. The clock is the shared engine/CGameTimeManager.h (FABLE_RETAIL_CGAMETIMEMANAGER_ISTIMEBETWEEN: Ego_r.pdb 0x006F6B80 + FableWin.pdb
//   0x024607E0 ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z); PBurglar is the shared CIntelligentPointer<const CThing> (engine/
//   CIntelligentPointer.h, out-of-line ctor + operator=: Ego_r.pdb 0x005A6190 + FableWin.pdb 0x01F2AE40
//   ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@PBVCThing@@@Z; FableWin.pdb 0x02057E90 ??4?$CIntelligentPointer@$$CBVCThing@@@@QAEAAV0@ABV0@@Z,
//   whose landed row 0x005ED300 and its constructor's RTTI vtable .?AV?$CIntelligentPointer@$$CBVCThing@@@@ name the instance), and the
//   home pointer the shared CBaseIntelligentPointer (GetPItem 0x00A01B50, ~ 0x00A01C10).
#include "fable_compiler.h"
// NF153: the retail CThing chain (as the self row 0x00487DD0) so the shared engine/CThingPlayerCreature.h gives
//   PeekPControlledCreature()'s return type its CThing base.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
class CTCBase;
class CThing;
class CWorld;
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#include "engine/CIntelligentPointer.h"

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};


#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_ISTIMEBETWEEN
#include "engine/CGameTimeManager.h"

class CThingAICreature { public:
    void GetSleepTimes(long& start, long& end) const; // 0x00833390 (NF35: Ego_r + FableWin ?GetSleepTimes@CThingAICreature@@QBEXAAJ0@Z)
};
struct CThingPos {
};
class CThingBuilding { public:
    bool IsPointInside(const C3DVector& point) const; // 0x0082D000
};
struct CHomeOwner {
    unsigned char _pad[0x168];
    CBaseIntelligentPointer PHome;              // +0x168
    CBaseObject* GetPHome() { return PHome.GetPItem(); }
};

class CPlayer { public:                         // (was the stand-in CPlayerManager: 0x0049D9D0 returns the world's CPlayer)
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0 ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ (Ego_r / FableWin; NF153: was the stand-in GetPlayerCharacter)
};
class CWorld { public:
    unsigned char _pad[0x1c];
    CGameTimeManager* PClock;                   // +0x1c (boost::scoped_ptr<CGameTimeManager> PGameTimeManager in retail)
    const CPlayer* PeekMyPlayer() const;        // 0x0049D9D0 Ego_r.pdb / FableWin.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ
};

class CThingAICreature;
namespace NCreatureAI { bool __fastcall IsAwareOfThing(const CThingAICreature* viewer, const CThing* target, bool ignoreLight); }   // 0x008645E0


struct CAIStateGroup_MinionReactToBurglary : CAIStateGroupBase {
    CIntelligentPointer<const CThing> PBurglar; // +0x14
    virtual bool IsRunnable();
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (was ?GetPosition@CThingPos@@QAEABVC3DVector@@XZ)
bool CAIStateGroup_MinionReactToBurglary::IsRunnable()
{
    long start;
    long end;
    ((CThingAICreature*)PBrain->PCreature)->GetSleepTimes(start, end);
    CGameTimeManager* clock = GetWorld().PClock;
    bool sleepTime = clock->IsTimeBetween(start, end);
    CThingBuilding* home = (CThingBuilding*)((CHomeOwner*)PBrain->PCreature)->GetPHome();
    if (sleepTime && home != 0) {
        const CThing* player = GetWorld().PeekMyPlayer()->PeekPControlledCreature();
        if (player != 0 && !player->IsDead() && NCreatureAI::IsAwareOfThing((const CThingAICreature*)(PBrain->PCreature), player, false) &&
            home->IsPointInside(reinterpret_cast<const CThing*>(((CThingPos*)player))->GetPos())) {
            CIntelligentPointer<const CThing> burglar(player);
            PBurglar = burglar;
            return true;
        }
    }
    return false;
}