#!/usr/bin/env python3
"""Third-party vendor libraries retail compiled from unmodified source: config, tree lookup, VC7.1 build, object cache.

    python3 tools/decomp_pipeline/libsig/vendor_lib.py [--build TAG ...]     # list the config / build TUs to the cache

The config is vendor_libs.tsv next to this file (one row per library: tag, version, tree, fetch URL, sha256, flags,
include dirs, the linked TUs). The trees live under FABLE_THIRDPARTY (pathsep-separated; default $S/thirdparty), never in
the repo; fetch_vendor_libs.py fetches and SHA-256-checks them. A TU is compiled with the lane's cl 13.10.3077
($FABLE_VC71/bin/cl.exe, through wine off Windows; INCLUDE = $FABLE_VC71/include only) with exactly the recorded flags,
into a cache keyed by the source tree hash, the flags and the compiler: FABLE_VENDOR_OBJ, default <first
FABLE_THIRDPARTY dir>/obj. lib_verify.py proves `// Vendor-library:` / `// Vendor-proof:` claims from these objects;
libsig_scan.py --vendor scans them like a library directory.
"""
from __future__ import annotations

import csv
import hashlib
import os
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
CONFIG = HERE / "vendor_libs.tsv"


def load_config(path=CONFIG):
    """{tag: row} with sources / not_built / includes split into lists."""
    rows = {}
    with open(path, encoding="utf-8") as f:
        lines = [l for l in f if not l.startswith("#")]
    for r in csv.DictReader(lines, delimiter="\t"):
        r = {k: (v or "").strip() for k, v in r.items()}
        for k in ("sources", "not_built", "includes", "flags"):
            r[k + "_list"] = r[k].split() if r.get(k) else []
        rows[r["tag"]] = r
    return rows


def thirdparty_dirs():
    env = os.environ.get("FABLE_THIRDPARTY")
    if env:
        return [Path(p) for p in env.split(os.pathsep) if p and Path(p).is_dir()]
    d = Path(os.environ["S"]) / "thirdparty" if os.environ.get("S") else None
    return [d] if d and d.is_dir() else []


def tree_dir(row):
    for d in thirdparty_dirs():
        p = d / row["tree"]
        if p.is_dir():
            return p
    return None


def tree_hash(root):
    """sha256 of the sorted `<sha256>  <relpath>` lines of every regular file under root."""
    lines = []
    for p in sorted(Path(root).rglob("*")):
        if p.is_file() and not p.is_symlink():
            lines.append(f"{hashlib.sha256(p.read_bytes()).hexdigest()}  {p.relative_to(root).as_posix()}\n")
    return hashlib.sha256("".join(sorted(lines)).encode()).hexdigest()


def source_kind(row, member):
    """'built' (a linked TU proven from unmodified source), 'not_built' (linked but excluded, see notes), or None."""
    if member in row["sources_list"]:
        return "built"
    if member in row["not_built_list"]:
        return "not_built"
    return None


def _wpath(p):
    s = str(p)
    return "Z:" + s.replace("/", "\\") if os.name != "nt" and s.startswith("/") else s


def _cl():
    vc = os.environ.get("FABLE_VC71")
    cl = Path(vc) / "bin" / "cl.exe" if vc else None
    return (cl, Path(vc)) if cl and cl.is_file() else (None, None)


# Absolute include directories named by environment variable in vendor_libs.tsv (`$VAR` or `$VAR/sub`), with the
# lane default used when the variable is unset. FABLE_WINDDK_INC: the WinDDK 3790.1830 `inc/wnet` set (windows.h,
# ks.h, mmreg.h ...) the DirectShow base classes were proven with (SDK_NEEDS.md section 0).
ENV_INCLUDE_DEFAULTS = {
    "FABLE_WINDDK_INC": lambda: Path(os.environ["S"]) / "wine" / "winddk" / "3790.1830" / "inc" / "wnet"
    if os.environ.get("S") else None,
}


def env_dir(var):
    v = os.environ.get(var)
    if v:
        return Path(v)
    d = ENV_INCLUDE_DEFAULTS.get(var)
    return d() if d else None


def include_dirs(cfg, row, tree):
    out = []
    for inc in row["includes_list"]:
        if inc.startswith("$"):
            var, _, rest = inc[1:].partition("/")
            base = env_dir(var)
            if base is None or not (base / rest).is_dir():
                raise FileNotFoundError(f"include {inc}: set {var} (now {base})")
            out.append(base / rest if rest else base)
        elif inc.startswith("{"):
            tag, rest = inc[1:].split("}", 1)
            other = tree_dir(cfg[tag]) if tag in cfg else None
            if other is None:
                raise FileNotFoundError(f"include {inc}: tree of {tag} not under FABLE_THIRDPARTY")
            out.append(other / rest.lstrip("/"))
        else:
            out.append(tree / inc)
    return out


def cache_root():
    env = os.environ.get("FABLE_VENDOR_OBJ")
    if env:
        return Path(env)
    tp = thirdparty_dirs()
    return tp[0] / "obj" if tp else None


_TREE_KEYS = {}


def build_key(cfg, row, tree):
    """Cache key: this tree's hash, the include trees' hashes, the flags and the compiler binary."""
    parts = [row["flags"], row["includes"]]
    for t in [tree] + [p for p in include_dirs(cfg, row, tree) if tree not in p.parents and p != tree]:
        k = str(t)
        if k not in _TREE_KEYS:
            _TREE_KEYS[k] = tree_hash(t)
        parts.append(_TREE_KEYS[k])
    cl, _ = _cl()
    parts.append(hashlib.sha256(cl.read_bytes()).hexdigest() if cl else "no-cl")
    return hashlib.sha256("|".join(parts).encode()).hexdigest()[:16]


def obj_name(member):
    return Path(member).stem + ".obj"


def build_tu(cfg, tag, member, rebuild=False):
    """(obj path, reason). Compile one TU of `tag` with its recorded flags (cached)."""
    row = cfg.get(tag)
    if row is None:
        return None, f"{tag} is not in vendor_libs.tsv"
    tree = tree_dir(row)
    if tree is None:
        return None, f"tree {row['tree']} of {tag} not under FABLE_THIRDPARTY ({thirdparty_dirs()}); run fetch_vendor_libs.py"
    src = tree / member
    if not src.is_file():
        return None, f"{member} not in {tree}"
    cl, vc = _cl()
    if cl is None:
        return None, "cl unavailable (no $FABLE_VC71/bin/cl.exe)"
    croot = cache_root()
    if croot is None:
        return None, "no object cache directory (set FABLE_VENDOR_OBJ or FABLE_THIRDPARTY)"
    try:
        key = build_key(cfg, row, tree)
        incs = include_dirs(cfg, row, tree)
    except FileNotFoundError as ex:
        return None, str(ex)
    outdir = croot / f"{tag}-{key}"
    obj = outdir / obj_name(member)
    if obj.is_file() and not rebuild:
        return obj, "cached"
    outdir.mkdir(parents=True, exist_ok=True)
    args = ["/nologo", "/c"] + row["flags_list"]
    for i in incs:
        args += ["/I", _wpath(i)]
    args += ["/Fo" + _wpath(obj), _wpath(src)]
    cmd = [str(cl)] + args
    if os.name != "nt":
        cmd = [os.environ.get("FABLE_WINE", "wine")] + cmd
    e = dict(os.environ)
    e["INCLUDE"] = _wpath(vc / "include")
    e.setdefault("WINEDEBUG", "-all")
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", env=e, timeout=240)
    except Exception as ex:  # wine missing, timeout
        return None, f"cl not run ({type(ex).__name__})"
    if r.returncode or not obj.is_file():
        return None, "cl failed: " + (r.stdout + r.stderr).strip().replace("\n", " | ")[-300:]
    return obj, "compiled"


def build_all(cfg, tag, rebuild=False):
    """{member: obj path} of every built TU of `tag` (failures in the second dict)."""
    out, fails = {}, {}
    for m in cfg[tag]["sources_list"]:
        o, why = build_tu(cfg, tag, m, rebuild)
        if o is None:
            fails[m] = why
        else:
            out[m] = o
    return out, fails


def main(argv):
    cfg = load_config()
    if argv[:1] == ["--build"]:
        rc = 0
        for tag in argv[1:] or list(cfg):
            ok, fails = build_all(cfg, tag)
            print(f"{tag}: {len(ok)} built, {len(fails)} failed" + "".join(f"\n  FAIL {m}: {w}" for m, w in fails.items()))
            rc |= bool(fails)
        return rc
    for tag, r in cfg.items():
        t = tree_dir(r)
        print(f"{tag:18} {r['version']:8} {'present' if t else 'MISSING':8} {r['tree']:24} {r['flags']}  "
              f"[{len(r['sources_list'])} TUs, {len(r['not_built_list'])} not built]")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
