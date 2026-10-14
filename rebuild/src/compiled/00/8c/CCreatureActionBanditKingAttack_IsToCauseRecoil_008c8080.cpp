// CCreatureAction_BanditKingAttack::IsToCauseRecoil() const -- recoil unless the force flag is clear and the live target is in front of the creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C8080, length 87 bytes. Vtable slot 46 of CCreatureAction_BanditKingAttack (0x01281B0C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8 twice for the alive test, then PCreature at
// +8 and PTarget again for the comparison), CompareActorRotations(CThingCreatureBase*, CThing*) @ 0x008C7E50 (__fastcall
// free function: creature in ecx, target in edx; bool).
// Shape: the ForceCauseRecoil byte (`mov al ; test al,al`) short-circuits to true first; the landed IS_THING_ALIVE idiom
// (pointer set, dead bit +0x91 bit 0 clear), then the creature in a named local (fetched BEFORE the target argument:
// a plain argument list evaluates right to left and would fetch the target first), the compare's false path falls
// through to `mov al,1`, its true path returns `xor al,al` -- a bool result. PTarget is the CCombatActionBase
// member at +0xa8 (retail CCreatureActionBase is 0xa8 bytes; the PDB build has it at +0xb4).
struct CThingCreatureBase;

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};

bool __fastcall CompareActorRotations(CThingCreatureBase* creature, CThing* target);   // 0x008C7E50 (manifest name, unique;
                                            // atan2 of the position delta against the creature's facing at +0x60)

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10 rest of CCreatureActionBase (0xa8 bytes in retail)
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing>
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
};

struct CCreatureAction_BanditKingAttack : CCombatActionBase {   // vtable 0x01281B0C
    unsigned char FaceTarget;               // +0x114 (engine header: bool at PDB +0x12c)
    unsigned char ForceCauseRecoil;         // +0x115 (PDB +0x12d)
    bool IsToCauseRecoil() const;
};

bool CCreatureAction_BanditKingAttack::IsToCauseRecoil() const
{
    if (!ForceCauseRecoil) {
        if (IS_THING_ALIVE(PTarget)) {
            CThingCreatureBase* creature = (CThingCreatureBase*)PCreature.GetPItem();
            if (CompareActorRotations(creature, PTarget.GetPItem()))
                return false;
        }
    }
    return true;
}