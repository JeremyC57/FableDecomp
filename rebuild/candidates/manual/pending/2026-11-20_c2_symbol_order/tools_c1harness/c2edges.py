# c2 gdb script: at the topological sort 0x1070CFDF (and at the needed-closure 0x1070D3DD) dump the function-entry list
# [0x1087F4C8]: entry -> sym name, sym+0x37, sym+0x69, entry+0x50, callee edges (callee name, edge+0xC word, callee sym+0x37 / +0x69).
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
def nm(sym):
    try: return cstr(u32(sym + 4))
    except Exception: return None
def dump(tag):
    p = u32(0x1087F4C8); res = []; k = 0
    while p and k < 400:
        try:
            s = u32(p + 4); ed = []; e = u32(p + 0xC); j = 0
            while e and j < 200:
                ce = u32(e + 4); cs = u32(ce + 4)
                ed.append([nm(cs), struct.unpack("<H", rd(e + 0xC, 2))[0], u32(cs + 0x37), u32(cs + 0x69)])
                e = u32(e); j += 1
            res.append([nm(s), u32(s + 0x37), u32(s + 0x69), struct.unpack("<H", rd(p + 0x50, 2))[0], ed])
            p = u32(p)
        except Exception as x: res.append(["ERR", str(x)]); break
        k += 1
    out["events"].append([tag, res])
done = set()
def mk(tag):
    def f():
        if tag in done: return
        done.add(tag); dump(tag)
    return f
Hook(0x1070D3DD, mk("closure"))
Hook(0x1070CFDF, mk("sort"))
run()
