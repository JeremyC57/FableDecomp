#!/usr/bin/env python3
"""census.py <rows file> <owner new obj> <out tsv> : the 0x00CD3D2E emitter census after the owner respell (agent NF350).
Per row (HEAD object work/hobj/<addr>.obj): does it DEFINE the four-argument ??A map<CCharString,STB> symbol (an emitter), reference it
UND, or neither; does its source use stlport/fable_map.h with FABLE_RETAIL_STLP_MAP (the only place FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
acts) and is the opt-in already on; resolve test = the opt-in is available, so its new UND resolves to the respelled owner, which defines
that exact symbol (select-any, RELOCATION_MATCH). Also, per symbol the respelled owner object defines, how many census objects define the
same symbol and whether their section bytes + relocation symbols equal the owner's (the renamed helper COMDATs now share their names)."""
import sys, re, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF350/tools')
import nf
W = nf.W
rows = [l.split()[0] for l in open(sys.argv[1]).read().splitlines() if l.strip()]
own = sys.argv[2]
A4 = '??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAEAAVCScriptGameResourceObjectScriptedThingBase@@ABVCCharString@@@Z'
def comdats(path):
    secs, sl = nf.coff(path); out = {}
    for i, n, v, sec, typ, scl in sl:
        if scl == 2 and sec > 0 and secs[sec - 1]['name'].startswith('.text'):
            s = secs[sec - 1]; out[n] = (s['data'], tuple((r[1], r[2]) for r in s['rel']))
    return out
oc = comdats(own)
cmp = collections.defaultdict(lambda: [0, 0])
out = open(sys.argv[3], 'w')
out.write('row\tA4\tfable_map_STLP_MAP\tINDEX_DECL_ONLY\tresolve_test\tflags\n')
cnt = collections.Counter()
for a in rows:
    p = W / 'hobj' / (a + '.obj')
    if not p.exists(): out.write('%s\tNOT_COMPILED\t-\t-\t-\t-\n' % a); cnt['not_compiled'] += 1; continue
    e = nf.ext_syms(p); st = 'DEF' if 'DEF ' + A4 in e else ('UND' if 'UND ' + A4 in e else 'none')
    src = open(nf.src_of(a), encoding='utf-8', errors='replace').read()
    # the opt-in acts where the map comes from stlport/fable_map.h (directly or through engine/fable_script_cutscene.h) under FABLE_RETAIL_STLP_MAP,
    # i.e. the row defines FABLE_RETAIL_STLP_MAP and has no row-local map model
    hdr = bool(re.search(r'^#define FABLE_RETAIL_STLP_MAP\s*$', src, re.M)) and not re.search(r'(class|struct)\s+map\b', src)
    dec = bool(re.search(r'^#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY\s*$', src, re.M))
    rt = 'pass' if st == 'DEF' and hdr and not dec else ('n/a-already-UND' if st == 'UND' else ('no-opt-in(local model)' if st == 'DEF' else 'n/a'))
    fl = ' '.join(nf.flags_of(a, src)) or '-'
    out.write('%s\t%s\t%s\t%s\t%s\t%s\n' % (a, st, 'y' if hdr else 'n', 'y' if dec else 'n', rt, fl))
    cnt['A4_' + st] += 1; cnt['resolve_' + rt] += 1
    c = comdats(p)
    for s, body in oc.items():
        if s in c: cmp[s][0 if c[s] == body else 1] += 1
out.write('# counts %s\n' % dict(cnt))
for s, (eq, ne) in sorted(cmp.items()): out.write('# owner COMDAT shared with census objects: %d equal, %d differ (bytes or relocation symbols): %s\n' % (eq, ne, s))
out.close(); print(dict(cnt))
