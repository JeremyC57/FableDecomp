// CAIStateGroup_BanditPlot::Act() -- listen to the bandits' plans for 375..674 frames: now and then (one in five, with a free plot leader slot and two or more plotters) take the lead and act out the sword-on-the-floor plan, otherwise loop the listening animation (one in ten a breakdown); return the first failing action result, else the result of the closing animation
// Grade: DIFFER(1131v1131) -- same length; retail divides Random()%300 before the GFGetCurrentFrame call (VC7.1 here defers the div past the call), so ebx/ebp swap in the frame window and the leader block reloads PBrain->PCreature; string pushes relocation only
// Retail: Fable.exe (TLC, VC7.1) @ 0x009289B0, 1136 bytes, plain ret.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCreatureAction_PlayAnimation(creature, name,
//   sound, bool, bool, bool, long, long) 0x008424F0, ~CCreatureActionBase 0x00693EF0, AIAction_PerformActionAndWait 0x0097DB70,
//   GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, GFFloatRandom 0x004978A0 (__fastcall), GFGetCurrentFrame 0x0049D870,
//   rand 0x00BFEB16, the plot's leader setter 0x00929550, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C). Chance 0.2f @ 0x012917C8.
#include <stdlib.h>
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CThingCreatureBase;
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CBanditPlot {
    unsigned char _pad00[0x0c];
    long NumPlotters;                           // +0x0c
    CIntelligentPointerThing Leader;            // +0x10
    void SetLeader(CThingCreatureBase* creature);   // 0x00929550
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
long GFGetCurrentFrame();                       // 0x0049D870
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    __forceinline float FloatRandom() { unsigned long* seed = GetPWorldSeed(); return GFFloatRandom(seed, 1.0f); }
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
};
struct CAIStateGroup_BanditPlot : CAIStateGroupBase {
    unsigned char _pad14[0x18 - 0x14];
    CBanditPlot* PPlot;                         // +0x18
    long Act();
};

long CAIStateGroup_BanditPlot::Act()
{
    long result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_INTO",
        CSoundPair(), false, true, false, 0, 0), false);
    if (result == 2) {
    unsigned long wait = Random() % 300;
    long endFrame = wait + GFGetCurrentFrame() + 375;
    while (GFGetCurrentFrame() < endFrame) {
        if (PPlot->NumPlotters >= 2 && !PPlot->Leader.GetPItem() && FloatRandom() < 0.2f) {
            PPlot->SetLeader(GetCreature());
            AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_OUTOF",
                CSoundPair(), false, true, false, 0, 0), false);
            AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_INTO",
                CSoundPair(), false, true, false, 0, 0), false);
            for (long i = 2; i != 0; --i)
                AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_LOOP",
                    CSoundPair(), false, true, false, 0, 0), false);
            AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_OUTOF",
                CSoundPair(), false, true, false, 0, 0), false);
            result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_INTO",
                CSoundPair(), false, true, false, 0, 0), false);
            PPlot->Leader = CIntelligentPointerThing(0);
        } else if (rand() % 10 == 0)
            result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_BREAKDOWN",
                CSoundPair(), false, true, false, 0, 0), false);
        else
            result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_LOOP",
                CSoundPair(), false, true, false, 0, 0), false);
        if (result != 2)
            return result;
    }
    result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_OUTOF",
        CSoundPair(), false, true, false, 0, 0), false);
    }
    return result;
}
