# vland.py with the two unlisted CUserProfileManager starts (0x00407A70 / 0x00407BA0, UNLISTED_STARTS.tsv split of 0x004075C0) added to the
# manifest address set IN MEMORY ONLY, so the self-check grades them instead of stopping at OUTSIDE_MANIFEST. Read-only: never pass --land.
import sys, subprocess
sys.path.insert(0, "tools/decomp_pipeline")
import verify_and_land as V
assert "--land" not in sys.argv
def run_tool(exe, args, e, _t=180):
    cmd = ([V.WINE] if V.WINE else []) + [str(exe)] + V.tool_args(args)
    try: return subprocess.run(cmd, capture_output=True, text=True, errors="replace", env=e, timeout=_t)
    except subprocess.TimeoutExpired:
        return subprocess.CompletedProcess(cmd, 124, "", "timeout")
V.run_tool = run_tool
_m = V.manifest_addresses
V.manifest_addresses = lambda *a, **k: set(_m(*a, **k)) | {"00407a70", "00407ba0"}
sys.argv = ["verify_and_land.py"] + sys.argv[1:]
V.main()
