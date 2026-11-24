# copiers363.py <copiers.txt> <owner.obj>: NF363 Part 1 item 3. For each landed strong GetDef<CThingBaseDef> copier (HEAD object
# work/obj/H_<addr>.obj): COMDAT selection / size / alignment of the PDB symbol, pk_all (row's own // Leaf, catalog row length) verdict
# and reach for that copy, retcmp against retail 0x0042AF3C (bytes with relocation fields masked + relocation targets against the table;
# relocation-target hex values are NOT printed), and cmpcomdat against the owner's new object. Run from the repo root, lane env set.
import sys, os, re, subprocess, struct
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF367'
R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
SYM = '??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z'
PK = R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py'
def secinfo(obj):
    secs, sl = nf.coff(obj)
    d = open(obj, 'rb').read(); psym, nsym = struct.unpack_from('<II', d, 8); sel = {}; i = 0
    while i < nsym:
        o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8)
        if scl == 3 and v == 0 and na == 1 and sec > 0: sel.setdefault(sec, d[o + 18 + 14])
        i += 1 + na
    for i, n, v, sec, t, scl in sl:
        if n == SYM and scl == 2 and sec > 0:
            s = secs[sec - 1]; al = (s['ch'] >> 20) & 0xF
            return sel.get(sec), len(s['data']), (1 << (al - 1)) if al else 0, s
    return None
own = secinfo(sys.argv[2])
print('addr\tdefines\tsel\tsize\talign\tpk_all(verdict@addr reach)\tretcmp_v_0042af3c\tv_owner_new_obj\tleaf')
for l in open(sys.argv[1]):
    a = l.split()[0]; obj = A + '/work/obj/H_%s.obj' % a
    src = nf.src_of(a).read_text(encoding='utf-8'); leaf = nf.leaf_of(src, a); ln = len(bytes.fromhex(nf.catalog()[a]['bytes']))
    si = secinfo(obj)
    if not si: print('%s\tNO\t-\t-\t-\t-\t-\t-\t%s' % (a, leaf)); continue
    sel, size, al, s = si
    r = subprocess.run(['python3', '-I', '-B', PK, obj, a, '--leaf=' + leaf, '--tsv', '--row-length=' + str(ln)], capture_output=True, text=True, cwd=R)
    open(A + '/evidence/copiers/pkall_%s.tsv' % a, 'w').write(r.stdout + r.stderr)
    pl = [x.split('\t') for x in r.stdout.splitlines() if x.endswith('\t' + SYM)]
    pk = ','.join('%s@%s reach=%s leafref=%s' % (p[0], p[2], p[8], p[7]) for p in pl) or 'not-listed'
    r2 = subprocess.run(['python3', '-I', '-B', A + '/tools/retcmp_coordinator.py', os.environ['FABLE_EXE'], obj, SYM + '=0042af3c'], capture_output=True, text=True, cwd=R)
    m = re.search(r'len (\d+) bytes (EQ|DIFF) relocs (EQ|\[.*\])', r2.stdout)
    rc = ('len=%s bytes=%s relocs=%s' % (m.group(1), m.group(2), 'EQ' if m.group(3) == 'EQ' else 'DIFF')) if m else 'ERR ' + (r2.stdout + r2.stderr)[-120:].replace('\n', ' ')
    ov = 'EQUAL' if (s['data'], s['rel']) == (own[3]['data'], own[3]['rel']) else 'DIFF bytes=%s relocs=%s' % (s['data'] == own[3]['data'], s['rel'] == own[3]['rel'])
    print('\t'.join([a, 'YES', str(sel), str(size), str(al), pk, rc, ov, leaf]))
