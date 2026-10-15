#!/usr/bin/env python3
"""Hot-function ordering file for the handheld link (lld --symbol-ordering-file).

The lifted code is ~56 MB of .text in guest-address order, so the functions a frame runs are
spread over thousands of 4 KiB pages: on the Cortex-A53 (10-entry L1 iTLB, 512-entry shared main
TLB, 32 KiB L1I, small L2) that costs iTLB walks and L2 misses on almost every call. This script
turns a profile into the list of symbols the linker places first, contiguously:

    fable_hot_text_begin      marker (posix/codelayout.cpp): start of the hot range
    <hot host functions>      only with --host (kept ahead of the lifted code: see below)
    F_xxxxxxxx ...            hot lifted functions, hottest first, then their direct callees
    recomp_dispatch_ic ...    the runtime's dispatch (one .text section with the rest of runtime.c)
    fable_hot_text_end        marker: end of the hot range (the huge-page remap's extent)

Profile inputs (any mix, weights add up):
  --log FILE        a game log with the sampling profiler's lines (FABLE_PROFILE): every
                    "profile: ... guest 0xADDR (N)" adds N to F_ADDR. Each window lists only its
                    top 60, so this alone seeds ~100 functions; --expand adds their callees.
  --counts FILE     "<count> <symbol>" per line (symbol: F_xxxxxxxx, guest 0xADDR or a host
                    symbol), e.g. from `perf report --no-children --sort sym -F overhead,sym`
                    on the x86-64 harness build (the F_ names are the same on every host).
  --profdata FILE   `llvm-profdata show --all-functions --counts <x.profdata>` output from an
                    instrumented (FABLE_PGO=gen) harness run: function entry counts for every
                    function the run executed, the best input (see README.md, "Profile").
Expansion (--gen-dir, the lifter output): a seed's direct callees (`F_xxxxxxxx(c)` in its body)
inherit a decayed weight, --expand levels deep, and its direct callers (--callers levels) a
decayed share split over all of them (the sampled PC is in the leaf; the loop that calls it runs
too); the list stops at --budget-kb of estimated code. Indirect (virtual) calls are not followed:
only a complete profile (--profdata, --counts) finds those.

Host functions are listed only with --host: the sampling profiler attributes any address between
the first and the last lifted function to the nearest lifted function before it, so host code is
kept out of that range (placed before the first lifted function).

    gen_hot_order.py --gen-dir gen_win --log log.txt [--counts perf.txt] [--profdata pd.txt] \
        -o rebuild/recomp/posix/rg34xx/hot_symbols.txt
"""
import argparse
import os
import re
import sys
from collections import defaultdict

FN_RE = re.compile(r"^void (F_[0-9A-F]{8})\(Ctx\* restrict c\) \{", re.M)
CALL_RE = re.compile(r"\b(F_[0-9A-F]{8})\(c\)")
PROF_GUEST_RE = re.compile(r"profile:\s+[\d.]+% guest 0x([0-9A-Fa-f]{8})\s.*\((\d+)\)\s*$")
PROF_HOST_RE = re.compile(r"profile:\s+[\d.]+% host FableRecomp!(\S+) \((\d+)\)\s*$")
GUEST_SYM_RE = re.compile(r"^(?:guest\s+)?(?:0x)?([0-9A-Fa-f]{8})$")
# the runtime's hot entry points (all in runtime.c's one .text section; profiler markers)
RUNTIME_SYMS = ["recomp_dispatch_ic", "recomp_dispatch", "recomp_lookup"]
BEGIN, END = "fable_hot_text_begin", "fable_hot_text_end"
CODE_PER_C_BYTE = 0.23  # aarch64 -O2: ~56 MB of .text from ~248 MB of lifted C


def norm_guest(sym):
    m = GUEST_SYM_RE.match(sym)
    return "F_%08X" % int(m.group(1), 16) if m else (sym if sym.startswith("F_") else None)


def read_logs(paths, guest, host):
    for p in paths:
        with open(p, errors="replace") as f:
            for line in f:
                m = PROF_GUEST_RE.search(line)
                if m:
                    guest["F_%08X" % int(m.group(1), 16)] += int(m.group(2))
                    continue
                m = PROF_HOST_RE.search(line)
                if m:
                    host[m.group(1)] += int(m.group(2))


def read_counts(paths, guest, host):
    for p in paths:
        with open(p, errors="replace") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                parts = line.split(None, 1)
                if len(parts) != 2:
                    continue
                try:
                    n = float(parts[0].rstrip("%"))
                except ValueError:
                    continue
                sym = parts[1].strip()
                if "[.] " in sym or "[k] " in sym:  # perf report: "<dso>  [.] symbol"
                    if "[k] " in sym:
                        continue
                    sym = sym.split("[.] ", 1)[1].strip().split()[0]
                if sym.startswith("host "):  # the profiler's own names: only this binary's symbols
                    if not sym.startswith("host FableRecomp!"):
                        continue
                    sym = sym.split("!", 1)[1]
                g = norm_guest(sym)
                if g:
                    guest[g] += n
                elif sym and "+0x" not in sym and not sym.startswith("0x"):
                    host[sym] += n


def read_profdata(paths, guest, host):
    """`llvm-profdata show --all-functions --counts`: "  <name>:", "    Function count: N",
    "    Block counts: [...]". Weight: entries plus every counted region's executions (a proxy for
    the code a function runs, loops included)."""
    def add(name, n):
        if name and n > 0:
            g = norm_guest(name)
            if g:
                guest[g] += n
            else:
                host[name] += n

    for p in paths:
        name, n = None, 0
        with open(p, errors="replace") as f:
            for line in f:
                s = line.rstrip("\n")
                if s.startswith("  ") and not s.startswith("    ") and s.endswith(":"):
                    add(name, n)
                    name, n = s.strip()[:-1].split(":")[-1], 0  # file-local functions are "file.c:name"
                elif name and s.strip().startswith("Function count:"):
                    n += int(s.split(":")[1])
                elif name and s.strip().startswith("Block counts:"):
                    n += sum(int(x) for x in re.findall(r"\d+", s.split(":", 1)[1]))
                elif not s.startswith("  "):
                    add(name, n)
                    name, n = None, 0
        add(name, n)


def scan_gen(gen_dir):
    """Direct callees and estimated code size of every lifted function."""
    callees, size = {}, {}
    files = sorted(f for f in os.listdir(gen_dir) if re.match(r"recomp_\d+\.c$", f))
    for fn in files:
        with open(os.path.join(gen_dir, fn), errors="replace") as f:
            text = f.read()
        starts = [(m.start(), m.group(1)) for m in FN_RE.finditer(text)]
        for i, (pos, name) in enumerate(starts):
            end = starts[i + 1][0] if i + 1 < len(starts) else len(text)
            body = text[pos:end]
            size[name] = int(len(body) * CODE_PER_C_BYTE)
            seen, order = set(), []
            for c in CALL_RE.findall(body):
                if c != name and c not in seen:
                    seen.add(c)
                    order.append(c)
            callees[name] = order
    return callees, size


def read_nm_sizes(path):
    """`nm -S --defined-only <unstripped FableRecomp>`: real sizes override the estimate."""
    size = {}
    with open(path) as f:
        for line in f:
            p = line.split()
            if len(p) == 4 and p[2] in "tT":
                size[p[3]] = int(p[1], 16)
    return size


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--log", action="append", default=[], help="game log with profiler lines")
    ap.add_argument("--counts", action="append", default=[], help='"<count> <symbol>" lines')
    ap.add_argument("--profdata", action="append", default=[], help="llvm-profdata show --all-functions --counts output")
    ap.add_argument("--gen-dir", help="lifter output (recomp_*.c): call graph and size estimates")
    ap.add_argument("--nm", help="nm -S output of an unstripped build: exact function sizes")
    ap.add_argument("--expand", type=int, default=3, help="callee levels added below each profiled function")
    ap.add_argument("--callers", type=int, default=1, help="caller levels added above each profiled function")
    ap.add_argument("--decay", type=float, default=0.25, help="weight a callee inherits per level")
    ap.add_argument("--budget-kb", type=int, default=4096, help="stop listing lifted functions at this much code")
    ap.add_argument("--max-fn-kb", type=int, default=24, help="skip expanded (unprofiled) functions larger than this")
    ap.add_argument("--min-count", type=float, default=0, help="ignore profiled functions below this weight")
    ap.add_argument("--host", action="store_true", help="also list host functions from the profile")
    ap.add_argument("-o", "--output", required=True)
    a = ap.parse_args()

    guest, host = defaultdict(float), defaultdict(float)
    read_logs(a.log, guest, host)
    read_counts(a.counts, guest, host)
    read_profdata(a.profdata, guest, host)
    if not guest:
        sys.exit("no lifted functions in the profile inputs")

    callees, size = ({}, {})
    if a.gen_dir:
        callees, size = scan_gen(a.gen_dir)
    if a.nm:
        size.update(read_nm_sizes(a.nm))
    known = set(size) if size else None

    weight = {k: v for k, v in guest.items() if v > a.min_count and (known is None or k in known)}
    dropped = sorted(k for k in guest if known is not None and k not in known)
    seeds = dict(weight)
    # callees inherit a decayed share of their caller's weight (summed over callers)
    frontier = dict(seeds)
    for _ in range(a.expand if callees else 0):
        nxt = defaultdict(float)
        for f, w in frontier.items():
            for c in callees.get(f, ()):
                if known is None or c in known:
                    nxt[c] += w * a.decay
        for c, w in nxt.items():
            weight[c] = weight.get(c, 0.0) + w
        frontier = nxt
    if callees and a.callers > 0:
        callers = defaultdict(list)
        for f, cs in callees.items():
            for c in cs:
                callers[c].append(f)
        frontier = dict(seeds)
        for _ in range(a.callers):
            nxt = defaultdict(float)
            for f, w in frontier.items():
                up = callers.get(f, ())
                for c in up:
                    nxt[c] += w * a.decay / len(up)
            for c, w in nxt.items():
                weight[c] = weight.get(c, 0.0) + w
            frontier = nxt

    # profiled functions first (hottest first), then the expansion, within the budget
    ranked = sorted(seeds, key=lambda k: -seeds[k])
    ranked += sorted((k for k in weight if k not in seeds), key=lambda k: -weight[k])
    budget = a.budget_kb * 1024
    out, used = [], 0
    for k in ranked:
        s = size.get(k, 1024)
        if k not in seeds:
            if s > a.max_fn_kb * 1024:
                continue  # a huge switch or initialiser reached through the call graph: rarely hot
            if used + s > budget:
                break
        out.append(k)
        used += s

    with open(a.output, "w") as f:
        f.write("# Generated by rebuild/recomp/posix/rg34xx/gen_hot_order.py: the hot functions the linker\n")
        f.write("# places first (lld --symbol-ordering-file); one symbol per line, '#' comments.\n")
        f.write("# inputs: %s\n" % " ".join(os.path.basename(p) for p in a.log + a.counts + a.profdata))
        f.write("# %d profiled + %d callees, ~%d KiB of lifted code\n" % (
            sum(1 for k in out if k in seeds), sum(1 for k in out if k not in seeds), used // 1024))
        f.write(BEGIN + "\n")
        if a.host:
            for k in sorted(host, key=lambda k: -host[k]):
                f.write(k + "\n")
        for k in out:
            f.write(k + "\n")
        for k in RUNTIME_SYMS:
            f.write(k + "\n")
        f.write(END + "\n")
    print("%s: %d lifted functions (%d profiled, %d callees), ~%d KiB; %d host; %d unknown names dropped" % (
        a.output, len(out), sum(1 for k in out if k in seeds), sum(1 for k in out if k not in seeds),
        used // 1024, len(host) if a.host else 0, len(dropped)))
    if dropped:
        print("  not in this lifter output: " + " ".join(dropped[:12]) + (" ..." if len(dropped) > 12 else ""))


if __name__ == "__main__":
    main()
