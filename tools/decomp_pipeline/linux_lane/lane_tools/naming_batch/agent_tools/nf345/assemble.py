#!/usr/bin/env python3
"""NF345: assemble land_scripted run dirs batches/bN/ from rows/, orig/, work/vl payloads and logs (copies only; never touches the repo)."""
import os, shutil, sys
F = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PW = '?PointIsWithinCell%s@CWorldMap@@IBE_NPBVC3DVector@@0@Z'
J = '?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@std@@@2@ABU?$pair@$$CBVCCharString@@J@2@@Z'
K = J.replace('@J@', '@K@')
U1 = ('NF345 U1 self swap (lands as one unit with 0063b890/0063b920/0063b9b0/0063ba30): ego_r CWorldMap::FindGround/FindTheme call order TR,BL,TL,BR '
      '= retail calls 0063b890,0063b920,0063b9b0,0063ba30; Ego_r object order + sizes 0x90/0x90/0x80/0x80 v retail 129/129/123/123; the fadd '
      'only in TR and BL in both builds; binders 0063d0a0/0063d4a0 already bind these names at these addresses (table lines 31716-31719, xr). '
      'objsec ALL_EQ under the rename; vland --recheck-landed HEAD and new RELOCATION_MATCH PASS; link HEAD v new 0 duplicates; no table change.')
B = {
 'b1': [
  ('0063b890', PW % 'BR', PW % 'TR', U1, 'u1_pointiswithincell'),
  ('0063b920', PW % 'TL', PW % 'BL', U1 + ' (row gains its // Leaf: line)', 'u1_pointiswithincell'),
  ('0063b9b0', PW % 'BL', PW % 'TL', U1, 'u1_pointiswithincell'),
  ('0063ba30', PW % 'TR', PW % 'BR', U1, 'u1_pointiswithincell'),
  ('0059697a', '?CreateNewProfile@CFrontEndManager@NUISystem@@IAE_NAAVCWideString@@@Z', '?CreateNewProfile@CFrontEndManager@NUISystem@@IAE_NAAVCWideString@@@Z',
   'NF345 U2 binder respell; target 0x0040D2F0 SetCameraYZInverted -> ?SetCameraBowYZInverted@CUserProfileManager@@QAEX_N0@Z (Ego_r + FableWin PDB name; 0x0040D2F0 stores '
   '+0x12/+0x14/+0xD2/+0xD4 = PDB CameraFPBowYZInverted 18 / PCBow 20 fields, 0x0040B050 stores +0x10/+0xD0; ego_r CreateNewProfile calls the Bow setter). Owner 0040d2f0 '
   'already defines the Bow name (MATCH), so the reference resolves after and was mis-resolved before. objsec ALL_EQ; vland HEAD/new RELOCATION_MATCH PASS, '
   'the HEAD RELOC_NAME warning goes; test respelled. TABLE_CHANGES: delete (SetCameraYZInverted, 0040d2f0), add (SetCameraYZInverted, 0040b050, 0041e6d3).', 'u2_camerabow'),
  ('004fede0', '?UnloadLevel@CWorldMap@@IAEXJ@Z', '?UnloadLevel@CWorldMap@@IAEXJ@Z',
   'NF345 U3 binder respell; target 0x00640430 RemoveMap -> ?SetMapAsUnloaded@CEngineManager@@QAEXJ@Z (Ego_r + FableWin PDB; ego_r UnloadLevel/UnloadRegion call SetMapAsUnloaded; '
   'owner 00640430 self = SetMapAsUnloaded MATCH; 004ff8b0 binds RemoveMap at 006403d0). objsec ALL_EQ; vland HEAD/new RELOCATION_MATCH PASS, HEAD RELOC_NAME warning goes; test respelled. '
   'TABLE_CHANGES: replace RemoveMap -> SetMapAsUnloaded at 00640430 (witness 004fede0), add RemoveMap 006403d0 (004ff8b0).', 'u3_setmapasunloaded'),
  ('004feec0', '?UnloadRegion@CWorldMap@@IAEXJ_N@Z', '?UnloadRegion@CWorldMap@@IAEXJ_N@Z',
   'NF345 U3 binder respell; target 0x00640430 RemoveMap -> ?SetMapAsUnloaded@CEngineManager@@QAEXJ@Z (as 004fede0; its landed test does not name it, unchanged). objsec ALL_EQ; '
   'vland HEAD/new RELOCATION_MATCH PASS, HEAD RELOC_NAME warning goes.', 'u3_setmapasunloaded'),
 ],
 'b2': [
  ('0041e6d3', '?Action@CManager@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z', '?Action@CManager@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z',
   'NF345 b2 (DECISION D1 first) binder respell; target 0x0047D008 PCSetToLoad -> ?LoadGame@CTCInGameMenu@@QAEXJ@Z (Ego_r + FableWin PDB; 0x0047D008 is the 10-byte LoadGameIndex +0x64 setter, '
   'ego_r LoadGame ~10 bytes v PCSetToLoad ~489 (row 0047dd9d 493); ego_r CManager::Action calls LoadGame). HEAD resolved the reference to the wrong body 0047dd9d; '
   'owner 0047d008 still defines the stand-in ?Set@T@@QAEXH@Z, so new is unresolved until the owner is ported (NF302 Q2 shape). objsec ALL_EQ; vland HEAD/new RELOCATION_MATCH PASS; test unchanged. '
   'TABLE_CHANGES (b2): replace PCSetToLoad -> LoadGame at 0047d008 (0041e6d3).', '0041e6d3'),
  ('00e1a990', '?Main@CQ_WizardBattleScript@NScript@@UAEXXZ', '?Main@CQ_WizardBattleScript@NScript@@UAEXXZ',
   'NF345 b2 (DECISION D1 first) binder respell; target 0x00E1AD30 Init -> ?InitialiseVariables@CQ_WizardBattleScript@NScript@@QAEXXZ (FableWin PDB; FableWin Main calls InitialiseVariables first, '
   'retail Main calls 0x00E1AD30 first and that body zeroes +0x48..+0x54; FableWin object order Init < Main < InitialiseVariables = retail 00e1a870 < 00e1a990 < 00e1ad30). '
   'HEAD resolved to the wrong body 00e1a870 (Init, correct self); owner 00e1ad30 still defines the stand-in ?Init@C@@QAEXXZ (engine/CQ_WizardBattleScript.h exists: owner fix is a port). '
   'objsec ALL_EQ; vland HEAD/new RELOCATION_MATCH PASS; test respelled. TABLE_CHANGES (b2): replace Init -> InitialiseVariables at 00e1ad30 (00e1a990).', '00e1a990'),
 ],
 'b3': [
  ('0042b587', J, K,
   'NF345 b3 self respell long -> unsigned long (map<CCharString, unsigned long>): the only retail caller 0x0042BCB5 is a proven map<CCharString,unsigned long> user and its object binds the K '
   'spelling at 0042b587 (xr); retail has no identical-code folding, so the J leaf belongs to 0046ece7''s instance. The object also renames its pick-any copies (pair<const CCharString,unsigned long> copy-ctor, '
   'EQ at 004297db before and after; _Construct). No landed binder of the old J _M_create_node spelling at 0042b587. objsec ALL_EQ; vland HEAD/new RELOCATION_MATCH PASS; test respelled. '
   'TABLE_CHANGES: replace pair<..J> copy-ctor -> K at 004297db (0042b587).', '0042b587'),
 ],
}
for b, rows in B.items():
    d = f'{F}/batches/{b}'
    for sub in ('rows', 'orig/rows', 'orig/tests', 'tests', 'head', 'logs'): os.makedirs(f'{d}/{sub}', exist_ok=True)
    with open(f'{d}/RESULTS.tsv', 'w') as o:
        o.write('address\tverdict\told\tnew\treason\n')
        for a, old, new, why, u in rows: o.write(f'{a}\tACCEPT\t{old}\t{new}\t{why}\n')
    with open(f'{d}/UNITS.tsv', 'w') as o:
        o.write('address\tunit\tnote\n')
        for a, old, new, why, u in rows: o.write(f'{a}\t{u}\tunit: lands whole or not at all (with its TABLE_CHANGES lines)\n')
    open(f'{d}/rows.txt', 'w').write(''.join(r[0] + '\n' for r in rows))
    for a, *_ in rows:
        shutil.copyfile(f'{F}/rows/{a}.cpp', f'{d}/rows/{a}.cpp')
        shutil.copyfile(f'{F}/orig/rows/{a}.cpp', f'{d}/orig/rows/{a}.cpp')
        shutil.copyfile(f'{F}/orig/tests/{a}_test.cpp', f'{d}/orig/tests/{a}_test.cpp')
        if os.path.exists(f'{F}/rows/{a}_test.cpp'): shutil.copyfile(f'{F}/rows/{a}_test.cpp', f'{d}/tests/{a}_test.cpp')
    shutil.copyfile(f'{F}/work/vl/{b}_head_payload.json', f'{d}/head/payload.json')
    shutil.copyfile(f'{F}/work/vl/{b}_new_payload.json', f'{d}/new_payload.json')
    open(f'{d}/head/HEAD_FROM_REPO.txt', 'w').write('head/payload.json was built by NF345 tools/nf.py pay (HEADPAY=1) from the repo tree (rebuild/src/compiled sources + landed tests, '
        'catalog flags) on 2026-11-24; new_payload.json is the same build from rows/ (+ tests/). The oracle TSVs stay in work/ (retail bytes).\n')
    for k in ('head', 'new'): shutil.copyfile(f'{F}/work/vland_{b}_{k}.log', f'{d}/logs/vland_{k}.log')
    shutil.copyfile(f'{F}/evidence/proof_{b}.txt', f'{d}/logs/objsec_proof.txt')
print('ok')
