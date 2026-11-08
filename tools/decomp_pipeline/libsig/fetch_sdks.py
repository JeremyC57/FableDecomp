#!/usr/bin/env python3
"""Fetch, hash-check and unpack the era SDKs retail Fable.exe was built against (docs/pipeline/SDK_NEEDS.md) into
$FABLE_SDK (default $S/sdk), never into the repo.

    python3 -I tools/decomp_pipeline/libsig/fetch_sdks.py [--sdk DIR] [NAME ...]        # default: all four
    python3 -I tools/decomp_pipeline/libsig/fetch_sdks.py --env                         # print the export lines

| NAME           | Unpacked to                   | Env var               | What                                              |
|----------------|-------------------------------|-----------------------|---------------------------------------------------|
| vs2003-include | $FABLE_SDK/vs2003/include     | FABLE_VS2003_INCLUDE  | VS .NET 2003 RTM Vc7/include (CRT + Dinkumware 313, ivec/fvec/dvec, comdef) |
| dxsdk-sum2004  | $FABLE_SDK/dxsdk_sum2004      | FABLE_DXSDK_SUM2004   | DirectX 9.0c SDK Summer 2004: Include, Lib (strmiids.lib), Samples/C++/DirectShow/BaseClasses |
| dxsdk-apr2005  | $FABLE_SDK/dxsdk_apr2005      | FABLE_DXSDK_APR2005   | DirectX SDK April 2005: Include (d3d9, d3dx9, dinput, dsound), Lib/x86 |
| psdk-feb2003   | $FABLE_SDK/psdk_feb2003       | FABLE_PSDK            | Platform SDK Feb 2003 (5.2.3790.0): include/ and lib/ (x86) of the CoreSDK-common.2.* cabs |

Sources are Microsoft's own files as archive.org preserves them: the VS 2003 item the lane's RTM cl.exe came from
(single zip members, SHA-1 per file in vs2003_include.sha1), the DirectX SDK installers (SHA-1 from the item metadata,
SHA-256 recorded at first fetch) and the 13 Platform SDK web-install cabs (SHA-1 from the item metadata). Every file is
checked before it is unpacked. Unpacking needs `7z`. The Platform SDK cabs carry MSI file keys
(`User32_Lib.95CE7DF8_...`); the real name is the key's stem with its last `_` turned into a dot (`User32.Lib`), only
i386 libraries are kept, and a name two cabs give different bytes for keeps the first (listed in
psdk_feb2003/RENAME_CONFLICTS.txt).
The lane's default include directory is NOT switched by this tool (coordinator decision pending a catalog re-grade).
"""
from __future__ import annotations

import argparse
import os
import shutil
import struct
import sys
import tempfile
import urllib.parse
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import fetch_vendor_libs as FV  # noqa: E402

VS2003_BASE = ("https://archive.org/download/visual-studio-2003-unpacked/VisualStudio2003.zip/VisualStudio2003%2F"
               "Program%20Files%2FMicrosoft%20Visual%20Studio%20.NET%202003%2FVc7%2Finclude%2F")
PSDK_BASE = "https://archive.org/download/psdk-full.-1/Feb%202003/"
PSDK_SHA1 = {
    1: "698345bf8ca8afd6f7e23fb0f9bf65b69d9f9576", 2: "1889b49d9deb58800d0209d2647d7c2620ee8fb5",
    3: "655c9b9cd381fe15afc4964f543d756e13ec245e", 4: "798a516764061a9c14dcee38db1fda967412345f",
    5: "4eab29210cbe9be846b245b2264ea0bc4b6dea9e", 6: "19c9ff0e237d579559c8c10eb9bae8db3b753c91",
    7: "674c8e001e05e7c9063dfade5818af2994c11a90", 8: "b50a356a481ac6b9c86d34ec96973cc3e325b419",
    9: "f18b3fc435f45e8d21cbda422b24b1b2705e5ecb", 10: "6dded60cc2e551ebec2693eab85f4967528f802a",
    11: "1d8932bb16a8257aee95ab2923605ff53e0b0412", 12: "94a40b3c954c08d6db624fa4db6bd2de841b34d5",
    13: "f69a071f3d82c464dee020760acd423af539e4bf",
}
DXSDK = {
    "dxsdk-sum2004": dict(
        url="https://archive.org/download/dxsdk_sum2004/dxsdk_sum2004.exe",
        sha1="73d875b97591f48707c38ec0dbc63982ff45c661",
        sha256="96cf9a2ee49ac42cca5c296a027c56a0928803ff16c693d3a9c5f95826a22402",
        unpack="7z2:dxsdk.exe|Include=Include|Lib=Lib|Samples/C++/DirectShow/BaseClasses=Samples/C++/DirectShow/BaseClasses",
        dest="dxsdk_sum2004", env="FABLE_DXSDK_SUM2004"),
    "dxsdk-apr2005": dict(
        url="https://archive.org/download/dxsdk_apr2005/dxsdk_apr2005.exe",
        sha1="ec4dd48826a399c6ce041745f14b90356d8c5cd1",
        sha256="2e525e4c9b26ab6145e4bbe17d3b74c31f764ae416f6383b4ea51f50d232f4ba",
        unpack="7z2:dxsdk.exe|Include=Include|Lib/x86=Lib/x86",
        dest="dxsdk_apr2005", env="FABLE_DXSDK_APR2005"),
}
ENV = {"vs2003-include": ("FABLE_VS2003_INCLUDE", "vs2003/include"), "dxsdk-sum2004": ("FABLE_DXSDK_SUM2004", "dxsdk_sum2004"),
       "dxsdk-apr2005": ("FABLE_DXSDK_APR2005", "dxsdk_apr2005"), "psdk-feb2003": ("FABLE_PSDK", "psdk_feb2003")}


def fetch_vs2003_include(sdk):
    dest = sdk / "vs2003" / "include"
    bad = 0
    for line in (HERE / "vs2003_include.sha1").read_text().splitlines():
        sha1, rel = line.split(None, 1)
        out = dest / rel
        if out.is_file() and FV.file_hash(out, "sha1") == sha1:
            continue
        if not FV.download(VS2003_BASE + urllib.parse.quote(rel, safe=""), out, want_sha1=sha1, max_time=600):
            print(f"SHA-1 MISMATCH vs2003 Vc7/include/{rel}", file=sys.stderr)
            out.unlink()
            bad += 1
    return bad == 0, dest


def fetch_dxsdk(sdk, name):
    d = DXSDK[name]
    exe = sdk / "dl" / d["url"].rsplit("/", 1)[1]
    if not FV.download(d["url"], exe, want_sha256=d["sha256"], want_sha1=d["sha1"]):
        print(f"HASH MISMATCH {exe.name}", file=sys.stderr)
        return False, None
    dest = sdk / d["dest"]
    with tempfile.TemporaryDirectory(dir=sdk) as td:
        top = FV.unpack_spec(exe, d["unpack"], td)
        if dest.exists():
            shutil.rmtree(dest)
        shutil.move(str(top), str(dest))
    return True, dest


def _machine(lib):
    """Machine of the first object / import record of an archive (0x14C = i386), or None."""
    data = lib.read_bytes()
    if data[:8] != b"!<arch>\n":
        return None
    o = 8
    while o + 60 <= len(data):
        name = data[o:o + 16].decode("latin-1").strip()
        size = int(data[o + 48:o + 58].split()[0])
        body = data[o + 60:o + 60 + size]
        if name not in ("/", "//"):
            return struct.unpack_from("<H", body, 6 if body[:4] == b"\0\0\xff\xff" else 0)[0]
        o += 60 + size + (size & 1)
    return None


def fetch_psdk(sdk):
    dl = sdk / "dl" / "psdk2003"
    for i, sha1 in PSDK_SHA1.items():
        if not FV.download(f"{PSDK_BASE}PSDK-FULL.{i}.cab", dl / f"PSDK-FULL.{i}.cab", want_sha1=sha1):
            print(f"SHA-1 MISMATCH PSDK-FULL.{i}.cab", file=sys.stderr)
            return False, None
    dest = sdk / "psdk_feb2003"
    with tempfile.TemporaryDirectory(dir=sdk) as td:
        FV.unpack_7z(dl / "PSDK-FULL.1.cab", Path(td) / "set")           # the 13 cabs are one multi-volume set
        cabs = sorted((Path(td) / "set" / "setup").glob("CoreSDK-common.2.*.cab"))   # 2.x: Include + Lib; 3.x has no headers or libs; 4.x are samples
        out = Path(td) / "out"
        conflicts, origin = [], {}
        for cab in cabs:
            x = Path(td) / "x" / cab.stem
            FV.unpack_7z(cab, x, ["*_h.*", "*_H.*", "*_Lib.*", "*_lib.*", "*_LIB.*"])
            for f in sorted(x.iterdir()):
                stem, ext = f.name.split(".", 1)[0].rsplit("_", 1)
                sub = "include" if ext.lower() == "h" else "lib"
                if sub == "lib" and _machine(f) != 0x14C:
                    continue
                tgt = out / sub / f"{stem}.{ext}"
                twin = next((p for p in tgt.parent.glob("*") if p.name.lower() == tgt.name.lower()), None) \
                    if tgt.parent.is_dir() else None
                if twin is not None:
                    if twin.read_bytes() != f.read_bytes():
                        conflicts.append(f"{sub}/{twin.name}: kept {origin.get(twin.name)}, {cab.name} differs")
                    continue
                tgt.parent.mkdir(parents=True, exist_ok=True)
                shutil.move(str(f), str(tgt))
                origin[tgt.name] = cab.name
        (out / "RENAME_CONFLICTS.txt").write_text("\n".join(conflicts) + "\n")
        if dest.exists():
            shutil.rmtree(dest)
        shutil.move(str(out), str(dest))
    return True, dest


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--sdk", default=os.environ.get("FABLE_SDK") or str(Path(os.environ.get("S", ".")) / "sdk"))
    ap.add_argument("--env", action="store_true", help="print the export lines for env.sh and exit")
    ap.add_argument("names", nargs="*")
    a = ap.parse_args(argv)
    sdk = Path(a.sdk)
    if a.env:
        print(f"export FABLE_SDK={sdk}")
        for var, rel in ENV.values():
            print(f"export {var}=$FABLE_SDK/{rel}")
        return 0
    sdk.mkdir(parents=True, exist_ok=True)
    rc = 0
    for name in a.names or list(ENV):
        if name == "vs2003-include":
            ok, dest = fetch_vs2003_include(sdk)
        elif name in DXSDK:
            ok, dest = fetch_dxsdk(sdk, name)
        elif name == "psdk-feb2003":
            ok, dest = fetch_psdk(sdk)
        else:
            raise SystemExit(f"unknown SDK {name}; one of {list(ENV)}")
        print(f"{'ok   ' if ok else 'FAIL '} {name:15} {dest}  ({ENV[name][0]})")
        rc |= not ok
    return rc


if __name__ == "__main__":
    sys.exit(main())
