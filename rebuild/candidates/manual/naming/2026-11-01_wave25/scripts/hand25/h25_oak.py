# h25_oak.py: naming wave 25, family oak -- the c692 OakValeRevisited rows.
#  0x00EE8C10: landed as a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor; it is slot 0 of the RTTI-less vtable
#    0x012F53C0 the ctor 0x00EE8100 stores, calling ~CCS_OakValeRevisitedScript 0x00EE8C30 then operator delete: the compiler-generated
#    ??_GCCS_OakValeRevisitedScript@NScript@@UAEPAXI@Z (FableWin.pdb). Re-authored on the wave 16 ??_G model (0x00E68E90): bytes unchanged.
#  0x00EE8100: the ctor, moved into namespace NScript with the PDB parameter pair (CScriptDataBase*, CGameScriptInterfaceBase&)
#    (Ego_r 0x00F60DC0 / FableWin ??0CCS_OakValeRevisitedScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z):
#    both are one pushed pointer, so the bytes are unchanged.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('00ee8c10', 'src', '''// NScript::CCS_OakValeRevisitedScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCCS_OakValeRevisitedScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8C10, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c692): slot 0 of the RTTI-less vtable 0x012F53C0, which the ctor 0x00EE8100 stores; FableWin.pdb
//   ??_GCCS_OakValeRevisitedScript@NScript@@UAEPAXI@Z (Ego_r.pdb names the class's ??_E / ??1 / ??0 in NScript).
// Callees: NScript::CCS_OakValeRevisitedScript::~CCS_OakValeRevisitedScript 0x00EE8C30 (??1CCS_OakValeRevisitedScript@NScript@@UAE@XZ),
//   operator delete 0x00BFE9BC.
// Naming wave 25 (c692 queue): landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor calling
//   CLandscapeBackgroundPatch_dtor_00ee8c30; re-authored on the compiler-generated ??_G as the wave 16 rows (0x00E68E90) were
//   (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CCS_OakValeRevisitedScript {
public:
    CCS_OakValeRevisitedScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00EE8100 (??0CCS_OakValeRevisitedScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z)
    virtual ~CCS_OakValeRevisitedScript();               // 0x00EE8C30 (??1CCS_OakValeRevisitedScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor defined with an empty body emits the vtable and with it this ??_G (the constructor body itself is its
//   own row, 0x00EE8100; not part of this graded body).
NScript::CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
''')
A = '00ee8100'
he.edit(A, 'src', '''// CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game) -- cut-scene''',
        '''// NScript::CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- cut-scene''')
he.edit(A, 'src', '''// Leaf: ??0CCS_OakValeRevisitedScript
''', '''// Leaf: ??0CCS_OakValeRevisitedScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Naming wave 25 (c692 queue): the class lives in namespace NScript and the ctor takes the PDB parameter pair (CScriptDataBase*,
//   CGameScriptInterfaceBase&) (Ego_r.pdb 0x00F60DC0 / FableWin.pdb ??0CCS_OakValeRevisitedScript@NScript@@QAE@PAVCScriptDataBase@@
//   AAVCGameScriptInterfaceBase@@@Z); was a global struct taking (void*, CGameScriptInterface*). Name-only: both are one pushed
//   pointer, stored as before (the bytes are unchanged).
''')
he.edit(A, 'src', '''struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};''', '''class CScriptDataBase;
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterfaceBase* Game;                // +0x40 (second argument)
    CScriptDataBase*          Data;                // +0x44 (first argument, the script data)
    CQuestScriptObject(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), Data(data) { }
};''')
he.edit(A, 'src', '''struct CCS_OakValeRevisitedScript : CQuestScriptObject {   // vtable 0x012F53C0''', '''namespace NScript {
class CCS_OakValeRevisitedScript : public CQuestScriptObject {   // vtable 0x012F53C0
public:''')
he.edit(A, 'src', '''    CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game);
};
CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}''', '''    CCS_OakValeRevisitedScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
}
NScript::CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CQuestScriptObject(data, game)
{
}''')
