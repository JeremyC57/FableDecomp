# h25_esb.py: naming wave 25, family esb -- 0x00F35B30 ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ (Ego_r.pdb 0x00FA9E70 /
#  FableWin.pdb 0x03B7B460: global class, const; 381 landed rows bind it; the label DB's "IsDone" (chunks 241 / 280 / 322) is superseded).
#  Seven rows declared it on the entity class itself (IsDone or IsActiveThreadTerminating, non-const): they now derive from the global
#  CEntityScriptBase the 194 landed SetPredicate rows model (an empty base declaring the accessors; the entity keeps its vptr / Game / Me
#  fields: empty-base layout, bytes unchanged). Where the row's SetPredicate 0x00F35B10 already takes const NScript::CPredicateBase&
#  (0x00D864B0, 0x00E05360) it moves to the base too: ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z (Ego_r 0x00FA9E50).
#  0x00EE76F0 / 0x00D9F270 (an NScript::CEntityScriptBase holding SetPredicate on a local filter class) wait for the predicate-header
#  re-model (DEFERRED).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = ('// Naming wave 25: ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30 (Ego_r.pdb / FableWin.pdb: global class, const)\n'
        '//   is declared on the global CEntityScriptBase the entity derives from (the landed SetPredicate rows\' empty-base model; the vptr /\n'
        '//   Game / Me fields stay on the entity: empty-base layout, bytes unchanged); was %s.\n')
def base(setpred):
    s = 'class CEntityScriptBase { public:                        // the PDB class the accessors belong to (Ego_r.pdb CEntityScriptBase, global)\n'
    s += '    bool IsActiveThreadTerminating() const;             // 0x00F35B30\n'
    if setpred: s += '    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10\n'
    return s + '};\n'
def note(a, was):
    p, rel, t = he._rw(a, 'src')
    if 'Naming wave 25: ?IsActiveThreadTerminating' in t: print('already', a, 'src'); return
    L = t.split('\n'); i = next((j for j, x in enumerate(L) if x.startswith('// Leaf: ')), 0)
    L.insert(i + 1, (NOTE % was).rstrip('\n')); he._w(p, '\n'.join(L)); print('ok', a, 'src')

A = '00d9c2b0'
he.edit(A, 'src', '''struct CDrinkersReactMarker {void* VTable;GameMethods* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 bool IsActiveThreadTerminating();void Main();
};''', base(False) + '''struct CDrinkersReactMarker : CEntityScriptBase {void* VTable;GameMethods* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 void Main();
};''')
note(A, 'CDrinkersReactMarker::IsActiveThreadTerminating (non-const)')

A = '00d864b0'
he.edit(A, 'src', '''struct CGuardInWoods {
 void* VTable;GameCalls* Game;CScriptThing Me;CQ_HeroSoulsGuildmasterScript* ParentClass;void* PMasterData;
 void SetPredicate(const NScript::CPredicateBase&);bool IsActiveThreadTerminating();void Main();
};''', base(True) + '''struct CGuardInWoods : CEntityScriptBase {
 void* VTable;GameCalls* Game;CScriptThing Me;CQ_HeroSoulsGuildmasterScript* ParentClass;void* PMasterData;
 void Main();
};''')
note(A, 'CGuardInWoods::SetPredicate / IsActiveThreadTerminating (non-const)')

A = '00e76920'
he.edit(A, 'src', '''#include "engine/CScriptBase.h"
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { public:''', '''#include "engine/CScriptBase.h"
''' + base(False) + '''namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { public:''')
he.edit(A, 'src', '''class CV_ExposeMayorScript::CEM_FlashLightPoint {''', '''class CV_ExposeMayorScript::CEM_FlashLightPoint : public CEntityScriptBase {''')
he.edit(A, 'src', '''    bool IsActiveThreadTerminating();                   // 0x00F35B30
''', '')
note(A, 'CV_ExposeMayorScript::CEM_FlashLightPoint::IsActiveThreadTerminating (non-const)')

A = '00e05360'
he.edit(A, 'src', '''extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
class CPredicateBase { public:''', '''extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript { class CPredicateBase; }
''' + base(True) + '''namespace NScript {
class CPredicateBase { public:''')
he.edit(A, 'src', '''struct CQ_TraderEscortScript::CTraderComment {''', '''struct CQ_TraderEscortScript::CTraderComment : CEntityScriptBase {''')
he.edit(A, 'src', '''    void SetPredicate(const CPredicateBase& predicate); // 0x00F35B10
    bool IsActiveThreadTerminating();                   // 0x00F35B30
''', '')
note(A, 'CQ_TraderEscortScript::CTraderComment::SetPredicate / IsActiveThreadTerminating (non-const)')

for A, head, cls in (('00efe8d0', 'class CBenTestScriptsScript::CFader {', 'CBenTestScriptsScript::CFader'),
                     ('00cdf620', 'class CPersonalScript_GlobalThingsScript::CHERO_GUILD_TELEPORT_MARKER {', 'CPersonalScript_GlobalThingsScript::CHERO_GUILD_TELEPORT_MARKER'),
                     ('00efb8c0', 'class CHitSwitchTestScript::CHitSwitch {', 'CHitSwitchTestScript::CHitSwitch')):
    p, rel, t = he._rw(A, 'src')
    if head[:-2] + ' : public CEntityScriptBase {' in t: print('already', A, 'src'); continue
    i = t.index(head); j = t.rindex('\nnamespace NScript {\n', 0, i)
    if 'class CEntityScriptBase {' not in t:
        t = t[:j + 1] + base(False) + t[j + 1:]
    t = t.replace(head, head[:-2] + ' : public CEntityScriptBase {')
    t = t.replace('    bool IsDone();                                      // 0x00F35B30\n', '')
    t = t.replace('IsDone()', 'IsActiveThreadTerminating()')
    he._w(p, t); print('ok', A, 'src')
    note(A, cls + '::IsDone (non-const; the label DB\'s IsDone)')
# 0x00D864B0 had no Leaf line (graded on its largest function): the Leaf is its current own spelling (the nested NScript class
# ?Main@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ of Ego_r.pdb is a separate own-row respell, DEFERRED).
he.edit('00d864b0', 'src', '''// NScript::CQ_HeroSoulsGuildmasterScript::CGuardInWoods::Main -- wait for guildmaster cutscene then target hero until terminated.
''', '''// NScript::CQ_HeroSoulsGuildmasterScript::CGuardInWoods::Main -- wait for guildmaster cutscene then target hero until terminated.
// Leaf: ?Main@CGuardInWoods@@QAEXXZ
''')
