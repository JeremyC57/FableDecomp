"""c1dbg.py -- run VC7.1 cl.exe under Wine with the C++ FRONT END (c1xx.dll 13.10.3077) observable from gdb.
Same method as lane_tools/c2probe/c2dbg.py, applied to c1xx: a copy of c1xx.dll with `jmp $` at its export
_InvokeCompilerPass@12 (RVA 0x68C9F, bytes E8 EC) is passed to cl with /Bx<copy>; cl spins there, winedbg --gdb attaches,
the gdb script restores the 2 bytes, plants breakpoints (c1xx loads at its preferred base 0x10400000) and lets it run.
Usage (python): probe(src, gdb_script, config, flags) -> (trace dict, cl log, run dir).  gdb side: gdb_setup()."""
import json, os, re, secrets, shutil, subprocess, sys, time
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg as C2
C1_BASE = 0x10400000
INVOKE_RVA = 0x68C9F
INVOKE_BYTES = b"\xe8\xec"
VC = C2.VC
C1 = VC / "bin" / "c1xx.dll"
RUNS = Path(os.environ.get("C1DBG_RUNS", str(Path(os.environ["W"]) / "tmp" / "c1runs")))

def make_run():
    RUNS.mkdir(parents=True, exist_ok=True)
    token = secrets.token_hex(4); run = RUNS / token; run.mkdir()
    data = bytearray(C1.read_bytes())
    assert data[INVOKE_RVA:INVOKE_RVA + 2] == INVOKE_BYTES, "unexpected c1xx.dll"
    data[INVOKE_RVA:INVOKE_RVA + 2] = b"\xeb\xfe"
    (run / f"c1p{token}.dll").write_bytes(data)
    exe = run / f"c1p{token}.exe"; shutil.copy(C2.CL, exe)
    for dll in VC.joinpath("bin").iterdir():
        if dll.suffix.lower() in (".dll", ".config") and dll.name.lower() != "c1xx.dll":
            (run / dll.name).symlink_to(dll)
    (run / "c1xx.dll").symlink_to(C1)
    return token, run, exe

def probe(src, gdb_script, config, flags=None, keep=False, timeout=900):
    src = Path(src); token, run, exe = make_run()
    staged = run / src.name; staged.write_text(C2.vc71_source(src.read_text(errors="replace")))
    obj = run / (src.stem + ".obj"); out_json = run / "trace.json"
    cfg = dict(config); cfg.update(out=str(out_json), exe=exe.name, c1=f"c1p{token}.dll")
    cfg_path = run / "config.json"; cfg_path.write_text(json.dumps(cfg))
    env = dict(os.environ); env["INCLUDE"] = C2.includes(); env.setdefault("WINEDEBUG", "-all")
    procs = []; trace = None; log = ""
    try:
        cl = subprocess.Popen(["wine", str(exe), "/nologo", "/c", *C2.BASE_FLAGS, *(flags or []),
                               f"/Bx{C2.winpath(run / f'c1p{token}.dll')}", f"/Fo{C2.winpath(obj)}", C2.winpath(staged)],
                              cwd=run, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        procs.append(cl)
        if not C2.wait_mapped(exe.name, f"c1p{token}.dll", proc=cl, timeout=timeout):
            return None, "c1 copy never mapped:\n" + cl.communicate(timeout=60)[0], run
        time.sleep(0.5)
        pid = C2.wine_pid(exe.name, proc=cl)
        if pid is None:
            return None, "no wine pid:\n" + cl.communicate(timeout=60)[0], run
        port = C2.free_port()
        srv = subprocess.Popen(["winedbg", "--gdb", "--no-start", "--port", str(port), str(pid)], env=env,
                               stdout=open(run / "winedbg.log", "w"), stderr=subprocess.STDOUT)
        procs.append(srv); cfg["port"] = port; cfg_path.write_text(json.dumps(cfg))
        g = subprocess.run(["gdb", "-nx", "-q", "-batch", "-x", str(gdb_script)],
                           env=dict(env, C1DBG_CONFIG=str(cfg_path)), capture_output=True, text=True, timeout=timeout)
        (run / "gdb.log").write_text(g.stdout + g.stderr)
        if out_json.exists(): trace = json.loads(out_json.read_text())
        try: log = cl.communicate(timeout=300)[0].replace("\r", "")
        except subprocess.TimeoutExpired: log = "cl did not finish after the trace"
        if trace is None: trace = {"error": "no trace; gdb log:\n" + g.stdout[-3000:] + g.stderr[-3000:]}
        trace["cl_log"] = log; trace["cl_rc"] = cl.returncode
        return trace, log, run
    finally:
        for p in procs:
            if p.poll() is None: p.kill()
        C2.kill_matching(exe.name)
        if not keep: shutil.rmtree(run, ignore_errors=True)

def gdb_setup():
    import gdb
    cfg = json.load(open(os.environ["C1DBG_CONFIG"]))
    for cmd in ("set pagination off", "set confirm off", "set auto-solib-add off", "set breakpoint always-inserted on",
                "set debuginfod enabled off"):
        try: gdb.execute(cmd, to_string=True)
        except gdb.error: pass
    gdb.execute(f"target remote localhost:{cfg['port']}", to_string=True)
    inf = gdb.selected_inferior()
    libs = gdb.execute("info sharedlibrary", to_string=True)
    bl = [l for l in libs.splitlines() if cfg["c1"].lower() in l.lower()]
    if not bl or not bl[0].strip().startswith(f"0x{C1_BASE + 0x1000:08x}"):
        raise RuntimeError("c1 copy not at 0x10400000:\n" + libs)
    entry = C1_BASE + INVOKE_RVA
    if bytes(inf.read_memory(entry, 2)) != b"\xeb\xfe": raise RuntimeError("entry not spinning")
    inf.write_memory(entry, INVOKE_BYTES)
    return gdb, inf, cfg
