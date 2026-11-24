# rtti366.py <vtable va...>: the RTTI type-descriptor name of a retail vtable (vtable[-1] -> CompleteObjectLocator -> TypeDescriptor). NF366.
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
secs = nf.exe()
def rd(va, n):
    for nm, b, d, ch in secs:
        if b <= va < b + len(d): return d[va - b: va - b + n]
def u32(va): return struct.unpack('<I', rd(va, 4))[0]
for v in sys.argv[1:]:
    va = int(v, 16)
    try: col = u32(va - 4); td = u32(col + 12); nm = rd(td + 8, 200).split(b"\0")[0].decode()
    except Exception: nm = "NO-RTTI (vtable[-1]=%08x)" % u32(va - 4)
    print(v, nm)
