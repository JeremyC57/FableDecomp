"""RTTI vftable port of the DELETING DESTRUCTORS (??_G<Class>@@ / ??_E<Class>@@), donor FableWin -> retail Fable.exe.

tools/rtti_port_all.py ports every own-class vftable slot, but its class_of() cannot parse `??_G` / `??_E` names, so it never ported a
deleting destructor (labels_rtti_port.tsv has none). This driver runs the same walk with a class filter that accepts only those names and
keeps the same guards:
  - matched vtables only (COL offset, identical slot counts);
  - the donor slot name's class must be the class being walked;
  - a retail address given two names is dropped.
Outputs ghidra_out/labels_rtti_port_dtors.tsv (addr<TAB>decorated-name). No bytes.

Usage (Linux lane, evidence binaries in the scratchpad, never committed):
  python3 tools/rtti_port_dtors.py <donor FableWin.exe> <retail Fable.exe> [out.tsv]
Check first that the same binaries reproduce the committed labels_rtti_port.tsv:
  python3 tools/rtti_port_dtors.py <donor> <retail> <out.tsv> --all-slots   (2026-11-23: 6,653 lines, identical)
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rtti_port_all as R

def main():
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    R.DONOR, R.RETAIL = a[0], a[1]
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    R.NAMES_TSV = os.path.join(root, "ghidra_out", "fablewin_pdb_names.tsv")
    R.OUT_TSV = a[2] if len(a) > 2 else os.path.join(root, "ghidra_out", "labels_rtti_port_dtors.tsv")
    if "--all-slots" not in sys.argv:
        def class_of(d):
            m = re.match(r"^\?\?_[GE]([^@]+)@@", d)
            return m.group(1) if m else "\x00none"
        R.class_of = class_of
    sys.argv = [sys.argv[0]]
    R.main()

if __name__ == "__main__":
    main()
