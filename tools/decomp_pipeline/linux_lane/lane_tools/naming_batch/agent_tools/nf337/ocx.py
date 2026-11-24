#!/usr/bin/env python3
"""ocx.py (NF337): objcheck that reports the CRT float marker separately.
  ocx(head_obj, new_obj, map) -> drv.objcheck verdict; when the only DEF/UND difference is a new UND `__fltused` (the compiler's
  reference to the CRT floating-point support, emitted for any object that uses a float type: the PDB members typed float), the
  objcheck is rerun with `__fltused` dropped from the new object's UND set and the verdict gets the suffix `+FLTUSED`."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import drv
def ocx(oh, on, mp):
    _, dh, uh = drv.coff(oh); _, dn, un = drv.coff(on)
    if "__fltused" in un and "__fltused" not in uh:
        real = drv.coff
        def patched(p):
            s, d, u = real(p)
            return (s, d, u - {"__fltused"}) if p == on else (s, d, u)
        drv.coff = patched
        try: v = drv.objcheck(oh, on, mp)
        finally: drv.coff = real
        return v + "+FLTUSED"
    return drv.objcheck(oh, on, mp)
