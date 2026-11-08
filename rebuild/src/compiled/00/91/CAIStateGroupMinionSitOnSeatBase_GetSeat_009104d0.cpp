// CAIStateGroup_MinionSitOnSeatBase::GetSeat(CThing* owner) -- gather the seats with space owned by the owner and take a random one as the seat
// Leaf: ?GetSeat@CAIStateGroup_MinionSitOnSeatBase@@MAEXPAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009104D0, 152 bytes, `ret 4`; Ego_r.pdb ?GetSeat@CAIStateGroup_MinionSitOnSeatBase@@MAEXPAVCThing@@@Z
//   (protected virtual, vtable slot 18).
// Callees: CAIStateGroupBase::PeekThingSearchTools() const 0x008FD000, CThingSearchTools::GetAllLeafThings<CThingObject,
//   vector<CThingObject*>, CIsSeatWithSpaceOwnedBy> 0x00910A90 (Ego_r.pdb), CAIStateGroupBase::PeekWorld() const 0x008FCFA0,
//   CWorld::GetWorldSeed() const 0x0049D810, GFROR13 0x00497890, CAIStateGroup_UsesSeat::SetSeat(CThingObject*) 0x009106E0,
//   free 0x00BFEA14 (~_Vector_base).
// Layout: CAIStateGroup_UsesSeat (Ego_r.pdb: PSeat +0x14, GotSeatPos +0x1C, Sitting +0x1D, SeatPosIndex +0x20, SeatPos +0x24,
//   SittingOnBench +0x30), CAIStateGroup_MinionSitOnSeatBase::POwner +0x34; no engine headers model them (the generated ones are flat).
// Shape: the GetAllLeafThings body is visible in this TU (overlay FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY, noinline), so VC7.1 knows the
//   seats vector is not captured and keeps its start in ebp across SetSeat (the free test); the random index is the inline
//   GFRandom(range, seed) (Ego_r.pdb ?GFRandom@@YIKKPAK@Z, landed 0x008EE600 model) with its `range ? .. : 0` folded by the count test.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"

long __fastcall GFROR13(unsigned long value);                       // 0x00497890 ?GFROR13@@YIJK@Z
// Ego_r.pdb ?GFRandom@@YIKKPAK@Z, expanded in retail (no out-of-line copy; landed 0x008EE600 model).
FABLE_FORCEINLINE unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CIsSeatWithSpaceOwnedBy {                                     // Ego_r.pdb: ctor (const CThing*), operator()(const CThingObject&) const
public:
    CIsSeatWithSpaceOwnedBy(const CThing* owner) : Owner(owner) { }
    bool operator()(const CThingObject& seat) const;
private:
    const CThing* Owner;
};

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {
protected:
    void SetSeat(CThingObject* seat);                               // 0x009106E0
    CIntelligentPointer<CThingObject> PSeat;                        // +0x14
    bool GotSeatPos;                                                // +0x1c
    bool Sitting;                                                   // +0x1d
    long SeatPosIndex;                                              // +0x20
    C3DVector SeatPos;                                              // +0x24
    bool SittingOnBench;                                            // +0x30
};

class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat {
protected:
    virtual void GetSeat(CThing* owner);
    CIntelligentPointer<CThing> POwner;                             // +0x34
};

void CAIStateGroup_MinionSitOnSeatBase::GetSeat(CThing* owner)
{
    std::vector<CThingObject*> seats;
    PeekThingSearchTools().GetAllLeafThings<CThingObject>(CIsSeatWithSpaceOwnedBy(owner), seats);
    unsigned long count = seats.size();
    if (count != 0)
        SetSeat(seats[GFRandom(count, PeekWorld().GetWorldSeed())]);
}