# h17_qctor.py: naming wave 17, family qctor -- 0x00D25410 is NScript::CQ_DragonBossFightScript's constructor (c676: its only caller is
# Alloc 0x00D27240, push 98h, registered at 0x00CD5D9D with "S_QDBF", conversion_queue Q_DragonBossFight; vtable 0x012CB70C is
# ??_7CQ_DragonBossFightScript@NScript@@6B@). The landed row spelled it ??0CQ_NewOakValeIntroScript, which duplicated the real
# CQ_NewOakValeIntroScript ctor 0x00DAAC00 (vtable 0x012D7A28, Alloc push 10Ch). Respelled on the PDB constructor
# ??0CQ_DragonBossFightScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z with the wave 16 qctor parameter
# pair (both one pushed pointer); members stay descriptive (the Ego_r layout is the debug build's). Name-only. File renamed (RENAMES).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
a = '00d25410'
P = 'CScriptDataBase* data, CGameScriptInterfaceBase& game'
edit(a, 'src', '''// CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game) -- quest-script ctor''',
     '''// NScript::CQ_DragonBossFightScript::CQ_DragonBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script ctor''')
edit(a, 'src', '// Leaf: ??0CQ_NewOakValeIntroScript\n', '''// Leaf: ??0CQ_DragonBossFightScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Identity (proven, chunk c676): the only caller is Alloc 0x00D27240 (push 98h; registered at 0x00CD5D9D with "S_QDBF",
//   conversion_queue Q_DragonBossFight); vtable 0x012CB70C is ??_7CQ_DragonBossFightScript@NScript@@6B@. The real
//   ??0CQ_NewOakValeIntroScript is 0x00DAAC00 (vtable 0x012D7A28, Alloc push 10Ch).
// Naming wave 17: landed as ??0CQ_NewOakValeIntroScript(void*, CGameScriptInterface*) (a duplicate of 0x00DAAC00's name); respelled
//   on the PDB constructor and its (CScriptDataBase*, CGameScriptInterfaceBase&) pair -- name-only (one pushed pointer each).
''')
edit(a, 'src', '''// Vtable 0x012CB70C (slot 3 = Init 0x00D254B0). The retail class does not follow the Ego_r PDB layout of
// CQ_NewOakValeIntroScript past +0x4c (it has a 12-byte map at +0x84 and timers at +0x90/+0x94), so the members are''',
     '''// Vtable 0x012CB70C (slot 3 = Init 0x00D254B0). The retail class does not follow the Ego_r (debug-build) layout of
// CQ_DragonBossFightScript past +0x4c (it has a 12-byte map at +0x84 and timers at +0x90/+0x94), so the members are''')
edit(a, 'src', '''    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }''',
     '''    CGameScriptInterfaceBase* Game;                // +0x40 (second argument; Ego_r CLevelScriptBase<T>::Game)
    CScriptDataBase*          Data;                // +0x44 (first argument, the script data)
    CQuestScriptObject(%s) : CScriptBase(), Game(&game), Data(data) { }''' % P)
edit(a, 'src', 'struct CQ_NewOakValeIntroScript : CQuestScriptObject {   // vtable 0x012CB70C\n',
     'class CScriptDataBase;\nnamespace NScript {\nclass CQ_DragonBossFightScript : public CQuestScriptObject {   // vtable 0x012CB70C (??_7CQ_DragonBossFightScript@NScript@@6B@)\npublic:\n')
edit(a, 'src', '''    CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game);
};
CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game)
''', '''    CQ_DragonBossFightScript(%s);
};
} // namespace NScript
NScript::CQ_DragonBossFightScript::CQ_DragonBossFightScript(%s)
''' % (P, P))
edit(a, 'tst', '// Behaviour test for 0x00d25410 (CQ_NewOakValeIntroScript ctor)', '// Behaviour test for 0x00d25410 (NScript::CQ_DragonBossFightScript ctor)')
