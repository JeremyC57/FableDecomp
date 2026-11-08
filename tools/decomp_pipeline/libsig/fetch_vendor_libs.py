#!/usr/bin/env python3
"""Fetch and SHA-256-check the vendor source trees of vendor_libs.tsv into FABLE_THIRDPARTY (never into the repo).

    python3 tools/decomp_pipeline/libsig/fetch_vendor_libs.py [--dest DIR] [--dl DLDIR] [TAG ...]   # default DIR $S/thirdparty

Each tarball goes to DIR/dl/<basename> and is accepted when its sha256 is the recorded one; it is unpacked into the
parent of DIR/<tree>. A generated archive (GitLab's /-/archive/) may be re-packed by the host with a new sha256: it is
then accepted only when the unpacked tree hashes to the recorded tree_sha256 (vendor_lib.tree_hash). A row with an
`unpack` spec is a vendor installer instead (`7z2:dxsdk.exe|Samples/C++/DirectShow/BaseClasses=BaseClasses|Include=Include`:
the DirectX SDK WEXTRACT exe is opened with 7z, then only the listed directories of the inner `dxsdk.exe`); its
sha256 must match. Then `export FABLE_THIRDPARTY=DIR`.
"""
from __future__ import annotations

import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import tarfile
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import vendor_lib as VL  # noqa: E402


def file_hash(p, algo="sha256"):
    h = hashlib.new(algo)
    with open(p, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def sha256(p):
    return file_hash(p)


def download(url, out, want_sha256=None, want_sha1=None, max_time=1800):
    """Fetch `url` to `out` unless a file with the pinned hash is already there; returns True when the hash agrees
    (a missing pin counts as agreeing only for the hash that is pinned)."""
    out = Path(out)
    out.parent.mkdir(parents=True, exist_ok=True)

    def ok(f):
        return f.is_file() and (not want_sha256 or file_hash(f) == want_sha256) and \
            (not want_sha1 or file_hash(f, "sha1") == want_sha1)
    if not ok(out):
        part = out.with_name(out.name + ".part")
        subprocess.run(["curl", "-sS", "-L", "--retry", "2", "--max-time", str(max_time), "-o", str(part), url], check=True)
        part.replace(out)
    return ok(out)


def _safe_tree(root):
    """Refuse links and anything outside `root` in an unpacked vendor archive."""
    root = Path(root).resolve()
    for p in Path(root).rglob("*"):
        if p.is_symlink() or root not in p.resolve().parents:
            raise SystemExit(f"unsafe member {p}")


def unpack_tar(tgz, td):
    with tarfile.open(tgz) as tf:
        members = [m for m in tf.getmembers() if m.isfile() or m.isdir()]   # no links, devices
        for m in members:                   # vendor archive: refuse absolute paths and `..`
            if m.name.startswith("/") or ".." in Path(m.name).parts:
                raise SystemExit(f"{Path(tgz).name}: unsafe member {m.name}")
        tf.extractall(td, members=members, **({"filter": "data"} if hasattr(tarfile, "data_filter") else {}))


def unpack_7z(archive, outdir, members=()):
    """`7z x` (only `members`, 7z wildcards, when given) into outdir; 7z opens WEXTRACT .exe and CAB files."""
    Path(outdir).mkdir(parents=True, exist_ok=True)
    r = subprocess.run(["7z", "x", "-y", "-bd", f"-o{outdir}", str(archive), *members], capture_output=True, text=True)
    if r.returncode:
        raise SystemExit(f"7z failed on {archive}: {(r.stdout + r.stderr)[-400:]}")
    _safe_tree(outdir)


def unpack_spec(archive, spec, td):
    """`7z2:<inner exe>|<src dir>=<dst dir>|...`: a DirectX SDK installer is a WEXTRACT exe holding a CAB-format
    `dxsdk.exe`; unpack the outer one, then only the listed directories of the inner one, renamed to <dst>."""
    kind, _, rest = spec.partition(":")
    if kind != "7z2":
        raise SystemExit(f"unknown unpack kind {kind}")
    inner, *maps = rest.split("|")
    unpack_7z(archive, Path(td) / "outer", [inner])
    pairs = [m.split("=", 1) for m in maps]
    unpack_7z(Path(td) / "outer" / inner, Path(td) / "inner", [f"{src}/*" for src, _ in pairs])
    top = Path(td) / "top"
    for src, dst in pairs:
        if not (Path(td) / "inner" / src).is_dir():
            raise SystemExit(f"{inner}: no {src}")
        (top / dst).parent.mkdir(parents=True, exist_ok=True)
        shutil.move(str(Path(td) / "inner" / src), str(top / dst))
    return top


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--dest", default=str(Path(os.environ.get("S", ".")) / "thirdparty"))
    ap.add_argument("--dl", default=None, help="download directory (default DEST/dl; fetch_sdks.py uses $FABLE_SDK/dl, "
                    "pass it here to share the DirectX SDK installer)")
    ap.add_argument("tags", nargs="*")
    a = ap.parse_args(argv)
    cfg = VL.load_config()
    dest = Path(a.dest)
    rc = 0
    for tag in a.tags or list(cfg):
        r = cfg[tag]
        tgz = Path(a.dl or dest / "dl") / r["url"].rsplit("/", 1)[1]
        tar_ok = download(r["url"], tgz, r["sha256"])
        if not tar_ok and r.get("unpack"):
            print(f"SHA-256 MISMATCH {tgz.name}: {sha256(tgz)} (want {r['sha256']})", file=sys.stderr)
            rc = 1
            continue
        tree = dest / r["tree"]
        with tempfile.TemporaryDirectory(dir=dest) as td:
            if r.get("unpack"):
                top = [unpack_spec(tgz, r["unpack"], td)]
            else:
                unpack_tar(tgz, td)
                top = [p for p in Path(td).iterdir()]
                if len(top) != 1 or top[0].name != tree.name:
                    raise SystemExit(f"{tgz.name}: expected one top directory {tree.name}, got {[p.name for p in top]}")
            th = VL.tree_hash(top[0])
            if th != r["tree_sha256"] and not tar_ok:
                print(f"SHA-256 MISMATCH {tgz.name}: archive {sha256(tgz)} (want {r['sha256']}), "
                      f"tree {th} (want {r['tree_sha256']})", file=sys.stderr)
                rc = 1
                continue
            if th != r["tree_sha256"]:
                print(f"WARN  {tag}: archive sha256 ok but the unpacked tree hashes to {th} (recorded {r['tree_sha256']})",
                      file=sys.stderr)
            if tree.exists():
                shutil.rmtree(tree)
            tree.parent.mkdir(parents=True, exist_ok=True)
            shutil.move(str(top[0]), str(tree))
        print(f"ok    {tag:22} {tree}  (archive sha256 {'ok' if tar_ok else 'changed, tree hash ok'}; tree {th[:16]}...)")
    return rc


if __name__ == "__main__":
    sys.exit(main())
