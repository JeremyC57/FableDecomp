// CCreatureAction_BanditKingStuck::IsToCauseRecoil() const -- recoil unless the live target is in front of the creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C80E0, length 77 bytes. Vtable slot 46 of CCreatureAction_BanditKingStuck (0x01281C54).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8 twice for the alive test, then PCreature at
// +8 and PTarget again for the comparison), CompareActorRotations(CThingCreatureBase*, CThing*) @ 0x008C7E50 (__fastcall
// free function: creature in ecx, target in edx; bool).
// Shape: the landed IS_THING_ALIVE idiom
// (pointer set, dead bit +0x91 bit 0 clear), then the creature in a named local (fetched BEFORE the target argument:
// a plain argument list evaluates right to left and would fetch the target first), the compare's false path falls
// through to `mov al,1`, its true path returns `xor al,al` -- a bool result. PTarget is the CCombatActionBase
// member at +0xa8 (retail CCreatureActionBase is 0xa8 bytes; the PDB build has it at +0xb4).
class CThingCreatureBase;

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};

bool __fastcall CompareActorRotations(CThingCreatureBase* creature, CThing* target);   // 0x008C7E50 (manifest name, unique;
                                            // atan2 of the position delta against the creature's facing at +0x60)

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10 rest of CCreatureActionBase (0xa8 bytes in retail)
    CIntelligentPointer<CThing> PTarget;        // +0xa8 CIntelligentPointer<CThing>
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
};

class CCreatureAction_BanditKingStuck : public CCombatActionBase { public:   // vtable 0x01281C54
    bool IsToCauseRecoil() const;
};

bool CCreatureAction_BanditKingStuck::IsToCauseRecoil() const
{
    if (IS_THING_ALIVE(PTarget)) {
        CThingCreatureBase* creature = (CThingCreatureBase*)PCreature.GetPItem();
        if (CompareActorRotations(creature, PTarget.GetPItem()))
            return false;
    }
    return true;
}