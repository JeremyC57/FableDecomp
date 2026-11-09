"""c2dbg.py -- run the lane's VC7.1 cl.exe under Wine with c2.dll's register allocator observable from gdb.

Library used by c2prio.py (and by ad-hoc exploration scripts). The method is the byte-tactics one
(HectorBailey/byte-tactics tools/c2prio.py) adapted to a c2 that is a DLL inside cl.exe:

  1. copy $FABLE_VC71/bin/c2.dll to <run>/c2p<token>.dll with `jmp $` (EB FE) at the entry of its export
     _InvokeCompilerPass@12 (RVA 0x596f), and copy cl.exe to <run>/c2p<token>.exe (plus symlinks to the
     other toolchain DLLs) so the process has a unique name;
  2. run `c2p<token>.exe /c <flags> /B2<copy of c2> <src>`: cl loads the copy in-process and spins at the
     patched entry (the compile hangs there);
  3. find the Wine pid by the unique exe name (`winedbg` "info process"), start `winedbg --gdb --no-start
     --port P <pid>`, and run gdb -batch with a Python script that connects, writes the two entry bytes
     back (55 8B), plants its breakpoints (all inside c2's image, which loads at its preferred base
     0x10700000 -- the script checks) and lets the compile finish.  The object produced is byte-identical
     to a plain compile (only the COFF timestamp differs).

Nothing under $FABLE_VC71 is modified.  Each run has its own directory and copies; runs can go in parallel.
Environment: source $S/wine/env.sh first (FABLE_VC71, WINEPREFIX...).  Requires gdb with Python and winedbg.
"""
import json
import os
import re
import secrets
import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path

C2_SHA256 = "bcd28f39b1798b07989c30a1df471cf87fc20e3802d130901a08f83800e6b81e"   # c2.dll 13.10.3077 (c2_l.dll)
C2_BASE = 0x10700000
INVOKE_RVA = 0x596F                 # _InvokeCompilerPass@12
INVOKE_BYTES = b"\x55\x8b"          # push ebp; mov ebp,esp
TEXT_FILE_OFF = 0x1000              # .text: VA 0x10701000 == file offset 0x1000 (RVA == file offset there)

VC = Path(os.environ.get("FABLE_VC71", ""))
if not VC.exists():
    sys.exit("c2dbg: FABLE_VC71 is not set; source $S/wine/env.sh first")
CL = VC / "bin" / "cl.exe"
C2 = VC / "bin" / "c2.dll"
REPO = Path(os.environ.get("FABLE_ROOT", "/home/user/FableDecomp"))
RUNS = Path(os.environ.get("C2PRIO_RUNS", str(Path(os.environ.get("S", "/tmp")) / "w22" / "c2probe" / "runs")))
BASE_FLAGS = ["/O2", "/Oy", "/W3"]  # verify_and_land.BASE_FLAGS


def winpath(p):
    return subprocess.run(["winepath", "-w", str(p)], capture_output=True, text=True).stdout.strip()


def includes():
    """INCLUDE as verify_and_land.env() sets it: the toolchain's include, then rebuild/include."""
    inc = [VC / "include"] + [Path(x) for x in os.environ.get("FABLE_HDR_OVERLAY", "").split(os.pathsep) if x] \
        + [REPO / "rebuild" / "include"]
    return ";".join(winpath(x) for x in inc)


def vc71_source(text):
    """verify_and_land.vc71(): the lane's source rewrites (__thiscall -> __fastcall, drop static_assert)."""
    for a, b in (("&lt;", "<"), ("&gt;", ">"), ("&quot;", '"'), ("&apos;", "'"), ("&amp;", "&")):
        text = text.replace(a, b)
    text = text.replace("__thiscall", "__fastcall")
    return "\n".join(l for l in text.splitlines() if "static_assert" not in l)


def make_run(keep_name=None):
    RUNS.mkdir(parents=True, exist_ok=True)
    for old in RUNS.iterdir():
        try:
            if time.time() - old.stat().st_mtime > 6 * 3600:
                shutil.rmtree(old, ignore_errors=True)
        except OSError:
            pass
    token = keep_name or secrets.token_hex(4)
    run = RUNS / token
    run.mkdir()
    data = bytearray(C2.read_bytes())
    import hashlib
    if hashlib.sha256(data).hexdigest() != C2_SHA256:
        sys.exit(f"{C2} is not the 13.10.3077 c2.dll these addresses belong to")
    assert data[INVOKE_RVA:INVOKE_RVA + 2] == INVOKE_BYTES
    data[INVOKE_RVA:INVOKE_RVA + 2] = b"\xeb\xfe"
    (run / f"c2p{token}.dll").write_bytes(data)
    exe = run / f"c2p{token}.exe"
    shutil.copy(CL, exe)
    for dll in VC.joinpath("bin").iterdir():
        if dll.suffix.lower() in (".dll", ".config") and dll.name.lower() != "c2.dll":
            (run / dll.name).symlink_to(dll)
    (run / "c2.dll").symlink_to(C2)
    return token, run, exe


def wine_pid(name, timeout=120, proc=None):
    """Wine pid of the process whose exe is `name` (winedbg 'info process')."""
    t0 = time.time()
    while time.time() - t0 < timeout:
        info = subprocess.run(["winedbg"], input="info process\nquit\n", capture_output=True, text=True,
                              timeout=60).stdout
        for line in info.splitlines():
            m = re.match(r"\s*=?\s*([0-9a-fA-F]{8})\s+\d+\s+(?:\\_ )?'(.*)'", line)
            if m and m.group(2).lower() == name.lower():
                return int(m.group(1), 16)
        if proc is not None and proc.poll() is not None:
            return None
        time.sleep(0.1)
    return None


def wait_mapped(exe_name, dll_name, proc=None, timeout=900):
    """True once a Linux process whose command line names exe_name has dll_name in its /proc maps (the c2 copy is
    loaded and spinning at its patched entry)."""
    t0 = time.time()
    exe_l, dll_l = exe_name.lower(), dll_name.lower()
    while time.time() - t0 < timeout:
        for pid in os.listdir("/proc"):
            if not pid.isdigit():
                continue
            try:
                cmd = open(f"/proc/{pid}/cmdline", "rb").read().decode("latin1").lower()
                if exe_l not in cmd:
                    continue
                maps = open(f"/proc/{pid}/maps").read().lower()
            except OSError:
                continue
            if dll_l in maps:
                return True
        if proc is not None and proc.poll() is not None:
            return False
        time.sleep(0.1)
    return False


def free_port():
    import socket
    with socket.socket() as sk:
        sk.bind(("", 0))
        return sk.getsockname()[1]


def kill_matching(name):
    import signal
    for pid in os.listdir("/proc"):
        if not pid.isdigit():
            continue
        try:
            cmd = open(f"/proc/{pid}/cmdline", "rb").read().decode("latin1").lower()
        except OSError:
            continue
        if name.lower() in cmd:
            try:
                os.kill(int(pid), signal.SIGKILL)
            except OSError:
                pass


def probe(src, gdb_script, config, flags=None, keep=False, timeout=900, obj_out=None, run_name=None):
    """Compile `src` with the spinning c2 copy, attach, run `gdb_script` (a Python file gdb executes with
    C2PRIO_CONFIG pointing at a JSON file holding `config` plus "out", "exe"). Returns (trace dict or None,
    cl log, run dir)."""
    src = Path(src)
    token, run, exe = make_run(run_name)
    staged = run / src.name
    staged.write_text(vc71_source(src.read_text(errors="replace")))
    obj = run / (src.stem + ".obj")
    out_json = run / "trace.json"
    cfg = dict(config)
    cfg.update(out=str(out_json), exe=exe.name, c2=f"c2p{token}.dll")
    cfg_path = run / "config.json"
    cfg_path.write_text(json.dumps(cfg))
    env = dict(os.environ)
    env["INCLUDE"] = includes()
    env.setdefault("WINEDEBUG", "-all")
    procs = []
    log = ""
    trace = None
    try:
        cl = subprocess.Popen(["wine", str(exe), "/nologo", "/c", *BASE_FLAGS, *(flags or []),
                               f"/B2{winpath(run / f'c2p{token}.dll')}", f"/Fo{winpath(obj)}", winpath(staged)],
                              cwd=run, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        procs.append(cl)
        # wait until the c2 copy is mapped (the front end c1xx runs first, seconds to a minute on a big TU)
        if not wait_mapped(exe.name, f"c2p{token}.dll", proc=cl, timeout=timeout):
            log = cl.communicate(timeout=60)[0]
            return None, f"compile ended before c2 ran (front-end error?):\n{log}", run
        pid = wine_pid(exe.name, proc=cl)
        if pid is None:
            log = cl.communicate(timeout=60)[0]
            return None, f"compile ended before c2 ran (front-end error?):\n{log}", run
        port = free_port()
        srv = subprocess.Popen(["winedbg", "--gdb", "--no-start", "--port", str(port), str(pid)], env=env,
                               stdout=open(run / "winedbg.log", "w"), stderr=subprocess.STDOUT)
        procs.append(srv)
        cfg["port"] = port
        cfg_path.write_text(json.dumps(cfg))
        g = subprocess.run(["gdb", "-nx", "-q", "-batch", "-x", str(gdb_script)],
                           env=dict(env, C2PRIO_CONFIG=str(cfg_path)), capture_output=True, text=True,
                           timeout=timeout)
        (run / "gdb.log").write_text(g.stdout + g.stderr)
        if out_json.exists():
            trace = json.loads(out_json.read_text())
        try:
            log = cl.communicate(timeout=300)[0].replace("\r", "")
        except subprocess.TimeoutExpired:
            log = "cl did not finish after the trace"
        if obj_out and obj.exists():
            shutil.copy(obj, obj_out)
        if trace is None:
            trace = {"error": "no trace written; gdb log:\n" + g.stdout[-4000:] + g.stderr[-4000:]}
        trace["cl_log"] = log
        trace["cl_rc"] = cl.returncode
        trace["obj"] = str(obj) if obj.exists() else None
        return trace, log, run
    finally:
        for p in procs:
            if p.poll() is None:
                p.kill()
        kill_matching(exe.name)
        if not keep:
            shutil.rmtree(run, ignore_errors=True)


# ---- helpers for the gdb side (imported by the scripts gdb runs) --------------------------------------
def gdb_setup():
    """Inside gdb: connect to winedbg's stub, check the c2 copy is at its base, restore the entry bytes.
    Returns (gdb, inferior, cfg)."""
    import gdb  # noqa
    cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))
    for cmd in ("set pagination off", "set confirm off", "set auto-solib-add off",
                "set breakpoint always-inserted on", "set debuginfod enabled off"):
        try:
            gdb.execute(cmd, to_string=True)
        except gdb.error:
            pass
    gdb.execute(f"target remote localhost:{cfg['port']}", to_string=True)
    inf = gdb.selected_inferior()
    libs = gdb.execute("info sharedlibrary", to_string=True)
    base_line = [l for l in libs.splitlines() if cfg["c2"].lower() in l.lower()]
    if not base_line or not base_line[0].strip().startswith(f"0x{C2_BASE + 0x1000:08x}"):
        raise RuntimeError("the c2 copy is not loaded at 0x10700000:\n" + libs)
    entry = C2_BASE + INVOKE_RVA
    cur = bytes(inf.read_memory(entry, 2))
    if cur != b"\xeb\xfe":
        raise RuntimeError(f"entry bytes are {cur.hex()}, not the spin")
    inf.write_memory(entry, INVOKE_BYTES)
    return gdb, inf, cfg
