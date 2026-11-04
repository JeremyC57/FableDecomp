#!/usr/bin/env python3
"""Wave 3: name RTTI-less entity-script vtables from TLC's own CEntityScriptBinding registrations.

Registration site (inlined CEntityScriptBinding<T> ctor in T::Main/Init):
    push -1 ; push <name literal> ; lea ecx,[tmp] ; call CCharString(const char*,long) 0x0099EBF0
    mov [B], <binding vtable> ; ... call CCharString copy 0x0099EC30
    mov [B+8], <script this> ; mov [B+0x10], <Alloc> ; mov byte [B+0x14],1 ; mov [B+0x18], flags
Alloc (static, __fastcall, returns CCountedPointer<CEntityScriptBase>):
    push size ; call operator new 0x00BFEA1A ; <ctor inline: base vt, CScriptThing copy, class vt> |  call ctor
    ... push 0xc ; call new ; mov [eax+4], 0x00CDEE00 (counted-pointer deleter)
Output: JSON of sites + allocs for name.py.
"""
import sys, os, struct, json, bisect, csv, re
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_IMM, X86_OP_REG

NEW = 0x00BFEA1A; CCS = 0x0099EBF0; CCS_COPY = 0x0099EC30; DELETER = 0x00CDEE00
E = Exe()
md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
starts = E.sstarts
RD_LO, RD_HI = [(s[1], s[1] + s[2]) for s in E.secs if s[0] == '.rdata'][0]

def fstart(va):
    i = bisect.bisect_right(starts, va) - 1
    return starts[i] if i >= 0 else None

def fnext(va):
    i = bisect.bisect_right(starts, va)
    return starts[i] if i < len(starts) else va + 0x4000

def insns(lo, hi):
    out = []
    b = E.read(lo, hi - lo)
    off = 0
    while off < len(b):
        got = False
        for i in md.disasm(b[off:], lo + off):
            out.append(i); off = i.address + i.size - lo; got = True
        if off < len(b):  # undecodable byte: skip one
            off += 1
    return out

def cstr(va):
    b = E.read(va, 200)
    if not b: return None
    s = b.split(b'\0')[0]
    try:
        t = s.decode('ascii')
    except Exception:
        return None
    return t if t and all(32 <= ord(c) < 127 for c in t) else None

def memop(i):
    for o in i.operands:
        if o.type == X86_OP_MEM: return o.mem
    return None

def regname(r): return md.reg_name(r) if r else None

def text(va): return E.tlo <= va < E.thi

# ---------- 1. registration sites
def find_sites():
    t = E.text; sites = []
    # C7 4r 10 imm32 (r != esp)  /  C7 8r 10 00 00 00 imm32
    for m in re.finditer(rb'\xC7[\x40-\x43\x45-\x47]\x10(....)', t, re.S):
        imm = struct.unpack('<I', m.group(1))[0]
        if text(imm): sites.append(E.tlo + m.start())
    for m in re.finditer(rb'\xC7[\x80-\x83\x85-\x87]\x10\x00\x00\x00(....)', t, re.S):
        imm = struct.unpack('<I', m.group(1))[0]
        if text(imm): sites.append(E.tlo + m.start())
    return sorted(set(sites))

def find_reg_sites():
    """`mov dword ptr [r+0x10], reg32` (89 /r, mod=01, disp8=0x10) -- the Alloc is a register loaded by `mov reg, imm32`."""
    t = E.text; out = []
    for m in re.finditer(rb'\x89([\x40-\x7f])\x10', t, re.S):
        modrm = m.group(1)[0]
        if modrm & 7 == 4: continue
        out.append(E.tlo + m.start())
    return out

def analyse_reg_site(sva):
    fs = fstart(sva)
    ins = insns(fs, sva + 3)
    idx = [k for k, i in enumerate(ins) if i.address == sva]
    if not idx: return None
    k = idx[0]; site = ins[k]
    if site.mnemonic != 'mov' or site.operands[1].type != X86_OP_REG: return None
    src = site.operands[1].reg; val = None
    for j in range(k - 1, max(k - 80, -1), -1):
        i = ins[j]
        if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_REG and i.operands[0].reg == src:
            if i.operands[1].type == X86_OP_IMM: val = i.operands[1].imm
            break
    if val is None or not text(val) or not is_alloc(val): return None
    d = analyse_site_core(ins, k, sva, fs, val)
    return d

def is_alloc(va):
    """body contains operator new + the counted-pointer deleter store, within 0x400 bytes, fastcall shape."""
    # do not trust manifest bounds (some Allocs are split / unknown starts): look 0x180 bytes ahead
    b = E.read(va, 0x180)
    if b[:1] not in (b'\x53', b'\x55', b'\x56', b'\x57'): return False
    if struct.pack('<I', DELETER) in b: return True
    # out-of-line counted-pointer variant: operator new in the prologue and a fastcall `ret 8` epilogue
    calls_new = any(i.mnemonic == 'call' and i.op_str == hex(NEW) for i in md.disasm(b[:0x20], va))
    return calls_new and b'\xc2\x08\x00' in b

def analyse_site(sva):
    fs = fstart(sva)
    ins = insns(fs, sva + 7 if E.read(sva, 2)[1] < 0x80 else sva + 10)
    idx = [k for k, i in enumerate(ins) if i.address == sva]
    if not idx: return dict(site=sva, func=fs, err='desync')
    k = idx[0]
    return analyse_site_core(ins, k, sva, fs, ins[k].operands[1].imm)

def analyse_site_core(ins, k, sva, fs, alloc):
    site = ins[k]
    breg = memop(site).base
    # walk back to the binding vtable store and the name literal
    bvt = None; name = None; nameaddr = None; this_reg = None; flags = None
    j = k - 1
    while j >= 0 and k - j < 60:
        i = ins[j]
        if i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM and i.operands[0].imm == NEW: break
        if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_MEM and i.operands[0].mem.base == breg:
            d = i.operands[0].mem.disp
            if d == 0 and i.operands[1].type == X86_OP_IMM: bvt = i.operands[1].imm
            if d == 8 and i.operands[1].type == X86_OP_REG: this_reg = regname(i.operands[1].reg)
        if i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM and i.operands[0].imm == CCS and name is None:
            # nearest push imm before this call
            for jj in range(j - 1, max(j - 6, -1), -1):
                p = ins[jj]
                if p.mnemonic == 'push' and p.operands[0].type == X86_OP_IMM and p.operands[0].imm > 0x10000:
                    nameaddr = p.operands[0].imm; name = cstr(nameaddr); break
        j -= 1
    # flags: forward few insns
    fwd = insns(sva, sva + 40)
    for i in fwd[1:6]:
        m_ = memop(i)
        if i.mnemonic == 'mov' and m_ and m_.base == breg and m_.disp == 0x18:
            o = i.operands[1]
            flags = o.imm if o.type == X86_OP_IMM else regname(o.reg)
    return dict(site=sva, func=fs, binding_vt=bvt, alloc=alloc, name=name, nameaddr=nameaddr,
                this_reg=this_reg, flags=flags, alloc_ok=is_alloc(alloc))

# ---------- 2. Alloc -> vtable
def last_vt_store(fva, entry_aliases, limit=0x500, depth=0):
    """Return (vt, ctor_called) : the last `mov dword [alias], imm(rdata)` along a linear walk; follows a ctor call
    `mov ecx, alias ; call X` (X then analysed with ecx alias) when it comes after the last inline store."""
    hi = min(fnext(fva), fva + limit)
    ins = insns(fva, hi)
    alias = set(entry_aliases); vt = None; ctor = None; after_new = 'eax' in alias
    ecx_obj = False
    for n, i in enumerate(ins):
        mn = i.mnemonic
        if mn == 'call' and i.operands[0].type == X86_OP_IMM:
            tgt = i.operands[0].imm
            if tgt == NEW:
                if depth == 0 and not after_new:
                    after_new = True; alias = {'eax'}; continue
                else:
                    break  # second new (counted-pointer block): stop
            if ecx_obj and depth < 2 and tgt not in (CCS, CCS_COPY):
                v2, c2 = last_vt_store(tgt, {'ecx'}, depth=depth + 1)
                if v2 is not None:
                    vt = v2; ctor = tgt
            ecx_obj = False
            if 'eax' in alias and depth == 0 and tgt != NEW:
                pass
            continue
        if mn in ('ret',): break
        if mn == 'mov' and len(i.operands) == 2:
            a, b = i.operands
            if a.type == X86_OP_REG and b.type == X86_OP_REG:
                if regname(b.reg) in alias: alias.add(regname(a.reg))
                elif regname(a.reg) in alias: alias.discard(regname(a.reg))
                if regname(a.reg) == 'ecx': ecx_obj = regname(b.reg) in alias
                continue
            if a.type == X86_OP_MEM and b.type == X86_OP_IMM and a.mem.disp == 0 and a.mem.index == 0 \
                    and regname(a.mem.base) in alias and RD_LO <= b.imm < RD_HI:
                vt = b.imm; ctor = None if depth == 0 else ctor
                continue
            if a.type == X86_OP_REG:
                alias.discard(regname(a.reg))
                if regname(a.reg) == 'ecx': ecx_obj = False
        if mn == 'lea' and i.operands[0].type == X86_OP_REG and regname(i.operands[0].reg) == 'ecx':
            ecx_obj = False
    return vt, ctor

def alloc_info(ava):
    b = E.read(ava, 0x20)
    ins = insns(ava, ava + 0x40)
    size = None
    for k, i in enumerate(ins):
        if i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM and i.operands[0].imm == NEW:
            for p in reversed(ins[:k]):
                if p.mnemonic == 'push' and p.operands[0].type == X86_OP_IMM: size = p.operands[0].imm; break
            break
    vt, ctor = last_vt_store(ava, set())
    return dict(size=size, vt=vt, ctor=ctor)

if __name__ == '__main__':
    sites = [analyse_site(s) for s in find_sites()]
    for s in find_reg_sites():
        r = analyse_reg_site(s)
        if r: r['form'] = 'reg'; sites.append(r)
    real = [s for s in sites if s.get('alloc_ok')]
    allocs = {}
    for s in real:
        a = s['alloc']
        if a not in allocs: allocs[a] = alloc_info(a)
    # all Alloc-shaped functions (deleter store) for coverage
    t = E.text; dels = []
    for m in re.finditer(re.escape(struct.pack('<I', DELETER)), t):
        dels.append(fstart(E.tlo + m.start()))
    json.dump(dict(sites=real, nonalloc_sites=len(sites) - len(real),
                   allocs={'%08x' % k: v for k, v in allocs.items()},
                   deleter_funcs=sorted(set('%08x' % d for d in dels if d))),
              open(sys.argv[1], 'w'), indent=0)
    print('candidate stores', len(sites), 'registration sites', len(real), 'allocs', len(allocs),
          'deleter funcs', len(set(dels)))
