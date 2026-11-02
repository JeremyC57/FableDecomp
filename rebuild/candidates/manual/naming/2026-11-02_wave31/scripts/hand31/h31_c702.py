# h31_c702.py: naming wave 31, family c702 (coordinator note, chunk 702 NAME_FIXES rows after the GetPlayer pair).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
# 0x00643FC8: slot 3 of CDrawAbilityButtons 0x0125A6C8 is Ego_r.pdb / FableWin.pdb
#  ?GetAssignableSpecialMoveIndexForButtonDef@CDrawAbilityButtons@NPlayerGui@@MBEJABVCAbilityButtonGuiDef@@@Z (protected virtual const; the PC
#  override 0x00657200, landed by chunk 702, is the same slot); slot 2 is Draw (chunk 702: donor Draw / this / DrawAbilityList = slots 2 / 3 / 4).
#  Its callee 0x00445EE0 is the const ?GetAssignableSpecialMoveIndexForButton@CGamePlayerInterface@@QBEJW4EXboxControllerButton@@@Z (the landed
#  own row's Leaf, both PDBs). PlayerManager is a CPlayerManager*, so the const member still calls the non-const GetMainPlayer 0x00449970.
a = '00643fc8'
he.edit(a, 'src', '// NPlayerGui::CDrawAbilityButtons::GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def) --',
        '// NPlayerGui::CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const --')
he.edit(a, 'src', 'NPlayerGui::CDrawAbilityButtons\' vtable 0x0125A6C8; the PC subclass\'s slot 3 (0x00657200) is the same body reading the def\'s\n'
        '// mouse button (+0x44) and calling GetAssignableSpecialMoveIndexForMouseButton 0x00445F40. The method name is descriptive.',
        'NPlayerGui::CDrawAbilityButtons\' vtable 0x0125A6C8; the PC subclass\'s slot 3 (0x00657200) is the same body reading the def\'s\n'
        '// mouse button (+0x44) and calling GetAssignableSpecialMoveIndexForMouseButton 0x00445F40.')
he.edit(a, 'src', '    long GetAssignableSpecialMoveIndexForButton(EXboxControllerButton button);   // 0x00445EE0',
        '    long GetAssignableSpecialMoveIndexForButton(EXboxControllerButton button) const;   // 0x00445EE0 (Ego_r.pdb / FableWin.pdb: const)')
he.edit(a, 'src', '''class CDrawAbilityButtons { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0
    virtual void ClearPrimitives();                // slot 1
    virtual void v02();                            // slot 2
    virtual long GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def);   // slot 3
''', '''class CEnginePrimitivePrepareInfo;
class CDrawAbilityButtons { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0
    virtual void ClearPrimitives();                // slot 1
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);   // slot 2 (chunk 702: Ego_r donor order Draw / this / DrawAbilityList)
protected:
    virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const;   // slot 3
public:
''')
he.edit(a, 'src', 'long CDrawAbilityButtons::GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def)\n',
        'long CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const\n')
note(a, 'c702', 'slot 3 is Ego_r.pdb / FableWin.pdb ?GetAssignableSpecialMoveIndexForButtonDef@CDrawAbilityButtons@NPlayerGui@@MBEJ\n'
     '//   ABVCAbilityButtonGuiDef@@@Z (protected virtual const; the c702 PC override 0x00657200 is the same slot), calling the const\n'
     '//   ?GetAssignableSpecialMoveIndexForButton@CGamePlayerInterface@@QBEJW4EXboxControllerButton@@@Z 0x00445EE0 (the landed own row\'s Leaf);\n'
     '//   was the descriptive non-const GetSpecialMoveIndexForButton calling the non-const QAE spelling; slot 2 is Draw (was v02)',
     leaf='?GetAssignableSpecialMoveIndexForButtonDef@CDrawAbilityButtons@NPlayerGui@@MBEJABVCAbilityButtonGuiDef@@@Z')
