# assemble_nf325.py: RENAMES.tsv (section 1 from evidence/libcp_*.txt self renames + the catalog; section 2 the two callee respellings),
# APPLIED.tsv, TABLE_CHANGES.tsv, NAME_FIXES.tsv, REJECTED.tsv for the NF325 hand-back.
import re, glob, sys
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333'
sys.path.insert(0, A + '/tools'); import nf
cat = nf.catalog()
ren = {}
for f in glob.glob(A + '/evidence/libcp_*.txt'):
    for l in open(f):
        m = re.match(r'([0-9a-f]{8}) LIBCP_OK renames=(\S+?)=(\S+)', l)
        if m: ren[m.group(1)] = (m.group(2), m.group(3))
rows = open(A + '/rows.txt').read().split()
assert set(rows) == set(ren), (set(rows) ^ set(ren))
ev = {'00a0d2b0': 'RTTI vftable 0x0129CA10 slot 1 + Ego_r.pdb (landed FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE declaration); header route',
      '00a85fb0': 'RTTI vftable 0x0129DE88 slot 1 + both PDB name sets; class-model move (CPhysicsFacesSegmentBranch opt-in)',
      '00bbddb0': 'RTTI vftable 0x012A5BFC slot 29 + both PDB name sets; class-model move over the fixed primitive bases'}
s1 = ['# Section 1: self renames', 'address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence']
ap = ['address\tfile\tchange']
for a in rows:
    old, new = ren[a]; m = re.match(r'\?([^@]+)@([^@]+)@@', new); M, X = m.group(1), m.group(2)
    e = ev.get(a, 'RTTI vftable placement (labels_rtti_port.tsv) + the PDB name set(s) of ENTRIES; class-model move onto the generated opt-in')
    s1.append('0x%s\t%s\t%s\t%s\t%s\t%s\t%s' % (a.upper(), X, M, new, cat[a]['name'], old, e))
    ap.append('0x%s\t%s\tclass-model / header move: %s -> %s' % (a.upper(), nf.field(a, 'Source'), old, new))
s2 = ['# Section 2 (caller\told_symbol\tnew_symbol\tcallee)',
      '0x008B3810\t?Init@CCharString@@QAEXPBDH@Z\t??0CCharString@@QAE@PBDJ@Z\t0x0099EBF0',
      '0x008B42E0\t?Init@CCharString@@QAEXPBDH@Z\t??0CCharString@@QAE@PBDJ@Z\t0x0099EBF0',
      '0x00A0D2B0\t?Register@CInputProcessManager@@QAEXPAVCAInputProcess@@@Z\t?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z\t0x00A0D4F0']
open(A + '/RENAMES.tsv', 'w').write('\n'.join(s1 + s2) + '\n')
open(A + '/APPLIED.tsv', 'w').write('\n'.join(ap) + '\n')
open(A + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n'
    'replace\t?Register@CInputProcessManager@@QAEXPAVCAInputProcess@@@Z -> ?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z\t00a0d4f0\t00a0d2b0\tthe witness row now binds the PDB name (both PDB name sets, protected); its new object UND (evidence/libcp_c5.txt); no other table line or landed row binds the Register stand-in\n')
open(A + '/NAME_FIXES.tsv', 'w').write('address\tcorrect_symbol\tevidence\twrong_spellings_seen\n'
    '0x00A0D4F0\t?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z\tboth PDB name sets (protected) + the donor call from AddToInterface (NF321); binder 0x00A0D2B0 moves onto it in this hand-back (unresolved before and after, NF302 Q2)\tself row defines ?AddInputProcess@CInputProcessManager@@QAEXPAUCInputProcessBase@@@Z (stand-in parameter type, public)\n'
    '0x0091D820\t?Initialise@CAIStateGroup_EnemyCreatureBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z\tRTTI vftable 0x0128B8BC slot 1 + both PDB name sets; declared by the new add-only FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_OVERRIDES\t?base_init@@YIXPAUC@@@Z (binder 0x0096A250); ?Initialise@CAIStateGroup_EnemyCreatureBase@@QAEXAAVCAIBrain@@AAVCThingAICreature@@JJABVCCharString@@@Z (binder 0x0095A890)\n'
    '0x0091DD30\t?IsRunnable@CAIStateGroup_EnemyCreatureBase@@UAE_NXZ\tRTTI vftable 0x0128B8BC slot 11 + both PDB name sets; declared by FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_OVERRIDES\t?IsRunnable@CAIStateGroup_EnemyCreatureBase@@QAE_NXZ (binder 0x00959F30)\n'
    '0x00B8FDA0\t?SetChildPrimitive@CEngineInternalPrimitiveBase@@UAEXKABVCEnginePrimitive@@K@Z\tRTTI vftable 0x012A3D7C slot 8 + labels_rtti_port.tsv\tself row defines the free stand-in CEngineInternalPrimitiveBase_SetChildPrimitive(int,int,int) (__stdcall)\n'
    '0x00B8FDB0\t?SetChildPrimitive@CEngineInternalPrimitiveBase@@UAEXKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z\tRTTI vftable 0x012A3D7C slot 7 + labels_rtti_port.tsv\tself row defines the free stand-in CEngineInternalPrimitiveBase_SetChildPrimitive(int,int,int,int) (__stdcall)\n')
open(A + '/REJECTED.tsv', 'w').write('address\treason\n')
print('rows', len(rows))
