#!/usr/bin/env python3
"""Fetch and SHA-256-check the vendor source trees of vendor_libs.tsv into FABLE_THIRDPARTY (never into the repo).

    python3 tools/decomp_pipeline/libsig/fetch_vendor_libs.py [--dest DIR] [TAG ...]     # default DIR $S/thirdparty

Each tarball goes to DIR/dl/<basename> and is accepted when its sha256 is the recorded one; it is unpacked into the
parent of DIR/<tree>. A generated archive (GitLab's /-/archive/) may be re-packed by the host with a new sha256: it is
then accepted only when the unpacked tree hashes to the recorded tree_sha256 (vendor_lib.tree_hash). Then
`export FABLE_THIRDPARTY=DIR`.
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


def sha256(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--dest", default=str(Path(os.environ.get("S", ".")) / "thirdparty"))
    ap.add_argument("tags", nargs="*")
    a = ap.parse_args(argv)
    cfg = VL.load_config()
    dest = Path(a.dest)
    rc = 0
    for tag in a.tags or list(cfg):
        r = cfg[tag]
        tgz = dest / "dl" / r["url"].rsplit("/", 1)[1]
        tgz.parent.mkdir(parents=True, exist_ok=True)
        if not (tgz.is_file() and sha256(tgz) == r["sha256"]):
            part = tgz.with_suffix(tgz.suffix + ".part")
            subprocess.run(["curl", "-sS", "-L", "--retry", "2", "--max-time", "600", "-o", str(part), r["url"]], check=True)
            part.replace(tgz)
        tar_ok = sha256(tgz) == r["sha256"]
        tree = dest / r["tree"]
        with tempfile.TemporaryDirectory(dir=dest) as td:
            with tarfile.open(tgz) as tf:
                members = [m for m in tf.getmembers() if m.isfile() or m.isdir()]   # no links, devices
                for m in members:                   # vendor archive: refuse absolute paths and `..`
                    if m.name.startswith("/") or ".." in Path(m.name).parts:
                        raise SystemExit(f"{tgz.name}: unsafe member {m.name}")
                tf.extractall(td, members=members, **({"filter": "data"} if hasattr(tarfile, "data_filter") else {}))
            top = [p for p in Path(td).iterdir()]
            if len(top) != 1 or top[0].name != tree.name:
                raise SystemExit(f"{tgz.name}: expected one top directory {tree.name}, got {[p.name for p in top]}")
            th = VL.tree_hash(top[0])
            if not tar_ok and th != r["tree_sha256"]:
                print(f"SHA-256 MISMATCH {tgz.name}: archive {sha256(tgz)} (want {r['sha256']}), "
                      f"tree {th} (want {r['tree_sha256']})", file=sys.stderr)
                rc = 1
                continue
            if tree.exists():
                shutil.rmtree(tree)
            tree.parent.mkdir(parents=True, exist_ok=True)
            shutil.move(str(top[0]), str(tree))
        print(f"ok    {tag:18} {tree}  (archive sha256 {'ok' if tar_ok else 'changed, tree hash ok'}; tree {th[:16]}...)")
    return rc


if __name__ == "__main__":
    sys.exit(main())
