# fam_gror9.py <ov> <rows.txt>: GFROR13 returns long (Ego_r.pdb + FableWin.pdb ?GFROR13@@YIJK@Z, 0x00497890; 88 landed rows
# already emit it). Rows declared `unsigned long __fastcall GFROR13(unsigned long)` (?GFROR13@@YIKK@Z). Transform: the
# declaration's return type becomes long. Kept only when the object names the PDB spelling.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib9
P = re.compile(r'\bunsigned\s+long(\s+__fastcall\s+GFROR13\s*\(\s*unsigned\s+long\b)')
def tf(t, a):
    n = P.subn(r'long\1', t)
    return (n[0], 'ok %d' % n[1]) if n[1] else (None, 'no unsigned-long declaration')
def ck(S0, S1, a):
    if '?GFROR13@@YIKK@Z' in S1: return 'old still emitted'
    if '?GFROR13@@YIJK@Z' not in S1: return 'final not emitted'
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'gror9')
