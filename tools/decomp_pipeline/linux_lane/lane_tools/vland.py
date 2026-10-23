# verify_and_land.py with a 180 s limit on every compile/link/test (a hung Wine cl.exe becomes a failed step).
# usage (repo root, lane env sourced): python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py <payload> <oracle> [--land] [--recheck-landed]
import sys, subprocess
sys.path.insert(0, "tools/decomp_pipeline")
import verify_and_land as V
def run_tool(exe, args, e, _t=180):
    cmd = ([V.WINE] if V.WINE else []) + [str(exe)] + V.tool_args(args)
    try: return subprocess.run(cmd, capture_output=True, text=True, errors="replace", env=e, timeout=_t)
    except subprocess.TimeoutExpired:
        print(f"  TIMEOUT {exe} {' '.join(map(str,args))[-120:]}", flush=True)
        return subprocess.CompletedProcess(cmd, 124, "", "timeout")
V.run_tool = run_tool
sys.argv = ["verify_and_land.py"] + sys.argv[1:]
V.main()
