# wave 7 family (generic reference accessors, fam_ref6 engine): retail accessors returning a reference, modelled as
# returning a pointer (same bytes, wrong mangling). See fam_ref6.py for the transform.
#   0x00661ED0 ?GetCombatManager@CThingCreatureBase@@QAEAAVCCombatManager@@XZ (Ego_r.pdb + FableWin.pdb; chunk 563: the
#              non-const overload -- its body uses the non-const CThing::GetWorld 0x004C7AA0, the const twin 0x00661EC0
#              uses 0x004C7AB0; 0x006AA230 passes the result as `this` straight on)
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fam_ref6
fam_ref6.SPEC[:] = [
    dict(addr='0x00661ED0', cls='CThingCreatureBase', ret='CCombatManager', olds=('GetCombatManager',), new='GetCombatManager',
         newdecl='CCombatManager& GetCombatManager();'),
]
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fam_ref6.fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
