// NScript::CV_SickChildScript::CManInLove::OnPredicateFail() -- the man in love is gone: when the hero killed him before getting the reward, give the hero the ingredient object and the killed-someone-for-the-mushroom morality
// Leaf: ?OnPredicateFail@CManInLove@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC8230, 170 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F1A94, which the landed
//   NScript::CV_SickChildScript::CManInLove::Alloc 0x00EC8160 (operator new 0x20) stores (site 0x00EC8194); Ego_r.pdb has ?OnPredicateFail@CManInLove@CV_SickChildScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CManInLove (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E conditional temporary); CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0; CDefString::operator CCharString() (non-const) 0x00415D70; interface slots 121
//   GiveHeroObject, 156 GiveHeroMorality. Data: pGlobalScriptDef 0x0143E90C.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString {                                      // Ego_r.pdb CDefString, 4 bytes
public:
    operator CCharString();                             // 0x00415D70 (non-const twin; the landed 0x008E7040 binds it so)
    operator CCharString() const;                       // 0x00415DD0 (const; the landed 0x0071C7C0 binds it so)
    long _index;
};
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets (agent501: AssassinThreatDistSq +0x720)
public:
    unsigned char _pad0[0x738];
    CDefString IngredientObject;                        // +0x738 (Ego_r +0x794)
    unsigned char _pad73c[0x768 - 0x73c];
    float KilledSomeoneForMushroom;                     // +0x768 (Ego_r +0x7c8, after the 16->12 byte vector SickChildsSisterAttentionAnims)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SickChildScript {
public:
    class CManInLove;
};
class CV_SickChildScript::CManInLove : public CEntityScriptBase {
public:
    CV_SickChildScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool GivenLetter;                                   // +0x1c
    bool GotReward;                                     // +0x1d
    bool HelpedGuyOut;                                  // +0x1e
private:
    virtual void OnPredicateFail();
};

void CV_SickChildScript::CManInLove::OnPredicateFail()
{
    if (!GotReward && Me.MsgIsKilledBy("")) {
        Game->GiveHeroObject(pGlobalScriptDef->IngredientObject, -1, false);
        Game->GiveHeroMorality(pGlobalScriptDef->KilledSomeoneForMushroom);
    }
}
}