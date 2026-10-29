#!/usr/bin/env python3
"""Wave-3 hand edits that no family script covers (CWorld::GetFrame 0x0049D870 callers whose shape is unique):
  0097d360  free stand-in `long FABLE_FASTCALL GetGameTick();` -> CWorld::GetFrame() (engine/CWorld.h now declares it);
  00730f90  local `class CWorld { long GetFrame() const { return GFGetCurrentFrame(); } }` -> the retail static member;
  008023a0  test stub `helper_49d870(CTCTargetingPlayer*)` -> `CWorld::GetFrame()` stub (retail takes no object; the
            assertion that the stub saw the player is dropped with it).
The edits are exact (old, new) line blocks from hand_reps.json, keyed by repo-relative path. Idempotent: a block
already in its new form is skipped; a block in neither form is reported and the file is left alone.
usage: fam_hand.py [--check] <file>...
"""
import json, os, sys
W = os.path.dirname(os.path.abspath(__file__))
REPS = json.load(open(os.path.join(W, 'hand_reps.json')))


def main(argv):
    check = '--check' in argv
    for f in [a for a in argv if not a.startswith('--')]:
        key = [k for k in REPS if f.replace(os.sep, '/').endswith(k)]
        if not key:
            continue
        text = open(f, encoding='utf-8', errors='surrogateescape').read()
        new = text; n = 0; bad = False
        for old, rep in REPS[key[0]]:
            if new.count(old) == 1 and (rep == '' or old not in rep or new.count(rep) == 0):
                new = new.replace(old, rep); n += 1
            elif rep and rep in new:
                continue
            else:
                print('note', f, 'BLOCK_MISSING %r' % old[:60]); bad = True
        if bad:
            continue
        if new != text:
            if not check:
                open(f, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new)
            print('edited', f, n)


if __name__ == '__main__':
    main(sys.argv[1:])
