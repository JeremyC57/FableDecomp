// CAIStateGroup_WhisperRetreat::UseWill() -- against a live creature target with interface 2, perform (and wait for) an assassin-rush cast whose follow-up action is a new dragon-roar action on the target
// Grade: DIFFER(258v258) nd=22 (was 25; size now equal) -- lever: local counted-pointer model moved onto engine/CCountedPointer.h (cpmove). Tried: IsDead accessor (22), target local as the rush target (20 but 251 bytes), named rush action (42). Residue: argument scheduling -- retail loads the follow-up Data/Info pair before the copy and the creature after the push 0 pair; ours reloads Info from the frame and loads the creature early. Next: try the header copy ctor at a by-value parameter of the ctor via a named copy.
// Leaf: ?UseWill@CAIStateGroup_WhisperRetreat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096AE50, length 258 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0x14), CThing::GetPosition 0x004C73D0 (twice, results
//          unused), operator new 0x00BFEA1A (0x114), CCreatureAction_DragonRoar(CThingCreatureBase*, CThing*) 0x0090A700,
//          CCountedPointer<CCreatureActionBase>(T*) 0x00695880 / ~ 0x0048C5C0, CCreatureAction_CastAssassinRush(
//          CThingCreatureBase*, CThing*, bool, long, CCountedPointer<CCreatureActionBase>) 0x008CE040 / ~ 0x0072D990,
//          CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70.
// Layout: the shared CAIStateGroupBase model; PTarget at +0x14 (CIntelligentPointer<CThing>).
#include "engine/CCountedPointer.h"
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
struct C3DVector { float x, y, z; };
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    unsigned char _pad44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                      // +0x6c bit 3: creature
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const { return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0; }
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct SCountInfo { long RefCount; };
struct CCreatureActionBase { unsigned char _body[0xa8]; };
struct CCreatureAction_DragonRoar : CCreatureActionBase {
    unsigned char _body2[0x114 - 0xa8];
    CCreatureAction_DragonRoar(CThingCreatureBase* creature, CThing* target);   // 0x0090A700
};
struct CCreatureAction_CastAssassinRush : CCreatureActionBase {
    unsigned char _body2[0x138 - 0xa8];
    CCreatureAction_CastAssassinRush(CThingCreatureBase* creature, CThing* target, bool flag, long mode, CCountedPointer<CCreatureActionBase> followUp);   // 0x008CE040
    ~CCreatureAction_CastAssassinRush();                // 0x0072D990
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                   // +0x04
    CThing* GetPItem() const;                          // 0x00A01B50
};
struct CAIBrain {
    unsigned char       _pad[0x18];
    CThingCreatureBase* PCreature;                     // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
    long ScriptAIPriority;                             // +0x08
    void* Name_PStringData;                            // +0x0c
    unsigned char Terminated;                          // +0x10
    unsigned char Priority;                            // +0x11
    unsigned char Enabled;                             // +0x12
    unsigned char _pad_0x13;
    long AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_WhisperRetreat : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;                   // +0x14
    void UseWill();
};

void CAIStateGroup_WhisperRetreat::UseWill()
{
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1) && (PTarget.GetPItem()->ThingTypeFlags & 8)) {
        CThing* target = PTarget.GetPItem();
        bool hasInterface = PTarget.GetPItem()->TestTC(2);
        if (hasInterface) {
            PBrain->PCreature->GetPosition();
            PBrain->PCreature->GetPosition();
            CCountedPointer<CCreatureActionBase> roar(new CCreatureAction_DragonRoar(PBrain->PCreature, target));
            AIAction_PerformActionAndWait(CCreatureAction_CastAssassinRush(PBrain->PCreature, PTarget.GetPItem(), false, 0, roar), false);
        }
    }
}
