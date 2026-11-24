# gdb script (DIAGNOSTIC): when a tree node's key == cfg target, overwrite its child #idx key (+0x10) with cfg value
exec(open("/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/auth4/tools/sched/common.py").read())
TGT = int(cfg["target"], 16); IDX = int(cfg.get("idx", 1)); VAL = int(cfg["value"], 16)
def on_ret():
    b = reg("ebx"); k = reg("eax")
    if k != TGT: return
    o = u32(b + 0x28); ch = []
    while o and len(ch) < 8: ch.append(o); o = u32(o)
    old = u32(ch[IDX] + 0x10)
    inf.write_memory(ch[IDX] + 0x10, struct.pack("<I", VAL))
    out["events"].append(["forced", b, k, old, VAL])
Hook(0x1075d3be, on_ret)
Hook(0x1075d427, on_ret)
run()
