#!/usr/bin/env python3
"""sim.py: model of c2.dll 13.10.3077's frame layout for a big (> 0x80) frame.
slots: list of dicts {size, w, align, flag} in packer creation order (index = slot number; params first).
qsort (0x107ae0c4, K&R: pivot = middle, swapped to left) on [nparams, n-1] by comparator 0x107ae9c2: density
w*1000//size descending (flag bit at +0x18 first); offsets then assigned from the last sorted slot upward."""
import sys, json, struct
sys.setrecursionlimit(100000)
def dens(s): return (s['w'] * 1000) // s['size']
def cmp(a, b):
    # 0x107ae9c2(a = pivot v[left], b = v[i]); flags assumed equal (all 0 here)
    ca, cb = dens(a), dens(b)
    if ca == cb: return 0
    return 1 if ca >= cb else -1
def qsort(v, left, right):
    while True:
        if left >= right: return
        mid = (left + right) // 2
        v[left], v[mid] = v[mid], v[left]
        last = left
        for i in range(left + 1, right + 1):
            if cmp(v[left], v[i]) < 0:
                last += 1
                v[last], v[i] = v[i], v[last]
        v[left], v[last] = v[last], v[left]
        qsort(v, left, last - 1)
        left = last + 1
def layout(slots, nparams, top=0x68, bottom_first=True):
    v = list(slots)
    qsort(v, nparams, len(v) - 1)
    off = {}
    if bottom_first:
        # allocate from the lowest-density slot (end of the array) with a downward-growing cursor, then shift so the
        # first sorted slot ends at `top`
        cur = 0; tmp = {}
        for s in reversed(v[nparams:]):
            a = min(max(s['align'], 1), 4)
            cur = cur - s['size']; cur = -((-cur + a - 1) // a * a)
            tmp[s['id']] = cur
        # cur is the lowest; map so that frame is [cur, 0): unknown bias -> caller fixes with `top`
        return v, tmp, cur
    return v, off, 0
def slots_from_raw(raw):
    b = bytes.fromhex(raw); out = []
    for i in range(len(b) // 0x1c):
        m, it, size, w, off, align, flag = struct.unpack_from('<7i', b, 0x1c * i)
        out.append(dict(id=i, size=size, w=w, off=off, align=align, flag=flag & 0xff))
    return out
