# h27_main.py: naming wave 27, family main (DECISIONS_W26_RESOLVED D1) -- the NScript entity Main rows on the PDB nesting with
#  Main a private virtual (EAE): ?Main@<entity>@<script>@NScript@@EAEXXZ (Ego_r.pdb + FableWin.pdb). The row-local
#  CEntityScriptBase carries the retail vtable 0x012C3224 slot list (engine/CEntityScriptBase.h: ~ 0, Main 1, Init 2,
#  GetParentScript 3, OnPersist 4, OnPredicateFail 5, OnInterrupted 6), so the entity's vptr is the base's (no `void* VTable`
#  member); the script class moves into NScript (PDB) and its callees take their PDB spellings.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
VS = ('    virtual void Main() = 0;                            // slot 1 (vtable 0x012C3224, engine/CEntityScriptBase.h)\n'
      '    virtual void Init();                                // slot 2  0x00CDEBB0\n'
      '    virtual CScriptBase* GetParentScript() const = 0;   // slot 3\n'
      '    virtual void OnPersist(CPersistContext&);           // slot 4  0x00CDEBC0\n'
      '    virtual void OnPredicateFail();                     // slot 5  0x00CDEBD0\n'
      '    virtual void OnInterrupted();                       // slot 6  0x00CDEBE0\n')
FWD = 'class CScriptBase; class CPersistContext;\n'
NOTE = ('// Naming wave 27 (family main, DECISIONS_W26 D1): Ego_r.pdb / FableWin.pdb nest the entity in its script class and spell\n'
        '//   Main a private virtual (%s); the local CEntityScriptBase carries the retail slot list of vtable 0x012C3224\n'
        '//   (engine/CEntityScriptBase.h). Was: %s.')
def note(a, leaf, was):
    p, rel, t = he._rw(a, 'src')
    if 'family main' in t: return
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    L[i[0]] = '// Leaf: ' + leaf
    L.insert(i[0] + 1, NOTE % (leaf, was)); he._w(p, '\n'.join(L))

# ---- 0x00D9F270 CQ_MinionCampScript::CMC_Minion (already nested by wave 26)
a = '00d9f270'
he.edit(a, 'src', '''class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''', FWD + '''class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''' + VS)
he.edit(a, 'src', '''    void*                PMasterData;                   // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};''', '''    void*                PMasterData;                   // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();                                // private virtual (Ego_r / FableWin EAEXXZ)
};''')
note(a, '?Main@CMC_Minion@CQ_MinionCampScript@NScript@@EAEXXZ', 'public non-virtual ?Main@CMC_Minion@CQ_MinionCampScript@NScript@@QAEXXZ')

# ---- 0x00EE76F0 CV_TrophyDealerScript::CTrophyDealerInCave
a = '00ee76f0'
he.edit(a, 'src', '''class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''', FWD + '''class CEntityScriptBase { public:                       // global PDB class, retail 0x14 bytes
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''' + VS)
he.edit(a, 'src', '''class CV_TrophyDealerScript;
class CQ_SunnyvaleMasterData;
struct CTrophyDealerInCave : CEntityScriptBase {
    CV_TrophyDealerScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    void Main();
};

void CTrophyDealerInCave::Main()''', '''class CV_TrophyDealerScript { public:
    struct CTrophyDealerInCave;                         // Ego_r.pdb / FableWin.pdb: nested entity script
};
class CQ_SunnyvaleMasterData;
struct CV_TrophyDealerScript::CTrophyDealerInCave : CEntityScriptBase {
    CV_TrophyDealerScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void Main();                                // private virtual (Ego_r / FableWin EAEXXZ)
};

void CV_TrophyDealerScript::CTrophyDealerInCave::Main()''')
note(a, '?Main@CTrophyDealerInCave@CV_TrophyDealerScript@NScript@@EAEXXZ', '?Main@CTrophyDealerInCave@NScript@@QAEXXZ (not nested, public non-virtual)')

# ---- the rows whose local CEntityScriptBase is an empty accessor holder: it takes the vptr, the entity drops `void* VTable`
EMPTY_A = '''class CEntityScriptBase { public:   // the PDB base the accessor belongs to (empty here: offset 0)
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
'''
FULL_A = FWD + '''class CEntityScriptBase { public:   // global PDB class (Game / Me are declared on the entity in this row's model)
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''' + VS + '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
'''
# 0x00E0E2E0 CQ_UndeadRisingScript::CUndeadRisingZombieAttackerBoss; the script class is NScript's, derived from CScriptBase
#  (engine/CScriptBase.h: AddSpawnedFunction 0x00CB7E50, the Ego_r spelling bound by 0x00CDD380; was a row-local Add)
a = '00e0e2e0'
he.edit(a, 'src', '''struct CQ_UndeadRisingScript {
    unsigned char _pad00[0x51];
    bool          BossCutscenePlayed;                   // +0x51
    void LookAfterHealth(CScriptThing thing);           // 0x00E0DBB0
    void Add(CSpawnedFuncBase* function, const CCharString& name);   // 0x00CB7E50
};
typedef void (CQ_UndeadRisingScript::*SpawnedThingCallback)(CScriptThing);''', '''#include "engine/CScriptBase.h"                       // : CBaseClassNonCopyable, 0x40; AddSpawnedFunction 0x00CB7E50
namespace NScript {
class CQ_UndeadRisingScript : public CScriptBase { public:   // Ego_r.pdb / FableWin.pdb NScript::CQ_UndeadRisingScript
    unsigned char _pad40[0x51 - 0x40];
    bool          BossCutscenePlayed;                   // +0x51
    void LookAfterHealth(CScriptThing thing);           // 0x00E0DBB0
    struct CUndeadRisingZombieAttackerBoss;             // nested entity script
};
}
using NScript::CQ_UndeadRisingScript;
typedef void (CQ_UndeadRisingScript::*SpawnedThingCallback)(CScriptThing);''')
he.edit(a, 'src', EMPTY_A, FULL_A)
he.edit(a, 'src', '''struct CUndeadRisingZombieAttackerBoss : public CEntityScriptBase {
    void*                  VTable;
    GameCalls*             Game;                        // +0x04''', '''struct CQ_UndeadRisingScript::CUndeadRisingZombieAttackerBoss : public CEntityScriptBase {
    GameCalls*             Game;                        // +0x04''')
he.edit(a, 'src', '''    float                  MaxHealth;                   // +0x28
    void Main();
};

void CUndeadRisingZombieAttackerBoss::Main()''', '''    float                  MaxHealth;                   // +0x28
private:
    virtual void Main();                                // private virtual (Ego_r / FableWin EAEXXZ)
};

void CQ_UndeadRisingScript::CUndeadRisingZombieAttackerBoss::Main()''')
he.edit_all(a, 'src', 'ParentClass->Add(new CSpawnedFunc1(', 'ParentClass->AddSpawnedFunction(new CSpawnedFunc1(', 2)
note(a, '?Main@CUndeadRisingZombieAttackerBoss@CQ_UndeadRisingScript@NScript@@EAEXXZ',
     '?Main@CUndeadRisingZombieAttackerBoss@@QAEXXZ over a global CQ_UndeadRisingScript with a row-local Add (0x00CB7E50 is\n//   CScriptBase::AddSpawnedFunction) and LookAfterHealth outside NScript')

# 0x00D3A1A0 CQ_FireHeartScript::CDF_Fireheart (PlayCutscene 0x00D39330: ?PlayCutscene@CQ_FireHeartScript@NScript@@, Ego_r)
a = '00d3a1a0'
he.edit(a, 'src', '''struct CQ_FireHeartScript {
    unsigned char _pad000[0x1a0];
    bool          DemonDoorCreated;                    // +0x1a0
    void PlayCutscene(CCharString name);               // 0x00D39330
};''', '''namespace NScript {
class CQ_FireHeartScript { public:                      // Ego_r.pdb / FableWin.pdb NScript::CQ_FireHeartScript
    unsigned char _pad000[0x1a0];
    bool          DemonDoorCreated;                    // +0x1a0
    void PlayCutscene(CCharString name);               // 0x00D39330
    struct CDF_Fireheart;                              // nested entity script
};
}
using NScript::CQ_FireHeartScript;''')
he.edit(a, 'src', EMPTY_A, FULL_A)
he.edit(a, 'src', '''struct CDF_Fireheart : public CEntityScriptBase {
    void*                       __vftable;             // +0x00
    CGameScriptInterface_Calls* Game;                  // +0x04''', '''struct CQ_FireHeartScript::CDF_Fireheart : public CEntityScriptBase {
    CGameScriptInterface_Calls* Game;                  // +0x04''')
he.edit(a, 'src', '''    CQ_FireHeartScript*         ParentClass;           // +0x14
    void Main();
};

void CDF_Fireheart::Main()''', '''    CQ_FireHeartScript*         ParentClass;           // +0x14
private:
    virtual void Main();                               // private virtual (Ego_r / FableWin EAEXXZ)
};

void CQ_FireHeartScript::CDF_Fireheart::Main()''')
note(a, '?Main@CDF_Fireheart@CQ_FireHeartScript@NScript@@EAEXXZ', '?Main@CDF_Fireheart@@QAEXXZ over a global CQ_FireHeartScript (PlayCutscene outside NScript)')

# 0x00E8B020 CV_FisticuffsClubScript::CRingFighter (GetCutsceneName 0x00E8A1D0: Ego_r ?GetCutsceneName@CV_FisticuffsClubScript@NScript@@)
a = '00e8b020'
he.edit(a, 'src', '''struct CV_FisticuffsClubScript {
    unsigned char _pad[0x54];''', '''namespace NScript {
class CV_FisticuffsClubScript { public:                 // Ego_r.pdb / FableWin.pdb NScript::CV_FisticuffsClubScript
    struct CRingFighter;                                // nested entity script
    unsigned char _pad[0x54];''')
he.edit(a, 'src', '''    CCharString GetCutsceneName(CCharString name);      // 0x00E8A1D0
};''', '''    CCharString GetCutsceneName(CCharString name);      // 0x00E8A1D0
};
}
using NScript::CV_FisticuffsClubScript;''')
he.edit(a, 'src', '''class CEntityScriptBase {
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
private: public:
    virtual void Main() = 0;
''', FWD + '''class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''' + VS + '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
''')
he.edit(a, 'src', '''struct CRingFighter : CEntityScriptBase {''', '''struct CV_FisticuffsClubScript::CRingFighter : CEntityScriptBase {''')
he.edit(a, 'src', '''    bool                     FirstRingFighterFought;    // +0x1c
    virtual void Main();
};

void CRingFighter::Main()''', '''    bool                     FirstRingFighterFought;    // +0x1c
private:
    virtual void Main();                                // private virtual (Ego_r / FableWin EAEXXZ)
};

void CV_FisticuffsClubScript::CRingFighter::Main()''')
note(a, '?Main@CRingFighter@CV_FisticuffsClubScript@NScript@@EAEXXZ', '?Main@CRingFighter@@UAEXXZ over a global CV_FisticuffsClubScript (GetCutsceneName outside NScript)')

# 0x00EF29E0 CPersonalScript_EmptyScript::CGoalChest / 0x00EF3000 CPersonalScript_EmptyScript::CDoor
EMPTY_B = '''class CEntityScriptBase { public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
'''
FULL_B = FWD + '''class CEntityScriptBase { public:   // global PDB class (Game / Me are declared on the entity in this row's model)
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
''' + VS + '''    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
'''
a = '00ef29e0'
he.edit(a, 'src', '''struct CPersonalScript_EmptyScript { unsigned char Unknown[0x48]; bool GoalChestFinished; };
''', '''namespace NScript {
class CPersonalScript_EmptyScript { public: unsigned char Unknown[0x48]; bool GoalChestFinished; struct CGoalChest; };   // Ego_r.pdb NScript::
}
using NScript::CPersonalScript_EmptyScript;
''')
he.edit(a, 'src', EMPTY_B, FULL_B)
he.edit(a, 'src', '''struct CGoalChest : CEntityScriptBase {                 // the PDB base (empty here: offset 0)
 void* VTable; GameCalls* Game; ThingCalls Me; CPersonalScript_EmptyScript* ParentClass; void* PMasterData;
 void Main();
};
void CGoalChest::Main() {''', '''struct CPersonalScript_EmptyScript::CGoalChest : CEntityScriptBase {   // the PDB base (its vptr at +0x00)
 GameCalls* Game; ThingCalls Me; CPersonalScript_EmptyScript* ParentClass; void* PMasterData;
private:
 virtual void Main();   // private virtual (Ego_r / FableWin EAEXXZ)
};
void CPersonalScript_EmptyScript::CGoalChest::Main() {''')
note(a, '?Main@CGoalChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ', '?Main@CGoalChest@@QAEXXZ over a global CPersonalScript_EmptyScript, the vptr a `void* VTable` member')
a = '00ef3000'
he.edit(a, 'src', EMPTY_B, FULL_B + '''namespace NScript {
class CPersonalScript_EmptyScript { public: struct CDoor; };   // Ego_r.pdb / FableWin.pdb NScript::CPersonalScript_EmptyScript
}
''')
he.edit(a, 'src', '''struct CDoor : CEntityScriptBase {void* VTable;GameCalls* Game;CScriptThing Me;   // the PDB base (empty here: offset 0)
 void Main();
};
void CDoor::Main(){''', '''struct NScript::CPersonalScript_EmptyScript::CDoor : CEntityScriptBase {GameCalls* Game;CScriptThing Me;   // the PDB base (its vptr at +0x00)
private:
 virtual void Main();   // private virtual (Ego_r / FableWin EAEXXZ)
};
void NScript::CPersonalScript_EmptyScript::CDoor::Main(){''')
note(a, '?Main@CDoor@CPersonalScript_EmptyScript@NScript@@EAEXXZ', '?Main@CDoor@@QAEXXZ (not nested), the vptr a `void* VTable` member')
