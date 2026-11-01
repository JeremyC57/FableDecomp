# fixren24.py: post-fixes RENAMES.tsv from mkren24.py where the leaf heuristic cannot tell:
#  00a0d410 (bcnc caller, no Leaf line: picked the base ??_G) and 00c309e0 (placeholder class, decision D2) dropped;
#  00c17260 names the row's own function LoadBank, not the same-TU dummy caller;
#  01224030 keeps the static-init module (wave 13 / 21 precedent: staticinit_<variable>).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'RENAMES.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]
for r in R[1:]:
    a = r[0]
    if a in ('00a0d410', '00c309e0'): continue
    if a == '00c17260':
        r[2] = 'LoadBank'; r[6] = r[6].replace('LoadBankSameTUCaller', 'LoadBank'); r[8] = r[8].replace('LoadBankSameTUCaller', 'LoadBank')
        r[9] = 'naming wave 24 (cstr): the row now defines static __stdcall LoadBank(CLoadLog*, void*, CRequest_00c09110*) (internal linkage, modelled on 0x00C172F0; decision D1); the file also carries the same-TU dummy caller ?LoadBank_SameTUCaller@@YAXPAXPAUCRequest_00c09110@@@Z'
    if a == '01224030':
        r[2] = '_static_init::NBankFileManager::SymbolHeaderDir'; r[4] = '_static_init'
        r[6] = r[6].replace('NBankFileManager_SymbolHeaderDir', 'staticinit_NBankFileManagerSymbolHeaderDir'); r[8] = r[8].replace('NBankFileManager_SymbolHeaderDir', 'staticinit_NBankFileManagerSymbolHeaderDir')
    out.append(r)
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'renames after fix')
# filt6 own rows: the defined Leaf still uses the CThingSearchToolsMethods view (decision D6); the PDB class is
# CThingSearchTools (Ego_r.pdb 0x007e8450 / 0x02490360 ??$GetPFirstThing@VCThingFilter_HasTC@@...@CThingSearchTools@@QBE_N...),
# so the file keeps the CThingSearchTools module.
R = [l.rstrip('\n').split('\t') for l in open(p)]
for r in R[1:]:
    if r[4] == 'CThingSearchToolsMethods':
        r[2] = r[2].replace('CThingSearchToolsMethods::', 'CThingSearchTools::'); r[4] = 'CThingSearchTools'
        for i in (6, 8): r[i] = r[i].replace('/CThingSearchToolsMethods_', '/CThingSearchTools_')
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in R))
