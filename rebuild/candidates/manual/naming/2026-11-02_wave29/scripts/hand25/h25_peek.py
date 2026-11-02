# h25_peek.py: naming wave 25, family peek -- the junk binders of 0x00686A80 ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (DECISIONS_W24_RESOLVED
#  item 7: Ego_r.pdb 0x006B1E70 / FableWin.pdb 0x02190D60, 16 bytes after GetWorld as retail 0x00686A70 / 0x00686A80; 0x004BC750 and
#  ~100 rows bind it). Each caller is re-authored on its PDB class over the empty-base CTCBase the landed PeekWorld rows model
#  (0x006819D0); the members read through the const CWorld& are the retail-offset fields of the landed CWorld models.
#  006AFD40 CTCBase::PeekThingSearchTools (was a free GetCombatManager over helper_686a80), 0076C810 CTCEnemy::SetFaction (free SetFaction
#  over Enemy_GetObj), 0077DB90 CTCHeroCentre::OnHeroWalkedThroughDoor (GFGetHeroCreature), 006AF630 (CViewBase::GetDisplayEngine),
#  007E00D0 CTCTeleporter::FrameUpdate (CTCTeleporter::GetWorld). 0x00681980 is in family coh; 0x00744580 / 0x007345D0 are DEFERRED.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('006afd40', 'src', '''// CTCBase::PeekThingSearchTools() const -- the world's thing-search tools
// Leaf: ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFD40, 9 bytes: `call 0x00686A80 ; mov eax,[eax+20h] ; ret`.
// Identity (naming wave 25): Ego_r.pdb ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ (0x006B2180, after PeekWorld 0x006B1E70)
//   and the catalog name; the body is `*PeekWorld().PThingSearchTools` (CWorld +0x20 scoped_ptr<CThingSearchTools>, Ego_r.pdb and the
//   landed engine/CWorld.h). Callee: ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7). Was a free
//   GetCombatManager(CThingCreatureBase*) over a helper_686a80 stand-in.
class CThingSearchTools;
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;                // +0x20 scoped_ptr<CThingSearchTools> (its one pointer, as the landed CWorld models)
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;                     // 0x00686A80
    const CThingSearchTools& PeekThingSearchTools() const;
};
const CThingSearchTools& CTCBase::PeekThingSearchTools() const
{
    return *PeekWorld().PThingSearchTools;
}
''')
he.put('0076c810', 'src', '''// CTCEnemy::SetFaction(const CCharString& faction) -- look the named faction up in the world's faction manager and keep it
// Leaf: ?SetFaction@CTCEnemy@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076C810, 30 bytes, thiscall, `ret 4`.
// Identity (naming wave 25): Ego_r.pdb ?SetFaction@CTCEnemy@@QAEXABVCCharString@@@Z and the catalog name. Callees:
//   ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7), ?GetPFaction@CFactionManager@@QBEPAVCFaction@@
//   ABVCCharString@@@Z 0x006B93D0 (Ego_r.pdb; bound by 0x00771540). Layout: CWorld +0x54 scoped_ptr<CFactionManager> PFactionManager
//   (Ego_r.pdb / engine/CWorld.h); CTCEnemy +0x18 CFaction* PFaction (Ego_r.pdb +0x28: the two STLport lists EnemyThings / AllyThings
//   are 4 bytes each in retail). Was a free SetFaction over an Enemy_GetObj stand-in and Sub::resolve.
class CCharString;
class CFaction;
class CFactionManager { public:
    CFaction* GetPFaction(const CCharString& name) const;   // 0x006B93D0
};
class CWorld { public:
    unsigned char _pad00[0x54];
    CFactionManager* PFactionManager;                    // +0x54 scoped_ptr<CFactionManager> (its one pointer, as the landed CWorld models)
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;                     // 0x00686A80
};
class CTCEnemy : public CTCBase { public:
    void*         __vftable;                             // +0x00 (CTCBase)
    unsigned char _pad04[0x18 - 0x04];
    CFaction*     PFaction;                              // +0x18
    void SetFaction(const CCharString& faction);
};
void CTCEnemy::SetFaction(const CCharString& faction)
{
    CFactionManager* manager = PeekWorld().PFactionManager;   // the manager in a local: the argument push follows the PeekWorld call
    PFaction = manager->GetPFaction(faction);
}
''')
he.put('0077db90', 'src', '''// CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType) -- the guild's main door (type 1) opens the training screen
//   when experience spending is enabled
// Leaf: ?OnHeroWalkedThroughDoor@CTCHeroCentre@@QAEXW4EHeroCentreDoorType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077DB90, 36 bytes, `ret 4`:
//   mov eax,[esp+4] ; dec eax ; push esi ; mov esi,ecx ; jne R ; call 0x00686A80 ; mov cl,[eax+0D6h] ; test cl,cl ; je R ;
//   mov ecx,esi ; call 0x0077D4B0 ; R: pop esi ; ret 4
// Identity (naming wave 25): Ego_r.pdb ?OnHeroWalkedThroughDoor@CTCHeroCentre@@QAEXW4EHeroCentreDoorType@@@Z; the callee 0x0077D4B0
//   is ?OpenTrainingScreen@CTCHeroCentre@@QAEXXZ (the catalog name; Ego_r order OnHeroCloseDoor / OpenTrainingScreen (0x100 bytes) /
//   RemoveQuestCard / OnHeroWalkedThroughDoor (0x30) = retail 0x0077D4B0 (254 bytes) .. 0x0077DB90 (36)); the world accessor is
//   ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7) and +0xD6 is CWorld::ExperienceSpendingEnabled
//   (retail offset, as the landed CWorld ctor 0x004A67D0 and 0x0077F7E0 model it). Was CTCHeroCentre_Methods with a free
//   GFGetHeroCreature() and OnHeroEnteredGuildDoor (descriptive).
// Lane feedback: retail `mov eax,[esp+4] ; dec eax ; jne` is a one-case switch, not an `==` test. RELOCATION_MATCH.
enum EHeroCentreDoorType { };
class CWorld { public:
    unsigned char _pad00[0xd6];
    bool          ExperienceSpendingEnabled;            // +0xd6
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
};
class CTCHeroCentre : public CTCBase { public:
    void OpenTrainingScreen();                          // 0x0077D4B0
    void OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType);
};

void CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType)
{
    // `dec eax ; jne` is the compiler's single-case switch (case 1); `if (doorType == 1)`
    // emits `cmp dword ptr [esp+4],1` instead.
    switch (doorType) {
    case 1:
        if (PeekWorld().ExperienceSpendingEnabled)
            OpenTrainingScreen();
        break;
    default:
        break;
    }
}
''')
A = '006af630'
he.edit(A, 'src', '''// _unattributed::DisplayEnginePredicate13_006af630() -- tail-calls virtual slot 13 of the display engine returned by CViewBase::GetDisplayEngine (a bool predicate); owner unknown
// Leaf: ?DisplayEnginePredicate13_006af630@_unattributed@@
''', '''// _unattributed::DisplayEnginePredicate13_006af630() -- tail-calls virtual slot 13 of the world returned by CTCBase::PeekWorld (a bool predicate); owner unknown
// Leaf: ?DisplayEnginePredicate13_006af630@_unattributed@@
// Naming wave 25 (DECISIONS_W24_RESOLVED item 7): 0x00686A80 is ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (Ego_r / FableWin, 16 bytes after
//   GetWorld; ~100 landed rows bind it), so the object whose slot 13 (+0x34) this tail-calls is the const CWorld; the owner class stays
//   descriptive (_unattributed), now deriving from the empty-base CTCBase the landed PeekWorld rows model. Was CViewBase::GetDisplayEngine.
''')
he.edit(A, 'src', '''class CDisplayEngine { public:''', '''class CWorld { public:''')
he.edit(A, 'src', '''    virtual bool v34(); // slot 0x34: the predicate this thunk tail-calls
};

struct CViewBase {
    CDisplayEngine *GetDisplayEngine();
};

struct _unattributed : CViewBase {''', '''    virtual bool v34() const; // slot 0x34: the predicate this thunk tail-calls
};

class CTCBase {
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

struct _unattributed : CTCBase {''')
he.edit(A, 'src', '''    return GetDisplayEngine()->v34();''', '''    return PeekWorld().v34();''')
A = '007e00d0'
he.edit(A, 'src', '''// Callees: CTCBase getters 0x00686A80 (world) / 0x00686D30''', '''// Naming wave 25 (DECISIONS_W24_RESOLVED item 7): 0x00686A80 is ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (Ego_r / FableWin), declared on
//   the CTCBase base (was CTCTeleporter::GetWorld returning CWorld*); the two reads (Paused, TeleportingEnabled) go through the const CWorld&.
// Callees: CTCBase getters 0x00686A80 (world) / 0x00686D30''')
he.edit(A, 'src', '''protected:
    CPlayerManager& GetPlayerManager();                 // 0x00686D40''', '''protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80 ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (Ego_r / FableWin)
    CPlayerManager& GetPlayerManager();                 // 0x00686D40''')
he.edit(A, 'src', '''    CWorld* GetWorld();                                 // 0x00686A80
''', '')
he.edit(A, 'src', '''GetWorld()->Paused''', '''PeekWorld().Paused''')
he.edit(A, 'src', '''GetWorld()->TeleportingEnabled''', '''PeekWorld().TeleportingEnabled''')
