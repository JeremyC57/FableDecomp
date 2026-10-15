import sys, os
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S = Path(os.environ["S"]); A = S/"w22"/"agentNN"
e = V.env()
only = sys.argv[1:]
for f in sorted((A/"test").glob("*.cpp")):
    a = f.stem
    if only and a not in only: continue
    work = S/"wine"/"tNN"/a; work.mkdir(parents=True, exist_ok=True)
    print(a, V.behaviour_of(f.read_text(), a, f"{a}_TEST PASS", work, e), flush=True)
