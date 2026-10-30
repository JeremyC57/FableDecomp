// CAIStateGroup_UsesSeat::SitInSeat() -- pick the bench or chair sit animation and mode, then perform the sit-down action
// Grade: DIFFER(489v499) nd=108 (was 513v499 nd=173) -- levers: engine/CCountedPointer.h for the sit mode (173->112: its Reset still calls 0x00938070 out of line like retail), the seat flag read into a local as (flags >> 16) & 1 before the filter (112->108); tried: the shift test inline (112); remaining: retail frame 0xcc v 0xc8 with the anim/mode/filter slots in a different order, the filter vtable stored just before its first push, the creature loaded per branch, the release through load/dec/store
// Leaf: SitInSeat
// Retail: Fable.exe (TLC, VC7.1) @ 0x00938380; best of the probes graded under VC7.1 RTM (probe/00938380.cpp).
#include "engine/CCountedPointer.h"
struct CModeSitBase;
#define V4(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3();
#define V16(p) V4(p##0) V4(p##1) V4(p##2) V4(p##3)
struct CThing {
    V16(a) V16(b) V16(c)                        // slots 0..47
    virtual unsigned long GetSeatFlags();       // slot 48 (+0xc0): bit 16 = bench-style seat
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCharString { void* m_pData; };
extern CCharString g_SitBenchAnimName;          // 0x013BB6DC
extern CCharString g_SitChairAnimName;          // 0x013BB6E4
struct CAnimFilterBase { virtual bool Accept(); };
struct CAnimFilter_Sit : CAnimFilterBase {      // vtable 0x0127261C
    virtual bool Accept();
};
struct CRefCountedAnim {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
};
struct CAnimationEntry {
    long m_index;                               // -1
    long m_variant;                             // -1
    CRefCountedAnim* m_pData;
    CAnimationEntry() : m_index(-1), m_variant(-1), m_pData(0) { }
    ~CAnimationEntry() { if (m_pData != 0 && --m_pData->m_refs == 0) m_pData->Destroy(); }
    CAnimationEntry& operator=(const CAnimationEntry& other);   // 0x00471A09
};
struct CModeSitBase;
void operator delete(void* p);                  // 0x00BFE9BC
struct CThingCreatureBase {
    unsigned char _pad[4];
    CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, CAnimFilterBase* filter);   // 0x00662EF0
};
struct CModeSitBase {
    unsigned char _body[0x38];
    CModeSitBase(CThingCreatureBase* creature);   // 0x00740040 (sit-on-chair mode)
};
void* operator new(unsigned int size);
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    CCountedPointer<CModeSitBase> PMode;         // +0xa8
    CCreatureAction_SitDown(CThingCreatureBase* creature, CAnimationEntry* anim, CCountedPointer<CModeSitBase> mode);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    V4(s0) V4(s1) V4(s2)
    virtual void s30(); virtual void s31(); virtual void s32();
    virtual CModeSitBase* CreateSitMode();      // slot 15 (+0x3c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CBaseIntelligentPointer PSeat;              // +0x14
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    float SeatPos[3];                           // +0x24
    bool SittingOnBench;                        // +0x30
    bool SitInSeat();
};

bool CAIStateGroup_UsesSeat::SitInSeat()
{
    CAnimationEntry anim;
    CCountedPointer<CModeSitBase> mode;
    unsigned long bench = (PSeat.GetPItem()->GetSeatFlags() >> 16) & 1;
    CAnimFilter_Sit filter;
    if (bench) {
        anim = *PBrain->PCreature->GetRandomAnimFromNameWithFilter(g_SitBenchAnimName, &filter);
        mode.Reset(CreateSitMode());
        SittingOnBench = false;
    } else {
        anim = *PBrain->PCreature->GetRandomAnimFromNameWithFilter(g_SitChairAnimName, &filter);
        mode.Reset(new CModeSitBase(PBrain->PCreature));
        SittingOnBench = true;
    }
    if (IsStopping())
        return false;
    Sitting = true;
    if (AIAction_PerformAction(&CCreatureAction_SitDown(PBrain->PCreature, &anim, mode)) != 2)
        return false;
    return true;
}
