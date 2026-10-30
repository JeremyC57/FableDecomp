# sub-wave mig8 (hand): 0x00EFBE20 is ?Main@CGuildSealTrigger@CGuildSealTriggerTestScript@NScript@@EAEXXZ (Ego_r.pdb 0x00F79F80 +
# FableWin.pdb 0x03AFD280; retail vtable 0x012F6C20 slot 1 is stored by the factory 0x00EFBD40 registered as "GuildSealTrigger");
# the row's invented RegisterFilter / IsDone are CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30
# (engine/CEntityScriptBase.h); the filter is NScript::CIsThingConscious (engine/CIsThingConscious.h, out-of-line dtor 0x00EF7AB0).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
A = '00efbe20'
edit(A, 'src', '''// CGuildSealTrigger::Main() -- guild seal trigger:''', '''// NScript::CGuildSealTriggerTestScript::CGuildSealTrigger::Main() -- guild seal trigger:''')
edit(A, 'src', '''// Leaf: Main
''', '''// Leaf: Main
// Identity (naming wave 11): ?Main@CGuildSealTrigger@CGuildSealTriggerTestScript@NScript@@EAEXXZ (Ego_r.pdb 0x00F79F80, FableWin.pdb
//   0x03AFD280); retail vtable 0x012F6C20 slot 1 is stored by the factory 0x00EFBD40 registered as "GuildSealTrigger". The filter
//   registration 0x00F35B10 is CEntityScriptBase::SetPredicate and IsDone 0x00F35B30 IsActiveThreadTerminating (both PDBs).
''')
edit(A, 'src', '''struct CIsThingConscious {                         // vtable 0x012C2FE8: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingConscious();                  // 0x00EF7AB0
};
''', '''namespace NScript { class CPredicateBase; }
// NScript::CIsThingConscious: the retail model (vtable 0x012C2FE8, Thing +0x04, 0x10); its dtor 0x00EF7AB0 is called out of line.
#include "engine/CIsThingConscious.h"
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
using NScript::CIsThingConscious;
''')
edit(A, 'src', '''class CGuildSealTrigger {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void RegisterFilter(CIsThingConscious& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CGuildSealTrigger::Main()
{
    {
        CIsThingConscious filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDone()) {''', '''// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CGuildSealTriggerTestScript {        // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CGuildSealTrigger;
};
class CGuildSealTriggerTestScript::CGuildSealTrigger : public CEntityScriptBase {
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
};
}

void NScript::CGuildSealTriggerTestScript::CGuildSealTrigger::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {''')
from he import resub
resub(A, 'src', r'\bIsDone\(\)', 'IsActiveThreadTerminating()')
