"""Shared engine for the w4a family scripts: exact (old, new) text blocks per repo-relative file.

A family keeps its blocks in <fam>.reps.json: {"rebuild/src/compiled/...cpp": [[old, new], ...], ...}.
Each block is applied when `old` occurs exactly once; a block already in its new form (old absent, new present)
is skipped (a deletion block: old absent), so a script is idempotent and composes with other agents' edits to other lines of the same file.
A block in neither form is reported (`note ... BLOCK_MISSING`) and the whole file is left alone.
usage (from a fam_*.py): w4lib.main(<fam>, argv[, extra_transform])   argv: [--check] <file>...
"""
import json, os, sys
W = os.path.dirname(os.path.abspath(__file__))


def load(fam):
    p = os.path.join(W, fam + '.reps.json')
    return json.load(open(p)) if os.path.exists(p) else {}


def apply_blocks(text, blocks):
    new = text; n = 0
    for old, rep in blocks:
        if old and new.count(old) == 1 and not (rep and old in rep and new.count(rep) == 1):
            new = new.replace(old, rep); n += 1
        elif rep and new.count(rep) >= 1 and new.count(old) == 0:
            continue
        elif rep and old in rep and new.count(rep) == 1:
            continue
        elif not rep and new.count(old) == 0:
            continue    # a deletion already applied (the file's other blocks pin it)
        else:
            return None, old
    return new, n


def main(fam, argv, extra=None):
    check = '--check' in argv
    reps = load(fam)
    for f in [a for a in argv if not a.startswith('--')]:
        norm = f.replace(os.sep, '/')
        key = [k for k in reps if norm.endswith('/' + k) or norm == k]
        text = open(f, encoding='utf-8', errors='surrogateescape', newline='').read()
        new = text; n = 0
        if extra:
            new, n = extra(norm, new)
        if key:
            r, m = apply_blocks(new, reps[key[0]])
            if r is None:
                print('note', f, 'BLOCK_MISSING %r' % m[:70]); continue
            new = r; n += m
        if new != text:
            if not check:
                open(f, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new)
            print('edited', f, n)
