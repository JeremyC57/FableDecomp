#!/usr/bin/env python3
"""xtry.py <addr> <variant.cpp> [leaf]: compile a scratch variant under the row's cached mcheck variant (flags, pragma) and
compare its leaf bytes + named relocations with the HEAD source's (as mcheck does). Prints SAME / RELOC_RENAMED / DIFFER."""
import sys, os, json, re, subprocess
from pathlib import Path
ROOT = Path(os.environ["FABLE_ROOT"]); os.chdir(ROOT); S = Path(os.environ["S"])
sys.path.insert(0, str(ROOT / "tools/decomp_pipeline"))
import verify_and_land as V
sys.argv = [sys.argv[0]] + sys.argv[1:]
a, vsrc = sys.argv[1].lower(), sys.argv[2]
exec(open(S / "mig9/scripts/mcheck_lib.py").read())
