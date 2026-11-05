// CAIStateGroup_BirdFlyToFood::IsRunnable() -- runnable for a flying bird that has not eaten for 20 seconds, on a 74% roll: fly to the nearest dead creature (64% roll) or else the nearest BIRD_FOOD thing, if a flight position near it is found
// Leaf: IsRunnable@CAIStateGroup_BirdFlyToFood
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Model: the landed sibling CAIStateGroup_BirdGroundWander::IsRunnable 0x009143E0 (same TU: interface 7 lookup, mode getter,
//   frame-rate local) and GetRandomFlightPos 0x00912700; layout local: FlightPos +0x24 (C3DVector), FrameLastAte +0x30.
// Shape: the two GetPBestThing<Filter, Compare> instantiations have same-TU out-only stand-in bodies that read the
//   comparator's position by value, so neither the out pointer nor the comparator temporary escapes: VC7.1 then stores
//   the out pointer's zero after loading the creature and loads the whole position before the stores (as retail).
class CTCBase;

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern long g_flyingMode;                       // 0x0129CBA4 (the mode compared by BirdGroundWander too)

class CWorld { public:
    static long GetFramesPerSecond() { return CMainGameComponent::GetConstantFPS(); }
public:
    static long __fastcall GetFrame();   // 0x0049D870
};

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};

class CThingFilter_None { public: CThingFilter_None() {} };
struct CThingFilter_DefName {
    CCharString DefName;
    CThingFilter_DefName(const CCharString& name) : DefName(name) {}
};
class CThingCompare_Nearest { public:
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) {}
};
class CThingSearchTools;
CThingPhysical* FindBest_009142B0(CThingSearchTools* tools, float x, float y, float z);
CThingPhysical* FindBest_00914330(CThingSearchTools* tools, const CThingFilter_DefName& filter, float x, float y, float z);
class CThingSearchTools { public:
    bool GetPBestThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingPhysical** out);      // 0x009142B0
    bool GetPBestThing(const CThingFilter_DefName& filter, const CThingCompare_Nearest& compare, CThingPhysical** out);   // 0x00914330
};
#pragma auto_inline(off)
// Same-TU out-only stand-ins for the two GetPBestThing<Filter, Compare> instantiations (compiled in the caller's TU):
// they only write the out pointer, so VC7.1 knows the caller's out local does not escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingPhysical** out)
{
    CThingPhysical* best = FindBest_009142B0(this, compare.Position.X, compare.Position.Y, compare.Position.Z);
    if (best == 0)
        return false;
    *out = best;
    return true;
}
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_DefName& filter, const CThingCompare_Nearest& compare, CThingPhysical** out)
{
    CThingPhysical* best = FindBest_00914330(this, filter, compare.Position.X, compare.Position.Y, compare.Position.Z);
    if (best == 0)
        return false;
    *out = best;
    return true;
}
#pragma auto_inline(on)

long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

class CTCCreatureModeManager { public:                 // interface 7
    long GetCurrentModeType();                  // 0x006CB6A0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingPhysical* Creature;                   // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    unsigned char _14[0x10];
    C3DVector FlightPos;                        // +0x24
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector* out, bool allowGround);   // 0x00912700
};
struct CAIStateGroup_BirdFlyToFood : CAIStateGroup_FlyingThingBase {
    unsigned long FrameLastAte;                 // +0x30
    bool IsRunnable();
};

bool CAIStateGroup_BirdFlyToFood::IsRunnable()
{
    CTCCreatureModeManager* modes;
    PBrain->Creature->GetTC(TCI_NAVIGATION, &modes);
    if (modes->GetCurrentModeType() == g_flyingMode) {
        long framesPerSecond;
        if ((FrameLastAte == 0
             || (framesPerSecond = PeekWorld().GetFramesPerSecond(),
                 CWorld::GetFrame() - FrameLastAte > (unsigned long)(framesPerSecond * 20)))
            && WorldRandomRange(*GetPWorldSeed(), 100) < 74) {
            if (WorldRandomRange(*GetPWorldSeed(), 100) < 64) {
                CThingPhysical* creature = PBrain->Creature;
                CThingPhysical* corpse = 0;
                if (PeekThingSearchTools()->GetPBestThing(CThingFilter_None(), CThingCompare_Nearest(creature->PeekPos()), &corpse)
                    && GetRandomFlightPos(corpse->PeekPos(), 0.5f, &FlightPos, true))
                    return true;
            }
            CThingPhysical* food = 0;
            bool found;
            {
                CCharString name("BIRD_FOOD");
                found = PeekThingSearchTools()->GetPBestThing(CThingFilter_DefName(name), CThingCompare_Nearest(PBrain->Creature->PeekPos()), &food);
            }
            if (found && GetRandomFlightPos(food->PeekPos(), 1.5f, &FlightPos, true))
                return true;
        }
    }
    return false;
}