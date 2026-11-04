# iteration-only wrapper: vland.py without the flag/pragma sweep (every row already matched under its ps1 flags); final grades use vland.py
import sys
sys.path.insert(0, "tools/decomp_pipeline"); sys.path.insert(0, "tools/decomp_pipeline/linux_lane/lane_tools")
import verify_and_land as V
V.EXTRA_FLAGSETS=[]
import runpy
sys.argv=["vland.py"]+sys.argv[1:]
runpy.run_path("tools/decomp_pipeline/linux_lane/lane_tools/vland.py", run_name="__main__")
