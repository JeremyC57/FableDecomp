# gdb script: raw dump of tuples per block at the first priority pass (BLKSTART) for the target function
import sys
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched")
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
TARGET = cfg.get("target", "AddToBoundingSphere")
seen = set()
def opname(opc):
    try: return rd(u32(0x10874398 + 4 * (opc + 4)), 16).split(b"\0")[0].decode("latin-1")
    except Exception: return hex(opc)
def on_blk():
    if not state["fn"] or TARGET not in state["fn"]: return
    b = reg("ebx")
    if b in seen: return
    seen.add(b)
    first = u32(b + 0x1c); end = u32(b + 0x20)
    tl = []; t = end; n = 0
    while t and n < 400:
        raw = rd(t, 0x40)
        ops = []; o = struct.unpack_from("<I", raw, 0x28)[0]; k = 0
        while o and k < 8:
            try: oraw = rd(o, 0x30)
            except Exception: break
            ops.append([o, oraw.hex()]); o = struct.unpack_from("<I", oraw, 0)[0]; k += 1
        opc = struct.unpack_from("<I", raw, 4)[0] & 0xffff
        tl.append({"a": t, "raw": raw.hex(), "op": opname(opc), "ops": ops})
        if t == first: break
        t = struct.unpack_from("<I", raw, 0x10)[0]; n += 1
    out["events"].append(["blk", b, rd(b, 0xb0).hex(), tl])
Hook(0x1078244D, on_blk)
run()
