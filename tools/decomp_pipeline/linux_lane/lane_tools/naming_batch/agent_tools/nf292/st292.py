# st292.py: writes STATUS.tsv (NF291 format: entry status binder_rows grade note) from the per-entry readings below, in ENTRIES order,
# then APPLIED.tsv through mkapp.py. Keys are ENTRIES data-line numbers (1-6 the opt-ins, 7.. the T1 "other" entries).
import subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF292/'
E=[l.split('\t') for l in open(A+'ENTRIES.tsv').read().splitlines()[1:]]
G='HEAD RELOCATION_MATCH -> new RELOCATION_MATCH, objsec ALL_EQ, vland PASS'
S={
1:('done','0x006e11f0',G,'order 1 (opt-in unit). Binder CTCDParticleEmitter::Create: CWorldMapAdapter : ::CWorldMap (flat CWorldMap.h) declared IsLoadingAnyMap; new add-only opt-in FABLE_RETAIL_CWORLDMAP_FLAT_ISLOADINGANYMAP (flat branch, declaration only) defined on row line 7, adapter emptied. Exe call 0x006E1211. Only this entry\'s line of 0x006E11F0 touched (its 0x004CA1D0 Create line is NF291 entry 20, T3).'),
2:('done','0x00a99840,0x00a9ba70',G,'order 1. Both binders\' CMemoryDataOutputStreamMethods : ::CMemoryDataOutputStream (flat) declared GetLength; flat branch assert pinned FABLE_STATIC_ASSERT_AT(58) under its own equiv_check (0 differ), then add-only opt-in FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_FLAT_GETLENGTH_PEEKDATA. Exe calls 0x00A99867 / 0x00A9BA97. The Methods ctor/dtor stand-ins (NF289 entry 10) stay.'),
3:('done','0x00a99840,0x00a9ba70',G,'order 1. As entry 2 (same opt-in, PeekData). Exe calls 0x00A9992A / 0x00A9BB5A.'),
4:('done','0x00895ed0',G,'order 1. BuyableHouseMethods : CTCBuyableHouse (flat, GENERATED marked) declared SetRented; add-only opt-in FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_SETRENTED_SETOWNED (ruling NF291 Q4), struct emptied. Exe call 0x00895F49.'),
5:('done','0x00895ed0',G,'order 1. As entry 4 (SetOwnedByPlayer). Exe call 0x00895F3D.'),
6:('done','0x007311f0',G,'order 1. CTCDoor_Methods : CTCDoor (flat) declared UpdateOpenStatusTriggerTypeScript public; add-only opt-in FABLE_RETAIL_CTCDOOR_FLAT_UPDATEOPENSTATUSTRIGGERTYPESCRIPT declares it protected (PDB IAE; called from the derived FrameUpdate on this). Exe jmp 0x00731212. The other two callees keep their manifest names (not in ENTRIES).'),
13:('done','0x00a99610',G,'order 2 (B1). Local C3DAnimFileObjectChunk model: GetFirstSequence -> FindFirstSequence (Ego_r + FableWin QBE). Sole binder (exe 1 site). Self 0x00AB03D0 defines the PDB name.'),
14:('skipped-port','0x00a99610','not changed','order 2. The binder defines a same-TU strong body C3DAnimFileRootChunk::GetFirstObject (0x00A99110) under the made-up name; renaming it onto the PDB FindFirstObject would add a second strong definition beside the self row (NF281 D2 / NF283 D3): T3.'),
15:('done','0x00a9f2f0',G,'order 2. Local C3DAnimation2 model: CalcLocalBoneTransformations -> CalcLocalTransformations (both PDB sets). Sole stand-in binder; 0x00A4CEC0 has no landed self row (link: unresolved, as the PDB binder 0x00A9F1D0).'),
16:('done','0x00a9f2f0',G,'order 2. As entry 15, ...Relative (self 0x00A4D770 resolves it in the link).'),
}
import json,os
X=A+'work/status_extra.json'
if os.path.exists(X):
    for k,v in json.load(open(X)).items(): S[int(k)]=tuple(v)
o=['entry\tstatus\tbinder_rows\tgrade\tnote']
for i in sorted(S):
    st,b,g,n=S[i]; o.append('0x%s\t%s\t%s\t%s\tentry %d: %s'%(E[i-1][0],st,b,g,i,n))
open(A+'STATUS.tsv','w').write('\n'.join(o)+'\n')
print(subprocess.run(['python3',A+'tools/mkapp.py'],capture_output=True,text=True).stdout)
