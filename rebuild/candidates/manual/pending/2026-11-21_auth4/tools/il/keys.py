# gdb script: log every tree-node key computation (0x1075d388, ebx = node): node, kind, key, children
exec(open("/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/auth4/tools/sched/common.py").read())
pend = {}
def on_entry():
    esp = reg("esp"); pend[esp - 0] = reg("ebx")
def on_ret():
    b = reg("ebx"); k = reg("eax")
    try:
        kind = rd(b + 8, 1)[0]
        ch = []
        if not ((1 << kind) & 0x1ffe):
            o = u32(b + 0x28); n = 0
            while o and n < 8:
                ch.append(o); o = u32(o); n += 1
        out["events"].append(["k", b, kind, k, ch])
    except Exception as e:
        out["events"].append(["kerr", b, str(e)])
Hook(0x1075d3be, on_ret)
Hook(0x1075d427, on_ret)
run()
