// NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::OnPredicateFail() -- the entity's predicate failed: when its thing was killed (by anyone), record it in the master data (BreakSiegeKilledLeader = true)
// Leaf: ?OnPredicateFail@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1DD80, 64 bytes (64 with int3 padding), thiscall, plain `ret`.
// Identity (proven): CQ_BreakSiegeScript::CBanditSiegeLeader::OnPredicateFail; evidence: the body is slot 5 (OnPredicateFail, the CSpawnedFuncBase
//   slot the landed entity scripts use) of the RTTI-less vtable 0x012CAEE4, which the entity Alloc 0x00D1DCC0 stores; that
//   Alloc is registered by 0x00D1D100 (site 0x00D1D365) under the name "BanditSiegeLeader", and the only Ego_r.pdb entity class
//   of that name is CBanditSiegeLeader@CQ_BreakSiegeScript (?Alloc@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@...), whose OnPredicateFail is in Ego_r.pdb and FableWin.pdb.
//   The field written (+0x117 of PMasterData) is CQ_SunnyvaleMasterData::BreakSiegeKilledLeader (Ego_r.pdb, offsets identical in retail).
// Label: the manifest's "sub_00d1dd80" is a gap-scan placeholder (family H035: seven entity OnPredicateFail bodies).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the literal "" at 0x0122D70E), CScriptThing::MsgIsKilledBy
//   (virtual slot 18, +0x48; CScriptThing vtable 0x01238C8C), CCharString::~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8 (12 bytes)}, then CBanditSiegeLeader::ParentClass +0x14 and
//   PMasterData +0x18. Local models -- Review at landing.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CScriptThing {                                    // vtable 0x01238C8C; slots 0..17 are not used here
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    long Data[2];
};

class CGameScriptInterfaceBase;

class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
private:
    virtual void OnPredicateFail();                     // slot 5 (+0x14)
public:
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x117];
    bool          BreakSiegeKilledLeader;      // +0x117
};
class CQ_BreakSiegeScript {
public:
    class CBanditSiegeLeader;
};

class CQ_BreakSiegeScript::CBanditSiegeLeader : public CEntityScriptBase {
public:
    CQ_BreakSiegeScript*            ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        PMasterData->BreakSiegeKilledLeader = true;
}