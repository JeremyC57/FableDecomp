"""assemble338.py: build the land_scripted run dirs batches/b1 (entries 62 / 119 / 120 / 149 binders) and batches/b2 (entries 223 / 224:
binder 0x00405650 + the fable_ime.h opt-in), from rows/, orig/, work/rc2h_payload.json, the recheck log and the evidence files."""
import os, json, shutil
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338'
R = '/home/user/FableDecomp'
P = json.load(open(A + '/work/rc2h_payload.json'))
leaf = {'00999110': '?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z', '0056fad7': '?OpenInventory@CTCInventoryExperience@@QAEXXZ',
        '006116e0': '?OpenInventory@CTCInventoryMap@@QAEXXZ', '00a3aa40': '?InternalCompress@CCompressorZlib@@QAEKPBXKPAXK@Z',
        '00405650': '?Initialise@CIME@NUISystem@@QAEXPAUHWND__@@@Z'}
why = {
 '00999110': 'binder respell (entry 149, nf301 T3): GetCurrentWorkingDirectory -> CAFile::GetCurrentPath (PDB public static, Ego_r + FableWin ?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ) on its local CAFile model; objsec ALL_EQ; no table line; target 0x00997F80',
 '0056fad7': 'binder respell (entry 62, unit u62 with 0x006116E0): free OnOpenMenus -> NUISystem::CManager::OnOpenMenus (PDB public static, Ego_r + FableWin) on its local CManager model; objsec ALL_EQ; target 0x0041D122',
 '006116e0': 'binder respell (entry 62, unit u62 with 0x0056FAD7; ref_scan found it binding the free spelling): OnOpenMenus -> NUISystem::CManager::OnOpenMenus; objsec ALL_EQ; target 0x0041D122',
 '00a3aa40': 'binder respell (entries 119 / 120): ZAlloc / ZFree stand-ins -> PDB private static CCompressorZlib::Alloc / Free (Ego_r + FableWin ?Alloc@CCompressorZlib@@CAPAXPAXII@Z / ?Free@CCompressorZlib@@CAXPAX0@Z); objsec ALL_EQ; target 0x00A3A9C0 / 0x00A3AA00',
 '00405650': 'binder respell (entries 223 / 224, nf309 T3): free Callback_malloc / Callback_DrawFans -> PDB public static NUISystem::CIME members (Ego_r + FableWin) through the fable_ime.h opt-in FABLE_RETAIL_CIME_CALLBACKS (hdr/); objsec ALL_EQ; target 0x00404A60 / 0x00404E10'}
TC = {
 'b1': [('delete', '?OnOpenMenus@@YIXXZ', '0041d122', '0056fad7', 'entry 62: 0x0056FAD7 / 0x006116E0 now bind ?OnOpenMenus@CManager@NUISystem@@SIXXZ (line exists, witness 0x004830A1); ref_scan rs2: only 0x006116E0 (same unit) referenced it'),
        ],
 'b3': [('delete', '?ZAlloc@CCompressorZlib@@SAPAXPAXII@Z', '00a3a9c0', '00a3aa40', 'entry 119: 0x00A3AA40 now binds ?Alloc@CCompressorZlib@@CAPAXPAXII@Z, whose line TABLE_ONLY.tsv adds (fn_a3a9c0 replace, witness 0x00A3AA10); ref_scan rs2 NONE; either apply order converges'),
        ('delete', '?ZFree@CCompressorZlib@@SAXPAX0@Z', '00a3aa00', '00a3aa40', 'entry 120: as 119 for ?Free@CCompressorZlib@@CAXPAX0@Z; ref_scan rs2 NONE')],
 'b2': [('replace', '?Callback_malloc@@YIPAXI@Z -> ?Callback_malloc@CIME@NUISystem@@SIPAXI@Z', '00404a60', '00405650', 'entry 223: the binder now references the PDB spelling; ref_scan rs3 NONE'),
        ('replace', '?Callback_DrawFans@@YIXPBUIMEUI_VERTEX@@I@Z -> ?Callback_DrawFans@CIME@NUISystem@@SIXPBUIMEUI_VERTEX@@I@Z', '00404e10', '00405650', 'entry 224: the binder now references the PDB spelling; ref_scan rs3 NONE')]}
B = {'b1': ['00999110', '0056fad7', '006116e0'], 'b2': ['00405650'], 'b3': ['00a3aa40']}
for b, rows in B.items():
    d = A + '/batches/' + b
    for sub in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence'): os.makedirs(d + '/' + sub, exist_ok=True)
    for a in rows:
        shutil.copyfile(A + '/rows/%s.cpp' % a, d + '/rows/%s.cpp' % a)
        shutil.copyfile(A + '/orig/%s.cpp' % a, d + '/orig/rows/%s.cpp' % a)
        if os.path.exists(A + '/orig/%s_test.cpp' % a): shutil.copyfile(A + '/orig/%s_test.cpp' % a, d + '/orig/tests/%s_test.cpp' % a)
    json.dump({'result': {'authored': [x for x in P['result']['authored'] if x['address'] in rows]}}, open(d + '/head/payload.json', 'w'))
    open(d + '/RESULTS.tsv', 'w').write('address\tverdict\told\tnew\treason\n' + ''.join('%s\tACCEPT\t%s\t%s\t%s\n' % (a, leaf[a], leaf[a], why[a]) for a in rows))
    open(d + '/rows.txt', 'w').write(''.join(a + '\n' for a in rows))
    open(d + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n' + ''.join('\t'.join(t) + '\n' for t in TC[b]))
    log = [l for l in open(A + '/work/recheck_rc2n.log', errors='replace') if l[:8] in rows or not l[:8].strip() == l[:8]]
    open(d + '/logs/recheck.log', 'w').write(''.join(l for l in open(A + '/work/recheck_rc2n.log', errors='replace') if any(a in l for a in rows)))
open(A + '/batches/b1/UNITS.tsv', 'w').write('address\tunit\tnote\n0056fad7\tu62\tentry 62: both binders of the free OnOpenMenus land together (ref_scan rs2)\n006116e0\tu62\tentry 62: as 0x0056FAD7\n')
os.makedirs(A + '/batches/b2/hdr', exist_ok=True); shutil.copyfile(A + '/hdr/fable_ime.h', A + '/batches/b2/hdr/fable_ime.h'); shutil.copyfile(A + '/orig/fable_ime.h.HEAD', A + '/batches/b2/orig/fable_ime.h')
for f in ('proof_g1.txt', 'proof_g2.txt', 'proof_g3.txt', 'proof_g4.txt', 'link_g1.txt', 'link_g2.txt', 'link_g3.txt', 'link_g4.txt', 'pdb_namesets_62_119_120_149.tsv'):
    for b in ('b1', 'b3'): shutil.copyfile(A + '/evidence/' + f, A + '/batches/%s/evidence/' % b + f)
shutil.copyfile(A + '/work/rs2/REFS.tsv', A + '/batches/b3/evidence/ref_scan_rs2.tsv')
shutil.copyfile(A + '/work/nf285_line.tsv', A + '/batches/b3/T3_READD.tsv')
for f in ('proof_g5.txt', 'link_g5.txt', 'probe_g5_head_overlay_v_head.txt', 'pdb_namesets_223_224.tsv'): shutil.copyfile(A + '/evidence/' + f, A + '/batches/b2/evidence/' + f)
shutil.copyfile(A + '/work/eq/includers.tsv', A + '/batches/b2/evidence/equiv_includers.tsv')
shutil.copyfile(A + '/work/eq/includers_v1_linesshift.tsv', A + '/batches/b2/evidence/equiv_includers_v1_REJECTED_line_shift.tsv')
for r in ('rs2', 'rs3'): shutil.copyfile(A + '/work/%s/REFS.tsv' % r, A + '/batches/%s/evidence/ref_scan_%s.tsv' % ('b1' if r == 'rs2' else 'b2', r))
open(A + '/TABLE_ONLY.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n' + ''.join('\t'.join(t) + '\n' for t in [
 ('delete', '?IsRegisteredCombatant@CThingCreatureBaseCalls@@QAE_NXZ', '00662410', '00733580', 'entry 38 (stale): the HEAD object of 0x00733580 binds ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ (line exists, witness 0x006EB390); ref_scan rs1 NONE'),
 ('delete', '??0CCharString@std@@QAE@ABU01@@Z', '0099ec30', '004d0f80', 'entry 88 (stale): 0x004D0F80 binds ??0CCharString@@QAE@ABV0@@Z (line exists, witness 0x0046381B); the only landed source declaring a CCharString inside namespace std is 0x004D0830 (brace-aware scan), and its HEAD object (compiled) does not reference this symbol (ref_scan would be TOO_MANY on the identifier)'),
 ('delete', '?AddSubChunk@CChunkedFileChunk@@QAEXABV?$CCountedPointer@UCChunkedFileChunk@@@@@Z', '00aa84f0', '00acd5a0', 'entry 112 (stale): 0x00ACD5A0 binds the PDB spelling (line exists, witness 0x00AC0C00); ref_scan rs1 NONE'),
 ('replace', '?fn_a3a9c0@@YAXXZ -> ?Alloc@CCompressorZlib@@CAPAXPAXII@Z', '00a3a9c0', '00a3aa10', 'entry 119 (stale): 0x00A3AA10 binds the PDB private static (compiled); ref_scan rs1 NONE'),
 ('replace', '?fn_a3aa00@@YAXXZ -> ?Free@CCompressorZlib@@CAXPAX0@Z', '00a3aa00', '00a3aa10', 'entry 120 (stale): as 119; ref_scan rs1 NONE'),
 ('delete', '@FableGetCurrentPath_00997F80@8', '00997f80', '00999230', 'entry 149 (stale): 0x00999230 binds ?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ (line exists, witness 0x009990B0); no landed source names FableGetCurrentPath_00997F80 (ref_scan rs4 TOO_MANY on the decorated C name: grep is the census)'),
 ('delete', '?SubmitDrag_00c25970@@YIXPBX@Z', '00c25970', '00c20340', 'entry 186 (stale): all 3 census binders bind ?GetMagnitude@C3DVector@@QBEMXZ (line exists, witness 0x00C45810); ref_scan rs1 NONE'),
 ('delete', '??0CElement56_0049d380@@QAE@ABU0@@Z', '0049d380', '00b2aa70', 'entry 195 (stale): all 11 census binders bind ??0CCamera@@QAE@ABV0@@Z (line exists, witness 0x0049DAA0); ref_scan rs1 NONE')]))
print('ok')
