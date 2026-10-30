// CQ_MinionCampScript::AddNewHeroAttacker(const CScriptThing& attacker) -- put a new attacker into the first free hero-attacker slot (null or dead) and give it the hero as its best enemy target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0B30, length 266 bytes, `ret 4`.
// Callees: CScriptThing::IsNull (virtual slot 77, +0x134, 0x004AB150) and IsAlive (slot 75, +0x12c, 0x004AB130) on each
// slot, CScriptBase::IsActiveThreadTerminating 0x00CB7940 (x3), CScriptThing::operator= 0x008AB980 (out of line for the
// second slot, expanded inline for the first), operator delete 0x00BFE9BC; through the game script interface (+0x40):
// GetHero (slot 70, +0x118), GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) (slot 451, +0x70c,
// 0x008A0260).
// Layout: Ego_r.pdb CQ_MinionCampScript - 0x14: HeroAttackerOne +0x78, HeroAttackerTwo +0x84.
// Shape: `if (one is taken) { if (two is taken) { IsActiveThreadTerminating(); return; } ... } else { ... }` puts the
// first-slot assignment last; retail expanded operator= inline at that site only and called the out-of-line copy
// for the second slot (the lane inlines both or neither), so the first-slot site is written through an inline
// Assign() helper (chunk 225 precedent: inline helper at the expanded sites) that forwards to the counted
// pointer's __forceinline operator= -- the member-object form addresses the pointer as [this+0x7c] while the
// slot's own address stays in ebp; a flat operator= body puts this in edi and the slot in esi.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);         // destroys the pointee
    void* Data;
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                // +0x04 Data, +0x08 Info
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
    __forceinline void Assign(const CScriptThing& other) { PImp = other.PImp; }   // the inline expansion of operator=
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                      // slot 75 (+0x12c), 0x004AB130
    virtual bool IsDead() const;                       // slot 76 (+0x130), 0x004AA890
    virtual bool IsNull() const;                       // slot 77 (+0x134), 0x004AB150
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x30];
    CScriptThing          HeroAttackerOne;           // +0x78
    CScriptThing          HeroAttackerTwo;           // +0x84
    void AddNewHeroAttacker(const CScriptThing& attacker);
};
} // namespace NScript
using NScript::CQ_MinionCampScript;

void NScript::CQ_MinionCampScript::AddNewHeroAttacker(const CScriptThing& attacker)
{
    if (!HeroAttackerOne.IsNull() && HeroAttackerOne.IsAlive()) {
        if (!HeroAttackerTwo.IsNull() && HeroAttackerTwo.IsAlive()) {
            IsActiveThreadTerminating();
            return;
        }
        if (IsActiveThreadTerminating())
            return;
        HeroAttackerTwo = attacker;
        m_pInterface->GiveThingBestEnemyTarget(HeroAttackerTwo, m_pInterface->GetHero());
    } else {
        if (IsActiveThreadTerminating())
            return;
        HeroAttackerOne.Assign(attacker);
        m_pInterface->GiveThingBestEnemyTarget(HeroAttackerOne, m_pInterface->GetHero());
    }
}