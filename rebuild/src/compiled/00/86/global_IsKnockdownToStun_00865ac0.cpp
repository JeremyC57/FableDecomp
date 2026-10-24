// IsKnockdownToStun(const CThingCreatureBase&) -- a knockdown stuns when the combat type allows it, the creature is killable, not player-controlled and its health is below the stunned threshold
// Leaf: ?IsKnockdownToStun@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
// Retail 0x00865AC0, 0x50 bytes, __fastcall (creature in ecx). PDB (Ego_r): ?IsKnockdownToStun@?A0x2d832f72@@YI_NABVCThingCreatureBase@@@Z
// (a file-static of the creature combat-response TU, next to the landed CCombatActionGenericStrikeResponse* rows).
// Callees: CThingCreatureBase::GetCombatTypeDef() const @ 0x00666D10, CThingCreatureBase vtable slot 77 (+0x134; the
// CThingPlayerCreature body returns true, CThingAICreature's false -- descriptive name IsPlayerControlled),
// CThingCreatureBase::GetStunnedHealthThreshold() const @ 0x00662030 (reads the creature def's +0x78 float; the manifest's
// "CTCPhysicsBase::GetAirResistance" is a propagation).
// Layout: CCombatTypeDef::KnockdownCausesStun retail +0x95 (Ego_r +0xA9 less the shrunk STL members); CThingPhysical retail
// Health +0xB4, Killable bit 1 of +0xBC (Ego_r Health +0xCC / bitfields +0xD4, CThing 0x10 smaller in retail). The engine
// CThingPhysical.h places Scale at +0xB4 (unshifted Ego_r offset; its own f9c note is the retail Scale), so the creature is
// modelled locally.
// Shape: three guards with a shared `xor al,al` exit; the health is read into a named float local BEFORE the threshold
// call (`mov ecx,[esi+0B4h] ; mov [esp+4],ecx`), then `health < threshold` (`fcomp [esp+4] ; test ah,41h ; jne`).
class CCombatTypeDef
{
public:
    unsigned char _pad0[0x95];
    bool          KnockdownCausesStun;               // +0x95
};

class CThingCreatureBase                             // polymorphic: the vptr is +0x00
{
public:
    unsigned char _pad4[0xb4 - 4];
    float         Health;                            // +0xB4
    unsigned char _padb8[0xbc - 0xb8];
    unsigned char Damageable : 1;                    // +0xBC bit 0
    unsigned char Killable : 1;                      // +0xBC bit 1

    const CCombatTypeDef* GetCombatTypeDef() const;  // 0x00666D10
    float GetStunnedHealthThreshold() const;         // 0x00662030
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76();
    virtual bool IsPlayerControlled() const;         // slot 77 (+0x134)
};

#pragma auto_inline(off)
namespace {
bool __fastcall IsKnockdownToStun(const CThingCreatureBase& creature)
{
    if (creature.GetCombatTypeDef()->KnockdownCausesStun && creature.Killable && !creature.IsPlayerControlled())
    {
        float health = creature.Health;
        if (health < creature.GetStunnedHealthThreshold())
            return true;
    }
    return false;
}
}
#pragma auto_inline(on)

bool __fastcall CallIsKnockdownToStun(const CThingCreatureBase& creature)
{
    return IsKnockdownToStun(creature);
}