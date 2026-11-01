# own-row respells fam_leaf skipped as static/virtual differ: the Ego_r spellings are virtual (UAE); the local models already
# carry the vptr (a placeholder virtual or an explicit __vftable at +0), so the method becomes the named virtual with the same
# offsets. 0x00759A00 / 0x007239C0 SetRHSet (slot 75), 0x007C75E0 OnRemove (slot 2), 0x007F7700 Deactivate (text/).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, path
a = '007f7700'
p, rel = path(a, 'src'); new = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'text', a + '.cpp')).read()
if open(p).read() == new: print('already', a, 'src')
else: open(p, 'w').write(new); print('ok', a, 'src')
a = '00759a00'
edit(a, 'src', '    void vfunc_75(const CRightHandedSet& set);', '    virtual void SetRHSet(const CRightHandedSet& set);   // slot 75 (Ego_r.pdb CTCPhysicsRigidBody::SetRHSet)')
edit(a, 'src', 'void CTCPhysicsRigidBody::vfunc_75(const CRightHandedSet& set)\n{', 'void CTCPhysicsRigidBody::SetRHSet(const CRightHandedSet& set)\n{')
edit(a, 'src', '// CTCPhysicsRigidBody::vfunc_75(const CRightHandedSet& set) --', '// CTCPhysicsRigidBody::SetRHSet(const CRightHandedSet& set) --')
edit(a, 'src', 'of CTCPhysicsStandard is SetFacingAnglesFromRHSet 0x007239C0, which', 'of CTCPhysicsStandard is SetRHSet 0x007239C0, which')
a = '007239c0'
edit(a, 'src', '    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& set);   // slot 75, +0x12c', '    virtual void SetRHSet(const CRightHandedSet& set);   // slot 75, +0x12c')
edit(a, 'src', '    void SetFacingAnglesFromRHSet(const CRightHandedSet& set);\n};', '    virtual void SetRHSet(const CRightHandedSet& set);   // slot 75 (Ego_r.pdb CTCPhysicsStandard::SetRHSet)\n};')
edit(a, 'src', 'void CTCPhysicsStandard::SetFacingAnglesFromRHSet(const CRightHandedSet& set)\n{', 'void CTCPhysicsStandard::SetRHSet(const CRightHandedSet& set)\n{')
edit(a, 'src', '// CTCPhysicsStandard::SetFacingAnglesFromRHSet(const CRightHandedSet& set) --', '// CTCPhysicsStandard::SetRHSet(const CRightHandedSet& set) --')
edit(a, 'src', ' virtual void v74();', ' virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& set);   // slot 74 (0x007238E0)\n   ')
a = '007c75e0'
edit(a, 'src', 'class CTCTurncoatSpell { public:\n    void*                   __vftable;      // +0x00\n', 'class CTCTurncoatSpell { public:\n    virtual void OnRemove();                // slot 2 (CTCBase::OnRemove; Ego_r.pdb CTCTurncoatSpell::OnRemove)\n')
edit(a, 'src', '    void FinalCleanup();\n', '')
edit(a, 'src', 'void CTCTurncoatSpell::FinalCleanup()\n{', 'void CTCTurncoatSpell::OnRemove()\n{')
edit(a, 'src', '// CTCTurncoatSpell::FinalCleanup() --', '// CTCTurncoatSpell::OnRemove() --')
