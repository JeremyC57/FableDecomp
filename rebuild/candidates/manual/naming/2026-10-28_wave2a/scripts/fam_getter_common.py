"""Driver shared by fam_uimanager.py / fam_profilemgr.py / fam_defmgr.py (static getter respells)."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, drop_nested, member_respell, skipped


def make_transform(cfgs, skip=None):
    def transform(text, path):
        if skip and skipped(path, skip):
            return text, 0, ['SKIP ' + skipped(path, skip)]
        notes, edits = [], []
        for cfg in cfgs:
            m = mask(text)
            sc = scopes(m)
            e, n, done = member_respell(text, m, sc, cfg)
            notes += n
            if done and e:
                text = apply_edits(text, drop_nested(e))
                edits += e
        return text, len(edits), notes
    return transform
