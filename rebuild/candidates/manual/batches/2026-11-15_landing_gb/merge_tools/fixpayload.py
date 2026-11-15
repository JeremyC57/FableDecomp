import json,sys,re
P=sys.argv[1]; d=json.load(open(P)); A={e['address']:e for e in d['result']['authored']}
names={'006c9c60':('CPersistContext','Transfer'),'005f8e6d':('std','__pop_heap_aux'),'005f8e8f':('std','__pop_heap_aux'),
       '00c298d0':('std::vector<CHandleBasedMemoryPool_012b4098::CHandleInfo>','_Insert_n')}
for a,(m,n) in names.items(): A[a]['module']=m; A[a]['name']=n
IB={
'0054ac90':'// Inline-budget: /d2inlT256 -- the CSlider string-equality helper: T64 and T128 leave two out-of-line equality calls (1374 bytes); T256 expands both as retail (rows/0054ac90/INLINE_BUDGET_PROBES.json in the Codex pass 22 hand-back).',
'00b4bcc0':'// Inline-budget: /d2inlT81 -- the clip-volume matrix helper: the default threshold (74) and T80 call it out of line (1439 bytes); T81 is the smallest threshold that expands it as retail (2179 bytes; INLINE_LADDER.json).',
'00b4c550':'// Inline-budget: /d2inlT81 -- the same matrix helper as 0x00B4BCC0: T80 calls it out of line (723 bytes); T81 expands it as retail (2242 bytes; INLINE_LADDER.json).',
'00bc3f30':'// Inline-budget: /d2inlT512 -- SetupBlendOpState stays called below T512 (T64 2449, T128/T256 2642 bytes); T512 expands it; SetupAlphaBlendingStatesFromBlendMode stays called at every threshold up to T4096, so its header body is FABLE_FORCEINLINE (see the note at its definition; INLINE_BUDGET_PROBES.json).',
'00bd71b0':'// Inline-budget: /d2inlT256 -- plain T256 leaves ClipSphere called and T512 also expands the portable float conversion, so ClipSphere is FABLE_FORCEINLINE at its one site; the first bounding-box constructor call (0x00BD7373) is a scoped inline_depth(0) statement, the second expands (MATCH_NOTES.md).',
}
for a,t in IB.items():
    s=A[a]['source_cpp']; L=s.split('\n'); k=[i for i,l in enumerate(L[:12]) if l.startswith('// Flags:')][0]
    L.insert(k+1,t); A[a]['source_cpp']='\n'.join(L)
t=A['00b98e10']['test_cpp']
assert 'CHECK(argc==2);std::FILE*f=std::fopen(argv[1],"wb");' in t
A['00b98e10']['test_cpp']=t.replace('CHECK(argc==2);std::FILE*f=std::fopen(argv[1],"wb");',
  'CHECK(argc<=2);std::FILE*f=std::fopen(argc==2?argv[1]:"00b98e10_trace.txt","wb");')
json.dump(d,open(P,'w'),indent=1); print('ok')
