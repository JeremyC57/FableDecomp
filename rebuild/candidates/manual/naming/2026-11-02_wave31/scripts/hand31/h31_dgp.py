# h31_dgp.py: naming wave 31, family dgp (c702 priority, coordinator note): the NPlayerGui::CDrawBase::GetPlayer pair. Retail 0x00643D7C /
#  0x00643D84 are two byte-identical 8-byte bodies `mov ecx,[ecx+8] (PlayerManager); jmp 0x00449970`; Ego_r.pdb places
#  ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ / ...QBEABVCPlayer@@XZ as two adjacent 8-byte bodies (0x00686C81 / 0x00686C89; FableWin.pdb
#  the same pair), non-const first. Chunk 702 binds the const one by its PDB name (caller 0x00657200) and the non-const caller is 0x00657D90.
#  Both reach the NON-const CPlayerManager::GetMainPlayer 0x00449970 (the const twin is 0x00449960): PlayerManager is a `CPlayerManager*`
#  member, so the const member calls through a pointer-to-non-const. Were the invented forwarders T::Run over Sub::Run (catalog
#  _global::Forward_GetMainPlayer_via0x8_*, Leaf "CEntry>_const&>").
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
SRC = '''// NPlayerGui::CDrawBase::GetPlayer()%(c)s @ 0x%(A)s -- the main player of the draw object's player manager
// Leaf: %(leaf)s
// Naming wave 31 (family dgp, c702): Ego_r.pdb / FableWin.pdb %(leaf)s. Ego_r places the non-const / const GetPlayer pair as two
//   adjacent 8-byte bodies (0x00686C81 / 0x00686C89), retail the same pair 0x00643D7C / 0x00643D84 (identical bytes, not folded); chunk 702
//   binds %(bound)s. Was the invented forwarder T::Run over Sub::Run (catalog "_global::Forward_GetMainPlayer_via0x8_%(a)s",
//   Leaf "CEntry>_const&>").
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%(A)s, 8 bytes: `mov ecx,[ecx+8] ; jmp 0x00449970` (CPlayerManager::GetMainPlayer, the
//   non-const one -- PlayerManager is a CPlayerManager* member, so the const member calls through a pointer to non-const).
// Layout: Ego_r.pdb NPlayerGui::CDrawBase (vptr, PlayerGui +0x04, PlayerManager +0x08, ...), as the landed c702 rows 0x00657200 / 0x00657D90.
#include "engine/CPlayerManager.h"                       // GetMainPlayer() 0x00449970 / const 0x00449960
class CPlayerGui;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo;

namespace NPlayerGui {
class CDrawBase {                                           // RTTI .?AVCDrawBase@NPlayerGui@@
public:
    virtual void ClearPrimitivesIfNecessary();              // slot 0
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);
    CPlayer&       GetPlayer();                             // 0x00643D7C
    const CPlayer& GetPlayer() const;                       // 0x00643D84
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
};
}

%(ret)s NPlayerGui::CDrawBase::GetPlayer()%(c)s
{
    return PlayerManager->GetMainPlayer();
}'''
for a, c, ret, leaf, bound in (('00643d7c', '', 'CPlayer&', '?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ', 'the const twin by name; its non-const caller 0x00657D90 still calls it as CDrawAbilityButtonsPC::GetPlayer (queued)'),
                               ('00643d84', ' const', 'const CPlayer&', '?GetPlayer@CDrawBase@NPlayerGui@@QBEABVCPlayer@@XZ', 'this spelling (caller 0x00657200)')):
    he.put(a, 'src', SRC % dict(a=a, A=a.upper(), c=c, ret=ret, leaf=leaf, bound=bound))
# 0x00657D90 (c702 caller): its GetPlayer is the non-const CDrawBase member (was ?GetPlayer@CDrawAbilityButtonsPC@NPlayerGui@@QAEPAVCPlayer@@XZ,
#  a pointer-returning member of the derived class): CDrawAbilityButtonsPC derives from the same CDrawBase model (PlayerGui / PlayerManager /
#  World move to the base; RTTI CDrawAbilityButtonsPC : CDrawAbilityButtons : CDrawBase is not modelled further here).
a = '00657d90'
he.edit(a, 'src', '''struct CDrawAbilityButtonsPC {
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    void*           World;                  // +0x0c
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
    CPlayer* GetPlayer();                   // 0x00643D7C
''', '''class CDrawBase {                           // RTTI .?AVCDrawBase@NPlayerGui@@ (layout as 0x00643D7C / 0x00657200)
public:
    CPlayer&        GetPlayer();            // 0x00643D7C ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ (Ego_r.pdb / FableWin.pdb)
    virtual void v00();
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    void*           World;                  // +0x0c
};
struct CDrawAbilityButtonsPC : CDrawBase {
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
''')
he.edit(a, 'src', '    virtual void v00(); virtual void v01(); virtual void v02();', '    virtual void v01(); virtual void v02();')
he.edit(a, 'src', 'GetPlayer()->PeekPControlledCreature()', 'GetPlayer().PeekPControlledCreature()')
from common import note
note(a, 'dgp', 'GetPlayer is the non-const NPlayerGui::CDrawBase member ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ 0x00643D7C\n'
     '//   (Ego_r.pdb / FableWin.pdb; returns CPlayer&), on a CDrawBase base of the local CDrawAbilityButtonsPC; was the invented\n'
     '//   ?GetPlayer@CDrawAbilityButtonsPC@NPlayerGui@@QAEPAVCPlayer@@XZ')
# 0x00658020 (c702 caller, flat model): the same respell -- GetPlayer is the non-const CDrawBase member returning CPlayer&, on a CDrawBase
#  base holding the row's flat vptr slot and PlayerGui / PlayerManager / World (the own row's model is otherwise unchanged).
a = '00658020'
he.edit(a, 'src', '''struct CDrawAbilityButtonsPC {
    void*           _vtable;
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    CWorld*         World;                  // +0x0c
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
    CPlayer* GetPlayer();                   // 0x00643D7C
''', '''class CDrawBase {                           // RTTI .?AVCDrawBase@NPlayerGui@@ (layout as 0x00643D7C / 0x00657200)
public:
    void*           _vtable;
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    CWorld*         World;                  // +0x0c
    CPlayer&        GetPlayer();            // 0x00643D7C ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ (Ego_r.pdb / FableWin.pdb)
};
struct CDrawAbilityButtonsPC : CDrawBase {
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
''')
he.edit(a, 'src', 'GetPlayer()->PeekPControlledCreature()', 'GetPlayer().PeekPControlledCreature()')
note(a, 'dgp', 'GetPlayer is the non-const NPlayerGui::CDrawBase member ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ 0x00643D7C\n'
     '//   (Ego_r.pdb / FableWin.pdb; returns CPlayer&) on a CDrawBase base; was the invented ?GetPlayer@CDrawAbilityButtonsPC@NPlayerGui@@QAEPAVCPlayer@@XZ')
