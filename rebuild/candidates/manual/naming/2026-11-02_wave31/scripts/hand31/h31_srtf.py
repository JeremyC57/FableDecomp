# h31_srtf.py: naming wave 31, family srtf (coordinator DECISION row 0x004C7590, chunk 703): settle ONE spelling for the CThing forwarder
#  0x004C7590 (`mov ecx,[ecx+64h] ; test ; jz -> ret 4 ; jmp CTCGraphicAppearance::SetAsChanged 0x004BF9E0`). Evidence says the PDB
#  name is ?SetAsRenderedThisFrame@CThing@@QAEXK@Z, not the coordinator's default descriptive ?SetGraphicAppearanceChanged@CThing@@QAEXK@Z:
#   (1) Ego_r.pdb block order at a constant 0x1BC0 delta: SetCurrentMapNumber 0x004C9140 = retail 0x004C7580 (landed, same PDB name),
#       SetAsRenderedThisFrame(unsigned long) 0x004C9150 = retail 0x004C7590 (16 bytes in both), ClearRenderedThisFrame() 0x004C9160 =
#       retail 0x004C75A0 (a bare `ret`: void, no arguments), SetAsActivated 0x004C9330 = retail 0x004C7770 (cited by the landed own row);
#   (2) FableWin.pdb carries the same ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (0x01CF9240), and the signature fits retail: one
#       unsigned long (`ret 4`) passed straight on as SetAsChanged's unsigned long reasons.
#  The own row and the six binders that spelled it seven ways are respelled to it.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
SYM = '?SetAsRenderedThisFrame@CThing@@QAEXK@Z'
DECL = 'void SetAsRenderedThisFrame(unsigned long reasons);'
def nb(a, was):
    note(a, 'srtf', '0x004C7590 is %s (Ego_r.pdb block order at 0x004C9150 + FableWin.pdb; was the\n'
         '//   invented %s)' % (SYM, was))

a = '004c7590'
he.edit(a, 'src', '// CThing::SetGraphicAppearanceChanged(unsigned long flags) -- marks', '// CThing::SetAsRenderedThisFrame(unsigned long reasons) -- marks')
he.edit(a, 'src', 'The name is descriptive.', 'Name: Ego_r.pdb\n'
        '// SetAsRenderedThisFrame 0x004C9150 sits between SetCurrentMapNumber 0x004C9140 (= retail 0x004C7580) and ClearRenderedThisFrame\n'
        '// 0x004C9160 (= retail 0x004C75A0, a bare `ret`), at the 0x1BC0 delta of SetAsActivated; FableWin.pdb has the same symbol.')
he.edit(a, 'src', '    void SetGraphicAppearanceChanged(unsigned long flags);\n', '    ' + DECL + '\n')
he.edit(a, 'src', '''void CThing::SetGraphicAppearanceChanged(unsigned long flags)
{
    if (PTCGraphicAppearance)
        PTCGraphicAppearance->SetAsChanged(flags);''', '''void CThing::SetAsRenderedThisFrame(unsigned long reasons)
{
    if (PTCGraphicAppearance)
        PTCGraphicAppearance->SetAsChanged(reasons);''')
note(a, 'srtf', 'own row is %s (Ego_r.pdb block order + FableWin.pdb; was the descriptive\n'
     '//   ?SetGraphicAppearanceChanged@CThing@@QAEXK@Z)' % SYM, leaf=SYM)

a = '00640bb0'
he.edit(a, 'src', '    void ForwardToGraphicAppearance(long request);  // 0x004C7590', '    ' + DECL + '  // 0x004C7590')
he.edit(a, 'src', 'ParentThing->ForwardToGraphicAppearance(4);', 'ParentThing->SetAsRenderedThisFrame(4);')
nb(a, '?ForwardToGraphicAppearance@CThing@@QAEXJ@Z')

a = '006afd60'
he.edit(a, 'src', '    void RefreshGraphic(bool flag);             // 0x004C7590', '    ' + DECL + ' // 0x004C7590')
he.edit_all(a, 'src', 'ParentThing->RefreshGraphic(true);', 'ParentThing->SetAsRenderedThisFrame(1);', 2)
nb(a, '?RefreshGraphic@CThing@@QAEX_N@Z')

a = '0082d810'
he.edit(a, 'src', 'class CThingGameObject { public:\n', 'class CThing { public:\n    ' + DECL + '   // 0x004C7590\n};\n\nclass CThingGameObject : public CThing { public:\n')
he.edit(a, 'src', '    void UpdateGraphicAppearance(long reason);    // 0x004C7590 (CThing)\n', '')
he.edit(a, 'src', '        UpdateGraphicAppearance(4);', '        SetAsRenderedThisFrame(4);')
nb(a, '?UpdateGraphicAppearance@CThingGameObject@@QAEXJ@Z')

a = '00752200'
he.edit(a, 'src', '    void UpdateGraphicAppearance(long reason);  // 0x004C7590', '    ' + DECL + '  // 0x004C7590')
he.edit(a, 'src', 'ParentThing->UpdateGraphicAppearance(4);', 'ParentThing->SetAsRenderedThisFrame(4);')
nb(a, '?UpdateGraphicAppearance@CThing@@QAEXJ@Z')

a = '007ce420'
he.edit(a, 'src', '    void SetActive(bool active);                    // 0x004C7590', '    ' + DECL + '   // 0x004C7590')
he.edit(a, 'src', 'thing->SetActive(true);', 'thing->SetAsRenderedThisFrame(1);')
nb(a, '?SetActive@CThing@@QAEX_N@Z')

a = '0077c740'
he.edit(a, 'src', '    void SetGraphicAppearanceChanged(unsigned long flags);   // 0x004C7590', '    ' + DECL + '        // 0x004C7590')
he.edit(a, 'src', 'ParentThing->SetGraphicAppearanceChanged(4);', 'ParentThing->SetAsRenderedThisFrame(4);')
nb(a, '?SetGraphicAppearanceChanged@CThing@@QAEXK@Z')

# 0x007CE370: the free __fastcall OnInitialActivate over `struct Sub` is CTCElectrocutedEffect::OnInitialActivate (CTCBase slot 6;
#  PDB ?OnInitialActivate@CTCElectrocutedEffect@@UAEXXZ) calling the parent thing's SetAsRenderedThisFrame / SetAsFrameUpdated
a = '007ce370'
he.put(a, 'src', '''// CTCElectrocutedEffect::OnInitialActivate() -- mark the parent thing rendered this frame (reason 1) and frame-updated
// Leaf: ?OnInitialActivate@CTCElectrocutedEffect@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CE370. Name: Ego_r.pdb / FableWin.pdb ?OnInitialActivate@CTCElectrocutedEffect@@UAEXXZ
//   (CTCBase slot 6, engine/CTCBase.h retail branch).
// Callees: CThing::SetAsRenderedThisFrame(unsigned long) 0x004C7590, CThing::SetAsFrameUpdated() 0x004C7C40 (both PDBs) on ParentThing.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CThing { public:
    void SetAsRenderedThisFrame(unsigned long reasons);   // 0x004C7590
    void SetAsFrameUpdated();                             // 0x004C7C40
};

class CTCElectrocutedEffect : public CTCBase { public:
    virtual void OnInitialActivate();
};

void CTCElectrocutedEffect::OnInitialActivate()
{
    ParentThing->SetAsRenderedThisFrame(1);
    ParentThing->SetAsFrameUpdated();
}
''')
note(a, 'srtf', 'own row is the PDB virtual member on engine/CTCBase.h (was the free __fastcall OnInitialActivate over\n'
     '//   `struct Sub`, bare Leaf); its callees bind %s and ?SetAsFrameUpdated@CThing@@QAEXXZ (were ?SetActive@Sub@@QAEHH@Z /\n'
     '//   ?Refresh@Sub@@QAEXXZ)' % SYM)

# the other sixteen landed binders (each declares the forwarder on a local `class CThing`, public, with a `// 0x004C7590` comment)
import re
OTHER = {'ForwardToGraphicAppearance': ('00640d00', '0070a430', '0070a490', '00781f10'),
         'UpdateGraphicAppearance': ('00640c40', '00640c70', '00640ca0', '00640cd0', '00641020', '006412b0', '006b0c10', '007034d0',
                                     '0077cd50', '00835e30', '00836260'),
         'SetGraphicAppearanceChanged': ('00836240',)}
WAS = {'ForwardToGraphicAppearance': '?ForwardToGraphicAppearance@CThing@@QAEXJ@Z', 'UpdateGraphicAppearance': '?UpdateGraphicAppearance@CThing@@QAEXJ@Z',
       'SetGraphicAppearanceChanged': '?SetGraphicAppearanceChanged@CThing@@QAEXK@Z'}
for old, rows in OTHER.items():
    for a in rows:
        he.resub(a, 'src', r'void %s\((?:long reason|long request|unsigned long flags)\);( +)// 0x004C7590( \(name unproven\))?' % old,
                 lambda m: DECL + m.group(1)[:max(1, len(m.group(1)) - 3)] + '// 0x004C7590', 1)
        he.resub(a, 'src', r'(?<![\w:])%s\((\d+)\);' % old, r'SetAsRenderedThisFrame(\1);', 1)
        nb(a, WAS[old])
