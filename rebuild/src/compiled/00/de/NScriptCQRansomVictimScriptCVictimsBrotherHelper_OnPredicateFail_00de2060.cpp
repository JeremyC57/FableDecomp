// NScript::CQ_RansomVictimScript::CVictimsBrotherHelper::OnPredicateFail() -- the entity's predicate failed: when its thing was killed (by anyone), set the parent script's HitEvilBrotherAndProtection to 2
// Leaf: ?OnPredicateFail@CVictimsBrotherHelper@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE2060, 64 bytes (64 with int3 padding), thiscall, plain `ret`.
// Identity (proven): CQ_RansomVictimScript::CVictimsBrotherHelper::OnPredicateFail; evidence: the body is slot 5 (OnPredicateFail, the CSpawnedFuncBase
//   slot the landed entity scripts use) of the RTTI-less vtable 0x012DD21C, which the entity Alloc 0x00DE1FB0 stores; that
//   Alloc is registered by 0x00DE0850 (site 0x00DE0A93) under the name "VictimsBrotherHelper", and the only Ego_r.pdb entity class
//   of that name is CVictimsBrotherHelper@CQ_RansomVictimScript (?Alloc@CVictimsBrotherHelper@CQ_RansomVictimScript@NScript@@...), whose OnPredicateFail is in Ego_r.pdb and FableWin.pdb.
//   The field written (+0x58 of ParentClass) is CQ_RansomVictimScript::HitEvilBrotherAndProtection (Ego_r.pdb +0x6C; the parent script's members sit 0x14 lower in retail (CScriptBase 0x40 vs 0x54)).
// Label: the manifest's "sub_00de2060" is a gap-scan placeholder (family H035: seven entity OnPredicateFail bodies).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the literal "" at 0x0122D70E), CScriptThing::MsgIsKilledBy
//   (virtual slot 18, +0x48; CScriptThing vtable 0x01238C8C), CCharString::~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8 (12 bytes)}, then CVictimsBrotherHelper::ParentClass +0x14 and
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
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimScript {
public:
    class CVictimsBrotherHelper;
    unsigned char _pad000[0x58];
    long HitEvilBrotherAndProtection; // +0x58
};

class CQ_RansomVictimScript::CVictimsBrotherHelper : public CEntityScriptBase {
public:
    CQ_RansomVictimScript*            ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_RansomVictimScript::CVictimsBrotherHelper::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        ParentClass->HitEvilBrotherAndProtection = 2;
}