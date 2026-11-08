// CAIStateGroup_MinionLeaveRegion::Update() -- leave through exit, suspend in limbo, then relocate to a random entrance
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail0x0091E4B0,273 bytes. Callees MoveToPos97EC00,SetInLimbo4C8CF0,DoNothing97E200,
// GetPWorld8FCFC0,GetAllRegionNodes91E660,GetPWorldSeed8FCFD0,GFROR13497890,GetPosition4C73D0,Terminate8FCF40.
#include "fable_compiler.h"
#define CAIStateGroup_MinionLeaveRegion CAIStateGroup_MinionLeaveRegionLayout
#include "engine/CAIStateGroup_MinionLeaveRegion.h"
#undef CAIStateGroup_MinionLeaveRegion

class CWorld;
class CAIStateGroupBase
{
public:
    void Terminate();   // 0x008FCF40
protected:
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
class CAIStateGroup_MinionLeaveRegion : public CAIStateGroupBase, public CAIStateGroup_MinionLeaveRegionLayout
{
};
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include <stdlib.h>
#include "engine/C3DVector.h"
struct CThingBody {
 virtual void v00();
 virtual void v01();
 virtual void v02();
 virtual void v03();
 virtual void v04();
 virtual void v05();
 virtual void v06();
 virtual void v07();
 virtual void v08();
 virtual void v09();
 virtual void v10();
 virtual void v11();
 virtual void v12();
 virtual void v13();
 virtual void v14();
 virtual void v15();
 virtual void v16();
 virtual void v17();
 virtual void v18();
 virtual void v19();
 virtual void v20();
 virtual void v21();
 virtual void v22();
 virtual void v23();
 virtual void v24();
 virtual void v25();
 virtual void v26();
 virtual void v27();
 virtual void v28();
 virtual void v29();
 virtual void v30();
 virtual void SetPosition(const C3DVector&); };
class CThing { public: unsigned char prefix[0x60]; CThingBody* Body; void SetInLimbo(bool); const C3DVector& GetPos() const; /* 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb) */ };
struct CBrainVirtual { virtual void v0(); virtual void v1(); virtual bool SuspendSuspendableProcess(); };
struct ThingVector { CThing** first; CThing** last; CThing** capacity; ThingVector():first(0),last(0),capacity(0){}  };
struct CThingFilter_IsRegionNode { bool Exit; CThingFilter_IsRegionNode(bool exit):Exit(exit){} };
struct CThingSearchToolsMethods : CThingSearchTools { void GetAllRegionNodes(const CThingFilter_IsRegionNode&,ThingVector*); };
long __fastcall GFROR13(unsigned long);   // 0x00497890
struct CAIStateGroup_MinionLeaveRegion_Adapter : CAIStateGroup_MinionLeaveRegion {
 CAIBrain* Brain() { return *(CAIBrain**)&_pad_0x04[0]; }
 C3DVector& ExitPosition() { return *(C3DVector*)&ExitPoint_X; }
 long AIAction_MoveToPos(long,C3DVector,bool*,long,bool,bool,CThing*,bool,float);
 long AIAction_DoNothing();  unsigned long* GetPWorldSeed(); void Update();
 FABLE_FORCEINLINE void GetNodes(const CThingFilter_IsRegionNode& filter,ThingVector* nodes) { CWorld* world=&GetWorld(); ((CThingSearchToolsMethods*)world->PThingSearchTools_ptr)->GetAllRegionNodes(filter,nodes); }
};
void CAIStateGroup_MinionLeaveRegion_Adapter::Update() {
 bool reached;
 if(AIAction_MoveToPos(1,ExitPosition(),&reached,3,true,true,0,true,0.0f)!=2) { Terminate(); return; }
 ((CThing*)Brain()->Creature)->SetInLimbo(true);
 ((CBrainVirtual*)Brain())->SuspendSuspendableProcess();
 while(AIAction_DoNothing()==4) {}
 ((CThing*)Brain()->Creature)->SetInLimbo(false);
 ThingVector nodes;
 GetNodes(CThingFilter_IsRegionNode(false),&nodes);
 unsigned long count=nodes.last-nodes.first;
 CThing** first=nodes.first;
 unsigned long* seed=GetPWorldSeed();
 *seed=*seed*0x24a1+0x24df;
 *seed=GFROR13(*seed);
 unsigned long index=count?*seed%count:0;
 CThing* chosen=first[index];
 ((CThing*)Brain()->Creature)->Body->SetPosition(chosen->GetPos());
 Terminate();
 if(first) free(first);
}