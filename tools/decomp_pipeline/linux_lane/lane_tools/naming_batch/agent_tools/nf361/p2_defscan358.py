# defscan358.py: every compiled census object (work/obj/H_*.obj): which of the three PDB symbols it DEFs / UNDs, whatever leg put the
# row in the census; flags a DEF in a row that is NOT a census candidate for that copy (cross-copy emitters). Output work/defscan.tsv.
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = nf.W
SYM = {'00429c6f': '?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z',
       '0042b312': '??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z',
       '0042b040': '??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z'}
cand = {}
for l in open(W / 'cand.tsv'):
    p = l.rstrip('\n').split('\t'); cand[p[0]] = set(p[1].split(','))
o = open(W / 'defscan.tsv', 'w'); n = 0
for obj in sorted(W.glob('obj/H_*.obj')):
    a = obj.stem[2:]; _, sl = nf.coff(obj); st = {}
    for i, nm, v, sec, typ, scl in sl:
        if scl != 2: continue
        for c, s in SYM.items():
            if nm == s: st[c] = 'DEF' if sec > 0 else st.get(c, 'UND')
    for c, v in sorted(st.items()):
        flag = '' if c in cand.get(a, set()) else 'NOT_A_CANDIDATE'
        o.write('%s\t%s\t%s\t%s\n' % (a, c, v, flag)); n += flag != ''
o.close(); print('defscan lines', sum(1 for _ in open(W / 'defscan.tsv')), 'cross-copy', n)
