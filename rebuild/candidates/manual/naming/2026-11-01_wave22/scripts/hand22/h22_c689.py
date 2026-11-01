# h22_c689.py: naming wave 22, family c689 -- the chunk-689 deferral 0x00D34470: the CQ_FireHeartScript constructor takes the PDB signature
# ??0CQ_FireHeartScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (Ego_r.pdb 0x00D42A30; the vtable store
# ??_7CQ_FireHeartScript@NScript@@6B@ already bound) on the precedent of the landed CQ_HangingTreeGoodScript ctor 0x00D6EF20 (naming wave 17):
# the novtable script-object base takes (CScriptDataBase*, CGameScriptInterfaceBase&) and stores Game = &game / the data -- both one pushed
# pointer, so the bytes are unchanged. The Leaf line names the full symbol.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
a = '00d34470'
he.edit(a, 'src', '// Leaf: ??0CQ_FireHeartScript\n', '// Leaf: ??0CQ_FireHeartScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z\n// Naming wave 22 (c689 deferral): the PDB parameter pair (CScriptDataBase*, CGameScriptInterfaceBase&) replaces the landed (void*,\n//   CGameScriptInterface*), as the landed CQ_HangingTreeGoodScript ctor 0x00D6EF20 has it; name-only (one pushed pointer each).\n')
he.edit(a, 'src', 'class CGameScriptInterface;\n', 'class CGameScriptInterfaceBase;\nclass CScriptDataBase;\n')
he.edit(a, 'src', '''    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pData;                                   // +0x44
    CScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pData(data) { }''', '''    CGameScriptInterfaceBase* m_pGame;               // +0x40 (Ego_r CLevelScriptBase<T>::Game)
    CScriptDataBase* m_pData;                        // +0x44 (the script data)
    CScriptObject(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), m_pGame(&game), m_pData(data) { }''')
he.edit(a, 'src', '    CQ_FireHeartScript(void* data, CGameScriptInterface* game);\n', '    CQ_FireHeartScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);\n')
he.edit(a, 'src', 'NScript::CQ_FireHeartScript::CQ_FireHeartScript(void* data, CGameScriptInterface* game)\n', 'NScript::CQ_FireHeartScript::CQ_FireHeartScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)\n')
