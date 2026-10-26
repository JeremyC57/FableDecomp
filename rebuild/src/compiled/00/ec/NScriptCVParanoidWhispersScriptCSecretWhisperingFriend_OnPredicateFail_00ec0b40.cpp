// NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::OnPredicateFail() -- the entity's predicate failed: when its thing was killed (by anyone), set the parent script's ParanoidState to 5
// Leaf: ?OnPredicateFail@CSecretWhisperingFriend@CV_ParanoidWhispersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC0B40, 64 bytes (64 with int3 padding), thiscall, plain `ret`.
// Identity (proven): CV_ParanoidWhispersScript::CSecretWhisperingFriend::OnPredicateFail; evidence: the body is slot 5 (OnPredicateFail, the CSpawnedFuncBase
//   slot the landed entity scripts use) of the RTTI-less vtable 0x012F11F8, which the entity Alloc 0x00EC0A50 stores; that
//   Alloc is registered by 0x00EBEF80 (site 0x00EBF033) under the name "SecretWhisperingFriend", and the only Ego_r.pdb entity class
//   of that name is CSecretWhisperingFriend@CV_ParanoidWhispersScript (?Alloc@CSecretWhisperingFriend@CV_ParanoidWhispersScript@NScript@@...), whose OnPredicateFail is in Ego_r.pdb and FableWin.pdb.
//   The field written (+0x48 of ParentClass) is CV_ParanoidWhispersScript::ParanoidState (Ego_r.pdb +0x5C; the parent script's members sit 0x14 lower in retail (CScriptBase 0x40 vs 0x54)).
// Label: the manifest's "sub_00ec0b40" is a gap-scan placeholder (family H035: seven entity OnPredicateFail bodies).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the literal "" at 0x0122D70E), CScriptThing::MsgIsKilledBy
//   (virtual slot 18, +0x48; CScriptThing vtable 0x01238C8C), CCharString::~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8 (12 bytes)}, then CSecretWhisperingFriend::ParentClass +0x14 and
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
class CV_ParanoidWhispersScript {
public:
    class CSecretWhisperingFriend;
    unsigned char _pad000[0x48];
    long ParanoidState;             // +0x48
};

class CV_ParanoidWhispersScript::CSecretWhisperingFriend : public CEntityScriptBase {
public:
    CV_ParanoidWhispersScript*            ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        ParentClass->ParanoidState = 5;
}