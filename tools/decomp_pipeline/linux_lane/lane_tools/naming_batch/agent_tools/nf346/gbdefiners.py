# gbdefiners.py: today's landed rows (catalog Source) whose source #defines FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY; marks REREAD definers.
import sys, re
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
nf.blk('0')
for a in sorted(nf._BLK):
    p = nf.src_of(a)
    if not p or not p.exists(): continue
    t = p.read_text(encoding='utf-8', errors='replace')
    if re.search(r'^\s*#\s*define\s+FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY\b', t, re.M):
        rr = bool(re.search(r'^\s*#\s*define\s+FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD\b', t, re.M))
        print('%s\t%s\t%s' % (a, 'REREAD' if rr else '-', p))
