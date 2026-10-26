// NScript::CV_SickChildScript::CMansLover::OnPredicateFail() -- the entity's predicate failed: when its thing was killed (by anyone), set the parent script's MansLoverState to 3
// Leaf: ?OnPredicateFail@CMansLover@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC9CD0, 64 bytes (64 with int3 padding), thiscall, plain `ret`.
// Identity (proven): CV_SickChildScript::CMansLover::OnPredicateFail; evidence: the body is slot 5 (OnPredicateFail, the CSpawnedFuncBase
//   slot the landed entity scripts use) of the RTTI-less vtable 0x012F1D74, which the entity Alloc 0x00EC9C00 stores; that
//   Alloc is registered by 0x00EC54E0 (site 0x00EC5720) under the name "MansLover", and the only Ego_r.pdb entity class
//   of that name is CMansLover@CV_SickChildScript (?Alloc@CMansLover@CV_SickChildScript@NScript@@...), whose OnPredicateFail is in Ego_r.pdb and FableWin.pdb.
//   The field written (+0x6C of ParentClass) is CV_SickChildScript::MansLoverState (Ego_r.pdb +0x80; the parent script's members sit 0x14 lower in retail (CScriptBase 0x40 vs 0x54)).
// Label: the manifest's "sub_00ec9cd0" is a gap-scan placeholder (family H035: seven entity OnPredicateFail bodies).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the literal "" at 0x0122D70E), CScriptThing::MsgIsKilledBy
//   (virtual slot 18, +0x48; CScriptThing vtable 0x01238C8C), CCharString::~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8 (12 bytes)}, then CMansLover::ParentClass +0x14 and
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
class CV_SickChildScript {
public:
    class CMansLover;
    unsigned char _pad000[0x6C];
    long MansLoverState;            // +0x6C
};

class CV_SickChildScript::CMansLover : public CEntityScriptBase {
public:
    CV_SickChildScript*            ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CV_SickChildScript::CMansLover::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        ParentClass->MansLoverState = 3;
}