# gdb script: log every msvcr71 _write(fd, buf, n) and fwrite(buf, sz, cnt, FILE*) made while c1xx runs (data hex, caller chain).
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25R/tools/c1/common.py").read())
LIM = cfg.get("lim", 1 << 22)
def on_write():
    esp = reg("esp"); ra, fd, buf, n = struct.unpack("<4I", rd(esp, 16))
    if not (0x10401000 <= ra < 0x10560000): return False
    out["events"].append(["w", fd, buf, n, rd(buf, min(n, LIM)).hex(), stack()])
def on_fwrite():
    esp = reg("esp"); ra, buf, sz, cnt, fp = struct.unpack("<5I", rd(esp, 20))
    if not (0x10401000 <= ra < 0x10560000): return False
    n = sz * cnt
    out["events"].append(["f", fp, buf, n, rd(buf, min(n, LIM)).hex(), stack()])
Hook(u32(0x10556298), on_write)
Hook(u32(0x10556184), on_fwrite)
run()
