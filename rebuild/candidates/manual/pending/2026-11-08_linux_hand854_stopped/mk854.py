import os,re
S=os.environ['S']
h=open(S+'/w22/head853.md').read()
levers=h[h.index('Levers to know'):h.index('The rows (lengths')]
levers=levers.replace("- chunk 851 (4 of 6)","- chunk 852 (0 of 6): a CPersistTraits<float> temporary retail never zeroes needs an empty user-declared ctor; a catalog name\n  from a PersistError literal can be wrong (check the `ret` size and what the body allocates); VC7.1 expands a plain-inline\n  CScriptThing::operator= at no site and forceinline expands all: park that split early.\n- chunk 851 (4 of 6)",1)
assert 'chunk 852 (0 of 6)' in levers
head="""# Chunk 854 -- six never-attempted 1.0-1.4 KB rows: four with PDB names, two placeholders to prove (overrides PROMPT.md where they differ)

Check every name against a second source before authoring and keep the exact PDB spelling. Use the shared headers (engine/<Class>.h) and
add missing members as additive `#ifdef FABLE_RETAIL_*` opt-ins (never a second model of a class).

""" + levers + """The rows (lengths exact; the oracle is exact):
- 0x00AB58E0 `?Update@CMouseDX@@UAEXXZ` (1056, ends with a switch table; the CMouseDX ctor follows at 0x00AB5D00).
- 0x0083EFE0 `?ReceiveDocumentAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ` (1074; ParseLocationData follows at 0x0083F420).
- 0x00501D30 `?GetLevelFilesFromWorldFile@CWorldMap@@IAEXABVCWideString@@AAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z`
  (1086; chunk 850's CWorldMap::LoadRegion notes and hdr opt-ins are in rebuild/candidates/manual/pending/2026-11-08_linux_hand850/).
- 0x007602F0 (1126): catalogued as the placeholder `Math_PolygonAngleSumClosureCheck`, right before
  NShape::GetFirstPointThatBreaksConvexity 0x00760760. PROVE its name (NShape entries of the Ego_r name set, its callers) and list it in
  LABELS.tsv / NAME_FIXES.tsv.
- 0x00924C70 `?Update@CAIStateGroup_KrakenAttack@@MAEXXZ` (1152, ends with a switch table).
- 0x00507070 (1432; `ret 4`): catalogued as the placeholder `RebuildStringPairTableFromTree`, right before CWorldMap::DeleteAllRegions
  0x00507610. PROVE its name (CWorldMap entries of the Ego_r name set, its callers) and list it.

Chunk 853 ($S/w22/agent853) runs beside you; none of your rows. You share the lane with ONE other agent: run at most 2 grades at once.

"""
open(S+'/w22/head854.md','w').write(head)
b=open(S+'/w22/agent853/CHUNK_BRIEF.md').read()
body=b[b.index('Known residues to recognise and park early'):]
body=body.replace('one other authoring agent (chunk 852, $S/w22/agent852)','one other authoring agent (chunk 85Y, $S/w22/agent85Y)').replace('with the chunk 852 agent','with the chunk 85Y agent')
body=body.replace('agent853','agent854').replace('chunk853','chunk854').replace('chunk 853','chunk 854')
body=body.replace('85Y','853')
t=head+body
open(S+'/w22/agent854/CHUNK_BRIEF.md','w').write(t)
print(sorted(set(re.findall(r'agent8\d\d',t))), re.findall(r'(?:one|two) other authoring agents? \([^)]*\)',t), re.findall(r'with the chunk 8\d\d agent',t))
p=open(S+'/w22/prompt853.txt').read()
p=p.replace('one other authoring agent (chunk 852)','one other authoring agent (chunk 85Y)')
p=p.replace('agent853','agent854').replace('chunk 853','chunk 854').replace('85Y','853')
old="Five rows are new (three with PDB names, two placeholders whose names you must prove) and 0x0050A650 is a retry of chunk 850's source failure (start from the probe source the brief names): check each name against a second source, in the exact PDB spelling."
assert old in p
p=p.replace(old,"All six rows are new: four carry PDB names and two are placeholders whose names you must prove: check each name against a second source, in the exact PDB spelling.")
open(S+'/w22/prompt854.txt','w').write(p)
print(sorted(set(re.findall(r'agent8\d\d|chunks? 8\d\d[^)]*\)',p))))
