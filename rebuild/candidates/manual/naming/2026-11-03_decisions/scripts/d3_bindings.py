# d3_bindings.py <base_pairs> <new_pairs> <out BINDINGS.tsv>: D3 RETIRE / ADD rows from the before/after relocation pairs.
import re, sys, collections
OLD = re.compile(r'CEaxContext|CEaxSource|CPKObject|CEAXContext|SEaxEvent|SGuid|CEAXEffect|CEAXListenerEffect|IsAnyNaN|g_EffectGuid_012b94|g_NullGuid_012b9438|g_ReverbEffectGuid|g_EAXReverbEffectGuid|^\?EAX_NULL_GUID@@|^\?EAX_REVERB_EFFECT@@|^\?EAXPROPERTYID_EAX40|DSPROPSETID_EAX30_ListenerProperties|g_eaxListenerGuid|g_eaxListenerPropertySetGuid_012bee90|UEAX[A-Z]*PROPERTIES@@|UEAXVECTOR@@|^_g_InformOfEventImpl$|^_g_HeroStatsEventNotify$|^\?g_pfnPK(Alloc|Free)@@|SEaxSlotProperties|CVtable_012b9338@@|^\?GetEffectProperty@CEAXManager@@QAEJKPAXKK@Z$')
base = [l.rstrip("\n").split("\t") for l in open(sys.argv[1])]
new = [l.rstrip("\n").split("\t") for l in open(sys.argv[2])]
new = [p for p in new if len(p) == 4]
T = set(p[1] for p in base + new if len(p) == 4)
emitted_syms = set(p[2] for p in new)
rb = [l.rstrip("\n").split("\t") for l in open("rebuild/oracles/reloc_bindings.tsv")][1:]
rb = [r for r in rb if len(r) == 3]
have = set((r[0], r[1]) for r in rb)
ret, add, seen = [], [], set()
for s, t, by in rb:
    if OLD.search(s) and s not in emitted_syms and (t in T or re.match(r"(00c[3-7]|012b|0121|0139c9)", t)): ret.append((s, t, by))
base_syms = set(p[2] for p in base if len(p) == 4)
have_syms = set(r[0] for r in rb)
first = {}
for a, t, s, k in new:
    if s.startswith("$L") or s in base_syms or s in have_syms: continue   # only the respelled (new) symbols
    if s not in first or int(t, 16) < int(first[s][0], 16): first[s] = (t, a)
for s, (t, a) in first.items(): add.append((s, t, a))
with open(sys.argv[3], "w") as f:
    f.write("action\tsymbol\ttarget\tby_row\tnote\n")
    for s, t, by in ret: f.write(f"RETIRE\t{s}\t{t}\t{by}\tD3 2026-11-03 (owner): pre-SDK spelling of a GameCODA EAX class / type / GUID / allocator; every landed binder respelled and rechecked byte-identical\n")
    for s, t, by in add: f.write(f"ADD\t{s}\t{t}\t{by}\tD3 2026-11-03 (owner): GameCODA 2.10 SDK spelling (CEAXListener / CEAXBuffer / CEAXBufferList / CCodaObjectBase / CodaMalloc, extern \"C\" EAX 4.0 GUIDs, SDK struct tags)\n")
print("retire", len(ret), "add", len(add))
by_t = collections.defaultdict(set)
for a, t, s, k in new:
    if not s.startswith("$L"): by_t[t].add(s)
for t, ss in sorted(by_t.items()):
    if len(ss) > 1: print("MULTI-EMITTED", t, sorted(ss))
