# mt.py <addr> <test.cpp> [overlay_dir]: compile+run a behaviour test (VC7.1 /Od, like vland) with an optional mutant header overlay
import sys,os
from pathlib import Path
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
if len(sys.argv)>3: os.environ["FABLE_HDR_OVERLAY"]=sys.argv[3]
import verify_and_land as V
a=sys.argv[1]; t=Path(sys.argv[2]).read_text()
work=Path(os.environ["W"])/"tmp"/"mt"/a; work.mkdir(parents=True,exist_ok=True)
print(a, V.behaviour_of(V.vc71(t) if hasattr(V,"vc71") else t, a, a+"_TEST PASS", work, V.env()))
