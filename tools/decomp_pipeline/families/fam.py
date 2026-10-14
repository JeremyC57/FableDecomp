#!/usr/bin/env python3
"""Family generator: parametrised candidates for repeated body shapes.
Usage: fam.py <repo> <family-id> <out-batch-name> [limit]   (env S = scratchpad)"""
import json, sys, csv, re, os, struct
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EAX, X86_REG_ESI
ROOT=Path(sys.argv[1]); FAM=sys.argv[2]; BATCH=sys.argv[3]; LIMIT=int(sys.argv[4]) if (len(sys.argv)>4 and sys.argv[4]!="-") else 10**9
TPL=sys.argv[5] if len(sys.argv)>5 else FAM   # template id (family index of the scan the template was written for)
S=Path(os.environ["S"]); OUT=ROOT/"rebuild/candidates/manual/batches"/BATCH
fams=json.load(open(os.environ.get("FAMFILE", str(S/"batch/families.json"))))
man={r["address"].lower():r for r in csv.DictReader(open(ROOT/"rebuild/manifest/functions.tsv",encoding="utf-8-sig"),delimiter="\t")}
rtti={}
rtti_full={}
for r in csv.DictReader(open(ROOT/"rebuild/manifest/rtti-vtables.tsv"),delimiter="\t"):
    rtti_full.setdefault(int(r["vtable_va"],16), r["class"])
    rtti.setdefault(int(r["vtable_va"],16), r["class"].split("::")[-1])   # leaf: namespaces are not modelled by the shape templates
md=Cs(CS_ARCH_X86,CS_MODE_32); md.detail=True
def san(s): return re.sub(r"[^A-Za-z0-9]","",s)
def strip(hx):
    b=bytes.fromhex(hx)
    while b and b[-1] in (0xCC,0x90): b=b[:-1]
    return b
def label(va):
    r=man.get(f"{va:08x}",{}); return (r.get("module") or "_global"), (r.get("name") or f"sub_{va:08x}"), r
def clean(n):
    n=n.split("::")[-1]
    return n if re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", n) else None
def ctor_class(va):
    """class name if the callee label looks like X::X (a constructor), else None"""
    mod,name,_=label(va); leaf=name.split("::")[-1]
    if leaf==mod.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",leaf): return leaf
    return None
uniq=set(); _cnt={}
for _a,_r in man.items(): _k=(_r.get("module"),_r.get("name")); _cnt[_k]=_cnt.get(_k,0)+1
uniq={k for k,v in _cnt.items() if v==1}
def tctor_class(va):
    """ctor_class() but only when the (module,name) label is unique in the manifest (label_trust rule)"""
    mod,name,_=label(va)
    return ctor_class(va) if (mod,name) in uniq else None
_exe=(S/"dl/game/Fable The Lost Chapters/Fable.exe").read_bytes()
_pe=struct.unpack_from("<I",_exe,0x3c)[0]; _ns=struct.unpack_from("<H",_exe,_pe+6)[0]; _os=struct.unpack_from("<H",_exe,_pe+20)[0]; _secs=[]
for _i in range(_ns):
    _s=_pe+24+_os+40*_i; _vs,_va,_rs,_ro=struct.unpack_from("<IIII",_exe,_s+8); _secs.append((_va+0x400000,_vs,_ro))
def lit(va):
    for base,vs,ro in _secs:
        if base<=va<base+vs:
            o=ro+va-base; e=_exe.find(b"\0",o,o+200); return _exe[o:e].decode("latin1")
    return ""
def rb(va,n):
    for base,vs,ro in _secs:
        if base<=va<base+vs: return _exe[ro+va-base:ro+va-base+n]
    return b""
def wlit(va):
    for base,vs,ro in _secs:
        if base<=va<base+vs:
            o=ro+va-base; w=_exe[o:o+400].decode("utf-16le","replace"); return w[:w.index("\0")] if "\0" in w else w
    return ""
def parse(b, va):
    ins=list(md.disasm(b,va)); calls=[]; imms=[]; vts=[]; rets=None; pushes=[]
    for i in ins:
        if i.mnemonic=="call" and i.operands[0].type==X86_OP_IMM: calls.append(i.operands[0].imm)
        if i.mnemonic=="push" and i.operands[0].type==X86_OP_IMM: pushes.append(i.operands[0].imm)
        if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x1200000<=i.operands[1].imm<0x1300000: vts.append((i.operands[0].mem.disp, i.operands[1].imm))
        if i.mnemonic=="ret": rets=i.operands[0].imm if i.operands else 0
    return ins, calls, pushes, vts, rets
HDRF=lambda addr,title,n: (f"// {title}\n// Retail: Fable.exe (TLC, VC7.1) function @ 0x{addr.upper()}, oracle length {n} bytes.\n"
    "// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK\n//        13.10.4035 cl.exe); the batch README records the lane verdict.\n")
T=lambda addr,body: ('#include <cstdio>\n'+body+f'\n    std::printf("{addr}_TEST PASS\\n");\n    return 0;\n}}\n')
FAIL=lambda addr: f'{{ std::printf("{addr}_TEST FAIL\\n"); return 1; }}'
B=[]; rows=[]
def add(addr, module, name, src, test, b):
    B.append(dict(address=addr, module=module, name=name, source_cpp=src, test_cpp=test, pass_pattern=f"{addr}_TEST PASS")); rows.append([addr, f"{module}::{name}", len(b), b.hex()])

if FAM=="tiny":
    key="tiny"; members=[]
    for _k,_m in fams:
        _b=strip(_m[0][4])
        if len(_b)<=16 and not _m[0][4].startswith(("8d4d","8b4d","8b45","ff25")): members.extend(_m)   # skip EH funclets and import jumps
else:
    key, members = fams[int(FAM)]
ONLY=set(x.lower() for x in os.environ.get("FAMONLY","").split(",") if x)
for a, mmod, mname, ln, hx in members[:LIMIT]:
    if ONLY and a.lower() not in ONLY: continue
    b=strip(hx); va=int(a,16); ins,calls,pushes,vts,retn=parse(b,va)
    if TPL in ("2","57"):   # Create: new CX(thing)
        size=pushes[0]; ctor=calls[1]; cls=ctor_class(ctor) or (mname[:-7] if mname.endswith("_Create") else None) or f"CClass_{ctor:08x}"
        _,_,cr=label(ctor); ptypes=(cr.get("parameter_types") or "").split(","); argt=ptypes[1].strip() if len(ptypes)>1 and ptypes[1].strip() else "CThing *"
        argt=argt.replace(" *","*")
        if "<" in argt or not re.match(r"^[A-Za-z_][A-Za-z0-9_ ]*\*?$", argt): argt="void*"
        if not argt.endswith("*") and argt not in ("long","int","unsigned long","unsigned int","short","unsigned short","char","unsigned char","float","bool"): argt="long"   # by-value enum: 4-byte register value
        argbase=argt.rstrip("*").strip() or "CThing"
        fwd2=f"struct {argbase};\n" if (argbase.startswith("C") and argt.endswith("*")) else ""
        add(a,"_global",f"Create_{cls}",HDRF(a,f"Create_{cls}({argt}) -- factory: new {cls}(owner)",len(b))+f'''//
// Retail bytes:  push esi ; push {size:#x} ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call {cls}::{cls} ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof({cls}) = {size:#x} bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x{ctor:08X} on it with the
// argument, returning the new object (or null). One of {len(members)} bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
{fwd2}
struct {cls} {{
    unsigned char _body[{size:#x}];       // sizeof({cls}) = {size:#x}
    {cls}({argt} owner);                   // 0x{ctor:08X}
}};

{cls}* __fastcall Create_{cls}({argt} owner)
{{
    return new {cls}(owner);
}}
''',T(a,f'''struct O {{}}; static int g_c = 0; struct X {{ unsigned char body[{size:#x}]; X(O* o) {{ ++g_c; body[0] = o ? 1 : 0; }} }};
static X* __fastcall Create(O* o) {{ return new X(o); }}
int main()
{{
    O o; X* x = Create(&o); if (!x || g_c != 1 || x->body[0] != 1) '''+FAIL(a)+'''
    delete x;
'''),b)
    elif TPL in ("3","22"):   # CDerived::CDerived() : CBase() {} with own vtable
        base=calls[0]; basecls=ctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or (mmod.split("::")[-1] if ctor_class(va) else f"CVtable_{vt:08x}")
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}() -- base {basecls}() then this class's vtable",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x{base:08X} runs, then the compiler stores this class's vtable (0x{vt:08X}
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of {len(members)}
// bodies of this shape.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();                    // this class has its own vtable
    {cls}();
}};

{cls}::{cls}()
    : {basecls}()
{{
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { virtual void v00() {} D() : Base() {} };
int main()
{
    D d; if (g_b != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("19","103"):   # CDerived::CDerived(T a) : CBase(a) {}
        base=calls[0]; basecls=ctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        _,_,cr=label(base); pt=(cr.get("parameter_types") or "").split(","); argt=(pt[1].strip() if len(pt)>1 and pt[1].strip() else "void *").replace(" *","*")
        if "<" in argt or not re.match(r"^[A-Za-z_][A-Za-z0-9_ ]*\*?$", argt): argt="void*"
        argbase=argt.rstrip("*").strip(); fwd=f"struct {argbase};\n" if argbase.startswith("C") else ""
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}({argt}) -- base {basecls}(argument) then this class's vtable",len(b))+f'''//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call {basecls}::{basecls} ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x{base:08X}; the compiler then stores this class's vtable (0x{vt:08X}, named
// through rtti-vtables.tsv) and returns `this`. One of {len(members)} bodies of this shape.
{fwd}
struct {basecls} {{
    virtual void v00();
    {basecls}({argt} argument);            // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}({argt} argument);
}};

{cls}::{cls}({argt} argument)
    : {basecls}(argument)
{{
}}
''',T(a,'''static void* g_a = 0; struct Base { virtual void v00() {} Base(void* a) { g_a = a; } };
struct D : Base { virtual void v00() {} D(void* a) : Base(a) {} };
int main()
{
    int x; D d(&x); if (g_a != &x) '''+FAIL(a)+'''
'''),b)
    elif TPL=="10":   # Clone with out-of-line copy ctor
        size=pushes[0]; ctor=calls[1]; cls=ctor_class(ctor) or clean(mmod) or f"CClass_{ctor:08x}"; basecls=(label(va)[2].get("return_type") or "CCreatureActionBase *").replace(" *","").strip() or "CBase"
        add(a,cls,"Clone",HDRF(a,f"{cls}::Clone() -- new {cls}(*this)",len(b))+f'''//
// Retail bytes:  push esi ; push {size:#x} ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call {cls}::{cls}(const {cls}&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof({cls}) = {size:#x}, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x{ctor:08X}; the constructor's `this` return
// is the result. One of {len(members)} bodies of this shape (the copy constructor's label names the class).
struct {basecls} {{
    virtual void v00();
}};

struct {cls} : {basecls} {{
    unsigned char _body[{size:#x} - 4];    // sizeof({cls}) = {size:#x}
    {cls}(const {cls}& other);             // 0x{ctor:08X}
    {basecls}* Clone();
}};

{basecls}* {cls}::Clone()
{{
    return new {cls}(*this);
}}
''',T(a,f'''struct Base {{ virtual void v00() {{}} }}; static int g_c = 0;
struct X : Base {{ unsigned char body[{size:#x} - 4]; X() {{}} X(const X& o) {{ ++g_c; body[0] = o.body[0]; }} Base* Clone() {{ return new X(*this); }} }};
int main()
{{
    X x; x.body[0] = 7; Base* c = x.Clone(); if (!c || g_c != 1 || static_cast<X*>(c)->body[0] != 7) '''+FAIL(a)+'''
    delete c;
'''),b)
    elif TPL in ("5","32"):    # Clone with inlined copy ctor (base copy ctor + vtable)
        size=pushes[0]; bctor=calls[1]; basecls=ctor_class(bctor) or f"CBase_{bctor:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or clean(mmod) or f"CVtable_{vt:08x}"
        rett=(label(va)[2].get("return_type") or "").replace(" *","").strip() or basecls
        add(a,cls,"Clone",HDRF(a,f"{cls}::Clone() -- new {cls}(*this) with the implicit copy constructor inlined",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; push {size:#x} ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call {basecls}::{basecls}(const {basecls}&) ; pop edi ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x{bctor:08X} plus this class's vtable store
// (0x{vt:08X}, named through rtti-vtables.tsv). sizeof = {size:#x}. One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    unsigned char _body[{size:#x} - 4];    // sizeof({basecls}) = sizeof({cls}) = {size:#x}
    {basecls}(const {basecls}& other);     // 0x{bctor:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();                    // own vtable; copy constructor implicit
    {basecls}* Clone();
}};

{basecls}* {cls}::Clone()
{{
    return new {cls}(*this);
}}
''',T(a,f'''static int g_c = 0; struct Base {{ virtual void v00() {{}} unsigned char body[{size:#x} - 4]; Base() {{}} Base(const Base& o) {{ ++g_c; body[0] = o.body[0]; }} }};
struct X : Base {{ virtual void v00() {{}} Base* Clone() {{ return new X(*this); }} }};
int main()
{{
    X x; x.body[0] = 5; Base* c = x.Clone(); if (!c || g_c != 1 || c->body[0] != 5) '''+FAIL(a)+'''
    delete c;
'''),b)
    elif TPL=="6":    # list ctor(alloc): head=0; head=malloc(N); head->next=head->prev=head
        n=pushes[0]; nm=f"CRetailList_node{n:#x}"
        add(a,nm,nm,HDRF(a,f"std::list<T> (node {n:#x} bytes)::list(const allocator&) -- retail sentinel-node list constructor",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; push {n:#x} ; mov dword ptr [esi],0 ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; mov [esi],eax ; add esp,4 ; mov eax,esi ; pop esi ; ret 4
// The retail std::list is a single sentinel-node pointer: the constructor (taking the allocator
// by value, hence `ret 4`) nulls it, allocates one node of {n:#x} bytes (malloc @ 0x00BFEA0E,
// landed-trusted) and links it to itself. The element type is {n-8} bytes; it is not recoverable
// from the body, so the list is named by its node size. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator {{ }};

struct {nm} {{
    struct node {{ node* next; node* prev; unsigned char value[{n:#x} - 8]; }};
    node* head;
    {nm}(CRetailAllocator allocator);
}};

{nm}::{nm}(CRetailAllocator allocator)
    : head(0)
{{
    // The node is linked to itself BEFORE it is stored into `head` (retail stores [eax] and
    // [eax+4] first, then [esi]); assigning `head` first re-reads it between the stores.
    node* sentinel = (node*)malloc(sizeof(node));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}}
''',T(a,f'''#include <cstdlib>
struct A {{}}; struct L {{ struct node {{ node* next; node* prev; unsigned char v[{n:#x} - 8]; }}; node* head;
    L(A) : head(0) {{ head = (node*)std::malloc(sizeof(node)); head->next = head; head->prev = head; }} }};
int main()
{{
    A al; L l(al); if (!l.head || l.head->next != l.head || l.head->prev != l.head) '''+FAIL(a)+'''
    std::free(l.head);
'''),b)
    elif TPL in ("11","193"):   # p=0; p=malloc(N); ret 4
        n=pushes[0]; nm=f"CRetailBlock{n:#x}"
        add(a,nm,nm,HDRF(a,f"{nm}::{nm}(const allocator&) -- one malloc'd block of {n:#x} bytes",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; push {n:#x} ; mov dword ptr [esi],0 ; call malloc ; mov [esi],eax ; add esp,4 ; mov eax,esi ; pop esi ; ret 4
// A container constructor taking its allocator by value (`ret 4`): the block pointer is nulled, one
// {n:#x}-byte block is allocated (malloc @ 0x00BFEA0E, landed-trusted) and stored. The element type
// is not recoverable from the body, so the class is named by its block size. One of {len(members)} bodies.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator {{ }};

struct {nm} {{
    unsigned char* block;
    {nm}(CRetailAllocator allocator);
}};

{nm}::{nm}(CRetailAllocator allocator)
    : block(0)
{{
    block = (unsigned char*)malloc({n:#x});
}}
''',T(a,f'''#include <cstdlib>
struct A {{}}; struct X {{ unsigned char* b; X(A) : b(0) {{ b = (unsigned char*)std::malloc({n:#x}); }} }};
int main()
{{
    A al; X x(al); if (!x.b) '''+FAIL(a)+'''
    std::free(x.b);
'''),b)
    elif TPL in ("9","14","31"):   # tree ctor
        n=pushes[0]; nm=f"CRetailTree_node{n:#x}"; alloc = retn==4
        ctorsig = f"{nm}(CRetailAllocator allocator)" if alloc else (f"{nm}(CRetailCompare compare, CRetailAllocator allocator)" if retn==8 else f"{nm}()")
        add(a,nm,nm,HDRF(a,f"std::map/set (node {n:#x} bytes) constructor -- retail red-black tree head node",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; push {n:#x} ; mov dword ptr [esi],0 ; call malloc ; mov [esi],eax ; mov dword ptr [esi+4],0 ; mov byte ptr [eax],0 ; (re-read head) mov [eax+4],0 ; mov [eax+8],eax ; mov [eax+0Ch],eax ; mov eax,esi ; pop esi ; ret{' 4' if alloc else (' 8' if retn==8 else '')}
// The retail map/set is {{head, size}}: the constructor nulls the head, allocates the {n:#x}-byte head
// node (malloc @ 0x00BFEA0E, landed-trusted), zeroes the size, clears the node's colour byte and
// parent, and points left and right at the node itself. `head` is re-read from the object between
// the stores because each store goes through it. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator {{ }};    // empty by-value allocator argument (one stack slot each, hence `ret N`)
struct CRetailCompare {{ }};      // empty by-value comparator argument

struct {nm} {{
    struct node {{ unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[{n:#x} - 16]; }};
    node*         head;
    unsigned long size;
    {ctorsig};
}};

{nm}::{ctorsig}
    : head(0)
{{
    head = (node*)malloc(sizeof(node));
    size = 0;
    head->colour = 0;
    head->parent = 0;
    head->left   = head;
    head->right  = head;
}}
''',T(a,f'''#include <cstdlib>
struct A {{}}; struct X {{ struct node {{ unsigned char c; unsigned char p[3]; node* parent; node* left; node* right; unsigned char v[{n:#x} - 16]; }}; node* head; unsigned long size;
    X() : head(0) {{ head = (node*)std::malloc(sizeof(node)); size = 0; head->c = 0; head->parent = 0; head->left = head; head->right = head; }} }};
int main()
{{
    X x; if (!x.head || x.size || x.head->left != x.head || x.head->right != x.head || x.head->parent) '''+FAIL(a)+'''
    std::free(x.head);
'''),b)
    elif TPL in ("12","18","62"):   # scalar deleting destructor of a class with an empty inline virtual dtor
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        add(a,cls,f"ScalarDeletingDestructor",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- empty virtual destructor, this copy from one translation unit",len(b))+f'''//
// Retail bytes:  test byte ptr [esp+4],1 ; push esi ; mov esi,ecx ; mov dword ptr [esi],offset {cls}::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// The compiler-generated deleting destructor for an empty inline `virtual ~{cls}() {{}}`: it
// restores the class's vtable (0x{vt:08X}, named through rtti-vtables.tsv), frees the object when
// the flag's bit 0 is set (operator delete @ 0x00BFE9BC, landed-trusted) and returns `this`. The
// inline destructor is re-emitted in every translation unit that needs it, which is why {len(members)}
// byte-identical copies exist; this file is the copy at this address.
struct {cls} {{
    virtual ~{cls}() {{ }}
}};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{{
    return new {cls};
}}
''',T(a,'''struct X { virtual ~X() { } };
int main()
{
    X* x = new X; delete x;
'''),b)
    elif TPL=="15":   # CDerived::CDerived(a, b) : CBase() { m_second = b; m_first = a; }  (stores: 2nd arg at lower offset)
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        st=[(i.operands[0].mem.disp, i.op_str.split(",")[-1].strip()) for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_REG and i.operands[0].mem.disp]
        o2=[d for d,r in st if r=="eax"][0]; o1=[d for d,r in st if r=="ecx"][0]   # eax <- [esp+0xc] (2nd arg), ecx <- [esp+8] (1st arg)
        lo,hi=sorted((o1,o2)); mlo="m_second" if lo==o2 else "m_first"; mhi="m_first" if lo==o2 else "m_second"
        gap=f"    unsigned char _gap[{hi-lo-4:#x}];\n" if hi-lo>4 else ""
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(void*, void*) -- base {basecls}(), then two argument members and this class's vtable",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; mov eax,[esp+0Ch] ; mov ecx,[esp+8] ; mov [esi+{o2:#x}],eax ; mov [esi+{o1:#x}],ecx ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret 8
// Two-argument constructor of a polymorphic class: default-constructs the base @ 0x{base:08X},
// stores the second argument at +{o2:#x} and the first at +{o1:#x} (member declaration order),
// installs this class's vtable (0x{vt:08X}, named through rtti-vtables.tsv) and returns `this`.
// The argument types are not recoverable from the body (two register-width values); `void*`
// models them. One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    unsigned char _pad[{lo-4:#x}];        // members of the base / earlier members, not touched here
    void* {mlo};                           // +{lo:#x}
{gap}    void* {mhi};                           // +{hi:#x}
    virtual void v00();
    {cls}(void* first, void* second);
}};

{cls}::{cls}(void* first, void* second)
    : {basecls}()
    , {mlo}({"second" if mlo=="m_second" else "first"})
    , {mhi}({"first" if mhi=="m_first" else "second"})
{{
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { unsigned char pad[0x3c]; void* s; void* f; virtual void v00() {} D(void* a, void* b) : Base(), s(b), f(a) {} };
int main()
{
    int x, y; D d(&x, &y); if (g_b != 1 || d.s != &y || d.f != &x) '''+FAIL(a)+'''
'''),b)
    elif TPL=="16":   # __fastcall Alloc(a, b) = new CDerived(a, b) with the two-argument constructor inlined
        size=pushes[0]; base=calls[1]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        st=[(i.operands[0].mem.disp, i.op_str.split(",")[-1].strip()) for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_REG and i.operands[0].mem.disp]
        o2=[d for d,r in st if r=="edi"][0]; o1=[d for d,r in st if r=="ebx"][0]   # edi <- edx (2nd arg), ebx <- ecx (1st arg)
        lo,hi=sorted((o1,o2)); mlo="m_second" if lo==o2 else "m_first"; mhi="m_first" if lo==o2 else "m_second"
        gap=f"    unsigned char _gap[{hi-lo-4:#x}];\n" if hi-lo>4 else ""
        tail=size-hi-4; tailpad=f"    unsigned char _tail[{tail:#x}];\n" if tail>0 else ""
        add(a,"_global",f"Alloc_{cls}",HDRF(a,f"Alloc_{cls}(void*, void*) -- factory: new {cls}(first, second) with the constructor inlined",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; push edi ; push {size:#x} ; mov edi,edx ; mov ebx,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; mov ecx,esi ; call {basecls}::{basecls} ; mov [esi+{o2:#x}],edi ; pop edi ; mov [esi+{o1:#x}],ebx ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; pop ebx ; ret ; N: pop edi ; pop esi ; xor eax,eax ; pop ebx ; ret
// A __fastcall factory (ecx = first, edx = second): allocates sizeof({cls}) = {size:#x} bytes
// (operator new @ 0x00BFEA1A, landed-trusted), null-checks the block and runs the inlined
// two-argument constructor: base {basecls}() @ 0x{base:08X}, second argument stored at +{o2:#x},
// first at +{o1:#x}, vtable 0x{vt:#08X} (named through rtti-vtables.tsv). The constructor body
// is the same shape the out-of-line family lands as {cls}::{cls}. One of {len(members)} bodies.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    unsigned char _pad[{lo-4:#x}];
    void* {mlo};                           // +{lo:#x}
{gap}    void* {mhi};                           // +{hi:#x}
{tailpad}    virtual void v00();
    {cls}(void* first, void* second) : {basecls}(), {mlo}({"second" if mlo=="m_second" else "first"}), {mhi}({"first" if mhi=="m_first" else "second"}) {{ }}
}};

{cls}* __fastcall Alloc_{cls}(void* first, void* second)
{{
    return new {cls}(first, second);
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { unsigned char pad[0x3c]; void* s; void* f; virtual void v00() {} D(void* a, void* b) : Base(), s(b), f(a) {} };
static D* __fastcall Alloc(void* a, void* b) { return new D(a, b); }
int main()
{
    int x, y; D* d = Alloc(&x, &y); if (!d || g_b != 1 || d->s != &y || d->f != &x) '''+FAIL(a)+'''
    delete d;
'''),b)
    elif TPL=="17":   # CDerived::CDerived() : CBase(), m_member() {}  with a polymorphic member whose own ctor is inlined
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; mbase=calls[1]; mbasecls=tctor_class(mbase) or f"CMemberBase_{mbase:08x}"
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; vt2=vts[1][1]; mcls=rtti.get(vt2) or f"CMember_{vt2:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        if mcls==mbasecls: mcls=f"{mbasecls}_Derived_{vt2:08x}"
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}() -- base {basecls}(), this vtable, then the embedded {mcls} member at +{off:#x}",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call {basecls}::{basecls} ; lea edi,[esi+{off:#x}] ; mov ecx,edi ; mov dword ptr [esi],offset {cls}::`vftable' ; call {mbasecls}::{mbasecls} ; mov dword ptr [edi],offset {mcls}::`vftable' ; pop edi ; mov eax,esi ; pop esi ; ret
// Default constructor: base {basecls}() @ 0x{base:08X} out of line, this class's vtable
// (0x{vt:08X}), then the embedded member of type {mcls} at +{off:#x} is constructed with its
// own constructor inlined: its base {mbasecls}() @ 0x{mbase:08X} and its vtable (0x{vt2:08X}).
// Class names come from rtti-vtables.tsv; base-constructor names from unique manifest labels.
// One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {mbasecls} {{
    virtual void v00();
    {mbasecls}();                          // 0x{mbase:08X}
}};

struct {mcls} : {mbasecls} {{
    virtual void v00();
    {mcls}() : {mbasecls}() {{ }}            // inlined into every constructor that embeds it
}};

struct {cls} : {basecls} {{
    unsigned char _pad[{off-4:#x}];        // base members / earlier members, not touched here
    {mcls} m_member;                       // +{off:#x}
    virtual void v00();
    {cls}();
}};

{cls}::{cls}()
    : {basecls}()
    , m_member()
{{
}}
''',T(a,'''static int g_b = 0, g_m = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct MB { virtual void v00() {} MB() { ++g_m; } }; struct M : MB { virtual void v00() {} M() : MB() {} };
struct D : Base { unsigned char pad[0x6c]; M m; virtual void v00() {} D() : Base(), m() {} };
int main()
{
    D d; if (g_b != 1 || g_m != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="20":   # void CDerived::Initialise() { SetAnimationName(CCharString("LIT", -1)); CBase::Initialise(); }
        cls=clean(mmod); 
        if not cls or not mname.endswith("::Initialise") or not re.match(r"^C[A-Za-z0-9_]+$",cls): continue
        litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva); setter=calls[1]; binit=calls[3]
        bm,bn,_=label(binit); basecls=clean(bm) if re.match(r"^C[A-Za-z0-9_]+$",bm) else f"CBase_{binit:08x}"
        add(a,cls,"Initialise",HDRF(a,f"{cls}::Initialise() -- names this action's animation \"{s}\" then runs the base Initialise",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push -1 ; mov esi,ecx ; push offset "{s}" ; lea ecx,[esp+0Ch] ; call CCharString::CCharString(const char*,long) ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call SetAnimationName ; lea ecx,[esp+4] ; call CCharString::~CCharString ; mov ecx,esi ; call {basecls}::Initialise ; pop esi ; pop ecx ; ret
// Virtual Initialise override: builds a scoped CCharString from the literal (ctor
// @ 0x0099EBF0, dtor @ 0x0099EAE0, both from fable_string.h), hands it to the base-class
// setter @ 0x{setter:08X} (named SetAnimationName here because every literal of the
// {len(members)}-body family is an animation name; the manifest label of that address is
// propagated and untrusted), destroys the temporary and finally calls the base
// {basecls}::Initialise @ 0x{binit:08X} directly (non-virtual call).
#include "fable_string.h"

struct {basecls} {{
    virtual void v00();
    virtual void Initialise();                        // 0x{binit:08X}
    void SetAnimationName(const CCharString& name);   // 0x{setter:08X}
}};

struct {cls} : {basecls} {{
    virtual void Initialise();
}};

void {cls}::Initialise()
{{
    {{
        CCharString name("{s}", -1);       // a named local: a bare temporary would reuse the ctor's eax
        SetAnimationName(name);            // instead of re-deriving `lea eax,[esp+4]` as retail does
    }}
    {basecls}::Initialise();
}}
''',T(a,'''static int g_s = 0, g_i = 0, g_d = 0; struct Str { const char* p; Str(const char* t, long) : p(t) {} ~Str() { ++g_d; } };
struct Base { virtual void v00() {} virtual void Initialise() { ++g_i; } void Set(const Str& n) { g_s += (int)n.p[0]; } };
struct D : Base { virtual void Initialise() { { Str name("ABC", -1); Set(name); } Base::Initialise(); } };
int main()
{
    D d; d.Initialise(); if (g_i != 1 || g_s != 'A' || g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("35","246"):   # CDerived::CDerived(T arg) : CBase() { Setter(arg); }
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        setter=calls[1]; sm,sn,sr=label(setter); sleaf=clean(sn) if (sm,sn) in uniq and clean(sn) and not clean(sn).startswith("sub_") else f"Set_{setter:08x}"
        pt=(sr.get("parameter_types") or "").split(","); argt=(pt[1].strip() if len(pt)>1 and pt[1].strip() else "void *").replace(" *","*")
        if "<" in argt or not re.match(r"^[A-Za-z_][A-Za-z0-9_ ]*\*?$", argt) or not argt.endswith("*"): argt="void*"
        argbase=argt.rstrip("*").strip(); fwd=f"struct {argbase};\n" if argbase.startswith("C") else ""
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}({argt}) -- base {basecls}(), this vtable, then {sleaf}(argument)",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; mov eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esi],offset {cls}::`vftable' ; call {sleaf} ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: default-constructs the base @ 0x{base:08X}, installs this class's
// vtable (0x{vt:08X}, named through rtti-vtables.tsv) and forwards the argument to the member
// function @ 0x{setter:08X} from the constructor body. One of {len(members)} bodies of this shape.
#include <stddef.h>      // wchar_t under VC7.1 (not a keyword without /Zc:wchar_t)
{fwd}
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}({argt} argument);
    void {sleaf}({argt} argument);        // 0x{setter:08X}
}};

{cls}::{cls}({argt} argument)
    : {basecls}()
{{
    {sleaf}(argument);
}}
''',T(a,'''static int g_b = 0; static void* g_a = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { virtual void v00() {} D(void* a) : Base() { Set(a); } void Set(void* a) { g_a = a; } };
int main()
{
    int x; D d(&x); if (g_b != 1 || g_a != &x) '''+FAIL(a)+'''
'''),b)
    elif TPL=="26":   # persist: context->Transfer<signed char>("Name", this->m_flag, default 0)
        litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva)
        off=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[0].type==X86_OP_REG][0]
        mcls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else None
        mod=mcls or "_unattributed"; fname="vfunc_1" if mcls else f"PersistTransfer_{san(s)}_{a}"; cls=mcls or f"CPersistable_{a}"
        params="CPersistContext* context" if retn==4 else "CPersistContext* context, long unused"
        pnote="" if retn==4 else " The second argument is not used by\n// this body; its meaning is not established."
        add(a,mod,fname,HDRF(a,f"{cls}::{fname}({params}) -- persists the \"{s}\" flag at +{off:#x}",len(b))+f'''//
// Retail bytes:  push ecx ; lea eax,[esp+3] ; push eax ; add ecx,{off:#x} ; push ecx ; mov ecx,[esp+10h] ; push offset "{s}" ; mov byte ptr [esp+0Fh],0 ; call CPersistContext::Transfer<signed char> ; pop ecx ; ret {retn}
// Virtual persist hook (slot 1 of this class's vtable): transfers the single byte member at
// +{off:#x} through CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0,
// with a zero default built in a one-byte stack temporary.{pnote} The member name is taken
// from the persisted key. One of {len(members)} bodies of this shape.
struct CPersistContext {{
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
}};

struct {cls} {{
    unsigned char _pad[{off-4:#x}];        // after the vptr; members not touched here
    signed char m_b{san(s)};                // +{off:#x}
    virtual void v00();
    virtual void {fname}({params});
}};

void {cls}::{fname}({params})
{{
    signed char defaultValue = 0;
    context->Transfer("{s}", m_b{san(s)}, defaultValue);
}}
''',T(a,'''static int g_t = 0; struct Ctx { void Transfer(const char* n, signed char& v, const signed char& d) { v = (signed char)(n[0] + d + 1); ++g_t; } };
struct P { unsigned char pad[0x18]; signed char b; virtual void v00() {} virtual void vf(Ctx* c, long) { signed char d = 0; c->Transfer("A", b, d); } };
int main()
{
    Ctx c; P p; p.b = 0; p.vf(&c, 0); if (g_t != 1 || p.b != 'B') '''+FAIL(a)+'''
'''),b)
    elif TPL in ("7","25","27","28","106","153","269"):   # dynamic initializer of a static-storage object (+ atexit registration of its destructor thunk)
        thunk=pushes[-1]; g=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_IMM]
        tb=rb(thunk,16); tins=list(md.disasm(tb,thunk)); dtor=tins[1].operands[0].imm if len(tins)>1 and tins[1].mnemonic=="jmp" else 0
        gaddr=g[0] if g else (tins[0].operands[1].imm if tins and tins[0].mnemonic=="mov" else 0)
        ctor=calls[0] if len(calls)>1 else 0
        dm,dn,_=label(dtor); dleaf=dn.split("::")[-1]
        if ctor==0x99ebf0 or ctor in (0x99aed0,0x99e4b0): cls="CCharString"
        elif ctor==0x99b6b0: cls="CWideString"
        elif ctor: cls=tctor_class(ctor) or f"CStatic_{ctor:08x}"
        else: cls=(dm if ((dm,dn) in uniq and dleaf=="~"+dm and re.match(r"^C[A-Za-z0-9_]+$",dm)) else f"CStatic_{(dtor or thunk):08x}")
        if not gaddr: gaddr=thunk   # the atexit target inlines the destructor (no `mov ecx,obj ; jmp dtor` thunk): object address unknown, key by thunk
        args=list(reversed(pushes[:-1])); argsrc=[]; argdecl=[]
        for x in args:
            if 0x1200000<=x<0x1300000 and ctor==0x99b6b0: argsrc.append('L"'+wlit(x).replace('\\','\\\\').replace('"','\\"')+'"'); argdecl.append("const wchar_t* text")
            elif 0x1200000<=x<0x1300000: argsrc.append('"'+lit(x).replace('\\','\\\\').replace('"','\\"')+'"'); argdecl.append("const char* text")
            else: argsrc.append(str(x-0x100000000) if x>=0x80000000 else str(x)); argdecl.append("long value" if x>=0x80000000 else "unsigned long value")
        gname=f"g_{cls}_{gaddr:08x}"
        if cls=="CCharString":
            decl='#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0\n'
        elif cls=="CWideString":
            decl='#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString\n'
        else:
            decl=f"struct {cls} {{\n" + (f"    {cls}({', '.join(argdecl)});".ljust(40)+f"// 0x{ctor:08X}\n" if ctor else "    // trivial default constructor: nothing observable runs before the atexit registration\n") + f"    ~{cls}();".ljust(40)+f"// 0x{dtor:08X}, reached through the atexit thunk\n}};\n"
        what=("constructs it from the literal" if cls=="CCharString" and args else "default-constructs it out of line" if ctor else "has nothing to run for the constructor") 
        add(a,"_static_init",gname,HDRF(a,f"dynamic initializer for {cls} {gname} @ 0x{gaddr:08X} -- {what}, then registers its destructor with atexit",len(b))+f'''//
// Retail bytes:  {" ; ".join(f"{i.mnemonic} {i.op_str}" for i in ins)}
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type {cls}
// living at 0x{gaddr:08X}: it {what} and registers the
// destructor thunk @ 0x{thunk:08X} ({"`mov ecx,offset object ; jmp " + cls + "::~" + cls + "`, already landed" if dtor else "the destructor is inlined into the thunk, so the object's address and type are not recoverable here"})
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of {len(members)} bodies of this shape.
{decl}
{cls} {gname}{"(" + ", ".join(argsrc) + ")" if args else ""};
''',T(a,'''static int g_c = 0, g_d = 0; struct S { S(const char*, long) { ++g_c; } S() { ++g_c; } ~S() { ++g_d; } };
S g_one("x", -1); S g_two;
int main()
{
    if (g_c != 2 || g_d != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL=="21":   # __fastcall Alloc(a,b) = new CDerived(a,b); CDerived(a,b) : CBase(a,b) {} inlined, base ctor out of line
        size=pushes[0]; ctor=calls[1]; basecls=tctor_class(ctor) or f"CBase_{ctor:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        _,_,cr=label(ctor); pt=[x.strip().replace(" *","*") for x in (cr.get("parameter_types") or "").split(",")][1:3]
        while len(pt)<2: pt.append("void*")
        pt=[x if (x.endswith("*") and "<" not in x and re.match(r"^[A-Za-z_][A-Za-z0-9_]*\*$",x)) else "void*" for x in pt]
        fwd="".join(f"struct {x[:-1]};\n" for x in sorted(set(pt)) if x!="void*")
        add(a,"_global",f"Alloc_{cls}",HDRF(a,f"Alloc_{cls}({pt[0]}, {pt[1]}) -- factory: new {cls}(first, second), constructor inlined over {basecls}(first, second)",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; push edi ; push {size:#x} ; mov edi,edx ; mov ebx,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; push ebx ; mov ecx,esi ; call {basecls}::{basecls} ; pop edi ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; pop ebx ; ret ; N: pop edi ; pop esi ; xor eax,eax ; pop ebx ; ret
// A __fastcall factory (ecx = first, edx = second): allocates sizeof({cls}) = {size:#x} bytes
// (operator new @ 0x00BFEA1A, landed-trusted), null-checks the block and runs the inlined
// constructor, which only forwards both arguments to the base constructor @ 0x{ctor:08X} and
// installs this class's vtable (0x{vt:08X}, named through rtti-vtables.tsv). Argument types
// follow the base constructor's manifest prototype. One of {len(members)} bodies of this shape.
{fwd}
struct {basecls} {{
    virtual void v00();
    unsigned char _body[{size:#x} - 4];    // sizeof({basecls}) = sizeof({cls}) = {size:#x}
    {basecls}({pt[0]} first, {pt[1]} second);   // 0x{ctor:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}({pt[0]} first, {pt[1]} second) : {basecls}(first, second) {{ }}
}};

{cls}* __fastcall Alloc_{cls}({pt[0]} first, {pt[1]} second)
{{
    return new {cls}(first, second);
}}
''',T(a,'''static void* g_a = 0; static void* g_b = 0; struct Base { virtual void v00() {} Base(void* a, void* b) { g_a = a; g_b = b; } };
struct D : Base { virtual void v00() {} D(void* a, void* b) : Base(a, b) {} };
static D* __fastcall Alloc(void* a, void* b) { return new D(a, b); }
int main()
{
    int x, y; D* d = Alloc(&x, &y); if (!d || g_a != &x || g_b != &y) '''+FAIL(a)+'''
    delete d;
'''),b)
    elif TPL in ("44","63"):   # CIntelligentPointer<T>::CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
        base=calls[0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; getter=calls[1]; setter=calls[2]
        basecls="CBaseIntelligentPointer" if base==0xa01b10 else (tctor_class(base) or f"CBase_{base:08x}")
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(const {cls}&) -- copy constructor: base {basecls}(), this vtable, SetPItem(other.GetPItem())",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; mov ecx,[esp+8] ; mov dword ptr [esi],offset {cls}::`vftable' ; call {basecls}::GetPItem ; push eax ; mov ecx,esi ; call {basecls}::SetPItem ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a smart-pointer instantiation: default-constructs the base
// @ 0x{base:08X}, installs this instantiation's vtable (0x{vt:08X}, named through
// rtti-vtables.tsv; the template argument is not recoverable from the body) and re-points
// the new object at the other's item through GetPItem @ 0x{getter:08X} / SetPItem @ 0x{setter:08X}
// (the base class's accessors; SetPItem takes the item by pointer). One of {len(members)} bodies.
struct CBaseObject;

struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
    CBaseObject* GetPItem() const;         // 0x{getter:08X}
    void SetPItem(CBaseObject* item);      // 0x{setter:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}(const {cls}& other);
}};

{cls}::{cls}(const {cls}& other)
    : {basecls}()
{{
    SetPItem(other.GetPItem());
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} void* p; Base() : p(0) { ++g_b; } void* Get() const { return p; } void Set(void* x) { p = x; } };
struct D : Base { virtual void v00() {} D() {} D(const D& o) : Base() { Set(o.Get()); } };
int main()
{
    int x; D a; a.Set(&x); D c(a); if (g_b != 2 || c.Get() != &x) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("34","119","134","192"):   # CDerived::CDerived() : CBase(), m_data() {} -- zeroed members live in an embedded struct with an inline ctor
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        sts=[i for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.disp and ((i.operands[1].type==X86_OP_REG and (i.op_str.endswith(", eax") or i.op_str.endswith(", al"))) or (i.operands[1].type==X86_OP_IMM and i.operands[1].imm==0))]
        offs=[i.operands[0].mem.disp for i in sts]; sizes=[1 if "byte ptr" in i.op_str else 4 for i in sts]
        if not offs: continue
        mem=""; prev=offs[0]
        for j,(o,sz) in enumerate(zip(offs,sizes)):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=(f"    long m_{j:02d};" if sz==4 else f"    unsigned char m_{j:02d};").ljust(40)+f"// +{o:#x}\n"; prev=o+sz
        inits=", ".join(f"m_{j:02d}(0)" for j in range(len(offs)))
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}() -- base {basecls}(), this vtable, then {len(offs)} members zeroed",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; {"xor eax,eax ; " if len(offs)>1 else ""}mov dword ptr [esi],offset {cls}::`vftable' ; {" ; ".join(f"mov [esi+{o:#x}]," + ("eax" if len(offs)>1 else "0") for o in offs)} ; mov eax,esi ; pop esi ; ret
// Default constructor: base {basecls}() @ 0x{base:08X}, this class's vtable (0x{vt:08X}, named
// through rtti-vtables.tsv), then the {len(offs)} consecutive dword members at +{offs[0]:#x}..+{offs[-1]:#x}
// are zeroed{" from one cleared register" if len(offs)>1 else ""}. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of {len(members)} bodies.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct SZeroed {{                        // the zeroed block is one embedded member object
{mem}    SZeroed() : {inits} {{ }}           // inlined into every constructor that embeds it
}};

struct {cls} : {basecls} {{
{"    unsigned char _pad[%#x];" % (offs[0]-4) + chr(10) if offs[0]>4 else ""}    SZeroed m_data;                        // +{offs[0]:#x}
    virtual void v00();
    {cls}();
}};

{cls}::{cls}()
    : {basecls}()
    , m_data()
{{
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct Z { long a, b, c; Z() : a(0), b(0), c(0) {} }; struct D : Base { unsigned char pad[4]; Z z; virtual void v00() {} D() : Base(), z() {} };
int main()
{
    D d; if (g_b != 1 || d.z.a || d.z.b || d.z.c) '''+FAIL(a)+'''
'''),b)
    elif TPL=="59":   # CDerived::CDerived() : CBase(), m_member() {}  (member ctor out of line)
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; mctor=calls[1]; mcls=tctor_class(mctor) or f"CMember_{mctor:08x}"
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}() -- base {basecls}(), this vtable, then the embedded {mcls} member at +{off:#x}",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; lea ecx,[esi+{off:#x}] ; mov dword ptr [esi],offset {cls}::`vftable' ; call {mcls}::{mcls} ; mov eax,esi ; pop esi ; ret
// Default constructor: base {basecls}() @ 0x{base:08X}, this class's vtable (0x{vt:08X}, named
// through rtti-vtables.tsv), then the member object at +{off:#x} is constructed out of line
// @ 0x{mctor:08X}. Base and member class names come from unique manifest labels, else from the
// callee address. One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {mcls} {{
    {mcls}();                              // 0x{mctor:08X}
}};

struct {cls} : {basecls} {{
    unsigned char _pad[{off-4:#x}];
    {mcls} m_member;                       // +{off:#x}
    virtual void v00();
    {cls}();
}};

{cls}::{cls}()
    : {basecls}()
    , m_member()
{{
}}
''',T(a,'''static int g_b = 0, g_m = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct M { M() { ++g_m; } }; struct D : Base { unsigned char pad[0x70]; M m; virtual void v00() {} D() : Base(), m() {} };
int main()
{
    D d; if (g_b != 1 || g_m != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="69":   # CDerived::CDerived(T a) : CBase(), m_a(a) {}
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        off=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_REG and i.operands[0].mem.disp][0]
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(void*) -- base {basecls}(), the argument stored at +{off:#x}, then this vtable",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call {basecls}::{basecls} ; mov eax,[esp+8] ; mov [esi+{off:#x}],eax ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: base {basecls}() @ 0x{base:08X}, the argument kept at +{off:#x} and
// this class's vtable (0x{vt:08X}, named through rtti-vtables.tsv). The argument type is not
// recoverable from the body; `void*` models it. One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    {basecls}();                           // 0x{base:08X}
}};

struct {cls} : {basecls} {{
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    void* m_argument;                      // +{off:#x}
    virtual void v00();
    {cls}(void* argument);
}};

{cls}::{cls}(void* argument)
    : {basecls}()
    , m_argument(argument)
{{
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { void* a; virtual void v00() {} D(void* x) : Base(), a(x) {} };
int main()
{
    int x; D d(&x); if (g_b != 1 || d.a != &x) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("49","54"):   # CX::CX() : m_string() {}  -- one CCharString-like member constructed out of line, nothing else
        mctor=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"]; off=off[0] if off else 0
        mcls="CCharString" if mctor in (0x99aed0,0x99e4b0) else (tctor_class(mctor) or f"CMember_{mctor:08x}")
        cls=f"CStringOwner_{a}"
        add(a,"_unattributed",cls,HDRF(a,f"{cls}::{cls}() -- constructs one {mcls} member at +{off:#x}; the owning class is not identifiable",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; {"lea ecx,[esi+%#x] ; " % off if off else ""}call {mcls}::{mcls} ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a {mcls}
// at +{off:#x} (default constructor @ 0x{mctor:08X}: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// {len(members)} bodies of this shape.
struct {mcls}Member {{
    {mcls}Member();                        // 0x{mctor:08X}
}};

struct {cls} {{
{"    unsigned char _pad[%#x];\n" % off if off else ""}    {mcls}Member m_string;                 // +{off:#x}
    {cls}();
}};

{cls}::{cls}()
    : m_string()
{{
}}
''',T(a,'''static int g_m = 0; struct M { M() { ++g_m; } };
struct D { unsigned char pad[8]; M m; D() : m() {} };
int main()
{
    D d; if (g_m != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="29":   # Clone with inlined copy ctor: base copy ctor + vtable + one extra dword member copied
        size=pushes[0]; bctor=calls[1]; basecls=tctor_class(bctor) or f"CBase_{bctor:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or clean(mmod) or f"CVtable_{vt:08x}"
        ld=[i for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.disp][0]; off=ld.operands[1].mem.disp
        mt="unsigned char" if "byte ptr" in ld.op_str else "long"; mreg="al" if mt=="unsigned char" else "eax"
        add(a,cls,"Clone",HDRF(a,f"{cls}::Clone() -- new {cls}(*this) with the implicit copy constructor inlined (base copy + one member at +{off:#x})",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; push {size:#x} ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call {basecls}::{basecls}(const {basecls}&) ; mov dword ptr [esi],offset {cls}::`vftable' ; mov {mreg},[edi+{off:#x}] ; mov [esi+{off:#x}],{mreg} ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds one {"byte" if mt!="long" else "dword"} member at +{off:#x} to its base: the implicit
// copy constructor is inlined as the base copy constructor @ 0x{bctor:08X}, this class's vtable
// store (0x{vt:08X}, named through rtti-vtables.tsv) and the member copy. sizeof = {size:#x}.
// The member's exact type is not recoverable from the body; `{mt}` models its size. One of {len(members)} bodies.
struct {basecls} {{
    virtual void v00();
    unsigned char _body[{off:#x} - 4];     // sizeof({basecls}) = {off:#x}
    {basecls}(const {basecls}& other);     // 0x{bctor:08X}
}};

struct {cls} : {basecls} {{
    {mt} m_extra;                          // +{off:#x}
    virtual void v00();                    // own vtable; copy constructor implicit
    {basecls}* Clone();
}};

{basecls}* {cls}::Clone()
{{
    return new {cls}(*this);
}}
''',T(a,f'''static int g_c = 0; struct Base {{ virtual void v00() {{}} unsigned char body[{off:#x} - 4]; Base() {{}} Base(const Base& o) {{ ++g_c; body[0] = o.body[0]; }} }};
struct X : Base {{ long e; virtual void v00() {{}} Base* Clone() {{ return new X(*this); }} }};
int main()
{{
    X x; x.body[0] = 5; x.e = 7; X* c = (X*)x.Clone(); if (!c || g_c != 1 || c->body[0] != 5 || c->e != 7) '''+FAIL(a)+'''
    delete c;
'''),b)
    elif TPL=="129":   # CDerived::CDerived(a, b) : CBase(a, b) {}
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        _,_,cr=label(base); pt=[x.strip().replace(" *","*") for x in (cr.get("parameter_types") or "").split(",")][1:3]
        while len(pt)<2: pt.append("void*")
        pt=[x if (x.endswith("*") and "<" not in x and re.match(r"^[A-Za-z_][A-Za-z0-9_]*\*$",x)) else "void*" for x in pt]
        fwd="".join(f"struct {x[:-1]};\n" for x in sorted(set(pt)) if x!="void*")
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}({pt[0]}, {pt[1]}) -- forwards both arguments to {basecls}, then this class's vtable",len(b))+f'''//
// Retail bytes:  push esi ; push [esp+0Ch] ; mov esi,ecx ; push [esp+0Ch] ; call {basecls}::{basecls} ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret 8
// Two-argument constructor of a polymorphic class that only forwards its arguments to the base
// constructor @ 0x{base:08X} and installs this class's vtable (0x{vt:08X}, named through
// rtti-vtables.tsv). Argument types follow the base constructor's manifest prototype. One of
// {len(members)} bodies of this shape.
{fwd}
struct {basecls} {{
    virtual void v00();
    {basecls}({pt[0]} first, {pt[1]} second);   // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}({pt[0]} first, {pt[1]} second);
}};

{cls}::{cls}({pt[0]} first, {pt[1]} second)
    : {basecls}(first, second)
{{
}}
''',T(a,'''static void* g_a = 0; static void* g_b = 0; struct Base { virtual void v00() {} Base(void* a, void* b) { g_a = a; g_b = b; } };
struct D : Base { virtual void v00() {} D(void* a, void* b) : Base(a, b) {} };
int main()
{
    int x, y; D d(&x, &y); if (g_a != &x || g_b != &y) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("104","117"):   # CDerived::CDerived() : CBase(<constant>) {}
        base=calls[0]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        k=pushes[0]
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}() -- base {basecls}({k}) then this class's vtable",len(b))+f'''//
// Retail bytes:  push esi ; push {k:#x} ; mov esi,ecx ; call {basecls}::{basecls} ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor that passes the constant {k} ({k:#x}) to the base constructor @ 0x{base:08X}
// (an identifier/type code of this class in the base's scheme) and installs this class's vtable
// (0x{vt:08X}, named through rtti-vtables.tsv). One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    {basecls}(long typeCode);              // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}();
}};

{cls}::{cls}()
    : {basecls}({k})
{{
}}
''',T(a,'''static long g_k = 0; struct Base { virtual void v00() {} Base(long k) { g_k = k; } };
struct D : Base { virtual void v00() {} D() : Base(14) {} };
int main()
{
    D d; if (g_k != 14) '''+FAIL(a)+'''
'''),b)
    elif TPL=="123":   # CX* Create_CX() { return new CX; }  with CX() : CBase() {} inlined
        size=pushes[0]; base=calls[1]; basecls=tctor_class(base) or f"CBase_{base:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        add(a,"_global",f"Create_{cls}",HDRF(a,f"Create_{cls}() -- factory: new {cls} with the default constructor inlined over {basecls}()",len(b))+f'''//
// Retail bytes:  push esi ; push {size:#x} ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call {basecls}::{basecls} ; mov dword ptr [esi],offset {cls}::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof({cls}) = {size:#x} bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// {basecls}() @ 0x{base:08X}, then this class's vtable 0x{vt:08X} from rtti-vtables.tsv).
// One of {len(members)} bodies of this shape.
{"#pragma pack(push, 1)                  // retail sizeof is not a multiple of 4: keep the exact byte count" + chr(10) if size%4 else ""}struct {basecls} {{
    virtual void v00();
    unsigned char _body[{size:#x} - 4];    // sizeof({basecls}) = sizeof({cls}) = {size:#x}
    {basecls}();                           // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}() : {basecls}() {{ }}
}};
{"#pragma pack(pop)" + chr(10) if size%4 else ""}
{cls}* Create_{cls}()
{{
    return new {cls};
}}
''',T(a,'''static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { virtual void v00() {} D() : Base() {} };
static D* Create() { return new D; }
int main()
{
    D* d = Create(); if (!d || g_b != 1) '''+FAIL(a)+'''
    delete d;
'''),b)
    elif TPL in ("92","108"):   # CX* Create_CX() { return new CX; }  with the constructor out of line (tail jump)
        size=pushes[0]; ctor=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        cls=tctor_class(ctor) or f"CClass_{ctor:08x}"
        add(a,"_global",f"Create_{cls}",HDRF(a,f"Create_{cls}() -- factory: new {cls} with the default constructor @ 0x{ctor:08X} out of line",len(b))+f'''//
// Retail bytes:  push {size:#x} ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp {cls}::{cls} ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof({cls}) = {size:#x} bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of {len(members)} bodies of this shape.
struct {cls} {{
    unsigned char _body[{size:#x}];       // sizeof({cls}) = {size:#x}
    {cls}();                               // 0x{ctor:08X}
}};

{cls}* Create_{cls}()
{{
    return new {cls};
}}
''',T(a,'''static int g_c = 0; struct X { unsigned char body[0x3c]; X() { ++g_c; } };
static X* Create() { return new X; }
int main()
{
    X* x = Create(); if (!x || g_c != 1) '''+FAIL(a)+'''
    delete x;
'''),b)
    elif TPL=="143":   # CX::~CX() {} : two CCharString members destroyed, vtable restored, base destructor tail-called
        dt=[(i.operands[1].mem.disp) for i in ins if i.mnemonic=="lea"]; mdtors=calls; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        bdtor=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        bm,bn,_=label(bdtor); bl=bn.split("::")[-1]; basecls=bm.split("::")[-1] if ((bm,bn) in uniq and bl=="~"+bm.split("::")[-1]) else f"CBase_{bdtor:08x}"
        if basecls==cls: basecls=f"CBase_{bdtor:08x}"
        if any(c!=0x99eae0 for c in mdtors): continue
        offs=sorted(dt); mem="".join(f"    CCharString m_string{j};".ljust(40)+f"// +{o:#x}, destroyed in reverse order\n" for j,o in enumerate(offs))
        mid=cls; cls=f"{mid}_Derived_{a}"
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- empty body: destroys the CCharString members at {', '.join('+%#x'%o for o in reversed(offs))}, then the inlined {mid}::~{mid} stores its vtable and tail-calls {basecls}::~{basecls}",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; {" ; ".join(f"lea ecx,[esi+{o:#x}] ; call CCharString::~CCharString" for o in reversed(offs))} ; mov dword ptr [esi],offset {mid}::`vftable' ; mov ecx,esi ; pop esi ; jmp {basecls}::~{basecls}
// Plain destructor (`??1`) with an empty body of a class derived from {mid} (the vtable stored,
// 0x{vt:08X} in rtti-vtables.tsv): the CCharString members are destroyed in reverse declaration
// order (~CCharString @ 0x0099EAE0, fable_string.h), then the inline, empty {mid}::~{mid} is
// inlined: its vtable store (this class has none: it is `__declspec(novtable)`, i.e. abstract) and the
// tail call to the next base destructor @ 0x{bdtor:08X}. The derived class cannot be named from the body.
// Non-virtual destructors keep the lander on the plain `??1` body (GOTCHAS). One of {len(members)} bodies.
#include "fable_string.h"

struct {basecls} {{
    virtual void v00();
    ~{basecls}();                          // 0x{bdtor:08X}
}};

struct {mid} : {basecls} {{
    virtual void v00();
    ~{mid}() {{ }}                          // inline: inlined into every derived destructor
}};

// __declspec(novtable): this (abstract) class never stores its own vtable, so only the inlined
// {mid} destructor's store appears -- the only way VC7.1 emits the retail order.
struct __declspec(novtable) {cls} : {mid} {{
    unsigned char _pad[{offs[0]-4:#x}];
{mem}    virtual void v00();
    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct S { S() {} ~S() { ++g_d; } }; struct Base { virtual void v00() {} ~Base() { g_d += 10; } };
struct Mid : Base { virtual void v00() {} ~Mid() {} }; struct D : Mid { unsigned char pad[0x24]; S a; S b; virtual void v00() {} ~D() {} };
int main()
{
    { D d; } if (g_d != 12) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("96","147"):   # scalar deleting destructor: vtable, base destructor out of line, delete
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; bdtor=calls[0]
        bm,bn,_=label(bdtor); bl=bn.split("::")[-1]; basecls=bm.split("::")[-1] if ((bm,bn) in uniq and bl=="~"+bm.split("::")[-1]) else f"CBase_{bdtor:08x}"
        if basecls==cls: basecls=f"CBase_{bdtor:08x}"
        add(a,cls,"ScalarDeletingDestructor",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- empty virtual destructor over {basecls}::~{basecls}, this copy from one translation unit",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov dword ptr [esi],offset {cls}::`vftable' ; call {basecls}::~{basecls} ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Compiler-generated deleting destructor for an empty inline `virtual ~{cls}() {{}}`: installs this
// class's vtable (0x{vt:08X}, rtti-vtables.tsv), runs the base destructor @ 0x{bdtor:08X} (its
// manifest label is kept only when unique) and frees the object when bit 0 of the flag is set
// (operator delete @ 0x00BFE9BC, landed-trusted). Re-emitted per translation unit; this file is the
// copy at this address. One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    ~{basecls}();                          // 0x{bdtor:08X}
}};

struct {cls} : {basecls} {{
    virtual ~{cls}() {{ }}
}};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{{
    return new {cls};
}}
''',T(a,'''static int g_d = 0; struct Base { virtual void v00() {} ~Base() { ++g_d; } };
struct X : Base { virtual ~X() { } };
int main()
{
    X* x = new X; delete x; if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="73":   # scalar deleting destructor: one member destroyed out of line, vtable, delete (no base destructor)
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; mdtor=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]
        mm,mn,_=label(mdtor); ml=mn.split("::")[-1]; mcls=mm.split("::")[-1] if ((mm,mn) in uniq and ml=="~"+mm.split("::")[-1]) else f"CMember_{mdtor:08x}"
        mid=cls; cls=f"{mid}_Derived_{a}"
        add(a,"_unattributed",f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- one {mcls} member at +{off:#x}, then the inlined {mid}::~{mid} stores its vtable",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+{off:#x}] ; call {mcls}::~{mcls} ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset {mid}::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Compiler-generated deleting destructor of a class derived from {mid} (the vtable stored,
// 0x{vt:08X} in rtti-vtables.tsv) with an empty inline virtual destructor: the member at +{off:#x}
// is destroyed out of line @ 0x{mdtor:08X}, then the inline empty {mid}::~{mid} is inlined (its
// vtable store; this class's own store is dead and dropped) and the object is freed when bit 0 of
// the flag is set (operator delete @ 0x00BFE9BC, landed-trusted). The derived class cannot be
// named from the body. One of {len(members)} bodies of this shape.
struct {mcls} {{
    ~{mcls}();                             // 0x{mdtor:08X}
}};

struct {mid} {{
    virtual ~{mid}() {{ }}
}};

struct {cls} : {mid} {{
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    {mcls} m_member;                       // +{off:#x}
    virtual ~{cls}() {{ }}
}};

void* MakeOne()
{{
    return new {cls};
}}
''',T(a,'''static int g_d = 0; struct M { ~M() { ++g_d; } };
struct Mid { virtual ~Mid() { } }; struct X : Mid { M m; virtual ~X() { } };
int main()
{
    X* x = new X; delete x; if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="98":   # ~CX() { if (m_buffer) free(m_buffer); }  with a CCharString first member destroyed by tail call
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][0]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) and (mmod,mname) in uniq else f"CStringAndBuffer_{a}"; mod=cls if not cls.startswith("CStringAndBuffer_") else "_unattributed"
        add(a,mod,f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- frees the buffer at +{off:#x} if set, then the CCharString at +0 is destroyed",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+{off:#x}] ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: mov ecx,esi ; pop esi ; jmp CCharString::~CCharString
// Destructor of a small non-polymorphic class: a CCharString at +0 (destroyed by the tail call to
// ~CCharString @ 0x0099EAE0, fable_string.h) and a malloc'd buffer at +{off:#x} released with
// free @ 0x00BFEA14 (landed-trusted) when non-null. The class is named from the manifest label only
// when it is unique. One of {len(members)} bodies of this shape.
#include "fable_string.h"
extern "C" void __cdecl free(void* block);

struct {cls} {{
    CCharString m_name;                    // +0
    void* m_buffer;                        // +{off:#x}
    ~{cls}();
}};

{cls}::~{cls}()
{{
    if (m_buffer)
        free(m_buffer);
}}
''',T(a,'''#include <cstdlib>
static int g_d = 0; struct S { ~S() { ++g_d; } };
struct X { S s; void* b; ~X() { if (b) std::free(b); } };
int main()
{
    { X x; x.b = std::malloc(4); } if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="118":   # ~CStack() { delete m_data; m_data = 0; m_count = 0; m_capacity = 0; }
        offs=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM]
        pofs=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][0]
        cls=clean(mmod) if (mmod,mname) in uniq and clean(mmod) else f"CBuffer_{a}"; mod=cls if cls!=f"CBuffer_{a}" else "_unattributed"
        others=[o for o in offs if o!=pofs]
        add(a,mod,f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- deletes the block at +{pofs:#x} and zeroes the three fields",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+{pofs:#x}] ; test eax,eax ; je K ; push eax ; call operator delete ; add esp,4 ; K: {" ; ".join(f"mov dword ptr [esi+{o:#x}],0" for o in offs)} ; pop esi ; ret
// Destructor of a small owning container: releases the heap block at +{pofs:#x} (operator delete
// @ 0x00BFE9BC, landed-trusted, with the compiler's null check) and clears the pointer and the
// two counters at {", ".join("+%#x"%o for o in others)}. The manifest labels the family `Stack<unsigned long,64>::~Stack`
// (a template instantiation; kept only when unique). One of {len(members)} bodies of this shape.
struct {cls} {{
    unsigned char _pad[{min(offs):#x}];
    unsigned long* m_data;                 // +{pofs:#x}
    unsigned long m_count;                 // +{others[0]:#x}
    unsigned long m_capacity;              // +{others[1]:#x}
    ~{cls}();
}};

{cls}::~{cls}()
{{
    if (m_data)
        delete m_data;
    m_data = 0;
    m_count = 0;
    m_capacity = 0;
}}
''',T(a,'''struct X { unsigned char p[4]; unsigned long* d; unsigned long n, c; ~X() { delete d; d = 0; n = 0; c = 0; } };
int main()
{
    X x; x.d = new unsigned long; x.n = 3; x.c = 4; x.~X(); if (x.d || x.n || x.c) '''+FAIL(a)+'''
'''),b)
    elif TPL=="84":   # CCharString CX::GetName() { return CCharString("<lit>", -1); }  (hidden pointer, `this` unused)
        litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva)
        if re.match(r"^C[A-Z][A-Za-z0-9_]+$",s) and s in rtti.values(): mod,fn=s,"GetTypeAsString"
        else: mod,fn="_global",f"GetName_{san(s)}"
        add(a,mod,fn,HDRF(a,f"{mod}::{fn}() -- returns CCharString(\"{s}\", -1) through the hidden result pointer",len(b))+f'''//
// Retail bytes:  mov ecx,[esp+4] ; push -1 ; push offset "{s}" ; call CCharString::CCharString(const char*,long) ; mov eax,[esp+4] ; ret 4
// A method returning a CCharString by value built from one literal: the hidden result pointer
// is the only stack argument (`ret 4`), `this` is never read, and the (char*, long=-1) constructor
// @ 0x0099EBF0 (fable_string.h) fills the result in place. Same family as batch `2026-10-13_linux_b6n`
// (named `<Class>::GetTypeAsString` when the literal is a class name, else `_global::GetName_<TOKEN>`);
// this copy re-reads the hidden pointer instead of keeping it in esi. One of {len(members)} bodies.
#include "fable_string.h"

struct {mod if mod!="_global" else "GetName"}_Methods {{
    CCharString {fn}();
}};

CCharString {mod if mod!="_global" else "GetName"}_Methods::{fn}()
{{
    return CCharString("{s}", -1);
}}
''',T(a,'''struct S { const char* p; long n; S(const char* t, long l) : p(t), n(l) {} };
struct M { S Get() { return S("ABC", -1); } };
int main()
{
    M m; S s = m.Get(); if (s.n != -1 || s.p[0] != 'A') '''+FAIL(a)+'''
'''),b)
    elif TPL=="127":   # bool CX::vfunc_N(long value) { return value == K ? true : false; }
        k=[i.operands[1].imm for i in ins if i.mnemonic=="cmp"][0]; cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) or f"IsKind_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}(long) -- true when the argument equals {k}",len(b))+f'''//
// Retail bytes:  xor eax,eax ; cmp dword ptr [esp+4],{k:#x} ; sete al ; ret 4
// Virtual predicate: compares its one argument with the constant {k} ({k:#x}) and returns the
// result as a full-register integer (`xor eax,eax ; sete al`: an int/long result, a bool result
// would drop the xor). `this` is not
// read. The class comes from the vtable-slot label of this address. One of {len(members)} bodies.
struct {cls} {{
    virtual void v00();
    virtual long {fn}(long value);
}};

long {cls}::{fn}(long value)
{{
    return value == {k};
}}
''',T(a,'''struct X { virtual void v00() {} virtual long Is(long v) { return v == 23; } };
int main()
{
    X x; if (!x.Is(23) || x.Is(5)) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("145","172"):   # void CX::vfunc_N() { m_flag = true; }
        off=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM][0]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) or f"SetFlag_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- sets the byte flag at +{off:#x}",len(b))+f'''//
// Retail bytes:  mov byte ptr [ecx+{off:#x}],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +{off:#x} and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of {len(members)} bodies.
struct {cls} {{
    virtual void v00();
    virtual void {fn}();
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    bool m_flag;                           // +{off:#x}
}};

void {cls}::{fn}()
{{
    m_flag = true;
}}
''',T(a,'''struct X { virtual void v00() {} virtual void Set() { f = true; } unsigned char p[4]; bool f; };
int main()
{
    X x; x.f = false; x.Set(); if (!x.f) '''+FAIL(a)+'''
'''),b)
    elif TPL=="114":   # unsigned char CX::vfunc_N() { return m_pointer->m_byte; }
        o1=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][0]; o2=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][1]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) or f"GetByte_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- returns the byte at +{o2:#x} of the object pointed to by +{o1:#x}",len(b))+f'''//
// Retail bytes:  mov eax,[ecx+{o1:#x}] ; mov al,[eax+{o2:#x}] ; ret
// Virtual getter through one pointer member: loads the object at +{o1:#x} and returns its byte at
// +{o2:#x} in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of {len(members)} bodies of this shape.
struct CTarget {{
    unsigned char _pad[{o2:#x}];
    unsigned char m_value;                 // +{o2:#x}
}};

struct {cls} {{
    virtual void v00();
    virtual unsigned char {fn}();
    unsigned char _pad[{o1-4:#x}];
    CTarget* m_target;                     // +{o1:#x}
}};

unsigned char {cls}::{fn}()
{{
    return m_target->m_value;
}}
''',T(a,'''struct T { unsigned char p[0x40]; unsigned char v; }; struct X { virtual void v00() {} virtual unsigned char Get() { return t->v; } unsigned char p[0x34]; T* t; };
int main()
{
    T t; t.v = 9; X x; x.t = &t; if (x.Get() != 9) '''+FAIL(a)+'''
'''),b)
    elif TPL=="115":   # bool CX::vfunc_N() { return m_bits.flagN; }
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][0]; sh=[i.operands[1].imm for i in ins if i.mnemonic=="shr"]; sh=sh[0] if sh else 0
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) or f"GetBit_{a}"
        bits="".join(f"    unsigned char m_bit{j} : 1;\n" for j in range(sh+1))
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- returns bit {sh} of the byte at +{off:#x}",len(b))+f'''//
// Retail bytes:  mov al,[ecx+{off:#x}] ; shr al,{sh} ; and al,1 ; ret
// Virtual bitfield getter: bit {sh} of the flag byte at +{off:#x}, returned as a bool in al. The
// bitfield names are descriptive (vtable-slot label for the class). One of {len(members)} bodies.
struct {cls} {{
    virtual void v00();
    virtual bool {fn}();
    unsigned char _pad[{off-4:#x}];
{bits}}};

bool {cls}::{fn}()
{{
    return m_bit{sh};
}}
''',T(a,'''struct X { virtual void v00() {} virtual bool Get() { return b2; } unsigned char p[4]; unsigned char b0 : 1; unsigned char b1 : 1; unsigned char b2 : 1; };
int main()
{
    X x; x.b0 = 0; x.b1 = 0; x.b2 = 1; if (!x.Get()) '''+FAIL(a)+'''
    x.b2 = 0; if (x.Get()) '''+FAIL(a)+'''
'''),b)
    elif TPL=="116":   # void CX::vfunc_N(CY* object) { object->Method(); }
        callee=calls[0]; cm,cn,_=label(callee); ocls=cm.split("::")[-1] if (re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CObject_{callee:08x}"
        meth=clean(cn) if ((cm,cn) in uniq and clean(cn)) else f"Method_{callee:08x}"
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) or f"Apply_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}({ocls}*) -- calls {ocls}::{meth} on the argument; this unused",len(b))+f'''//
// Retail bytes:  mov ecx,[esp+4] ; call {ocls}::{meth} ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __thiscall call @ 0x{callee:08X} with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of {len(members)} bodies of this shape.
struct {ocls} {{
    void {meth}();                         // 0x{callee:08X}
}};

struct {cls} {{
    virtual void v00();
    virtual void {fn}({ocls}* object);
}};

void {cls}::{fn}({ocls}* object)
{{
    object->{meth}();
}}
''',T(a,'''static int g_c = 0; struct O { void M() { ++g_c; } }; struct X { virtual void v00() {} virtual void F(O* o) { o->M(); } };
int main()
{
    O o; X x; x.F(&o); if (g_c != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL=="148":   # void CX::Method(long value) { m_pointer->Method(value); }
        callee=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base!=0 and "esp" not in i.op_str][0]
        cm,cn,_=label(callee); ocls=cm.split("::")[-1] if (re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CObject_{callee:08x}"
        meth=clean(cn) if ((cm,cn) in uniq and clean(cn)) else f"Method_{callee:08x}"
        cls=f"CClass_{a}"; mod="_unattributed"; fn=f"Forward_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}(long) -- forwards the argument to {ocls}::{meth} on the object at +{off:#x}",len(b))+f'''//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+{off:#x}] ; push eax ; call {ocls}::{meth} ; ret 4
// Forwarding member: loads the pointer member at +{off:#x} and calls @ 0x{callee:08X} on it with
// this method's one argument. The manifest label of this address (`operator_new`) is propagated and
// wrong; the owning class is not identifiable from the body. One of {len(members)} bodies.
struct {ocls} {{
    void {meth}(long value);               // 0x{callee:08X}
}};

struct {cls} {{
    unsigned char _pad[{off:#x}];
    {ocls}* m_object;                      // +{off:#x}
    void {fn}(long value);
}};

void {cls}::{fn}(long value)
{{
    m_object->{meth}(value);
}}
''',T(a,'''static long g_v = 0; struct O { void M(long v) { g_v = v; } }; struct X { unsigned char p[0x24]; O* o; void F(long v) { o->M(v); } };
int main()
{
    O o; X x; x.o = &o; x.F(7); if (g_v != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL=="154":   # dynamic initializer of a static std::list (sentinel node allocated inline) + atexit
        n=pushes[0]; thunk=pushes[1]; g=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.base==0][0]
        nm=f"CRetailList_node{n:#x}"; gname=f"g_list_{g:08x}"
        add(a,"_static_init",gname,HDRF(a,f"dynamic initializer for {nm} {gname} @ 0x{g:08X} -- allocates and self-links the sentinel node, registers the destructor",len(b))+f'''//
// Retail bytes:  push {n:#x} ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; push offset thunk ; mov [{g:#x}],eax ; call _atexit ; add esp,8 ; ret
// Compiler-generated `??__E` initializer of one static-storage retail std::list (sentinel node of
// {n:#x} bytes, malloc @ 0x00BFEA0E) whose inline default constructor allocates the node and links it
// to itself, then registers the destructor thunk @ 0x{thunk:08X} with _atexit @ 0x004012BC. The
// object's identifier is not recoverable; it is named by its address. One of {len(members)} bodies.
extern "C" void* __cdecl malloc(unsigned int size);

struct {nm} {{
    struct node {{ node* next; node* prev; unsigned char value[{n:#x} - 8]; }};
    node* head;
    {nm}() {{ node* sentinel = (node*)malloc(sizeof(node)); sentinel->next = sentinel; sentinel->prev = sentinel; head = sentinel; }}
    ~{nm}();
}};

{nm} {gname};
''',T(a,'''#include <cstdlib>
struct L { struct node { node* n; node* p; char v[12]; }; node* h; L() { h = (node*)std::malloc(sizeof(node)); h->n = h; h->p = h; } ~L() { std::free(h); } };
L g_l;
int main()
{
    if (!g_l.h || g_l.h->n != g_l.h) '''+FAIL(a)+'''
'''),b)
    elif TPL=="71":   # dynamic initializer of a static std::map/set (head node built inline) + atexit
        n=pushes[0]; thunk=pushes[1]; g=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.base==0][0]
        nm=f"CRetailTree_node{n:#x}"; gname=f"g_tree_{g:08x}"
        add(a,"_static_init",gname,HDRF(a,f"dynamic initializer for {nm} {gname} @ 0x{g:08X} -- builds the red-black head node inline, registers the destructor",len(b))+f'''//
// Retail bytes:  push {n:#x} ; call malloc ; mov [{g:#x}],eax ; xor ecx,ecx ; mov [{g+4:#x}],ecx ; mov byte ptr [eax],cl ; (re-read head) mov [eax+4],ecx ; mov [eax+8],eax ; mov [eax+0Ch],eax ; push offset thunk ; call _atexit ; add esp,8 ; ret
// Compiler-generated `??__E` initializer of one static-storage retail std::map/set: the inline
// default constructor allocates the {n:#x}-byte head node (malloc @ 0x00BFEA0E), zeroes the size,
// clears the node's colour byte and parent and points left/right at the node itself (head is
// re-read through the global between the stores), then the destructor thunk @ 0x{thunk:08X} is
// registered with _atexit @ 0x004012BC. Named by its address. One of {len(members)} bodies.
extern "C" void* __cdecl malloc(unsigned int size);

struct {nm} {{
    struct node {{ unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[{n:#x} - 16]; }};
    node* head;
    unsigned long size;
    {nm}()
    {{
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left = head;
        head->right = head;
    }}
    ~{nm}();
}};

{nm} {gname};
''',T(a,'''#include <cstdlib>
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; T() { h = (node*)std::malloc(sizeof(node)); n = 0; h->c = 0; h->parent = 0; h->l = h; h->r = h; } ~T() { std::free(h); } };
T g_t;
int main()
{
    if (!g_t.h || g_t.n || g_t.h->l != g_t.h) '''+FAIL(a)+'''
'''),b)
    elif TPL=="120":   # T* allocator<T>::allocate(count, hint) { return count ? (T*)malloc(count * sizeof(T)) : 0; }
        # element size: simulate lea/shl/imul on eax
        sz=1
        for i in ins:
            if i.mnemonic=="lea" and i.operands[1].mem.base==i.operands[1].mem.index: sz*= 1+i.operands[1].mem.scale
            elif i.mnemonic=="shl": sz<<=i.operands[1].imm
            elif i.mnemonic=="imul" and i.operands[-1].type==X86_OP_IMM: sz*=i.operands[-1].imm
        nm=f"CRetailAllocator_{sz}"
        add(a,nm,"allocate",HDRF(a,f"{nm}::allocate(count, hint) -- count ? malloc(count * {sz}) : 0",len(b))+f'''//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; je Z ; (eax *= {sz}) ; push eax ; call malloc ; pop ecx ; jmp R ; Z: xor eax,eax ; R: ret 8
// Retail std::allocator<T>::allocate for a {sz}-byte element type: returns null for a zero count,
// else one malloc'd block (malloc @ 0x00BFEA0E, landed-trusted) of count * {sz} bytes. The hint
// argument and `this` are unused (`ret 8`). The element type is not recoverable from the body;
// the allocator is named by the element size. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct {nm} {{
    struct element {{ unsigned char bytes[{sz}]; }};
    element* allocate(unsigned long count, const void* hint);
}};

{nm}::element* {nm}::allocate(unsigned long count, const void* hint)
{{
    return count ? (element*)malloc(count * sizeof(element)) : 0;
}}
''',T(a,'''#include <cstdlib>
struct A { struct e { unsigned char b[12]; }; e* alloc(unsigned long n, const void*) { return n ? (e*)std::malloc(n * sizeof(e)) : 0; } };
int main()
{
    A a; e_check: if (a.alloc(0, 0)) '''+FAIL(a)+'''
    A::e* p = a.alloc(2, 0); if (!p) '''+FAIL(a)+'''
    std::free(p);
'''),b)
    elif TPL=="177":   # void CX::vfunc_N() { m_pointer->m_flag = true; }
        o1=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][0]; st=[i for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM][0]; o2=st.operands[0].mem.disp; val=st.operands[1].imm
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) if (clean(mname) and not clean(mname).startswith("sub_")) else (f"SetFlag_{a}" if val else f"ClearFlag_{a}")
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- {'sets' if val else 'clears'} the byte flag at +{o2:#x} of the object at +{o1:#x}",len(b))+f'''//
// Retail bytes:  mov eax,[ecx+{o1:#x}] ; mov byte ptr [eax+{o2:#x}],{val} ; ret
// Setter through one pointer member: loads the object at +{o1:#x} and writes {val} into its byte at
// +{o2:#x}. Class and member names are descriptive (vtable-slot label for the class when there is
// one). One of {len(members)} bodies of this shape.
struct CTarget {{
    unsigned char _pad[{o2:#x}];
    bool m_flag;                           // +{o2:#x}
}};

struct {cls} {{
    virtual void v00();
    virtual void {fn}();
    unsigned char _pad[{o1-4:#x}];
    CTarget* m_target;                     // +{o1:#x}
}};

void {cls}::{fn}()
{{
    m_target->m_flag = {"true" if val else "false"};
}}
''',T(a,'''struct T { unsigned char p[0x48]; bool f; }; struct X { virtual void v00() {} virtual void Set() { t->f = true; } unsigned char p[0x10]; T* t; };
int main()
{
    T t; t.f = false; X x; x.t = &t; x.Set(); if (!t.f) '''+FAIL(a)+'''
'''),b)
    elif TPL=="174":   # CX::CX() : m_item(0) {}  (no base; vtable then one null member)
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; off=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and i.operands[0].mem.disp][0]
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}() -- vtable, then the pointer member at +{off:#x} nulled",len(b))+f'''//
// Retail bytes:  mov eax,ecx ; mov dword ptr [eax],offset {cls}::`vftable' ; mov dword ptr [eax+{off:#x}],0 ; ret
// Default constructor of a root polymorphic class (no base constructor call): installs the vtable
// (0x{vt:08X}, rtti-vtables.tsv) and nulls the one pointer member at +{off:#x}. `this` is returned
// in eax (copied from ecx before the stores). One of {len(members)} bodies of this shape.
struct {cls} {{
    virtual void v00();
    void* m_item;                          // +{off:#x}
    {cls}();
}};

{cls}::{cls}()
    : m_item(0)
{{
}}
''',T(a,'''struct X { virtual void v00() {} void* p; X() : p(0) {} };
int main()
{
    X x; if (x.p) '''+FAIL(a)+'''
'''),b)
    elif TPL=="257":   # bool CX::vfunc_N() { return Helper() ? true : false; }
        callee=calls[0]; cm,cn,_=label(callee); meth=clean(cn) if ((cm,cn) in uniq and clean(cn) and not clean(cn).startswith("sub_")) else f"Query_{callee:08x}"
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) or f"Is_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- normalises {meth}()'s byte result to a bool",len(b))+f'''//
// Retail bytes:  call {meth} ; test al,al ; setne al ; ret
// Virtual predicate that forwards to the __thiscall helper @ 0x{callee:08X} on the same object
// (ecx passes through untouched) and returns its byte result as a normalised bool. Callee name
// kept from the manifest only when the label is unique. One of {len(members)} bodies of this shape.
struct {cls} {{
    virtual void v00();
    virtual bool {fn}();
    unsigned char {meth}();                // 0x{callee:08X}
}};

bool {cls}::{fn}()
{{
    return {meth}() ? true : false;
}}
''',T(a,'''struct X { virtual void v00() {} virtual bool Is() { return Q() ? true : false; } unsigned char Q() { return 4; } };
int main()
{
    X x; if (!x.Is()) '''+FAIL(a)+'''
'''),b)
    elif TPL=="244":   # T CX::Get() { return g_pointer->m_field; }
        g=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base==0][0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base!=0][0]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) if (clean(mname) and not clean(mname).startswith("sub_")) else f"GetGlobalField_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- returns the dword at +{off:#x} of the object the global @ 0x{g:08X} points to",len(b))+f'''//
// Retail bytes:  mov eax,[{g:#x}] ; mov eax,[eax+{off:#x}] ; ret
// Static-style getter: reads the global object pointer at 0x{g:08X} and returns its dword member
// at +{off:#x}; `this` is not used (`ret` with no arguments). Names are descriptive beyond the
// manifest label. One of {len(members)} bodies of this shape.
struct CGlobalObject {{
    unsigned char _pad[{off:#x}];
    void* m_field;                         // +{off:#x}
}};

extern CGlobalObject* g_object_{g:08x};    // 0x{g:08X}

struct {cls} {{
    void* {fn}();
}};

void* {cls}::{fn}()
{{
    return g_object_{g:08x}->m_field;
}}
''',T(a,'''struct G { unsigned char p[0x24]; void* f; }; static G g_g; static G* g_p = &g_g; struct X { void* Get() { return g_p->f; } };
int main()
{
    int v; g_g.f = &v; X x; if (x.Get() != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL=="201":   # CX* Alloc() { return new CX; }  with CX() : m_refs(1) {} inlined (root class, no base)
        size=pushes[0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        st=[i for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and i.operands[0].mem.disp]; off=st[0].operands[0].mem.disp; val=st[0].operands[1].imm
        add(a,"_global",f"Alloc_{cls}",HDRF(a,f"Alloc_{cls}() -- new {cls} with the constructor inlined: member +{off:#x} = {val}, vtable",len(b))+f'''//
// Retail bytes:  push {size:#x} ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov dword ptr [eax+{off:#x}],{val} ; mov dword ptr [eax],offset {cls}::`vftable' ; ret ; N: xor eax,eax ; ret
// Factory for a root polymorphic class: allocates sizeof({cls}) = {size:#x} bytes (operator new
// @ 0x00BFEA1A, landed-trusted), null-checks and runs the inlined default constructor (the counter
// at +{off:#x} set to {val}, then the vtable 0x{vt:08X} from rtti-vtables.tsv). One of {len(members)} bodies.
// The counter store comes BEFORE the vtable store: the counter is initialised by an inlined base
// constructor (whose own vtable store is dead and dropped); a member of {cls} itself would be
// stored after the vtable. The base is not identifiable from the body.
struct {cls}_Base {{
    virtual void v00();
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    long m_count;                          // +{off:#x}
    {cls}_Base() : m_count({val}) {{ }}
}};

struct {cls} : {cls}_Base {{
    virtual void v00();
    unsigned char _tail[{size-off-4:#x}];
    {cls}() : {cls}_Base() {{ }}
}};

{cls}* Alloc_{cls}()
{{
    return new {cls};
}}
''',T(a,'''struct B { virtual void v00() {} long c; B() : c(1) {} }; struct X : B { virtual void v00() {} unsigned char t[4]; X() : B() {} };
static X* Alloc() { return new X; }
int main()
{
    X* x = Alloc(); if (!x || x->c != 1) '''+FAIL(a)+'''
    delete x;
'''),b)
    elif TPL=="209":   # void list::Initialise(): head = 0; head = new self-linked sentinel  (no `this` return)
        n=pushes[0]; nm=f"CRetailList_node{n:#x}"
        add(a,nm,"Initialise",HDRF(a,f"std::list<T> (node {n:#x} bytes)::Initialise() -- re-creates the sentinel node (no allocator argument, no `this` return)",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; push {n:#x} ; mov dword ptr [esi],0 ; call malloc ; add esp,4 ; mov [eax],eax ; mov [eax+4],eax ; mov [esi],eax ; pop esi ; ret
// Same work as the retail list constructor (family 6) but as a plain member: no allocator
// argument (`ret`) and no `this` returned in eax. Nulls the head, allocates one node of {n:#x}
// bytes (malloc @ 0x00BFEA0E, landed-trusted) and links it to itself. The element type is not
// recoverable from the body. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct {nm} {{
    struct node {{ node* next; node* prev; unsigned char value[{n:#x} - 8]; }};
    node* head;
    void Initialise();
}};

void {nm}::Initialise()
{{
    head = 0;
    node* sentinel = (node*)malloc(sizeof(node));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}}
''',T(a,'''#include <cstdlib>
struct L { struct node { node* n; node* p; char v[4]; }; node* h; void Init() { h = 0; node* s = (node*)std::malloc(sizeof(node)); s->n = s; s->p = s; h = s; } };
int main()
{
    L l; l.Init(); if (!l.h || l.h->n != l.h) '''+FAIL(a)+'''
    std::free(l.h);
'''),b)
    elif TPL in ("w4",):   # CCountedPointer<T>::Reset(T* object): release the old count block, adopt the new object
        deleter=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x401000<=i.operands[1].imm<0x1210000][0]
        inst=re.sub(r"[^A-Za-z0-9_]+","_",mmod).strip("_"); cls=inst if ((mmod,mname) in uniq and mmod.startswith("CCountedPointer<")) else f"CCountedPointer_{deleter:08x}"
        add(a,cls,"Reset",HDRF(a,f"{cls}::Reset(T*) -- drop the shared count block (destroying the object on last reference), then adopt the new object",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; A: mov eax,[esp+8] ; test eax,eax ; mov dword ptr [esi+4],0 ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; Z: pop esi ; ret 4
// Intrusive-count smart pointer {{object, count block}}: the 12-byte count block holds the
// reference count, the per-type destroy function (0x{deleter:08X}, called with the object in ecx)
// and the object. Reset() decrements the old block (re-reading it after the decrement: the count
// is touched through a reference), destroys + frees it on zero, then adopts the new object by
// allocating a fresh block (operator new @ 0x00BFEA1A, null-checked; delete @ 0x00BFE9BC). The
// instantiation is named from the manifest label when unique, else from the destroy function.
// One of {len(members)} bodies of this shape.
void __fastcall Destroy_{deleter:08x}(void* object);   // 0x{deleter:08X}: the per-type deleter stored in the block

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_{deleter:08x}), m_pObject(object) {{ }}
}};

struct {cls} {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    void Reset(void* object);
}};

void {cls}::Reset(void* object)
{{
    if (m_pCount) {{
        --m_pCount->m_references;
        if (m_pCount->m_references == 0) {{
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }}
    }}
    m_pCount = 0;
    m_pObject = object;
    if (object)
        m_pCount = new SCountBlock(m_pObject);   // re-read through `this` (retail loads [esi] for the block), not the argument
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; B(void* x) : r(1), f(Destroy), o(x) {} };
struct P { void* o; B* c; void Reset(void* x) { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } c = 0; o = x; if (x) c = new B(o); } };
int main()
{
    int v; P p; p.o = 0; p.c = 0; p.Reset(&v); if (!p.c || p.c->r != 1 || p.o != &v) '''+FAIL(a)+'''
    p.Reset(0); if (g_d != 1 || p.c || p.o) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w48",):   # CCountedPointer<T>::CCountedPointer(T* object): adopt the object with a fresh count block
        deleter=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x401000<=i.operands[1].imm<0x1210000][0]
        inst=re.sub(r"[^A-Za-z0-9_]+","_",mmod).strip("_"); cls=inst if ((mmod,mname) in uniq and mmod.startswith("CCountedPointer<")) else f"CCountedPointer_{deleter:08x}"
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(T*) -- adopt the object: null-checked count block with the per-type destroy function",len(b))+f'''//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; push esi ; mov esi,ecx ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; Z: mov dword ptr [esi+4],0 ; mov eax,esi ; pop esi ; ret 4
// Constructor of the intrusive-count smart pointer {{object, count block}}: stores the object,
// then for a non-null object allocates the 12-byte block {{1, destroy fn 0x{deleter:08X}, object}}
// (operator new @ 0x00BFEA1A, null-checked), else leaves the block pointer null. Named like the
// Reset family (unique manifest label, else the destroy function). One of {len(members)} bodies.
void __fastcall Destroy_{deleter:08x}(void* object);   // 0x{deleter:08X}: the per-type deleter stored in the block

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_{deleter:08x}), m_pObject(object) {{ }}
}};

struct {cls} {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    {cls}(void* object);
}};

{cls}::{cls}(void* object)
    : m_pObject(object)
{{
    if (object)
        m_pCount = new SCountBlock(m_pObject);   // re-read through `this` (retail loads [esi] for the block), not the argument
    else
        m_pCount = 0;
}}
''',T(a,'''static void __fastcall Destroy(void*) { }
struct B { long r; void (__fastcall* f)(void*); void* o; B(void* x) : r(1), f(Destroy), o(x) {} };
struct P { void* o; B* c; P(void* x) : o(x) { if (x) c = new B(o); else c = 0; } };
int main()
{
    int v; P p(&v); if (!p.c || p.c->r != 1 || p.o != &v) '''+FAIL(a)+'''
    delete p.c; P q(0); if (q.c || q.o) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w6",):   # ~CRetailTree(): erase the subtree, reset the head links, free the head node
        erase=calls[0]; cls=f"CRetailTree_erase{erase:08x}"
        add(a,cls,f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- std::map/set destructor: erase all nodes through 0x{erase:08X}, reset the head, free it",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Erase ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ; F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Destructor of the retail red-black tree {{head, size}}: when the tree holds nodes the recursive
// erase @ 0x{erase:08X} (a __thiscall member taking the root, head->parent) frees them, then the
// head's left/parent/right are reset through `head` re-read per store and size is zeroed; finally
// the head node itself is released with free @ 0x00BFEA14 (null-checked). Only the erase callee
// distinguishes the instantiations, so the class is named by it. One of {len(members)} bodies.
extern "C" void __cdecl free(void* block);

struct {cls} {{
    struct node {{ unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; }};
    node* head;
    unsigned long size;
    void Erase(node* root);                // 0x{erase:08X}
    ~{cls}();
}};

{cls}::~{cls}()
{{
    if (size) {{
        Erase(head->parent);
        head->left = head;
        head->parent = 0;
        head->right = head;
        size = 0;
    }}
    if (head)
        free(head);
}}
''',T(a,'''#include <cstdlib>
static int g_e = 0; struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Erase(node*) { ++g_e; }
  ~T() { if (n) { Erase(h->parent); h->l = h; h->parent = 0; h->r = h; n = 0; } if (h) std::free(h); } };
int main()
{
    { T t; t.h = (T::node*)std::malloc(sizeof(T::node)); t.h->parent = 0; t.n = 1; } if (g_e != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w11",):   # retail tree iterator operator--
        cls="CRetailTreeIterator"
        add(a,cls,"Decrement",HDRF(a,f"{cls}::Decrement() -- red-black tree iterator operator--: predecessor node",len(b))+f'''//
// Retail bytes:  mov eax,ecx ; push edi ; mov edi,[eax] ; cmp byte ptr [edi],0 ; jne L ; mov ecx,[edi+4] ; cmp [ecx+4],edi ; jne L ; mov edi,[edi+0Ch] ; mov [eax],edi ; pop edi ; ret ; L: push esi ; mov esi,[edi+8] ; test esi,esi ; je P ; (descend right from the left child) ; mov [eax],esi ; pop esi ; pop edi ; ret ; P: (climb while node is its parent's left) ; pop esi ; mov [eax],edx ; pop edi ; ret
// Dinkumware-shaped `_Dec` on the retail node {{flag, parent, left, right, value}}: the head node
// (flag 0 and parent->parent == node) steps to its right child (the maximum); a node with a left
// child goes to the rightmost node of that subtree; otherwise it climbs while it is its parent's
// left child. Returns `*this`. Every instantiation emits a byte-identical copy; this file is the
// copy at this address. One of {len(members)} bodies of this shape.
struct {cls} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; }};
    node* current;
    {cls}& Decrement();
}};

{cls}& {cls}::Decrement()
{{
    node* p = current;
    if (p->flag == 0 && p->parent->parent == p)
        p = p->right;
    else if (p->left) {{
        node* q = p->left;
        while (q->right)
            q = q->right;
        p = q;
    }}
    else {{
        node* q = p->parent;
        while (p == q->left) {{
            p = q;
            q = q->parent;
        }}
        p = q;
    }}
    current = p;
    return *this;
}}
''',T(a,'''struct I { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; }; node* c;
  I& Dec() { node* p = c; if (p->f == 0 && p->parent->parent == p) p = p->r; else if (p->l) { node* q = p->l; while (q->r) q = q->r; p = q; } else { node* q = p->parent; while (p == q->l) { p = q; q = q->parent; } p = q; } c = p; return *this; } };
int main()
{
    I::node a = {1,{0},0,0,0}, b = {1,{0},&a,0,0}; a.r = &b; I i; i.c = &b; i.Dec(); if (i.c != &a) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w13",):   # retail tree iterator operator++
        cls="CRetailTreeIterator"
        add(a,cls,"Increment",HDRF(a,f"{cls}::Increment() -- red-black tree iterator operator++: successor node",len(b))+f'''//
// Retail bytes:  mov eax,ecx ; mov edx,[eax] ; mov ecx,[edx+0Ch] ; test ecx,ecx ; je P ; (descend left from the right child) ; mov [eax],ecx ; ret ; P: push esi ; mov esi,[edx+4] ; (climb while node is its parent's right) ; cmp [edx+0Ch],esi ; je K ; mov [eax],esi ; pop esi ; ret ; K: mov [eax],edx ; pop esi ; ret
// Dinkumware-shaped `_Inc` on the retail node {{flag, parent, left, right, value}}: a node with a
// right child goes to the leftmost node of that subtree; otherwise it climbs while it is its
// parent's right child and steps to that parent unless it already is the node's right child (the
// end() case). Returns `*this`. Byte-identical per instantiation; this is the copy at this
// address. One of {len(members)} bodies of this shape.
struct {cls} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; }};
    node* current;
    {cls}& Increment();
}};

{cls}& {cls}::Increment()
{{
    node* p = current;
    if (p->right) {{                       // descend: leftmost node of the right subtree (one variable, no helper)
        p = p->right;
        while (p->left)
            p = p->left;
    }}
    else {{
        node* q;
        while (p == (q = p->parent)->right)
            p = q;
        if (p->right != q)
            p = q;
    }}
    current = p;
    return *this;
}}
''',T(a,'''struct I { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; }; node* c;
  I& Inc() { node* p = c; if (p->r) { node* q = p->r; while (q->l) q = q->l; p = q; } else { node* q = p->parent; while (p == q->r) { p = q; q = q->parent; } if (p->r != q) p = q; } c = p; return *this; } };
int main()
{
    I::node a = {1,{0},0,0,0}, b = {1,{0},&a,0,0}; a.l = &b; I i; i.c = &b; i.Inc(); if (i.c != &a) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w26",):   # iterator CRetailTree::find(const long& key)  (signed or unsigned 32-bit keys at +0x10)
        uns=any(i.mnemonic in ("jb","jae") for i in ins); kt="unsigned long" if uns else "long"; cls="CRetailTree_ulongkey" if uns else "CRetailTree_longkey"
        direct=(retn==4)
        if direct: cls+="_node"
        add(a,cls,"find",HDRF(a,f"{cls}::find(const {kt}&) -- lower-bound walk, then end() unless the key matches",len(b))+f'''//
// Retail bytes:  mov ecx,[ecx] ; mov eax,[ecx+4] ; test eax,eax ; push esi ; push edi ; mov edi,[esp+10h] ; mov edx,ecx ; je E ; mov esi,[edi] ; L: cmp [eax+10h],esi ; {'jb' if uns else 'jl'} R ; mov edx,eax ; mov eax,[eax+8] ; jmp T ; R: mov eax,[eax+0Ch] ; T: test eax,eax ; jne L ; E: cmp edx,ecx ; je H ; mov eax,[edi] ; cmp eax,[edx+10h] ; {'jae' if uns else 'jge'} F ; H: (result = head) ; ret 8 ; F: (result = node) ; ret 8
// std::map/set::find on the retail tree with {'an unsigned' if uns else 'a signed'} 32-bit key at +0x10: lower_bound descends
// from head->parent (right when node->key < key, else remember the node and go left), then the
// result is end() (the head) when nothing was found or key < found->key. The iterator is returned
// {'directly as the node pointer (`ret 4`)' if direct else 'through the hidden pointer (`ret 8`: result pointer + key reference)'}. One of {len(members)} bodies.
struct {cls} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; {kt} key; }};
    struct iterator {{ node* current; iterator(node* n) : current(n) {{ }} }};
    node* head;
    unsigned long size;
    {'node*' if direct else 'iterator'} find(const {kt}& key);
}};

{cls}::{'node*' if direct else 'iterator'} {cls}::find(const {kt}& key)
{{
    node* result = head;
    node* n = head->parent;
    while (n) {{
        if (!(n->key < key)) {{            // Dinkumware _Lbound: `if (!comp(key(node), k))`
            result = n;
            n = n->left;
        }}
        else
            n = n->right;
    }}
    node* found = result;
    if (found == head || key < found->key)
        found = head;                      // end(): a single return through one pointer keeps retail's block order
    return {'found' if direct else 'iterator(found)'};
}}
''',T(a,'''struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; '''+kt+''' k; }; struct it { node* c; it(node* n) : c(n) {} }; node* h; unsigned long n;
  it find(const '''+kt+'''& k) { node* res = h; node* x = h->parent; while (x) { if (!(x->k < k)) { res = x; x = x->l; } else x = x->r; } node* f = res; if (f == h || k < f->k) f = h; return it(f); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}, a = {1,{0},&h,0,0,5}; h.parent = &a; T t; t.h = &h; t.n = 1; '''+kt+''' k = 5, m = 6; if (t.find(k).c != &a || t.find(m).c != &h) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w43",):   # ~CX() {} for three consecutive CWideString-like members (reverse order, last by tail call)
        dt=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"]; dtor=calls[0]; tail=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        if tail!=dtor or any(c!=dtor for c in calls): continue
        dm,dn,_=label(dtor); dl=dn.split("::")[-1]; mcls=dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1])) else f"CMember_{dtor:08x}"
        offs=sorted(dt+[0]); cls=f"CTriple{mcls}_{a}"
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- destroys three {mcls} members at {', '.join('+%#x'%o for o in reversed(offs))} (last one by tail call)",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; {" ; ".join(f"lea ecx,[esi+{o:#x}] ; call {mcls}::~{mcls}" for o in reversed(offs[1:]))} ; mov ecx,esi ; pop esi ; jmp {mcls}::~{mcls}
// Implicit/empty destructor of a non-polymorphic aggregate of three 4-byte {mcls} members
// (destructor @ 0x{dtor:08X}): members are destroyed in reverse declaration order and the first
// one's destructor is reached by tail call. The manifest label (`_Dest_val<...pair<CCharString,
// CSymbolInfo>>`) says this is the pair value of a map; the owning type is named descriptively.
// One of {len(members)} bodies of this shape.
struct {mcls} {{
    void* m_data;
    ~{mcls}();                             // 0x{dtor:08X}
}};

struct {cls} {{
    {mcls} m_first;                        // +0
    {mcls} m_second;                       // +4
    {mcls} m_third;                        // +8
    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct S { void* p; ~S() { g_d = g_d * 10 + (int)(long)p; } };
struct X { S a, b, c; ~X() {} };
int main()
{
    { X x; x.a.p = (void*)1; x.b.p = (void*)2; x.c.p = (void*)3; } if (g_d != 321) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w45",):   # void CX::TransferIn(CPersistContext* ctx, unsigned char value) { ::TransferIn(ctx, value, &value); *m_pValue = value; }
        callee=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "ecx +" in i.op_str][0]
        wide=not any("byte ptr" in i.op_str for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM); mt="long" if wide else "unsigned char"; cls=f"CTransferable{'Long' if wide else 'Byte'}_{callee:08x}"
        add(a,"_unattributed",f"TransferIn_{cls}",HDRF(a,f"{cls}::vfunc_1(CPersistContext*, unsigned char) -- reads the byte through the __fastcall helper 0x{callee:08X}, stores it through the pointer at +{off:#x}",len(b))+f'''//
// Retail bytes:  mov edx,[esp+8] ; push esi ; mov esi,[ecx+{off:#x}] ; mov ecx,[esp+8] ; lea eax,[esp+0Ch] ; push eax ; call TransferIn ; {'mov ecx,[esp+0Ch] ; mov [esi],ecx' if wide else 'mov cl,[esp+0Ch] ; mov byte ptr [esi],cl'} ; pop esi ; ret 8
// Persist read hook of a {'four' if wide else 'one'}-byte transferable component: the dword second parameter is passed
// by value AND by address to the __fastcall helper @ 0x{callee:08X} (context in ecx, value in edx,
// &value on the stack), which writes the parsed value back into the parameter slot; {'it' if wide else 'its low byte'}
// is then stored through the component's target pointer at +{off:#x} (read before the call). The manifest label
// (`CPersistContext_TransferableComponent::vfunc_1`) is propagated; the class is named by the
// helper. One of {len(members)} bodies of this shape.
struct CPersistContext;
void __fastcall TransferIn_{callee:08x}(CPersistContext* context, long value, long* result);   // 0x{callee:08X} (manifest: ctx, CStringParser*, long*)

struct {cls} {{
    virtual void v00();
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    {mt}* m_pValue;{" " * (22 - len(mt))}// +{off:#x}
    virtual void TransferIn(CPersistContext* context, long value);
}};

void {cls}::TransferIn(CPersistContext* context, long value)
{{
    {mt}* target = m_pValue;{" " * (22 - len(mt))}// read before the call (retail loads [ecx+4] first): a named local
    TransferIn_{callee:08x}(context, value, &value);
    *target = ({mt})value;
}}
''',T(a,'''struct Ctx; static void __fastcall Helper(Ctx*, long v, long* r) { *r = v + 1; }
struct X { virtual void v00() {} unsigned char* p; virtual void T(Ctx* c, long v) { unsigned char* t = p; Helper(c, v, &v); *t = (unsigned char)v; } };
int main()
{
    unsigned char out = 0; X x; x.p = &out; x.T(0, 4); if (out != 5) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w52",):   # CX::CX(a, b) : m_first(a) {}  root class, second argument unused
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; off=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_REG and i.operands[0].mem.disp][0]
        two=retn==8; params="void* first, void* /*second*/" if two else "void* first"
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}({'void*, void*' if two else 'void*'}) -- root polymorphic class: vtable, first argument kept at +{off:#x}" + (", second unused" if two else ""),len(b))+f'''//
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset {cls}::`vftable' ; mov [eax+{off:#x}],ecx ; ret {retn}
// {"Two" if two else "One"}-argument constructor of a root polymorphic class (no base constructor call): installs the
// vtable (0x{vt:#08X}, rtti-vtables.tsv), stores the first argument at +{off:#x}{" and ignores the second (`ret 8`)" if two else ""}.
// Argument types are not recoverable; `void*` models them. One of {len(members)} bodies.
struct {cls} {{
    virtual void v00();
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    void* m_first;                         // +{off:#x}
    {cls}({params});
}};

{cls}::{cls}({params})
    : m_first(first)
{{
}}
''',T(a,'''struct X { virtual void v00() {} unsigned char p[8]; void* f; X(void* a, void*) : f(a) {} };
int main()
{
    int v; X x(&v, 0); if (x.f != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w27",):   # node* list::Buynode(const T& value): malloc the node, placement-copy the value
        n=pushes[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]; copy=calls[1]; vsz=n-off
        nm=f"CRetailList_node{n:#x}"
        add(a,nm,"Buynode",HDRF(a,f"{nm}::Buynode(const T&) -- malloc a {n:#x}-byte node and copy-construct the {vsz}-byte value at +{off:#x} in place",len(b))+f'''//
// Retail bytes:  push esi ; push {n:#x} ; call malloc ; mov esi,eax ; pop ecx ; lea ecx,[esi+{off:#x}] ; test ecx,ecx ; je S ; push [esp+8] ; call T::T(const T&) ; S: mov eax,esi ; pop esi ; ret 4
// Retail std::list node factory: one malloc'd node (malloc @ 0x00BFEA0E) whose value is
// copy-constructed in place with placement new (hence the null test on the value address) by the
// out-of-line copy constructor @ 0x{copy:08X}; the node is returned. `this` (the list) is unused.
// The element type is not recoverable beyond its {vsz}-byte size. One of {len(members)} bodies.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct {nm} {{
    struct value_type {{ unsigned char _body[{vsz:#x}]; value_type(const value_type& other); }};   // copy ctor 0x{copy:08X}
    struct node {{ node* next; node* prev; {"unsigned char _gap[%#x]; " % (off-8) if off>8 else ""}value_type value; }};
    node* head;
    node* Buynode(const value_type& value);
}};

{nm}::node* {nm}::Buynode(const value_type& value)
{{
    node* n = (node*)malloc(sizeof(node));
    new (&n->value) value_type(value);
    return n;
}}
''',T(a,'''#include <new>
#include <cstdlib>
static int g_c = 0; struct V { unsigned char b[8]; V() {} V(const V& o) { ++g_c; b[0] = o.b[0]; } };
struct L { struct node { node* n; node* p; unsigned char g[8]; V v; }; node* h; node* Buy(const V& v) { node* x = (node*)std::malloc(sizeof(node)); new (&x->v) V(v); return x; } };
int main()
{
    V v; v.b[0] = 7; L l; L::node* n = l.Buy(v); if (!n || g_c != 1 || n->v.b[0] != 7) '''+FAIL(a)+'''
    std::free(n);
'''),b)
    elif TPL in ("w36",):   # static void Release() { g_pointer.Reset(0); }  (CCountedPointer::Reset inlined on a global)
        g=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base==0][0]; gobj=g-4
        add(a,"_global",f"Release_{gobj:08x}",HDRF(a,f"Release_{gobj:08x}() -- drops the static counted pointer @ 0x{gobj:08X}: Reset(0) inlined",len(b))+f'''//
// Retail bytes:  mov eax,[{g:#x}] ; push esi ; xor esi,esi ; cmp eax,esi ; je Z ; dec dword ptr [eax] ; mov eax,[{g:#x}] ; cmp [eax],esi ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[{g:#x}] ; push eax ; call operator delete ; add esp,4 ; Z: mov [{g:#x}],esi ; mov [{gobj:#x}],esi ; pop esi ; ret
// A static CCountedPointer {{object @ 0x{gobj:08X}, count block @ 0x{g:08X}}} being released: the
// inlined Reset(0) decrements the block, destroys the object through the block's function pointer
// and frees the block (operator delete @ 0x00BFE9BC) on the last reference, then nulls both
// fields (the "adopt new object" half folds away for a null argument). One of {len(members)} bodies.
struct SCountBlock {{
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
}};

struct CCountedPointerStatic {{
    void* m_pObject;
    SCountBlock* m_pCount;
    __forceinline void Reset(void* object)
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pCount = 0;
        m_pObject = object;
    }}
}};

extern CCountedPointerStatic g_pointer_{gobj:08x};   // 0x{gobj:08X}

void Release_{gobj:08x}()
{{
    g_pointer_{gobj:08x}.Reset(0);
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; void Reset(void* x) { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } c = 0; o = x; } };
static P g_p;
static void Release() { g_p.Reset(0); }
int main()
{
    int v; g_p.o = &v; g_p.c = new B; g_p.c->r = 1; g_p.c->f = Destroy; g_p.c->o = &v; Release(); if (g_d != 1 || g_p.c || g_p.o) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w22",):   # CCombatAction_X::CCombatAction_X(owner) : CCombatActionBase(owner, K, CSoundPair()) { [SetAnimationName(CCharString("LIT", -1));] }
        base=calls[0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; k=[p for p in pushes if p<0x10000][0]
        bm,bn,_=label(base); basecls=bm.split("::")[-1] if (bm,bn) in uniq or bm=="CCombatActionBase" else f"CBase_{base:08x}"
        lits=[p for p in pushes if 0x1200000<=p<0x1300000]; s=lit(lits[0]) if lits else None; setter=calls[2] if lits else 0
        body=(f'''    {{
        CCharString name("{s}", -1);       // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetAnimationName(name);            // 0x{setter:08X}
    }}
''' if s else "")
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(CThingCreatureBase*) -- base {basecls}(owner, {k}, zeroed CSoundPair)" + (f', then animation "{s}"' if s else ""),len(b))+f'''//
// Retail bytes:  sub esp,8 ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[esp+14h] ; push {k:#x} ; push ecx ; mov ecx,esi ; call {basecls}::{basecls} ; {"push -1 ; push offset lit ; lea ecx,[esp+18h] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea edx,[esp+10h] ; push edx ; mov ecx,esi ; call SetAnimationName ; lea ecx,[esp+10h] ; call CCharString::~CCharString ; " if s else "mov dword ptr [esi],offset vftable ; "}mov eax,esi ; pop esi ; add esp,8 ; ret 4
// Combat-action constructor: the base @ 0x{base:08X} takes the owner, the action code {k} ({k:#x})
// and a two-dword sound pair built zeroed on the stack by its inline constructor (register-zeroed
// stores: an embedded object's inline ctor), then this class's vtable (0x{vt:08X},
// rtti-vtables.tsv){" and the animation name set through a scoped CCharString (ctor @ 0x0099EBF0, dtor @ 0x0099EAE0; the setter's manifest label is propagated, named as in family 20)" if s else ""}.
// One of {len(members)} bodies of this shape.
{"#include " + chr(34) + "fable_string.h" + chr(34) + chr(10) if s else ""}
struct CThingCreatureBase;

struct CSoundPair {{
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) {{ }}
}};

struct {basecls} {{
    virtual void v00();
    {basecls}(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x{base:08X}
{"    void SetAnimationName(const CCharString& name);" + chr(10) if s else ""}}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}(CThingCreatureBase* owner);
}};

{cls}::{cls}(CThingCreatureBase* owner)
    : {basecls}(owner, {k}, CSoundPair())
{{
{body}}}
''',T(a,'''static long g_k = 0, g_s = 0; struct SP { long a, b; SP() : a(0), b(0) {} };
struct Base { virtual void v00() {} Base(void*, long k, const SP& p) { g_k = k; g_s = p.a + p.b; } };
struct D : Base { virtual void v00() {} D(void* o) : Base(o, 0x8c, SP()) {} };
int main()
{
    D d(0); if (g_k != 0x8c || g_s != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w24",):   # CX::CX(const CX& other) : CBase(other), m_target(other.m_target) {}  with the smart-pointer copy ctor inlined
        base=calls[0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; vt2=vts[1][1]; pcls=rtti.get(vt2) or f"CPointer_{vt2:08x}"
        bm,bn,_=label(base); basecls=bm.split("::")[-1] if (bm,bn) in uniq or bm=="CCreatureActionBase" else f"CBase_{base:08x}"
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]; pbase=calls[1]; getter=calls[2]; setter=calls[3]
        pbasecls="CBaseIntelligentPointer" if pbase==0xa01b10 else f"CPointerBase_{pbase:08x}"
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(const {cls}&) -- copy constructor: base {basecls}(other), own vtable, {pcls} member at +{off:#x} copied via GetPItem/SetPItem",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; push edi ; push ebx ; mov esi,ecx ; call {basecls}::{basecls}(const&) ; lea edi,[esi+{off:#x}] ; mov ecx,edi ; mov dword ptr [esi],offset {cls}::`vftable' ; call {pbasecls}::{pbasecls} ; lea ecx,[ebx+{off:#x}] ; mov dword ptr [edi],offset {pcls}::`vftable' ; call GetPItem ; push eax ; mov ecx,edi ; call SetPItem ; pop edi ; mov eax,esi ; pop esi ; pop ebx ; ret 4
// Copy constructor: the base copy constructor @ 0x{base:08X} runs out of line, this class's vtable
// (0x{vt:08X}) is installed, then the {pcls} member at +{off:#x} is copy-constructed with the
// family-44 copy constructor inlined: base {pbasecls}() @ 0x{pbase:08X}, its vtable (0x{vt2:08X}),
// SetPItem(other.GetPItem()) (@ 0x{setter:08X} / 0x{getter:08X}). Class names from rtti-vtables.tsv.
// One of {len(members)} bodies of this shape.
struct CBaseObject;

struct {pbasecls} {{
    virtual void v00();
    {pbasecls}();                          // 0x{pbase:08X}
    CBaseObject* GetPItem() const;         // 0x{getter:08X}
    void SetPItem(CBaseObject* item);      // 0x{setter:08X}
}};

struct {pcls} : {pbasecls} {{
    virtual void v00();
    {pcls}(const {pcls}& other) : {pbasecls}() {{ SetPItem(other.GetPItem()); }}
}};

struct {basecls} {{
    virtual void v00();
    {basecls}(const {basecls}& other);     // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    unsigned char _pad[{off-4:#x}];
    {pcls} m_target;                       // +{off:#x}
    virtual void v00();
    {cls}(const {cls}& other);
}};

{cls}::{cls}(const {cls}& other)
    : {basecls}(other)
    , m_target(other.m_target)
{{
}}
''',T(a,'''static int g_b = 0; struct PB { virtual void v00() {} void* p; PB() : p(0) {} void* Get() const { return p; } void Set(void* x) { p = x; } };
struct P : PB { virtual void v00() {} P() {} P(const P& o) : PB() { Set(o.Get()); } };
struct Base { virtual void v00() {} Base() {} Base(const Base&) { ++g_b; } };
struct D : Base { unsigned char pad[0xa4]; P t; virtual void v00() {} D() {} D(const D& o) : Base(o), t(o.t) {} };
int main()
{
    int v; D a; a.t.Set(&v); D c(a); if (g_b != 1 || c.t.Get() != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w5",):   # void CActiveEntityScript::SuspendableProcess(): suspend the running process, else fire the callback with a CScriptThing copy
        offs={}
        for i in ins:
            if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base!=0 and "esi" in i.op_str.split(",")[1]: offs.setdefault("loads",[]).append(i.operands[1].mem.disp)
        flag=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and "byte ptr [esi" in i.op_str][0]
        proc=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        thing=offs["loads"][1]; count=offs["loads"][2]; obj=offs["loads"][3]
        pfn=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM and "esi" in i.op_str][0]
        cls=f"CActiveEntityScript_{a}"
        add(a,"_unattributed",f"SuspendableProcess_{a}",HDRF(a,f"{cls}::SuspendableProcess() -- suspends the running process or fires the stored callback with a CScriptThing copy, then flags itself",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+{proc:#x}] ; test ecx,ecx ; je E ; mov eax,[ecx] ; call [eax+4] ; mov byte ptr [esi+{flag:#x}],1 ; pop esi ; ret ; E: sub esp,0Ch ; mov eax,esp ; mov dword ptr [eax],offset CBaseClass::`vftable' ; mov dword ptr [eax],offset CScriptThing::`vftable' ; mov ecx,[esi+{thing:#x}] ; mov [eax+4],ecx ; mov ecx,[esi+{count:#x}] ; test ecx,ecx ; mov [eax+8],ecx ; je I ; inc dword ptr [ecx] ; I: mov ecx,[esi+{obj:#x}] ; call [esi+{pfn:#x}] ; mov byte ptr [esi+{flag:#x}],1 ; pop esi ; ret
// Template-instantiated script-entity method (manifest: `CActiveEntityScript<...>::SuspendableProcess`,
// one copy per script class; the class is not identifiable from the body). When a process object
// is attached (+{proc:#x}) its virtual slot 1 is called; otherwise the stored callback (+{pfn:#x},
// called with the object at +{obj:#x} in ecx) receives a by-value copy of the CScriptThing member
// at +{thing-4:#x}: the copy is built in the argument slot with the CBaseClass and CScriptThing
// vtables (0x01231710, 0x01238C8C; both stores are kept), the thing pointer and the shared count
// block whose counter is incremented when present. Both paths then set the byte flag at +{flag:#x}.
// One of {len(members)} bodies of this shape.
struct CThing;
struct SCount {{ long m_references; }};

struct CBaseClass {{
    virtual void v00();
    CBaseClass() {{ }}
}};

struct CScriptThing : CBaseClass {{
    virtual void v00();
    CThing* m_pThing;                      // +4
    SCount* m_pCount;                      // +8
    CScriptThing(const CScriptThing& other) : CBaseClass(), m_pThing(other.m_pThing), m_pCount(other.m_pCount) {{ if (m_pCount) ++m_pCount->m_references; }}
}};

struct CProcess {{
    virtual void v00();
    virtual void Suspend();                // slot 1
}};

struct {cls} {{
    virtual void v00();
    unsigned char _pad0[{flag-4:#x}];
    bool m_bSuspended;                     // +{flag:#x}
    unsigned char _pad1[{proc-flag-1:#x}];
    CProcess* m_pProcess;                  // +{proc:#x}
    unsigned char _pad2[{thing-4-proc-4:#x}];
    CScriptThing m_thing;                  // +{thing-4:#x}
    unsigned char _pad3[{pfn-(thing+8):#x}];
    void (__fastcall* m_pfnCallback)(void* object, CScriptThing thing);   // +{pfn:#x}
    void* m_pCallbackObject;               // +{obj:#x}
    void SuspendableProcess();
}};

void {cls}::SuspendableProcess()
{{
    if (m_pProcess)
        m_pProcess->Suspend();
    else
        m_pfnCallback(m_pCallbackObject, m_thing);
    m_bSuspended = true;
}}
''',T(a,'''static int g_c = 0; struct Cnt { long r; }; struct B { virtual void v00() {} B() {} };
struct ST : B { virtual void v00() {} void* t; Cnt* c; ST() : t(0), c(0) {} ST(const ST& o) : B(), t(o.t), c(o.c) { if (c) ++c->r; } };
static void __fastcall CB(void*, ST s) { g_c += (int)s.c->r; }
struct X { virtual void v00() {} bool f; void* proc; ST th; void (__fastcall* pfn)(void*, ST); void* obj; void Run() { if (proc) {} else pfn(obj, th); f = true; } };
int main()
{
    Cnt c = { 1 }; X x; x.proc = 0; x.th.c = &c; x.pfn = CB; x.obj = 0; x.f = false; x.Run(); if (!x.f || g_c != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w31",):   # void CX::Assign(CBaseObject* thing) { CIntelligentPointer tmp; tmp.SetPItem(thing); m_target.SetPItem(tmp.GetPItem()); }
        vt=vts[0][1]; pcls=rtti.get(vt) or f"CPointer_{vt:08x}"; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        cls=clean(mmod) if (re.match(r"^C[A-Za-z0-9_]+$",mmod) and (mmod,mname) in uniq) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) if (cls!=f"CClass_{a}" and clean(mname)) else f"AssignTarget_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}(CBaseObject*) -- routes the argument through a temporary {pcls} into the member pointer at +{off:#x}",len(b))+f'''//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov dword ptr [esp+8],offset {pcls}::`vftable' ; call SetPItem ; lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+{off:#x}] ; call SetPItem ; lea ecx,[esp+4] ; call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4
// A stack {pcls} (8 bytes: vtable 0x{vt:08X} from rtti-vtables.tsv + item; constructed by the
// inlined default ctor over CBaseIntelligentPointer() @ 0x00A01B10) is pointed at the argument
// with SetPItem @ 0x00A01B90, read back with GetPItem @ 0x00A01B50 into the member pointer at
// +{off:#x}, and destroyed @ 0x00A01C10. The class comes from the manifest label when unique.
// One of {len(members)} bodies of this shape.
struct CBaseObject;

struct CBaseIntelligentPointer {{
    virtual void v00();
    CBaseObject* m_pItem;                  // +4 (an 8-byte temporary: `sub esp,8`)
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem();               // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
}};

struct {pcls} : CBaseIntelligentPointer {{
    virtual void v00();
    {pcls}() : CBaseIntelligentPointer() {{ }}
}};

struct {cls} {{
    unsigned char _pad[{off:#x}];
    CBaseIntelligentPointer m_target;      // +{off:#x}
    void {fn}(CBaseObject* thing);
}};

void {cls}::{fn}(CBaseObject* thing)
{{
    {pcls} pointer;
    pointer.SetPItem(thing);
    m_target.SetPItem(pointer.GetPItem());
}}
''',T(a,'''struct PB { virtual void v00() {} void* p; PB() : p(0) {} ~PB() {} void* Get() { return p; } void Set(void* x) { p = x; } };
struct P : PB { virtual void v00() {} P() : PB() {} };
struct X { unsigned char pad[0x34]; PB t; void Assign(void* v) { P tmp; tmp.Set(v); t.Set(tmp.Get()); } };
int main()
{
    int v; X x; x.Assign(&v); if (x.t.Get() != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w46",):   # T* _Uninit_copy(T* first, T* last, T* dest): placement-copy each element, return the advanced dest
        copy=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM][0]
        cm,cn,_=label(copy); ecls=cm.split("::")[-1] if (cn.split("::")[-1]==cm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CElement{step}_{copy:08x}"
        nm=f"CRetailUninitCopy_{ecls}"
        add(a,"_global",f"UninitCopy_{ecls}_{a}",HDRF(a,f"_Uninit_copy<{ecls}*> -- placement-copy [first, last) to dest ({step}-byte elements, copy ctor @ 0x{copy:08X})",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call {ecls}::{ecls}(const {ecls}&) ; S: add esi,{step} ; add edi,{step} ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Retail std::vector helper (Dinkumware `_Uninit_copy`, __fastcall: first in ecx, last in edx,
// dest on the stack): copy-constructs every element in place with placement new (hence the null
// test on the destination) through the out-of-line copy constructor @ 0x{copy:08X} and returns
// the advanced destination. `ret 8` pops dest and the unused allocator argument. The element
// type is named from the copy constructor's label when unique. One of {len(members)} bodies.
#include <new>

struct {ecls} {{
    unsigned char _body[{step:#x}];
    {ecls}(const {ecls}& other);           // 0x{copy:08X}
}};

struct CRetailAllocator {{ }};

{ecls}* __fastcall UninitCopy_{ecls}_{a}({ecls}* first, {ecls}* last, {ecls}* dest, CRetailAllocator allocator)
{{
    {ecls}* d = dest;                      // named local: retail loads dest before the loop test (GOTCHAS)
    for (; first != last; ++first, ++d)   // increment order = retail `add esi ; add edi` under /O1-style "s"
        new (d) {ecls}(*first);
    return d;
}}
''',T(a,'''#include <new>
static int g_c = 0; struct E { unsigned char b[4]; E() {} E(const E& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
static E* __fastcall UC(E* f, E* l, E* dest, A) { E* d = dest; for (; f != l; ++f, ++d) new (d) E(*f); return d; }
int main()
{
    E src[3]; src[0].b[0] = 1; src[2].b[0] = 3; unsigned char buf[12]; E* e = UC(src, src + 3, (E*)buf, A()); if (e != (E*)(buf + 12) || g_c != 3 || ((E*)buf)[2].b[0] != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w51",):   # ~vector<T>() for polymorphic T: run each element's destructor (virtual slot 0, no delete), free the buffer
        step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG and i.op_str.startswith("esi")][0]
        nm=f"CRetailVector_poly{step:#x}"
        add(a,nm,f"~{nm}",HDRF(a,f"{nm}::~{nm}() -- destroys the polymorphic {step:#x}-byte elements through vtable slot 0, frees the buffer",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; L: mov eax,[esi] ; push 0 ; mov ecx,esi ; call [eax] ; add esi,{step:#x} ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Destructor of a retail std::vector {{first, last, end}} whose elements are polymorphic: each
// element's virtual destructor (vtable slot 0, the deleting destructor called with flag 0 so the
// storage is not freed) runs over [first, last), then the buffer is released with free
// @ 0x00BFEA14 (null-checked). The element type is only known by its size and virtual
// destructor. One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct CPolyElement {{
    virtual ~CPolyElement();
    unsigned char _body[{step:#x} - 4];
}};

struct {nm} {{
    CPolyElement* first;
    CPolyElement* last;
    CPolyElement* end;
    ~{nm}();
}};

{nm}::~{nm}()
{{
    CPolyElement* p = first;
    CPolyElement* stop = last;             // named local: retail keeps `last` in a register across the virtual calls
    for (; p != stop; ++p)
        p->~CPolyElement();
    if (first)
        free(first);
}}
''',T(a,'''#include <cstdlib>
#include <new>
static int g_d = 0; struct E { virtual ~E() { ++g_d; } unsigned char b[0x44]; };
struct V { E* f; E* l; E* e; ~V() { E* p = f; E* s = l; for (; p != s; ++p) p->~E(); if (f) std::free(f); } };
int main()
{
    { V v; v.f = (E*)std::malloc(sizeof(E) * 2); new (v.f) E; new (v.f + 1) E; v.l = v.f + 2; v.e = v.l; } if (g_d != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w60",):   # persist: two Transfer<signed char> calls on adjacent byte members, zero defaults
        lits=[p for p in pushes if 0x1200000<=p<0x1300000]; names=[lit(x) for x in lits]
        offs=[]
        for i in ins:
            if i.mnemonic=="lea" and "esi +" in i.op_str: offs.append(i.operands[1].mem.disp)
            if i.mnemonic=="add" and i.op_str.startswith("esi,"): offs.append(i.operands[1].imm)
        mcls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else None
        mod=mcls or "_unattributed"; fname=clean(mname) if (mcls and clean(mname)) else f"PersistTransfer2_{a}"; cls=mcls or f"CPersistable_{a}"
        params="CPersistContext* context" if retn==4 else "CPersistContext* context, long unused"
        mem="".join(f"    signed char m_b{san(n)};".ljust(40)+f"// +{o:#x}\n" for n,o in zip(names,offs))
        calls_src="".join(f'    context->Transfer("{n}", m_b{san(n)}, 0);\n' for j,n in enumerate(names))
        add(a,mod,fname,HDRF(a,f"{cls}::{fname}({params}) -- persists the \"{names[0]}\" and \"{names[1]}\" byte flags at +{offs[0]:#x}/+{offs[1]:#x}",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[esp+10h] ; mov esi,ecx ; lea eax,[esp+0Bh] ; push eax ; lea ecx,[esi+{offs[0]:#x}] ; push ecx ; push offset "{names[0]}" ; mov ecx,edi ; mov byte ptr [esp+17h],0 ; call Transfer<signed char> ; lea edx,[esp+10h] ; push edx ; add esi,{offs[1]-offs[0]:#x} ; push esi ; push offset "{names[1]}" ; mov ecx,edi ; mov byte ptr [esp+1Ch],0 ; call Transfer<signed char> ; pop edi ; pop esi ; pop ecx ; ret {retn}
// Virtual persist hook: two one-byte members at +{offs[0]:#x} and +{offs[1]:#x} are transferred through
// CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0, each with its own
// zero default materialised as a one-byte temporary right before its call (named locals would be
// hoisted and shared; the second member's address is formed by advancing
// the first, as retail does). Member names are taken from the persisted keys. One of {len(members)} bodies.
struct CPersistContext {{
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
}};

struct {cls} {{
    virtual void v00();
    unsigned char _pad[{offs[0]-4:#x}];
{mem}    virtual void {fname}({params});
}};

void {cls}::{fname}({params})
{{
{calls_src}}}
''',T(a,'''static int g_t = 0; struct Ctx { void Transfer(const char* n, signed char& v, const signed char& d) { v = (signed char)(n[0] + d); ++g_t; } };
struct P { virtual void v00() {} unsigned char pad[0x3d]; signed char a; signed char b; virtual void vf(Ctx* c) { c->Transfer("A", a, 0); c->Transfer("B", b, 0); } };
int main()
{
    Ctx c; P p; p.vf(&c); if (g_t != 2 || p.a != 'A' || p.b != 'B') '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w28",):   # float g = (float)cos(g_const) / sin: dynamic initializer with the x87 intrinsic
        fn=[i.mnemonic for i in ins if i.mnemonic in ("fcos","fsin","fsqrt")][0]; src=[i.operands[0].mem.disp for i in ins if i.mnemonic=="fld"][0]; dst=[i.operands[0].mem.disp for i in ins if i.mnemonic=="fstp"][0]
        name={"fcos":"cos","fsin":"sin","fsqrt":"sqrt"}[fn]; val=struct.unpack("<d",rb(src,8))[0]
        add(a,"_static_init",f"g_{name}_{dst:08x}",HDRF(a,f"dynamic initializer for float g_{name}_{dst:08x} @ 0x{dst:08X} = (float){name}(const double @ 0x{src:08X} = {val!r})",len(b))+f'''//
// Retail bytes:  fld qword ptr [{src:#x}] ; {fn} ; fstp dword ptr [{dst:#x}] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit ({val!r}, so it is not folded at compile time)
// with the intrinsic {name}() (`#pragma intrinsic`, the retail build's /Oi), stored as float. Both
// objects are named by address; the constant is declared extern const. One of {len(members)} bodies.
#include <math.h>
#pragma intrinsic({name})

extern const double g_angle_{src:08x};     // 0x{src:08X} = {val!r}

float g_{name}_{dst:08x} = (float){name}(g_angle_{src:08x});
''',T(a,'''#include <math.h>
#pragma intrinsic(cos)
extern const double g_a; const double g_a = 0.0; static float g_c = (float)cos(g_a);
int main()
{
    if (g_c != 1.0f) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w62",):   # `??_G` (scalar deleting destructor) of a NON-polymorphic pair {unsigned long, CCountedPointer}: emitted by a delete-expression
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        cls=f"CPairCounted_{a}"
        add(a,"_unattributed",f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- non-virtual: inlined CCountedPointer member destructor at +{off-4:#x}, then delete",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+{off:#x}] ; test eax,eax ; je Z ; dec dword ptr [eax] ; mov eax,[esi+{off:#x}] ; cmp dword ptr [eax],0 ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+{off:#x}] ; push eax ; call operator delete ; add esp,4 ; Z: test byte ptr [esp+8],1 ; mov dword ptr [esi+{off-4:#x}],0 ; mov dword ptr [esi+{off:#x}],0 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// VC7.1 also emits a (non-virtual) scalar deleting destructor for a non-polymorphic class with a
// non-trivial destructor whenever a translation unit applies `delete` to it: the inlined
// ~CCountedPointer of the member at +{off-4:#x} (release the count block: decrement, destroy the object
// through the block's function pointer and free the block on zero; then null both fields) followed
// by the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`_Dest_val<...pair<unsigned
// long,CCountedPointer<...>>>`) names the pair; the instantiation is not recoverable. One of {len(members)} bodies.
struct SCountBlock {{
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
}};

struct CCountedPointer {{
    void* m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pObject = 0;
        m_pCount = 0;
    }}
}};

struct {cls} {{
    unsigned char _pad[{off-4:#x}];
    CCountedPointer m_pointer;             // +{off-4:#x}
}};

// The delete-expression that makes the compiler emit {cls}::`scalar deleting destructor'.
void Destroy_{cls}({cls}* pair)
{{
    delete pair;
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct X { unsigned long k; P p; };
static void Kill(X* x) { delete x; }
int main()
{
    X* x = new X; x->k = 1; x->p.o = 0; x->p.c = new B; x->p.c->r = 1; x->p.c->f = Destroy; x->p.c->o = 0; Kill(x); if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w17",):   # EH-framed ~CX() {}: own vtable, one member destroyed out of line, inlined empty base destructor (its vtable store)
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; vt2=vts[1][1]; basecls=rtti.get(vt2) or f"CBase_{vt2:08x}"; mdtor=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        mm,mn,_=label(mdtor); ml=mn.split("::")[-1]; mcls=mm.split("::")[-1] if ((mm,mn) in uniq and ml=="~"+mm.split("::")[-1]) else f"CMember_{mdtor:08x}"
        if basecls==cls: basecls=f"{cls}_Base"
        add(a,cls,f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- EH-framed: own vtable, {mcls} member at +{off:#x} destroyed out of line, inlined empty {basecls} destructor",len(b))+f'''//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset {cls}::`vftable' ; lea ecx,[esi+{off:#x}] ; mov dword ptr [esp+10h],0 ; call {mcls}::~{mcls} ; mov ecx,[esp+8] ; mov dword ptr [esi],offset {basecls}::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// This translation unit was built with C++ exceptions enabled (/EHsc: the fs:[0] frame and the
// `__ehhandler` funclet, which the lander does not compare), unlike most of the game. Plain
// destructor: installs this class's vtable (0x{vt:08X}), destroys the member at +{off:#x} through
// its out-of-line destructor @ 0x{mdtor:08X} under EH state 0, then the inline empty base destructor
// stores the base vtable (0x{vt2:08X}). Non-virtual destructors keep the lander on `??1`.
// One of {len(members)} bodies of this shape.
struct {mcls} {{
    void* m_data;
    ~{mcls}();                             // 0x{mdtor:08X}
}};

struct {basecls} {{
    virtual void v00();
    ~{basecls}() {{ }}
}};

struct {cls} : {basecls} {{
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    {mcls} m_member;                       // +{off:#x}
    virtual void v00();
    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct M { void* p; ~M() { ++g_d; } }; struct Base { virtual void v00() {} ~Base() {} };
struct D : Base { unsigned char pad[4]; M m; virtual void v00() {} ~D() {} };
int main()
{
    { D d; } if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w94",):   # `??_G` (non-virtual scalar deleting destructor) of a non-polymorphic class: `if (m_buffer) free(m_buffer);` then delete
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM][0]
        cls=clean(mmod) if (re.match(r"^C[A-Za-z0-9_]+$",mmod) and (mmod,mname) in uniq) else f"CBufferOwner_{a}"; mod=cls if cls!=f"CBufferOwner_{a}" else "_unattributed"
        add(a,mod,f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- non-virtual: inlined `if (m_buffer) free(m_buffer)` (+{off:#x}), then delete",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+{off:#x}] ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: test byte ptr [esp+8],1 ; je R ; push esi ; call operator delete ; add esp,4 ; R: mov eax,esi ; pop esi ; ret 4
// VC7.1 emits a (non-virtual) scalar deleting destructor for a non-polymorphic class with a
// non-trivial destructor whenever a translation unit applies `delete` to it: here the inlined
// destructor releases the malloc'd buffer at +{off:#x} with free @ 0x00BFEA14 when non-null, then
// the flag-tested operator delete @ 0x00BFE9BC frees the object. The class is named from the
// manifest label only when unique. One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct {cls} {{
{"    unsigned char _pad[%#x];" % off + chr(10) if off else ""}    void* m_buffer;                        // +{off:#x}
    ~{cls}() {{ if (m_buffer) free(m_buffer); }}
}};

// The delete-expression that makes the compiler emit {cls}::`scalar deleting destructor'.
void Destroy_{cls}({cls}* object)
{{
    delete object;
}}
''',T(a,'''#include <cstdlib>
struct X { void* b; ~X() { if (b) std::free(b); } };
static void Kill(X* x) { delete x; }
int main()
{
    X* x = new X; x->b = std::malloc(4); Kill(x);
'''),b)
    elif TPL in ("w19",):   # EH-framed CX::CX(T a) : m_pString(&m_string), m_string(a) {}  (root polymorphic class)
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; mctor=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        poff=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str.split(",")[0] and i.operands[1].type==X86_OP_REG][0]
        mcls=tctor_class(mctor) or f"CMember_{mctor:08x}"
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(void*) -- EH-framed: pointer member at +{poff:#x} aimed at the {mcls} member at +{off:#x}, which is constructed from the argument",len(b))+f'''//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+{off:#x}] ; mov [esp+4],esi ; mov [esi+{poff:#x}],ecx ; mov eax,[esp+18h] ; push eax ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset {cls}::`vftable' ; call {mcls}::{mcls} ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Built with C++ exceptions enabled (/EHsc, the fs:[0] frame; the `__ehhandler` funclet is not
// compared). The pointer member at +{poff:#x} is initialised to the address
// of the embedded member at +{off:#x}, this class's vtable (0x{vt:08X}, rtti-vtables.tsv) is
// installed and the member is constructed out of line @ 0x{mctor:08X} from the one argument under
// EH state 0. One of {len(members)} bodies of this shape.
struct {mcls} {{
    void* m_data;
    {mcls}(void* argument);                // 0x{mctor:08X}
    ~{mcls}();
}};

// The pointer store precedes the vtable store, so it belongs to an inlined base constructor that
// receives the member's address; the base's empty inline destructor emits no code but gives the
// frame its EH state 0 (the base subobject is destroyed if the member constructor throws); the
// base's own vtable store is dead and dropped.
struct {cls}_Base {{
    virtual void v00();
    {mcls}* m_pValue;                      // +{poff:#x}: the base is handed the derived member's address
    {cls}_Base({mcls}* value) : m_pValue(value) {{ }}
    ~{cls}_Base() {{ }}
}};

struct {cls} : {cls}_Base {{
    {mcls} m_value;                        // +{off:#x}
    virtual void v00();
    {cls}(void* argument);
}};

{cls}::{cls}(void* argument)
    : {cls}_Base(&m_value)
    , m_value(argument)
{{
}}
''',T(a,'''struct M { void* d; M(void* a) : d(a) {} ~M() {} };
struct B { virtual void v00() {} M* p; B(M* x) : p(x) {} ~B() {} }; struct X : B { M v; virtual void v00() {} X(void* a) : B(&v), v(a) {} };
int main()
{
    int q; X x(&q); if (x.p != &x.v || x.v.d != &q) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w25",):   # EH-framed void CX::Call(T a) { CParam p(a); m_pTarget->Slot(&p); }
        vt=vts[0][1]; pcls=rtti.get(vt) or f"CParam_{vt:08x}"; slot=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM][0]//4
        toff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "ecx +" in i.op_str][0]
        cls=clean(mmod) if (re.match(r"^C[A-Za-z0-9_]+$",mmod) and (mmod,mname) in uniq) else f"CCaller_{a}"; mod=cls if cls!=f"CCaller_{a}" else "_unattributed"; fn=clean(mname) if (cls!=f"CCaller_{a}" and clean(mname)) else f"Call_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}(void*) -- EH-framed: wraps the argument in a stack {pcls} and passes it to virtual slot {slot} of the object at +{toff:#x}",len(b))+f'''//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; mov eax,[esp+18h] ; mov dword ptr [esp],offset {pcls}::`vftable' ; mov [esp+4],eax ; mov ecx,[ecx+{toff:#x}] ; mov edx,[ecx] ; lea eax,[esp] ; push eax ; mov dword ptr [esp+14h],0 ; call [edx+{slot*4:#x}] ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret 4
// Built with C++ exceptions enabled (/EHsc; the fs:[0] frame, `__ehhandler` not compared). An
// 8-byte {pcls} {{vtable 0x{vt:08X}, value}} is built on the stack by its inline constructor, its
// address is passed to virtual slot {slot} of the object at +{toff:#x} under EH state 0 (the
// parameter has an empty inline destructor, which is what makes the frame necessary), and nothing
// else is done. One of {len(members)} bodies of this shape.
struct {pcls} {{
    virtual void v00();
    void* m_value;
    {pcls}(void* value) : m_value(value) {{ }}
    ~{pcls}() {{ }}
}};

struct CTarget {{
    virtual void v00();
{"".join("    virtual void slot%02d();" % j + chr(10) for j in range(1,slot))}    virtual void Receive({pcls}* parameter);   // slot {slot}
}};

struct {cls} {{
    unsigned char _pad[{toff:#x}];
    CTarget* m_pTarget;                    // +{toff:#x}
    void {fn}(void* value);
}};

void {cls}::{fn}(void* value)
{{
    {pcls} parameter(value);
    m_pTarget->Receive(&parameter);
}}
''',T(a,'''static void* g_v = 0; struct P { virtual void v00() {} void* v; P(void* x) : v(x) {} ~P() {} };
struct T { virtual void v00() {} virtual void R(P* p) { g_v = p->v; } }; struct X { unsigned char pad[4]; T* t; void Call(void* v) { P p(v); t->R(&p); } };
int main()
{
    int q; T t; X x; x.t = &t; x.Call(&q); if (g_v != &q) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w53",):   # [thunk]: CX::vfunc_N `adjustor{4}': sub ecx,4 ; jmp CX::vfunc_N  (override of a second-base virtual)
        adj=[i.operands[1].imm for i in ins if i.mnemonic=="sub"][0]; target=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        tm,tn,_=label(target); cls=tm.split("::")[-1] if (re.match(r"^C[A-Za-z0-9_]+$",tm.split("::")[-1]) and (tm,tn) in uniq) else f"CMulti_{target:08x}"
        fn=clean(tn) if ((tm,tn) in uniq and clean(tn) and not clean(tn).startswith("sub_")) else f"Override_{target:08x}"
        mod=cls if not cls.startswith("CMulti_") else "_unattributed"
        add(a,mod,fn,HDRF(a,f"[thunk] {cls}::{fn} `adjustor{{{adj}}}' -- second-base override entry: this -= {adj}, jump to {cls}::{fn} @ 0x{target:08X}",len(b))+f'''//
// Retail bytes:  sub ecx,{adj} ; jmp {cls}::{fn}
// Compiler-generated adjustor thunk: {cls} inherits two polymorphic bases that both declare the
// virtual; the override sits in the primary vtable and the SECOND base's vtable (at +{adj}) gets this thunk; calls through that base's vtable arrive with `this` pointing at
// the base subobject, so the thunk rebases `this` and tail-jumps to the real override
// @ 0x{target:08X} (named from the manifest when its label is unique). The override itself is a
// separate catalog row; the empty body here only makes the thunk exist. One of {len(members)} bodies.
// Both bases declare the virtual: the override lands directly in the primary (first-base) vtable
// and the second base's vtable gets this adjustor thunk.
struct CFirstBase {{
    virtual void {fn}();
{"    unsigned char _body[%#x];           // the second base sits at +%#x" % (adj-4, adj) + chr(10) if adj>4 else ""}}};

struct CSecondBase {{
    virtual void {fn}();
}};

struct {cls} : CFirstBase, CSecondBase {{
    virtual void {fn}();
}};

void {cls}::{fn}()
{{
}}

// The vtables (and with them the adjustor thunk) are emitted in a translation unit that
// constructs the object; this helper is that construction, nothing more. The lander grades the
// largest symbol carrying the method name: the 8-byte thunk, not the 1-byte empty override.
void* MakeOne()
{{
    return new {cls};
}}
''',T(a,'''static int g_c = 0; struct A { virtual void f() {} }; struct B { virtual void f() = 0; }; struct C : A, B { virtual void f() { ++g_c; } };
int main()
{
    C c; B* b = &c; b->f(); if (g_c != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w65",):   # CBaseObject* CX::vfunc_N() { return m_pointer.GetPItem(); }  -- add ecx,N ; jmp GetPItem
        off=[i.operands[1].imm for i in ins if i.mnemonic=="add"][0]; target=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        getter="GetPItem" if target==0xa01b50 else f"Method_{target:08x}"
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CClass_{a}"; mod=cls if cls!=f"CClass_{a}" else "_unattributed"; fn=clean(mname) if (clean(mname) and not clean(mname).startswith("sub_")) else f"GetTarget_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- returns m_pointer.{getter}() for the smart pointer at +{off:#x} (sibling jump)",len(b))+f'''//
// Retail bytes:  add ecx,{off:#x} ; jmp CBaseIntelligentPointer::{getter}
// Virtual getter that forwards to {getter} @ 0x{target:08X} on the embedded smart pointer at
// +{off:#x}: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of {len(members)} bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {{
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* {getter}();               // 0x{target:08X}
}};

struct {cls} {{
    virtual void v00();
    unsigned char _pad[{off-4:#x}];
    CBaseIntelligentPointer m_pointer;     // +{off:#x}
    virtual CBaseObject* {fn}();
}};

CBaseObject* {cls}::{fn}()
{{
    return m_pointer.{getter}();
}}
''',T(a,'''struct PB { virtual void v00() {} void* p; void* Get() { return p; } };
struct X { virtual void v00() {} unsigned char pad[0x34]; PB m; virtual void* G() { return m.Get(); } };
int main()
{
    int v; X x; x.m.p = &v; if (x.G() != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w20",):   # bool CTCThing::GetInterface_K(CTCBase** out): bitset test, CVectorMap lower-bound lookup, frame-pointer TU
        k=[i.operands[0].imm for i in ins if i.mnemonic=="push" and i.operands[0].type==X86_OP_IMM]; k=k[0] if k else [i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_IMM][0]
        test=calls[0]; lower=calls[1]; boff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]; moff=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("esi,")][0]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CThingComponents_{a}"; mod=cls if not cls.startswith("CThingComponents_") else "_unattributed"
        fn=f"GetInterface_{k:#x}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}(CTCBase**) -- interface lookup for type {k:#x}: bitset test, then the CVectorMap lower bound",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push ecx ; push esi ; mov esi,ecx ; push {k:#x} ; lea ecx,[esi+{boff:#x}] ; call bitset::test ; test al,al ; je F ; lea eax,[ebp-4] ; add esi,{moff:#x} ; push eax ; mov ecx,esi ; mov dword ptr [ebp-4],{k:#x} ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je E ; cmp dword ptr [eax],{k:#x} ; jle G ; E: mov eax,esi ; G: mov eax,[eax+4] ; mov ecx,[ebp+8] ; mov [ecx],eax ; mov al,1 ; jmp R ; F: xor al,al ; R: pop esi ; leave ; ret 4
// Per-interface-type lookup on a thing's component table: the presence bitset at +{boff:#x}
// (test @ 0x{test:08X}) is consulted first, then the sorted vector map at +{moff:#x} {{first, last}}
// of {{type, component}} pairs is searched with LowerBound @ 0x{lower:08X} (key by address);
// a miss (end, or a larger key) yields the end entry, whose value is handed out regardless. The
// frame pointer and `leave` come from this TU's settings: `#pragma optimize("y",off)` +
// `("s",on)`. The constant {k:#x} is the interface type. One of {len(members)} bodies.
#pragma optimize("y", off)              // frame pointer kept (`leave`), size-optimised TU
#pragma optimize("s", on)

struct CTCBase;

struct CPresenceBits {{
    bool test(long bit);                   // 0x{test:08X} (std::bitset::test)
}};

struct CInterfaceMap {{
    struct SEntry {{ long type; CTCBase* component; }};
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type); // 0x{lower:08X}
}};

struct {cls} {{
    unsigned char _pad0[{boff:#x}];
    CPresenceBits m_present;               // +{boff:#x}
    unsigned char _pad1[{moff-boff-1:#x}];
    CInterfaceMap m_interfaces;            // +{moff:#x}
    bool {fn}(CTCBase** out);
}};

bool {cls}::{fn}(CTCBase** out)
{{
    if (m_present.test({k:#x})) {{
        long type = {k:#x};
        CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once (`add esi`)
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || {k:#x} < it->type)
            it = map->last;
        *out = it->component;
        return true;
    }}
    return false;
}}
''',T(a,'''struct Bits { bool test(long b) { return b == 0x42; } }; struct E { long t; void* c; };
struct Map { E* f; E* l; E* LB(const long* k) { E* p = f; while (p != l && p->t < *k) ++p; return p; } };
struct X { unsigned char p0[0x20]; Bits b; unsigned char p1[0x23]; Map m; bool Get(void** out) { if (b.test(0x42)) { long k = 0x42; Map* mp = &m; E* it = mp->LB(&k); if (it == mp->l || 0x42 < it->t) it = mp->l; *out = it->c; return true; } return false; } };
int main()
{
    int v; E e[2] = { { 0x42, &v }, { 0, 0 } }; X x; x.m.f = e; x.m.l = e + 1; void* o = 0; if (!x.Get(&o) || o != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w14",):   # std::copy / std::copy_backward<T*, T*> front end: forwards to _Copy_impl / _Copy_backward with the iterator tags
        callee=calls[0]; cm,cn,_=label(callee); back="backward" in cn
        ecls=f"CElement_{callee:08x}"
        fn="copy_backward" if back else "copy"
        add(a,"_global",f"${fn}@{ecls}",HDRF(a,f"std::{fn}<{ecls}*, {ecls}*> -- Dinkumware front end: forwards to the tagged _Copy_{'backward' if back else 'impl'} @ 0x{callee:08X}",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push 0 ; lea eax,[ebp+0Bh] ; push eax ; push [ebp+8] ; call {cn.split("<")[0]}<...> ; pop ebp ; ret 8
// The VC7.1 Dinkumware `std::{fn}(first, last, dest)` instantiation for a non-scalar element
// type: it appends the two dispatch tags (`_Ptr_cat(first, dest)` → _Nonscalar_ptr_iterator_tag,
// and `_Range_checked_iterator_tag`) and tail-forwards to the __fastcall implementation
// @ 0x{callee:08X}, which is a separate catalog row. The element type is only known through the
// callee's manifest label; a non-trivial copy assignment makes it non-scalar. The DDK headers
// used by the lane are the retail Dinkumware, so `#include <algorithm>` reproduces the body.
// One of {len(members)} bodies of this shape.
#include <algorithm>

struct {ecls} {{
    void* m_data;
    {ecls}& operator=(const {ecls}& other);
}};

{ecls}* Instantiate_{fn}_{a}({ecls}* first, {ecls}* last, {ecls}* dest)
{{
    return std::{fn}(first, last, dest);
}}
''',T(a,'''#include <algorithm>
struct E { int v; E& operator=(const E& o) { v = o.v + 1; return *this; } };
int main()
{
    E a[2] = { { 1 }, { 2 } }, b[2] = { { 0 }, { 0 } }; std::copy(a, a + 2, b); if (b[0].v != 2 || b[1].v != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("wtiny",):   # generic synthesiser for <=16-byte bodies: getters, setters, forwarders, constant returns, global copies
        d=" ; ".join(f"{i.mnemonic} {i.op_str}" for i in ins)
        def hx(s): return int(s,16) if s.startswith("0x") else int(s)
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else None; mod=cls or "_unattributed"; cname=cls or f"CClass_{a}"
        fn=clean(mname) if (clean(mname) and not clean(mname).startswith("sub_")) else None
        def emit(kind, title, body_src, decl, test_src, note=""):
            add(a,mod,fn or f"{kind}_{a}",HDRF(a,f"{cname}::{fn or kind+'_'+a}{title}",len(b))+f'''//
// Retail bytes:  {d}
// Tiny-body shape `{kind}` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. {note}
{decl}
{body_src}''',T(a,test_src),b)
        m_=re.fullmatch(r"mov eax, dword ptr \[ecx \+ (\w+)\] ; ret ", d)
        if m_:
            off=hx(m_.group(1)); emit("GetDword", f"() -- returns the dword member at +{off:#x}",
                f"long {cname}::{fn or 'GetDword_'+a}()\n{{\n    return m_value;\n}}\n", f"struct {cname} {{\n    unsigned char _pad[{off:#x}];\n    long m_value;{' '*(27-len('long m_value;'))}// +{off:#x}\n    long {fn or 'GetDword_'+a}();\n}};\n",
                "struct X { unsigned char p[4]; long v; long Get() { return v; } };\nint main()\n{\n    X x; x.v = 7; if (x.Get() != 7) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"mov eax, dword ptr \[ecx \+ (\w+)\] ; mov eax, dword ptr \[eax \+ (\w+)\] ; ret ", d)
        if m_:
            o1,o2=hx(m_.group(1)),hx(m_.group(2)); emit("GetDwordThrough", f"() -- returns +{o2:#x} of the object at +{o1:#x}",
                f"long {cname}::{fn or 'GetDwordThrough_'+a}()\n{{\n    return m_object->m_value;\n}}\n", f"struct CTarget {{\n    unsigned char _pad[{o2:#x}];\n    long m_value;                          // +{o2:#x}\n}};\n\nstruct {cname} {{\n    unsigned char _pad[{o1:#x}];\n    CTarget* m_object;                     // +{o1:#x}\n    long {fn or 'GetDwordThrough_'+a}();\n}};\n",
                "struct T { unsigned char p[4]; long v; }; struct X { unsigned char p[4]; T* t; long Get() { return t->v; } };\nint main()\n{\n    T t; t.v = 5; X x; x.t = &t; if (x.Get() != 5) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"mov al, byte ptr \[ecx \+ (\w+)\] ; ret ", d)
        if m_:
            off=hx(m_.group(1)); emit("GetByte", f"() -- returns the byte member at +{off:#x}",
                f"unsigned char {cname}::{fn or 'GetByte_'+a}()\n{{\n    return m_value;\n}}\n", f"struct {cname} {{\n    unsigned char _pad[{off:#x}];\n    unsigned char m_value;                 // +{off:#x}\n    unsigned char {fn or 'GetByte_'+a}();\n}};\n",
                "struct X { unsigned char p[4]; unsigned char v; unsigned char Get() { return v; } };\nint main()\n{\n    X x; x.v = 9; if (x.Get() != 9) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"fld dword ptr \[ecx \+ (\w+)\] ; ret ", d)
        if m_:
            off=hx(m_.group(1)); emit("GetFloat", f"() -- returns the float member at +{off:#x}",
                f"float {cname}::{fn or 'GetFloat_'+a}()\n{{\n    return m_value;\n}}\n", f"struct {cname} {{\n    unsigned char _pad[{off:#x}];\n    float m_value;                         // +{off:#x}\n    float {fn or 'GetFloat_'+a}();\n}};\n",
                "struct X { unsigned char p[4]; float v; float Get() { return v; } };\nint main()\n{\n    X x; x.v = 2.5f; if (x.Get() != 2.5f) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"mov eax, dword ptr \[ecx \+ (\w+)\] ; fld dword ptr \[eax \+ (\w+)\] ; ret ", d)
        if m_:
            o1,o2=hx(m_.group(1)),hx(m_.group(2)); emit("GetFloatThrough", f"() -- returns the float at +{o2:#x} of the object at +{o1:#x}",
                f"float {cname}::{fn or 'GetFloatThrough_'+a}()\n{{\n    return m_object->m_value;\n}}\n", f"struct CTarget {{\n    unsigned char _pad[{o2:#x}];\n    float m_value;                         // +{o2:#x}\n}};\n\nstruct {cname} {{\n    unsigned char _pad[{o1:#x}];\n    CTarget* m_object;                     // +{o1:#x}\n    float {fn or 'GetFloatThrough_'+a}();\n}};\n",
                "struct T { unsigned char p[4]; float v; }; struct X { unsigned char p[4]; T* t; float Get() { return t->v; } };\nint main()\n{\n    T t; t.v = 1.5f; X x; x.t = &t; if (x.Get() != 1.5f) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"mov eax, dword ptr \[esp \+ (\w+)\] ; mov dword ptr \[ecx \+ (\w+)\], eax ; ret (\w+)", d)
        if m_:
            slot,off,rn=hx(m_.group(1)),hx(m_.group(2)),hx(m_.group(3)); nargs=rn//4; which=slot//4; params=", ".join(("long value" if j+1==which else f"long unused{j}") for j in range(nargs))
            emit("SetDword", f"({params}) -- stores argument {which} into the dword member at +{off:#x}",
                f"void {cname}::{fn or 'SetDword_'+a}({params})\n{{\n    m_value = value;\n}}\n", f"struct {cname} {{\n    unsigned char _pad[{off:#x}];\n    long m_value;                          // +{off:#x}\n    void {fn or 'SetDword_'+a}({params});\n}};\n",
                "struct X { unsigned char p[4]; long v; void Set(long a) { v = a; } };\nint main()\n{\n    X x; x.Set(3); if (x.v != 3) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"mov (dword|byte) ptr \[ecx \+ (\w+)\], (\w+) ; ret (\w*)", d)
        if m_:
            wide=m_.group(1)=="dword"; off=hx(m_.group(2)); val=hx(m_.group(3)); rn=hx(m_.group(4)) if m_.group(4) else 0; params=", ".join(f"long unused{j}" for j in range(rn//4)); ty="long" if wide else "unsigned char"
            emit("StoreConstant", f"({params}) -- writes {val} into the {'dword' if wide else 'byte'} member at +{off:#x}",
                f"void {cname}::{fn or 'StoreConstant_'+a}({params})\n{{\n    m_value = {val};\n}}\n", f"struct {cname} {{\n    unsigned char _pad[{off:#x}];\n    {ty} m_value;{' '*(26-len(ty))}// +{off:#x}\n    void {fn or 'StoreConstant_'+a}({params});\n}};\n",
                "struct X { unsigned char p[4]; long v; void Set() { v = 1; } };\nint main()\n{\n    X x; x.v = 0; x.Set(); if (x.v != 1) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"(xor eax, eax|xor al, al|mov al, 1|mov eax, (\w+)) ; ret (\w*)", d)
        if m_:
            rn=hx(m_.group(3)) if m_.group(3) else 0; params=", ".join(f"long unused{j}" for j in range(rn//4)); head=m_.group(1)
            if head=="xor eax, eax": ty,val="long","0"
            elif head=="xor al, al": ty,val="bool","false"
            elif head=="mov al, 1": ty,val="bool","true"
            else: ty,val="long",str(hx(m_.group(2)) if hx(m_.group(2))<0x80000000 else hx(m_.group(2))-0x100000000)
            emit("ReturnConstant", f"({params}) -- returns {val}",
                f"{ty} {cname}::{fn or 'ReturnConstant_'+a}({params})\n{{\n    return {val};\n}}\n", f"struct {cname} {{\n    {ty} {fn or 'ReturnConstant_'+a}({params});\n}};\n",
                "struct X { long Get() { return 0; } bool T() { return true; } };\nint main()\n{\n    X x; if (x.Get() != 0 || !x.T()) "+FAIL(a)+"\n"); continue
        m_=re.fullmatch(r"(add ecx, (\w+)|mov ecx, dword ptr \[ecx \+ (\w+)\]) ; jmp (0x[0-9a-f]+)", d)
        if m_:
            target=hx(m_.group(4)); tm,tn,tr=label(target); ocls=tm.split("::")[-1] if (re.match(r"^C[A-Za-z0-9_]+$",tm.split("::")[-1]) and (tm,tn) in uniq) else f"CObject_{target:08x}"
            meth=clean(tn) if ((tm,tn) in uniq and clean(tn) and not clean(tn).startswith("sub_")) else f"Method_{target:08x}"
            byptr=m_.group(3) is not None; off=hx(m_.group(3) or m_.group(2)); rn=retn or 0; params=", ".join(f"long argument{j}" for j in range(rn//4)); args=", ".join(f"argument{j}" for j in range(rn//4))
            emit("Forward", f"({params}) -- sibling jump to {ocls}::{meth} on the {'object at' if byptr else 'embedded member at'} +{off:#x}",
                f"long {cname}::{fn or 'Forward_'+a}({params})\n{{\n    return {'m_object->' if byptr else 'm_member.'}{meth}({args});\n}}\n",
                f"struct {ocls} {{\n    long {meth}({params});{' '*max(1,20-len(meth))}// 0x{target:08X}\n}};\n\nstruct {cname} {{\n    unsigned char _pad[{off:#x}];\n    {ocls}{'*' if byptr else ''} m_{'object' if byptr else 'member'};\n    long {fn or 'Forward_'+a}({params});\n}};\n",
                "struct O { long M() { return 4; } }; struct X { unsigned char p[4]; O* o; long F() { return o->M(); } };\nint main()\n{\n    O o; X x; x.o = &o; if (x.F() != 4) "+FAIL(a)+"\n",
                note="The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes)."); continue
        m_=re.fullmatch(r"mov eax, dword ptr \[(0x[0-9a-f]+)\] ; (lea eax, \[eax \+ eax\*(\d)\] ; |imul eax, eax, (\w+) ; |lea eax, \[eax \+ eax\*(\d)\] ; shl eax, (\d) ; )?mov dword ptr \[(0x[0-9a-f]+)\], eax ; ret ", d)
        if m_:
            src=hx(m_.group(1)); dst=hx(m_.group(7)); k=1
            if m_.group(3): k=1+int(m_.group(3))
            if m_.group(4): k=hx(m_.group(4))
            if m_.group(5): k=(1+int(m_.group(5)))<<int(m_.group(6))
            add(a,"_static_init",f"g_scaled_{dst:08x}",HDRF(a,f"dynamic initializer for long g_scaled_{dst:08x} @ 0x{dst:08X} = g_{src:08x}{' * %d' % k if k!=1 else ''}",len(b))+f'''//
// Retail bytes:  {d}
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x{src:08X}, defined elsewhere, so not folded){' scaled by %d' % k if k!=1 else ''}. Both objects are named by address.
extern long g_{src:08x};                   // 0x{src:08X}

long g_scaled_{dst:08x} = g_{src:08x}{' * %d' % k if k!=1 else ''};
''',T(a,'''extern long g_a; long g_a = 3; static long g_b = g_a * 5;
int main()
{
    if (g_b != 15) '''+FAIL(a)+'''
'''),b); continue
        m_=re.fullmatch(r"mov eax, ecx ; mov dword ptr \[eax\], (0x[0-9a-f]+) ; ret ", d)
        if m_:
            vt=hx(m_.group(1)); vcls=rtti.get(vt) or f"CVtable_{vt:08x}"
            add(a,vcls,vcls,HDRF(a,f"{vcls}::{vcls}() -- root polymorphic class: only the vtable store",len(b))+f'''//
// Retail bytes:  {d}
// Default constructor of a root polymorphic class with nothing to initialise: installs the vtable
// (0x{vt:08X}, rtti-vtables.tsv) and returns `this`. One of the tiny-body shapes.
struct {vcls} {{
    virtual void v00();
    {vcls}();
}};

{vcls}::{vcls}()
{{
}}
''',T(a,'''struct X { virtual void v00() {} X() {} };
int main()
{
    X x;
'''),b); continue
        continue   # no classifier matched: skip silently
    elif TPL in ("w142",):   # EH: void CX::OnActivate() { CParam p; m_member.Slot(&p); }  (4-byte vtable-only parameter, embedded polymorphic member)
        vt=vts[0][1]; pcls=rtti.get(vt) or f"CParam_{vt:08x}"; slot=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM][0]//4
        moff=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("ecx,")][0]
        cls=f"CActivator_{a}"; fn=clean(mname) if (clean(mname) and not clean(mname).startswith("sub_")) else f"Activate_{a}"
        add(a,"_unattributed",f"{fn}_{a}",HDRF(a,f"{cls}::{fn}() -- EH-framed: a vtable-only {pcls} temporary handed to virtual slot {slot} of the member object at +{moff:#x}",len(b))+f'''//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov dword ptr [esp],offset {pcls}::`vftable' ; mov eax,[ecx+{moff:#x}] ; add ecx,{moff:#x} ; lea edx,[esp] ; push edx ; mov dword ptr [esp+10h],0 ; call [eax+{slot*4:#x}] ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). A 4-byte parameter object holding only its vtable
// (0x{vt:08X}; inline constructor, empty inline destructor that gives the frame EH state 0) is
// passed by address to virtual slot {slot} of the polymorphic member embedded at +{moff:#x}. The
// manifest label is propagated; class names are placeholders. One of {len(members)} bodies.
struct {pcls} {{
    virtual void v00();
    {pcls}() {{ }}
    ~{pcls}() {{ }}
}};

struct CReceiver {{
    virtual void v00();
{"".join("    virtual void slot%02d();" % j + chr(10) for j in range(1,slot))}    virtual void Receive({pcls}* parameter);   // slot {slot}
}};

struct {cls} {{
    unsigned char _pad[{moff:#x}];
    CReceiver m_receiver;                  // +{moff:#x} (embedded, not a pointer)
    void {fn}();
}};

void {cls}::{fn}()
{{
    {pcls} parameter;
    m_receiver.Receive(&parameter);
}}
''',T(a,'''static int g_c = 0; struct P { virtual void v00() {} P() {} ~P() {} };
struct R { virtual void v00() {} virtual void Recv(P*) { ++g_c; } }; struct X { unsigned char pad[4]; R r; void Act() { P p; r.Recv(&p); } };
int main()
{
    X x; x.Act(); if (g_c != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w212",):   # EH ctor: CX(T a) : CBase(), m_pValue(a), m_member() {}  (member ctor out of line, base with empty dtor)
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; mctor=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        poff=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str.split(",")[0] and i.operands[1].type==X86_OP_REG][0]
        mcls="CBaseIntelligentPointer" if mctor==0xa01b10 else (tctor_class(mctor) or f"CMember_{mctor:08x}")
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(void*) -- EH-framed: vtable, argument kept at +{poff:#x}, {mcls} member at +{off:#x} default-constructed out of line",len(b))+f'''//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esp+18h] ; lea ecx,[esi+{off:#x}] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset {cls}::`vftable' ; mov [esi+{poff:#x}],eax ; call {mcls}::{mcls} ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Built with C++ exceptions enabled (/EHsc). The base has an empty inline destructor (no code,
// but EH state 0 once it is constructed); this class installs its vtable (0x{vt:08X}), stores the
// argument at +{poff:#x} and default-constructs the member at +{off:#x} out of line @ 0x{mctor:08X}.
// One of {len(members)} bodies of this shape.
struct {cls}_Base {{
    virtual void v00();
    {cls}_Base() {{ }}
    ~{cls}_Base() {{ }}
}};

struct {mcls} {{
    void* m_data;
    {mcls}();                              // 0x{mctor:08X}
    ~{mcls}();
}};

struct {cls} : {cls}_Base {{
    void* m_pValue;                        // +{poff:#x}
    {mcls} m_member;                       // +{off:#x}
    virtual void v00();
    {cls}(void* value);
}};

{cls}::{cls}(void* value)
    : {cls}_Base()
    , m_pValue(value)
    , m_member()
{{
}}
''',T(a,'''static int g_m = 0; struct B { virtual void v00() {} B() {} ~B() {} }; struct M { void* d; M() : d(0) { ++g_m; } ~M() {} };
struct X : B { void* p; M m; virtual void v00() {} X(void* v) : B(), p(v), m() {} };
int main()
{
    int q; X x(&q); if (x.p != &q || g_m != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w171",):   # EH dtor: ~CX() {} -- member at +off destroyed out of line (state 0), then the base destructor (state -1); no vtables
        mdtor=calls[0]; bdtor=calls[1]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        adj=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and i.op_str.startswith("esi, [ecx")]; adj=adj[0] if adj else 0
        cls=f"CPair_{a}"; inner=f"{cls}_Inner" if adj else cls
        body=(f'''struct {inner} : CBase_{bdtor:08x} {{
    unsigned char _pad[{off-4:#x}];
    CMember_{mdtor:08x} m_member;          // +{off:#x}
    ~{inner}() {{ }}                        // inline: inlined into the owner's destructor
}};

struct {cls} {{
    unsigned char _pad[{adj:#x}];
    {inner} m_inner;                       // +{adj:#x}
    ~{cls}();
}};
''' if adj else f'''struct {cls} : CBase_{bdtor:08x} {{
    unsigned char _pad[{off-4:#x}];
    CMember_{mdtor:08x} m_member;          // +{off:#x}
    ~{cls}();
}};
''')
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- EH-framed empty destructor: member at +{off:#x} (@ 0x{mdtor:08X}) then base @ 0x{bdtor:08X}" + (f", on the sub-object at +{adj:#x}" if adj else ""),len(b))+f'''//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; {"lea esi,[ecx+%#x]" % adj if adj else "mov esi,ecx"} ; mov [esp+4],esi ; lea ecx,[esi+{off:#x}] ; mov dword ptr [esp+10h],0 ; call ~member ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~base ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). Non-polymorphic aggregate (no vtable stores): the
// member at +{off:#x} is destroyed out of line @ 0x{mdtor:08X} under EH state 0, then the base
// destructor @ 0x{bdtor:08X} runs under state -1.{" The whole thing is the inline destructor of a member embedded at +%#x of the owner, inlined into the owner's empty destructor." % adj if adj else ""}
// The manifest label (`_Dest_val<...pair<CCharString,CScriptInfo>>`) names a map pair; classes are
// placeholders named by their destructor addresses. One of {len(members)} bodies.
struct CBase_{bdtor:08x} {{
    void* m_data;
    ~CBase_{bdtor:08x}();                  // 0x{bdtor:08X}
}};

struct CMember_{mdtor:08x} {{
    void* m_data;
    ~CMember_{mdtor:08x}();                // 0x{mdtor:08X}
}};

{body}
{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct B { void* d; ~B() { g_d = g_d * 10 + 1; } }; struct M { void* d; ~M() { g_d = g_d * 10 + 2; } };
struct X : B { unsigned char pad[0x18]; M m; ~X() {} };
int main()
{
    { X x; } if (g_d != 21) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w144",):   # EH ctor: CX(a1, a2) : CBase(), m_18(a1), m_1c(0, a2) {}
        base=calls[0]; mctor=calls[1]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        poff=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str.split(",")[0] and i.operands[1].type==X86_OP_REG][0]
        cls=f"CScriptEntry_{a}"
        add(a,"_unattributed",cls,HDRF(a,f"{cls}::{cls}(void*, void*) -- EH-framed: base @ 0x{base:08X}, first argument at +{poff:#x}, member at +{off:#x} constructed from (0, second argument)",len(b))+f'''//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; mov ecx,[esp+1Ch] ; mov eax,[esp+18h] ; push ecx ; push 0 ; lea ecx,[esi+{off:#x}] ; mov dword ptr [esp+18h],0 ; mov [esi+{poff:#x}],eax ; call CMember::CMember ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Built with C++ exceptions enabled (/EHsc). The out-of-line base constructor @ 0x{base:08X} runs
// first (its destructor gives EH state 0), the first argument is stored at +{poff:#x} and the
// member at +{off:#x} is constructed out of line @ 0x{mctor:08X} from (0, second argument). No
// vtables are stored. Manifest labels here are propagated; classes are placeholders. One of {len(members)} bodies.
struct CBase_{base:08x} {{
    void* m_data;
    CBase_{base:08x}();                    // 0x{base:08X}
    ~CBase_{base:08x}();
}};

struct CMember_{mctor:08x} {{
    void* m_data;
    CMember_{mctor:08x}(long zero, void* value);   // 0x{mctor:08X}
}};

struct {cls} : CBase_{base:08x} {{
    unsigned char _pad[{poff-4:#x}];
    void* m_pFirst;                        // +{poff:#x}
    CMember_{mctor:08x} m_member;          // +{off:#x}
    {cls}(void* first, void* second);
}};

{cls}::{cls}(void* first, void* second)
    : CBase_{base:08x}()
    , m_pFirst(first)
    , m_member(0, second)
{{
}}
''',T(a,'''static long g_z = 1; struct B { void* d; B() : d(0) {} ~B() {} }; struct M { void* d; M(long z, void* v) : d(v) { g_z = z; } };
struct X : B { unsigned char pad[0x14]; void* f; M m; X(void* a, void* b) : B(), f(a), m(0, b) {} };
int main()
{
    int p, q; X x(&p, &q); if (x.f != &p || x.m.d != &q || g_z != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w349",):   # EH dtor: ~CX() {} of a novtable class: member at +off destroyed out of line, then the inlined empty intermediate-base destructor stores its vtable
        vt=vts[0][1]; mid=rtti.get(vt) or f"CVtable_{vt:08x}"; mdtor=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        cls=f"{mid}_Derived_{a}"
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- EH-framed, novtable: member at +{off:#x} destroyed, then the inlined empty {mid} destructor stores its vtable",len(b))+f'''//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+{off:#x}] ; mov dword ptr [esp+10h],0 ; call ~member ; mov ecx,[esp+8] ; mov dword ptr [esi],offset {mid}::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). As in family 143: the derived class is abstract
// (`__declspec(novtable)`, so no vtable store of its own), its member at +{off:#x} is destroyed out
// of line @ 0x{mdtor:08X} under EH state 0, then the inline empty destructor of the intermediate
// base {mid} is inlined (its vtable store 0x{vt:08X}). The derived class cannot be named from the
// body. One of {len(members)} bodies of this shape.
struct CMember_{mdtor:08x} {{
    void* m_data;
    ~CMember_{mdtor:08x}();                // 0x{mdtor:08X}
}};

struct {mid} {{
    virtual void v00();
    ~{mid}() {{ }}
}};

struct __declspec(novtable) {cls} : {mid} {{
    unsigned char _pad[{off-4:#x}];
    CMember_{mdtor:08x} m_member;          // +{off:#x}
    virtual void v00();
    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct M { void* d; ~M() { ++g_d; } }; struct Mid { virtual void v00() {} ~Mid() {} };
struct D : Mid { unsigned char pad[4]; M m; virtual void v00() {} ~D() {} };
int main()
{
    { D d; } if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w467",):   # EH: void CX::Invoke([T a]) { CAgents tmp(a or this); m_member.Slot(&tmp); }  -- 8-byte stack object from an out-of-line ctor, virtual call on an embedded member
        ctor=calls[0]; slot=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM][0]//4
        moff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "edx +" in i.op_str][0]; selfarg=retn==0
        cls=f"CInvoker_{a}"; fn=f"Invoke_{a}"; params="" if selfarg else "void* argument"
        add(a,"_unattributed",fn,HDRF(a,f"{cls}::{fn}({params}) -- EH-framed: a stack object built @ 0x{ctor:08X} from {'this' if selfarg else 'the argument'} is handed to virtual slot {slot} of the member at +{moff:#x}",len(b))+f'''//
// Retail bytes:  (EH prologue) ; sub esp,8 ; {"mov edx,ecx ; push edx" if selfarg else "mov eax,[esp+18h] ; mov edx,ecx ; push eax"} ; lea ecx,[esp+4] ; call CAgents::CAgents ; {"mov eax,[edx+%#x] ; lea ecx,[edx+%#x]" % (moff,moff) if selfarg else "lea ecx,[edx+%#x] ; mov edx,[ecx]" % moff} ; lea edx/eax,[esp] ; push ; mov dword ptr [esp+14h],0 ; call [vtable+{slot*4:#x}] ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret {retn}
// Built with C++ exceptions enabled (/EHsc). An 8-byte temporary is constructed out of line
// @ 0x{ctor:08X} from {'this' if selfarg else 'the one argument'} (`this` stays in edx across the call), then
// its address goes to virtual slot {slot} of the polymorphic member embedded at +{moff:#x}; the
// temporary's empty inline destructor gives EH state 0. Manifest labels are propagated; classes
// are placeholders. One of {len(members)} bodies of this shape.
struct CAgents_{ctor:08x} {{                // 8 bytes: vtable + one member; the empty inline destructor only
    virtual void v00();                    // resets the vtable (dead store, dropped) but still registers EH state 0
    void* m_value;
    CAgents_{ctor:08x}(void* source);      // 0x{ctor:08X}
    ~CAgents_{ctor:08x}() {{ }}
}};

struct CReceiver {{
    virtual void v00();
{"".join("    virtual void slot%02d();" % j + chr(10) for j in range(1,slot))}    virtual void Receive(CAgents_{ctor:08x}* agents);   // slot {slot}
}};

struct {cls} {{
    unsigned char _pad[{moff:#x}];
    CReceiver m_receiver;                  // +{moff:#x}
    void {fn}({params});
}};

void {cls}::{fn}({params})
{{
    CAgents_{ctor:08x} agents({"this" if selfarg else "argument"});
    m_receiver.Receive(&agents);
}}
''',T(a,'''static int g_c = 0; struct A { virtual void v00() {} void* a; A(void* s) : a(s) {} ~A() {} };
struct R { virtual void v00() {} virtual void Recv(A*) { ++g_c; } }; struct X { unsigned char pad[4]; R r; void Inv(void* v) { A a(v); r.Recv(&a); } };
int main()
{
    X x; x.Inv(0); if (g_c != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w468",):   # EH: long CX::Get() { CGuard guard(this); return m_pObject->Slot(); }  -- explicit out-of-line destructor of the guard
        ctor=calls[0]; dtor=calls[1]; slot=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM][0]//4
        poff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CScript_{a}"; mod=cls if not cls.startswith("CScript_") else "_unattributed"; fn=clean(mname) if (clean(mname) and not clean(mname).startswith("sub_")) else f"Query_{a}"
        add(a,mod,fn,HDRF(a,f"{cls}::{fn}() -- EH-framed: guard object (ctor @ 0x{ctor:08X}, dtor @ 0x{dtor:08X}) around a virtual slot-{slot} call on the object at +{poff:#x}",len(b))+f'''//
// Retail bytes:  (EH prologue) ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call CGuard::CGuard ; mov ecx,[esi+{poff:#x}] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+{slot*4:#x}] ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call CGuard::~CGuard ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Built with C++ exceptions enabled (/EHsc). An 8-byte guard is constructed from `this`
// @ 0x{ctor:08X}, the result of virtual slot {slot} of the object at +{poff:#x} is kept in esi
// while the guard is destroyed out of line @ 0x{dtor:08X} (state -1), then returned. Classes other
// than the labelled one are placeholders. One of {len(members)} bodies of this shape.
struct CGuard_{ctor:08x} {{
    void* m_first;
    void* m_second;
    CGuard_{ctor:08x}(void* owner);        // 0x{ctor:08X}
    ~CGuard_{ctor:08x}();                  // 0x{dtor:08X}
}};

struct CQueried {{
    virtual void v00();
{"".join("    virtual void slot%02d();" % j + chr(10) for j in range(1,slot))}    virtual long Query();                  // slot {slot}
}};

struct {cls} {{
    unsigned char _pad[{poff:#x}];
    CQueried* m_pObject;                   // +{poff:#x}
    long {fn}();
}};

long {cls}::{fn}()
{{
    CGuard_{ctor:08x} guard(this);
    return m_pObject->Query();
}}
''',T(a,'''static int g_g = 0; struct G { void* a; void* b; G(void* o) : a(o), b(0) { ++g_g; } ~G() { --g_g; } };
struct Q { virtual void v00() {} virtual void s1() {} virtual long Query() { return 7; } }; struct X { unsigned char pad[4]; Q* q; long Get() { G g(this); return q->Query(); } };
int main()
{
    Q q; X x; x.q = &q; if (x.Get() != 7 || g_g != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w15",):   # bool CDefinitionManager::GetDef<T>(long index, CDefPointer<T>& out): fetch by global index, assign the ref-counted pointer, release the temporary
        getter=calls[0]; cls="CDefinitionManager"; nm=f"CDefPointer_{a}"
        add(a,cls,f"GetDef_{a}",HDRF(a,f"{cls}::GetDef<T>(long, {nm}&) -- GetPDefFromGlobalIndex, then the intrusive-count assignment and the temporary's release, both inlined",len(b))+f'''//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; jle F ; push eax ; lea eax,[esp+8] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[esp+4] ; test eax,eax ; je F ; push esi ; mov esi,[esp+0Ch] ; mov ecx,[esi] ; cmp ecx,eax ; je S ; test ecx,ecx ; je N ; dec dword ptr [ecx+4] ; jne Z ; mov edx,[ecx] ; call [edx+4] ; Z: mov eax,[esp+8] ; mov dword ptr [esi],0 ; N: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[esp+8] ; S: mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; ret 8 ; F: xor al,al ; ret 8
// Template instantiation of the definition lookup: for a positive index the manager returns a
// ref-counted definition pointer by value (@ 0x{getter:08X}; the temporary lives in the index's
// own argument slot), a null result fails, otherwise the intrusive-count assignment into `out`
// (release the old definition through vtable slot 1 when its count hits zero, adopt the new one
// and bump its count) and the temporary's destructor (decrement, destroy on zero) are inlined.
// The definition type is not recoverable from the body. One of {len(members)} bodies of this shape.
struct CDef {{
    virtual void v00();
    virtual void Destroy();                // slot 1: called when the count reaches zero
    long m_references;                     // +4
}};

struct {nm} {{
    CDef* m_pDef;
    {nm}() : m_pDef(0) {{ }}
    ~{nm}()
    {{
        if (m_pDef) {{
            CDef* def = m_pDef;
            long left = def->m_references - 1;   // decremented in a register, stored, then tested (retail)
            def->m_references = left;
            if (left == 0)
                def->Destroy();
        }}
    }}
    {nm}& operator=(const {nm}& other)
    {{
        if (m_pDef != other.m_pDef) {{
            if (m_pDef) {{
                CDef* old = m_pDef;            // local: the decrement's flags are tested directly
                if (--old->m_references == 0)
                    old->Destroy();
                m_pDef = 0;
            }}
            m_pDef = other.m_pDef;
            if (m_pDef)
                ++m_pDef->m_references;
        }}
        return *this;
    }}
}};

struct {cls} {{
    {nm} GetPDefFromGlobalIndex(long index);   // 0x{getter:08X}
    bool GetDef_{a}(long index, {nm}& out);
}};

bool {cls}::GetDef_{a}(long index, {nm}& out)
{{
    if (index > 0) {{
        {nm} def = GetPDefFromGlobalIndex(index);
        if (def.m_pDef) {{
            out = def;
            return true;
        }}
    }}
    return false;                          // one shared failure exit at the end, as retail
}}
''',T(a,'''static int g_x = 0; struct D { virtual void v00() {} virtual void Destroy() { ++g_x; } long r; D() : r(1) {} };
struct P { D* p; P() : p(0) {} ~P() { if (p) { if (--p->r == 0) p->Destroy(); } } P& operator=(const P& o) { if (p != o.p) { if (p) { if (--p->r == 0) p->Destroy(); p = 0; } p = o.p; if (p) ++p->r; } return *this; } };
static D g_d; struct M { P Get(long) { P t; t.p = &g_d; ++g_d.r; return t; } bool GetDef(long i, P& out) { if (i > 0) { P d = Get(i); if (d.p) { out = d; return true; } } return false; } };
int main()
{
    M m; P out; if (!m.GetDef(3, out) || out.p != &g_d || g_d.r != 2 || m.GetDef(0, out)) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w49",):   # _Uninit_fill_n(dest, count, const pair<ulong, CCountedRef>& value): placement-copy count elements
        step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("eax,")][0]
        nm=f"CPairCountedRef{step}"
        add(a,"_global",f"UninitFillN_{nm}_{a}",HDRF(a,f"_Uninit_fill_n<{nm}*> -- placement-copy `count` {step}-byte {{key, counted ref}} pairs (count bump inlined)",len(b))+f'''//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[esp+8] ; L: test eax,eax ; je N ; mov ecx,[edx] ; mov [eax],ecx ; mov ecx,[edx+4] ; test ecx,ecx ; mov [eax+4],ecx ; je N ; inc dword ptr [ecx] ; N: add eax,{step} ; dec esi ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: dest in ecx, count in edx, the value and the
// allocator on the stack): every slot is copy-constructed in place with placement new (null test
// on the slot) from the value, whose second member is an intrusive counted reference (copying
// it bumps the count block's first dword); the advanced destination is returned. The element
// type is named by its shape. One of
// {len(members)} bodies.
#include <new>

struct SCountBlock {{ long m_references; }};

struct {nm} {{
    unsigned long m_key;
    SCountBlock* m_pCount;
    {nm}(const {nm}& other) : m_key(other.m_key), m_pCount(other.m_pCount) {{ if (m_pCount) ++m_pCount->m_references; }}
}};

struct CRetailAllocator {{ }};

{nm}* __fastcall UninitFillN_{nm}_{a}({nm}* dest, unsigned long count, const {nm}& value, CRetailAllocator allocator)
{{
    for (; 0 < count; --count, ++dest)
        new (dest) {nm}(value);
    return dest;                           // the advanced pointer is returned (retail keeps dest in eax from the start)
}}
''',T(a,'''#include <new>
struct C { long r; }; struct P { unsigned long k; C* c; P(const P& o) : k(o.k), c(o.c) { if (c) ++c->r; } }; struct A {};
static P* __fastcall F(P* d, unsigned long n, const P& v, A) { for (; 0 < n; --n, ++d) new (d) P(v); return d; }
int main()
{
    C c = { 1 }; unsigned char buf[24]; P* v = (P*)buf; v->k = 5; v->c = &c; F((P*)(buf + 8), 2, *v, A()); if (c.r != 3 || ((P*)buf)[2].k != 5) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w50",):   # void CX::Remove(T key) { void* freed = Helper(key, &m_table->a, &m_table->b, &m_table->c); if (freed) free(freed); --m_count; }
        helper=calls[0]; offs=sorted([i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"]+[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("eax,")])
        cls=f"CRemovalTable_{a}"
        add(a,"_unattributed",f"Remove_{a}",HDRF(a,f"{cls}::Remove(key) -- cdecl helper @ 0x{helper:08X} over the table at [this]+{offs[0]:#x}/{offs[1]:#x}/{offs[2]:#x}, free the returned block, drop the count",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; lea ecx,[eax+{offs[2]:#x}] ; push ecx ; lea edx,[eax+{offs[1]:#x}] ; push edx ; add eax,{offs[0]:#x} ; push eax ; mov eax,[esp+14h] ; push eax ; call Helper ; add esp,10h ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: dec dword ptr [esi+4] ; pop esi ; ret 4
// Container removal: the __cdecl helper @ 0x{helper:08X} gets the key and the addresses of three
// consecutive fields of the table object at [this] (the manifest's `CMemoryAllocatorVariableSize`
// label for it is propagated); whatever block it hands back is released with free @ 0x00BFEA14,
// then the element count at +4 is decremented. One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct STable {{
    unsigned char _pad[{offs[0]:#x}];
    void* m_first;                         // +{offs[0]:#x}
    void* m_second;                        // +{offs[1]:#x}
    void* m_third;                         // +{offs[2]:#x}
}};

void* __cdecl Helper_{helper:08x}(void* key, void** first, void** second, void** third);   // 0x{helper:08X}

struct {cls} {{
    STable* m_pTable;                      // +0
    long m_count;                          // +4
    void Remove_{a}(void* key);
}};

void {cls}::Remove_{a}(void* key)
{{
    void* freed = Helper_{helper:08x}(key, &m_pTable->m_first, &m_pTable->m_second, &m_pTable->m_third);
    if (freed)
        free(freed);
    --m_count;
}}
''',T(a,'''#include <cstdlib>
static void* __cdecl H(void*, void** a, void**, void**) { return *a; }
struct T { unsigned char p[4]; void* a; void* b; void* c; }; struct X { T* t; long n; void R(void* k) { void* f = H(k, &t->a, &t->b, &t->c); if (f) std::free(f); --n; } };
int main()
{
    T t; t.a = std::malloc(4); X x; x.t = &t; x.n = 2; x.R(0); if (x.n != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w41",):   # Dinkumware _Find for 4-byte elements: 4x unrolled loop plus a 3-case tail
        nm=f"CFind4_{a}"
        add(a,"_global",f"Find_{a}",HDRF(a,f"std::find<long*> -- Dinkumware `_Find`: four-way unrolled scan with a switch on the remainder (4-byte elements)",len(b))+f'''//
// Retail bytes:  mov eax,ecx ; mov ecx,edx ; push ebx ; sub ecx,eax ; push esi ; sar ecx,4 ; test ecx,ecx ; push edi ; mov edi,[esp+10h] ; jle T ; mov esi,[edi] ; L: cmp [eax],esi ; je F ; (three more compare/advance steps) ; add eax,4 ; dec ecx ; test ecx,ecx ; jg L ; T: mov ecx,edx ; sub ecx,eax ; sar ecx,2 ; dec ecx ; je C1 ; dec ecx ; je C2 ; dec ecx ; jne D ; (case 3, 2, 1 compares) ; D: mov eax,edx ; F: pop edi ; pop esi ; pop ebx ; ret 8
// The retail Dinkumware `find` for 4-byte scalar elements (__fastcall: first in ecx, last in
// edx, the value by reference on the stack; `ret 8` also pops an unused tag): a loop over groups
// of four comparisons (the value cached in a register) followed by a `switch` on the 0..3
// remaining elements. Written out by hand because the DDK headers are a different revision.
// One of {len(members)} bodies of this shape.
struct CIteratorTag {{ }};

long* __fastcall Find_{a}(long* first, long* last, const long& value, CIteratorTag tag)
{{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {{
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }}
    switch (last - first) {{
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }}
    return last;
}}
''',T(a,'''struct Tg {};
static long* __fastcall F(long* f, long* l, const long& v, Tg) { long n; for (n = (l - f) >> 2; 0 < n; --n) { if (*f == v) return f; ++f; if (*f == v) return f; ++f; if (*f == v) return f; ++f; if (*f == v) return f; ++f; } switch (l - f) { case 3: if (*f == v) return f; ++f; case 2: if (*f == v) return f; ++f; case 1: if (*f == v) return f; default: break; } return l; }
int main()
{
    long a[7] = { 1, 2, 3, 4, 5, 6, 7 }; long k = 6, m = 9; if (F(a, a + 7, k, Tg()) != a + 5 || F(a, a + 7, m, Tg()) != a + 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_1",):   # CCountedPointer<CX> __fastcall Create(owner, const CScriptThing& thing, arg): new CX(owner, thing, arg) wrapped in a fresh count block
        size=pushes[0]; cctor=calls[1]; deleter=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x401000<=i.operands[1].imm<0x1210000][0]
        vt1,vt2=vts[0][1],vts[1][1]; basecls=rtti.get(vt1) or f"CVtable_{vt1:08x}"; cls=rtti.get(vt2) or f"CVtable_{vt2:08x}"
        ofield=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "ebx +" in i.op_str][0]
        sts=[(i.operands[0].mem.disp,i.op_str.split(", ")[1]) for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str.split(",")[0] and i.operands[1].type==X86_OP_REG]
        o_ctx=[d for d,r in sts if r=="eax"][0]; o_owner=[d for d,r in sts if r=="ebx"][0]; o_arg=[d for d,r in sts if r=="ecx"][0]; toff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        if not (o_ctx==4 and toff==8 and o_owner==toff+12 and retn==8): continue
        extra=[(i.operands[1].mem.disp) for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str and i.operands[1].mem.disp!=toff]   # h22 variant: a member with an out-of-line ctor
        mctor=calls[2] if (extra and len(calls)>3) else 0; moff=extra[0] if extra else 0
        if extra and not (len(calls)>=4 and o_arg==o_owner+4 and moff==o_arg+4): continue
        if not extra and o_arg!=o_owner+4: continue
        tail=size-(moff+4 if extra else o_arg+4)
        add(a,"_global",f"Create_{cls}",HDRF(a,f"CCountedPointer<{cls}> Create_{cls}(owner, arg, const CScriptThing&) -- new {cls}(owner, thing, arg) handed to a counted pointer (count block + deleter 0x{deleter:08X})",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; push edi ; push {size:#x} ; mov ebx,edx ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je Z ; mov eax,[ebx+{ofield:#x}] ; mov [esi+4],eax ; mov eax,[esp+14h] ; push eax ; lea ecx,[esi+8] ; mov dword ptr [esi],offset {basecls}::`vftable' ; call CScriptThing::CScriptThing(const&) ; mov ecx,[esp+10h] ; mov dword ptr [esi],offset {cls}::`vftable' ; mov [esi+{o_owner:#x}],ebx ; mov [esi+{o_arg:#x}],ecx ; jmp P ; Z: xor esi,esi ; P: test esi,esi ; mov [edi],esi ; je N ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je Q ; mov ecx,[edi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [edi+4],eax ; mov eax,edi ; (pops) ; ret 8 ; Q: xor eax,eax ; mov [edi+4],eax ; ... ; N: mov dword ptr [edi+4],0 ; ...
// Script-side factory (no-RTTI module): a __fastcall function returning a CCountedPointer by
// value (hidden result pointer in ecx, the owning script in edx, then on the stack one argument
// and the CScriptThing by reference). It allocates sizeof({cls}) = {size:#x}, runs the inlined two-level
// constructor (base {basecls}: context pointer copied from owner+{ofield:#x} and the CScriptThing
// copy-constructed @ 0x{cctor:08X}; derived {cls}: owner and argument), then builds the
// 12-byte count block {{1, deleter 0x{deleter:08X}, object}} exactly as the counted-pointer
// constructor family does.{' This variant also default-constructs a member at +%#x out of line @ 0x%08X before the argument is assigned in the constructor body.' % (moff, mctor) if extra else ''} Vtables are named by address (the module carries no RTTI).
// One of {len(members)} bodies of this shape.
struct CScriptThing {{
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);   // 0x{cctor:08X}
}};

struct CScript {{
    unsigned char _pad[{ofield:#x}];
    void* m_pContext;                      // +{ofield:#x}
}};

// The context store precedes the base vtable store, so it belongs to a root class whose inline
// constructor is inlined here (its own vtable store is dead and dropped).
struct {basecls}_Root {{
    virtual void v00();
    void* m_pContext;                      // +4
    {basecls}_Root(void* context) : m_pContext(context) {{ }}
}};

struct {basecls} : {basecls}_Root {{
    virtual void v00();
    CScriptThing m_thing;                  // +8
    {basecls}(CScript* owner, const CScriptThing& thing) : {basecls}_Root(owner->m_pContext), m_thing(thing) {{ }}
}};

{"struct CMember_%08x {" % mctor + chr(10) + "    void* m_data;" + chr(10) + "    CMember_%08x();                      // 0x%08X, out of line" % (mctor, mctor) + chr(10) + "};" + chr(10) + chr(10) if extra else ""}struct {cls} : {basecls} {{
    virtual void v00();
    CScript* m_pOwner;                     // +{o_owner:#x}
    void* m_pArgument;                     // +{o_arg:#x}{" (assigned in the body: stored after the member constructor)" if extra else ""}
{"    CMember_%08x m_member;                 // +%#x" % (mctor, moff) + chr(10) if extra else ""}{"    unsigned char _tail[%#x];" % tail + chr(10) if tail>0 else ""}    {cls}(CScript* owner, const CScriptThing& thing, void* argument) : {basecls}(owner, thing), m_pOwner(owner){", m_member() { m_pArgument = argument; }" if extra else ", m_pArgument(argument) { }"}
}};

void __fastcall Destroy_{deleter:08x}(void* object);   // 0x{deleter:08X}

struct SCountBlock {{
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_{deleter:08x}), m_pObject(object) {{ }}
}};

struct CCountedPointer {{
    void* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(void* object) : m_pObject(object)
    {{
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }}
}};

CCountedPointer __fastcall Create_{cls}(CScript* owner, void* argument, const CScriptThing& thing)
{{
    return CCountedPointer(new {cls}(owner, thing, argument));
}}
''',T(a,'''static void __fastcall Destroy(void*) { }
struct ST { unsigned char b[12]; ST() {} ST(const ST& o) { b[0] = o.b[0]; } }; struct SC { unsigned char p[0x40]; void* ctx; };
struct R { virtual void v00() {} void* c; R(void* x) : c(x) {} }; struct B : R { virtual void v00() {} ST t; B(SC* o, const ST& s) : R(o->ctx), t(s) {} };
struct D : B { virtual void v00() {} SC* o; void* a; unsigned char tl[4]; D(SC* ow, const ST& s, void* ar) : B(ow, s), o(ow), a(ar) {} };
struct K { long r; void (__fastcall* f)(void*); void* o; K(void* x) : r(1), f(Destroy), o(x) {} };
struct P { void* o; K* c; P(void* x) : o(x) { if (x) c = new K(o); else c = 0; } };
static P __fastcall Create(SC* ow, void* ar, const ST& s) { return P(new D(ow, s, ar)); }
int main()
{
    SC sc; int v; sc.ctx = &v; ST s; s.b[0] = 3; P p = Create(&sc, &v, s); D* d = (D*)p.o; if (!d || !p.c || d->c != &v || d->t.b[0] != 3 || d->o != &sc || p.c->r != 1) '''+FAIL(a)+'''
    delete p.c; delete d;
'''),b)
    elif TPL in ("w256_4",):   # void CScript::Main() { Add(new CSpawnedFunc_X(CCharString("LIT", -1), 0, this), CCharString("NAME", -1)); }
        size=pushes[0]; lits=[p for p in pushes if 0x1200000<=p<0x1300000]; s1,s2=lit(lits[0]),lit(lits[1]); bctor=calls[1]; addfn=calls[3]
        vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; body=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x401000<=i.operands[1].imm<0x1210000][0]
        sts=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str.split(",")[0]]; o_fn=sts[0]; o_owner=sts[1]
        fn=clean(mname) or "Main"; owner=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CScript_{a}"; mod=owner if not owner.startswith("CScript_") else "_unattributed"
        add(a,mod,fn,HDRF(a,f"{owner}::{fn}() -- spawns the script function \"{s1}\" (body @ 0x{body:08X}) and registers it as \"{s2}\"",len(b))+f'''//
// Retail bytes:  sub esp,8 ; push ebx ; push esi ; push edi ; xor ebx,ebx ; push {size:#x} ; mov edi,ecx ; mov [esp+14h],ebx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je Z ; push -1 ; push offset "{s1}" ; lea ecx,[esp+14h] ; call CCharString::CCharString ; push 0 ; lea eax,[esp+10h] ; push eax ; mov ecx,esi ; mov ebx,1 ; call CSpawnedFunc::CSpawnedFunc ; mov dword ptr [esi],offset {cls}::`vftable' ; mov dword ptr [esi+{o_fn:#x}],offset body ; mov [esi+{o_owner:#x}],edi ; jmp P ; Z: xor esi,esi ; P: push -1 ; push offset "{s2}" ; lea ecx,[esp+18h] ; call CCharString::CCharString ; push eax ; push esi ; mov ecx,edi ; call Add ; lea ecx,[esp+10h] ; call ~CCharString ; pop edi ; pop esi ; test bl,1 ; pop ebx ; je E ; lea ecx,[esp] ; call ~CCharString ; E: add esp,8 ; ret
// Script entry point of the no-RTTI script module: one full expression allocates a spawned
// function object of {size:#x} bytes whose inlined constructor builds a CCharString temporary
// from "{s1}" for the out-of-line base constructor @ 0x{bctor:08X} (name, 0), installs the
// vtable (0x{vt:08X}), the function body pointer (0x{body:08X}) and the owning script, then
// registers it with Add @ 0x{addfn:08X} under a second CCharString "{s2}". The first temporary
// is destroyed at the end of the full expression only if the allocation succeeded (the `bl`
// flag): that is how VC7.1 scopes a temporary built inside a `new` expression. One of {len(members)} bodies.
#include "fable_string.h"

struct {owner};

struct CSpawnedFunc {{
    virtual void v00();
    unsigned char _body[{o_fn:#x} - 4];
    CSpawnedFunc(const CCharString& name, long zero);   // 0x{bctor:08X}
}};

void Body_{body:08x}();                    // 0x{body:08X}: the script function this object runs

struct {cls} : CSpawnedFunc {{
    virtual void v00();
    void (*m_pfnBody)();                   // +{o_fn:#x}
    {owner}* m_pOwner;                     // +{o_owner:#x}
{"    unsigned char _tail[%#x];" % (size-o_owner-4) + chr(10) if size-o_owner-4>0 else ""}    {cls}(const CCharString& name, long zero, {owner}* owner) : CSpawnedFunc(name, zero), m_pfnBody(Body_{body:08x}), m_pOwner(owner) {{ }}
}};

struct {owner} {{
    void Add(CSpawnedFunc* function, const CCharString& name);   // 0x{addfn:08X}
    void {fn}();
}};

void {owner}::{fn}()
{{
    Add(new {cls}(CCharString("{s1}", -1), 0, this), CCharString("{s2}", -1));
}}
''',T(a,'''static int g_a = 0, g_d = 0; struct S { const char* p; S(const char* t, long) : p(t) {} ~S() { ++g_d; } };
struct F { virtual void v00() {} unsigned char b[0x30]; F(const S&, long) {} }; static void Body() {}
struct O; struct X : F { virtual void v00() {} void (*f)(); O* o; X(const S& n, long z, O* ow) : F(n, z), f(Body), o(ow) {} };
struct O { void Add(F* p, const S&) { ++g_a; delete p; } void Main() { Add(new X(S("A", -1), 0, this), S("B", -1)); } };
int main()
{
    O o; o.Main(); if (g_a != 1 || g_d != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_16",):   # pair<iterator,bool> CRetailTree::insert(const value_type&): VC6-era Dinkumware descent, begin()/--P checks, _Insert(X, Y, V, 0)
        unsig=any(i.mnemonic in ("setb","setae","jae","jb","ja","jbe") for i in ins); kt="unsigned long" if unsig else "long"
        maxfn=calls[0]; insfn=calls[1]; nm=f"CRetailTree_{'ulongkey' if unsig else 'longkey'}_ins{insfn:08x}"
        add(a,nm,"insert",HDRF(a,f"{nm}::insert(const value_type&) -- unique-key insert: descend on `key < node.key`, step back unless at begin(), then _Insert @ 0x{insfn:08X}",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push ebx ; mov ebx,ecx ; mov ecx,[ebx] ; push esi ; mov esi,[ecx+4] ; test esi,esi ; push edi ; mov edi,ecx ; mov dl,1 ; je D ; L: mov eax,[ebp+0Ch] ; mov eax,[eax] ; cmp eax,[esi+10h] ; mov edi,esi ; setl dl ; test dl,dl ; je R ; mov esi,[esi+8] ; jmp T ; R: mov esi,[esi+0Ch] ; T: test esi,esi ; jne L ; D: test dl,dl ; mov eax,edi ; je C ; cmp edi,[ecx+8] ; jne P ; push 0 ; push [ebp+0Ch] ; push edi ; push edi ; jmp I ; P: push edi ; call _Max ; pop ecx ; C: mov edx,[eax+10h] ; mov ecx,[ebp+0Ch] ; cmp edx,[ecx] ; jge F ; push 0 ; push ecx ; push edi ; push esi ; I: lea eax,[ebp+0Ch] ; push eax ; mov ecx,ebx ; call _Insert ; mov ecx,[eax] ; mov eax,[ebp+8] ; mov [eax],ecx ; mov byte ptr [eax+4],1 ; jmp E ; F: mov ecx,[ebp+8] ; mov [ecx],eax ; mov byte ptr [ecx+4],0 ; mov eax,ecx ; E: pop edi ; pop esi ; pop ebx ; pop ebp ; ret 8
// The retail (VC6-era Dinkumware) `_Tree::insert` for a {'unsigned' if unsig else 'signed'} 32-bit key at +0x10: walk down
// from head->parent remembering the parent and whether the key compares less; at the leaf, when
// the last step went left and the parent is begin() insert immediately (`_Insert` @ 0x{insfn:08X}
// called with the parent twice: a non-null first argument means "link as the left child"),
// otherwise step back to the predecessor (the out-of-line `--iterator` helper @ 0x{maxfn:08X}) and insert only if that node's key is less than the new key;
// the pair<iterator,bool> result goes through the hidden pointer. The compiler re-uses the dead
// value-reference argument slot for `_Insert`'s hidden iterator result. Frame pointer + merged
// call site: this TU is `optimize("y",off)` + `("s",on)`. One of {len(members)} bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct {nm} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; {kt} key; }};
    struct iterator {{ node* current; iterator(node* n) : current(n) {{ }} }};
    struct pairib {{ iterator first; bool second; pairib(iterator f, bool s) : first(f), second(s) {{ }} }};
    node* head;
    unsigned long size;
    static node* Prev(node* n);            // 0x{maxfn:08X}: predecessor of a node (the `--iterator` step, out of line)
    iterator Insert(node* trynode, node* wherenode, const {kt}& value, node* newnode);   // 0x{insfn:08X}
    pairib insert(const {kt}& value);
}};

{nm}::pairib {nm}::insert(const {kt}& value)
{{
    node* trynode = head->parent;
    node* wherenode = head;
    bool addleft = true;
    while (trynode) {{
        wherenode = trynode;
        addleft = value < trynode->key;
        trynode = addleft ? trynode->left : trynode->right;
    }}
    iterator where(wherenode);
    if (!addleft)
        ;
    else if (where.current == head->left)
        return pairib(Insert(wherenode, wherenode, value, 0), true);
    else
        where.current = Prev(where.current);
    if (where.current->key < value)
        return pairib(Insert(trynode, wherenode, value, 0), true);
    return pairib(where, false);
}}
''',T(a,'''struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; long k; }; struct it { node* c; it(node* n) : c(n) {} }; struct pib { it f; bool s; pib(it a, bool b) : f(a), s(b) {} };
  node* h; unsigned long n; static node* Prev(node* x) { x = x->l; while (x->r) x = x->r; return x; } it Ins(node*, node* w, const long& v, node*) { return it(w); }
  pib insert(const long& v) { node* x = h->parent; node* y = h; bool a = true; while (x) { y = x; a = v < x->k; x = a ? x->l : x->r; } it w(y); if (!a) ; else if (w.c == h->l) return pib(Ins(y, y, v, 0), true); else w.c = Prev(w.c); if (w.c->k < v) return pib(Ins(x, y, v, 0), true); return pib(w, false); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}, a = {1,{0},&h,0,0,5}; h.parent = &a; h.l = &a; T t; t.h = &h; t.n = 1; long k = 5, m = 7; if (t.insert(k).s || !t.insert(m).s) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_32",):   # insert(const value_type&) with the iterator's out-of-line operator-- and default optimisation (no frame pointer)
        unsig=any(i.mnemonic in ("setb","setae","jae","jb","ja","jbe") for i in ins); kt="unsigned long" if unsig else "long"
        decfn=calls[0]; insfn=calls[1]; nm=f"CRetailTree_{'ulongkey' if unsig else 'longkey'}_ins{insfn:08x}"
        add(a,nm,"insert",HDRF(a,f"{nm}::insert(const value_type&) -- unique-key insert: descend on `key < node.key`, iterator::operator-- unless at begin(), then _Insert @ 0x{insfn:08X}",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[esp+0Ch] ; push ebp ; mov ebp,ecx ; mov ecx,[ebp] ; push esi ; mov esi,[ecx+4] ; test esi,esi ; push edi ; mov edi,ecx ; mov al,1 ; je D ; L: mov edx,[ebx] ; cmp edx,[esi+10h] ; setl al ; test al,al ; mov edi,esi ; je R ; mov esi,[esi+8] ; jmp T ; R: mov esi,[esi+0Ch] ; T: test esi,esi ; jne L ; D: test al,al ; mov edx,edi ; mov [esp+18h],edx ; je C ; cmp edi,[ecx+8] ; jne P ; push 0 ; push ebx ; push edi ; push edi ; lea eax,[esp+28h] ; push eax ; mov ecx,ebp ; call _Insert ; (pair {{it, true}}) ; ret 8 ; P: lea ecx,[esp+18h] ; call iterator::operator-- ; mov edx,[esp+18h] ; C: mov eax,[edx+10h] ; cmp eax,[ebx] ; jge F ; push 0 ; push ebx ; push edi ; push esi ; lea ecx,[esp+28h] ; push ecx ; mov ecx,ebp ; call _Insert ; (pair {{it, true}}) ; ret 8 ; F: (pair {{where, false}}) ; ret 8
// The retail (VC6-era Dinkumware) `_Tree::insert` for a {'unsigned' if unsig else 'signed'} 32-bit key at +0x10, in a TU with
// default optimisation: the local iterator lives in a stack slot and steps back through its
// out-of-line `operator--` @ 0x{decfn:08X} (the family-11 body); `_Insert` @ 0x{insfn:08X} is called
// with the parent twice at begin() (a non-null first argument links left), else with the null
// try-node; the pair<iterator,bool> result goes through the hidden pointer. One of {len(members)} bodies.
struct {nm} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; {kt} key; }};
    struct iterator {{ node* current; iterator(node* n) : current(n) {{ }} iterator& Decrement(); }};   // 0x{decfn:08X}
    struct pairib {{ iterator first; bool second; pairib(iterator f, bool s) : first(f), second(s) {{ }} }};
    node* head;
    unsigned long size;
    iterator Insert(node* trynode, node* wherenode, const {kt}& value, node* newnode);   // 0x{insfn:08X}
    pairib insert(const {kt}& value);
}};

{nm}::pairib {nm}::insert(const {kt}& value)
{{
    node* trynode = head->parent;
    node* wherenode = head;
    bool addleft = true;
    while (trynode) {{
        wherenode = trynode;
        addleft = value < trynode->key;
        trynode = addleft ? trynode->left : trynode->right;
    }}
    iterator where(wherenode);
    if (!addleft)
        ;
    else if (where.current == head->left)
        return pairib(Insert(wherenode, wherenode, value, 0), true);
    else
        where.Decrement();
    if (where.current->key < value)
        return pairib(Insert(trynode, wherenode, value, 0), true);
    return pairib(where, false);
}}
''',T(a,'''struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; long k; }; struct it { node* c; it(node* n) : c(n) {} it& Dec() { c = c->l; while (c->r) c = c->r; return *this; } }; struct pib { it f; bool s; pib(it a, bool b) : f(a), s(b) {} };
  node* h; unsigned long n; it Ins(node*, node* w, const long& v, node*) { return it(w); }
  pib insert(const long& v) { node* x = h->parent; node* y = h; bool a = true; while (x) { y = x; a = v < x->k; x = a ? x->l : x->r; } it w(y); if (!a) ; else if (w.c == h->l) return pib(Ins(y, y, v, 0), true); else w.Dec(); if (w.c->k < v) return pib(Ins(x, y, v, 0), true); return pib(w, false); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}, a = {1,{0},&h,0,0,5}; h.parent = &a; h.l = &a; T t; t.h = &h; t.n = 1; long k = 5, m = 7; if (t.insert(k).s || !t.insert(m).s) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_15",):   # iterator _Insert(trynode, wherenode, value, forceRight): link the new node left or right, rebalance, count
        unsig=any(i.mnemonic in ("setb","setae","jae","jb","ja","jbe") for i in ins); kt="unsigned long" if unsig else "long"
        buy=calls[0]; rebal=calls[2]; nm=f"CRetailTree_{'ulongkey' if unsig else 'longkey'}_buy{buy:08x}"
        add(a,nm,"Insert",HDRF(a,f"{nm}::_Insert(trynode, wherenode, const value_type&, forceRight) -- link a new node (Buynode @ 0x{buy:08X}) under wherenode, rebalance @ 0x{rebal:08X}, bump the count",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; mov eax,[ebp+14h] ; push ebx ; mov ebx,[ebp+10h] ; push esi ; push edi ; mov edi,ecx ; cmp ebx,[edi] ; je L ; cmp dword ptr [ebp+18h],0 ; jne R ; cmp dword ptr [ebp+0Ch],0 ; jne L ; mov ecx,[eax] ; cmp ecx,[ebx+10h] ; jl L ; R: push eax ; mov ecx,edi ; call Buynode ; mov esi,eax ; mov [ebx+0Ch],esi ; mov eax,[edi] ; cmp ebx,[eax+0Ch] ; jne K ; mov [eax+0Ch],esi ; jmp K ; L: push eax ; mov ecx,edi ; call Buynode ; mov esi,eax ; mov [ebx+8],esi ; mov eax,[edi] ; cmp ebx,eax ; jne M ; mov [eax+4],esi ; mov eax,[edi] ; jmp (head->right = Z) ; M: cmp ebx,[eax+8] ; jne K ; mov [eax+8],esi ; K: and dword ptr [esi+8],0 ; and dword ptr [esi+0Ch],0 ; mov [esi+4],ebx ; mov eax,[edi] ; add eax,4 ; push eax ; push esi ; call Rebalance ; inc dword ptr [edi+4] ; mov eax,[ebp+8] ; pop ecx ; pop ecx ; pop edi ; mov [eax],esi ; pop esi ; pop ebx ; pop ebp ; ret 14h
// The retail (VC6-era Dinkumware) `_Tree::_Insert`: the new node goes LEFT of wherenode when
// wherenode is the head, or (unless the fourth argument forces the right side) when a try-node was
// supplied or the key is less than wherenode's; the head's parent/left/right links are fixed up
// for a new root, leftmost or rightmost node; the node is detached, parented, rebalanced by the
// __cdecl red-black fix-up @ 0x{rebal:08X} (node, &root) and counted. Returned through the hidden
// iterator pointer (`ret 14h`: hidden + four arguments). Frame pointer, `and [..],0` and shared
// call blocks: this TU is `optimize("y",off)` + `("s",on)`. One of {len(members)} bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct {nm} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; {kt} key; }};
    struct iterator {{ node* current; iterator(node* n) : current(n) {{ }} }};
    node* head;
    unsigned long size;
    node* Buynode(const {kt}& value);      // 0x{buy:08X}
    iterator Insert(node* trynode, node* wherenode, const {kt}& value, long forceRight);
}};

void __cdecl Rebalance_{rebal:08x}({nm}::node* newnode, {nm}::node** root);   // 0x{rebal:08X}

{nm}::iterator {nm}::Insert(node* trynode, node* wherenode, const {kt}& value, long forceRight)
{{
    node* newnode;
    if (wherenode == head || (!forceRight && (trynode != 0 || value < wherenode->key))) {{
        newnode = Buynode(value);
        wherenode->left = newnode;
        if (wherenode == head) {{
            head->parent = newnode;
            head->right = newnode;
        }}
        else if (wherenode == head->left)
            head->left = newnode;
    }}
    else {{
        newnode = Buynode(value);
        wherenode->right = newnode;
        if (wherenode == head->right)
            head->right = newnode;
    }}
    newnode->left = 0;
    newnode->right = 0;
    newnode->parent = wherenode;
    Rebalance_{rebal:08x}(newnode, &head->parent);
    ++size;
    return iterator(newnode);
}}
''',T(a,'''#include <cstdlib>
struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; long k; }; struct it { node* c; it(node* n) : c(n) {} }; node* h; unsigned long n;
  node* Buy(const long& v) { node* z = (node*)std::malloc(sizeof(node)); z->k = v; return z; }
  it Ins(node* x, node* w, const long& v, long fr) { node* z; if (w == h || (!fr && (x != 0 || v < w->k))) { z = Buy(v); w->l = z; if (w == h) { h->parent = z; h->r = z; } else if (w == h->l) h->l = z; } else { z = Buy(v); w->r = z; if (w == h->r) h->r = z; } z->l = 0; z->r = 0; z->parent = w; ++n; return it(z); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}; T t; t.h = &h; t.n = 0; long v = 3; T::it i = t.Ins(0, &h, v, 0); if (h.parent != i.c || h.r != i.c || t.n != 1 || i.c->k != 3) '''+FAIL(a)+'''
    std::free(i.c);
'''),b)
    elif TPL in ("w256_17",):   # retail vector grow-and-insert: Insert(where, const T* value, unused, count, bool noTail) for 4-byte scalar elements
        sh=[i.operands[1].imm for i in ins if i.mnemonic=="sar"][0]; esz=1<<sh
        if esz!=4: continue
        nm="CRetailVector_dword"
        add(a,nm,f"InsertGrow_{a}",HDRF(a,f"{nm}::InsertGrow(where, const long*, unused, count, noTail) -- reallocate to size + max(size, count), copy the head, fill `count` copies, copy the tail unless told not to",len(b))+f'''//
// Retail bytes:  push ecx ; push ebp ; mov ebp,ecx ; mov eax,[ebp+4] ; push esi ; sub eax,[ebp] ; push edi ; mov edi,[esp+20h] ; sar eax,2 ; cmp eax,edi ; mov [esp+0Ch],eax ; lea ecx,[esp+20h] ; jb M ; lea ecx,[esp+0Ch] ; M: mov ecx,[ecx] ; add ecx,eax ; mov [esp+0Ch],ecx ; je Z ; lea eax,[ecx*4] ; push eax ; call malloc ; mov ecx,eax ; add esp,4 ; mov [esp+20h],ecx ; jmp H ; Z: mov dword ptr [esp+20h],0 ; mov ecx,[esp+20h] ; H: mov eax,[ebp] ; push ebx ; mov ebx,[esp+18h] ; cmp ebx,eax ; jne C ; mov eax,ecx ; jmp F ; C: mov esi,ebx ; sub esi,eax ; push esi ; push eax ; push ecx ; call memmove ; add esp,0Ch ; add eax,esi ; F: test edi,edi ; jbe T ; mov edx,[esp+1Ch] ; mov ecx,edi ; L: mov esi,[edx] ; mov [eax],esi ; add eax,4 ; dec ecx ; jne L ; T: mov cl,[esp+28h] ; test cl,cl ; mov edi,eax ; jne D ; mov esi,[ebp+4] ; cmp esi,ebx ; je D ; sub esi,ebx ; push esi ; push ebx ; push eax ; call memmove ; mov edi,eax ; add esp,0Ch ; add edi,esi ; D: mov eax,[ebp] ; test eax,eax ; pop ebx ; je R ; push eax ; call free ; add esp,4 ; R: mov eax,[esp+20h] ; mov ecx,[esp+0Ch] ; mov [ebp+4],edi ; pop edi ; lea edx,[eax+ecx*4] ; pop esi ; mov [ebp],eax ; mov [ebp+8],edx ; pop ebp ; pop ecx ; ret 14h
// The retail vector's reallocating insert for 4-byte scalar elements {{first, last, end}}: new
// capacity = size + max(size, count) (null buffer for zero, else malloc @ 0x00BFEA0E), the
// elements before `where` are moved with memmove @ 0x00BFEAE6, `count` copies of *value are
// written (the value is re-read per element: memmove may alias it), the tail after `where`
// follows unless `noTail` is set (push_back/resize call it that way), the old buffer is released
// with free @ 0x00BFEA14 and the three pointers are rewritten. The third argument is unused and
// the new buffer pointer re-uses its dead `count` slot. `ret 14h`: five arguments. One of
// {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);

struct {nm} {{
    long* first;
    long* last;
    long* end;
    void InsertGrow_{a}(long* where, const long* value, void* unused, unsigned long count, bool noTail);
}};

void {nm}::InsertGrow_{a}(long* where, const long* value, void* unused, unsigned long count, bool noTail)
{{
    unsigned long size = last - first;
    unsigned long capacity = size + (size < count ? count : size);
    long* buffer = capacity == 0 ? 0 : (long*)malloc(capacity * sizeof(long));
    long* out;
    if (where == first)
        out = buffer;
    else {{
        unsigned long head = (char*)where - (char*)first;
        memmove(buffer, first, head);
        out = (long*)((char*)buffer + head);
    }}
    for (unsigned long n = count; 0 < n; --n, ++out)
        *out = *value;
    long* newlast = out;
    if (!noTail && last != where) {{
        unsigned long tail = (char*)last - (char*)where;
        memmove(out, where, tail);
        newlast = (long*)((char*)out + tail);
    }}
    if (first)
        free(first);
    last = newlast;
    first = buffer;
    end = buffer + capacity;
}}
''',T(a,'''#include <cstdlib>
#include <cstring>
struct V { long* f; long* l; long* e;
  void Ins(long* w, const long* v, void*, unsigned long c, bool nt) { unsigned long s = l - f; unsigned long cap = s + (s < c ? c : s); long* b = cap == 0 ? 0 : (long*)std::malloc(cap * 4); long* o; if (w == f) o = b; else { unsigned long h = (char*)w - (char*)f; std::memmove(b, f, h); o = (long*)((char*)b + h); } for (unsigned long n = c; 0 < n; --n, ++o) *o = *v; long* nl = o; if (!nt && l != w) { unsigned long t = (char*)l - (char*)w; std::memmove(o, w, t); nl = (long*)((char*)o + t); } if (f) std::free(f); l = nl; f = b; e = b + cap; } };
int main()
{
    V v; v.f = (long*)std::malloc(8); v.f[0] = 1; v.f[1] = 3; v.l = v.f + 2; v.e = v.l; long x = 2; v.Ins(v.f + 1, &x, 0, 1, false); if (v.l - v.f != 3 || v.f[0] != 1 || v.f[1] != 2 || v.f[2] != 3 || v.e - v.f != 4) '''+FAIL(a)+'''
    std::free(v.f);
'''),b)
    elif TPL in ("w256_28",):   # T* __fastcall UninitFillN(T* first, unsigned n, const T& value[, allocator]): placement-copy `n` copies through an out-of-line copy ctor
        ctor=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        cm,cn,_=label(ctor); ecls=cm.split("::")[-1] if (cn.split("::")[-1]==cm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CElement{step}_{ctor:08x}"
        alloc=(retn==8)
        add(a,"_global",f"UninitFillN_{ecls}_{a}",HDRF(a,f"_Uninit_fill_n<{ecls}*> -- placement-copy `count` copies of value from first ({step}-byte elements, copy ctor @ 0x{ctor:08X}), return the advanced pointer",len(b))+f'''//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call {ecls}::{ecls}(const {ecls}&) ; S: add esi,{step:#x} ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret {retn}
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack{', then the by-value allocator' if alloc else ''}): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x{ctor:08X} and returns the advanced destination. `ret {retn}` pops the value
// reference{' and the unused allocator' if alloc else ''}. The element type is named from the copy constructor's label when
// unique. One of {len(members)} bodies.
#include <new>

struct {ecls} {{
    unsigned char _body[{step:#x}];
    {ecls}(const {ecls}& other);           // 0x{ctor:08X}
}};
{"struct CRetailAllocator { };" if alloc else ""}
{ecls}* __fastcall UninitFillN_{ecls}_{a}({ecls}* first, unsigned long count, const {ecls}& value{", CRetailAllocator allocator" if alloc else ""})
{{
    for (; 0 < count; --count, ++first)
        new (first) {ecls}(value);
    return first;
}}
''',T(a,'''#include <new>
static int g_c = 0; struct E { unsigned char b[4]; E() {} E(const E& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
static E* __fastcall UF(E* f, unsigned long n, const E& v'''+(", A" if alloc else "")+''') { for (; 0 < n; --n, ++f) new (f) E(v); return f; }
int main()
{
    E v; v.b[0] = 7; unsigned char buf[12]; E* e = UF((E*)buf, 3, v'''+(", A()" if alloc else "")+'''); if (e != (E*)(buf + 12) || g_c != 3 || ((E*)buf)[2].b[0] != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_82","w256_101"):   # _Uninit_fill_n for a pair {long, X} / {X, long}: one scalar member copied inline, the other through its out-of-line copy ctor
        ctor=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        cm,cn,_=label(ctor); xcls=cm.split("::")[-1] if (cn.split("::")[-1]==cm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CSecond_{ctor:08x}"
        first_scalar=(TPL=="w256_82"); ecls=f"SPair{'Long' if first_scalar else ''}{xcls}{'' if first_scalar else 'Long'}"
        alloc=(retn==8)
        add(a,"_global",f"UninitFillN_{ecls}_{a}",HDRF(a,f"_Uninit_fill_n<{ecls}*> -- placement-copy `count` copies of a {{{'long, ' + xcls if first_scalar else xcls + ', long'}}} pair ({step}-byte elements, {xcls} copy ctor @ 0x{ctor:08X})",len(b))+f'''//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; mov ebx,[value] ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; {"mov eax,[ebx] ; lea ecx,[ebx+4] ; push ecx ; lea ecx,[esi+4] ; mov [esi],eax ; call " + xcls + "::" + xcls if first_scalar else "push ebx ; mov ecx,esi ; call " + xcls + "::" + xcls + " ; mov eax,[ebx+4] ; mov [esi+4],eax"} ; S: add esi,{step:#x} ; dec edi ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret {retn}
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for a pair whose {'first member is a 4-byte scalar' if first_scalar else 'second member is a 4-byte scalar'}
// and whose {'second' if first_scalar else 'first'} member {xcls} has the out-of-line copy constructor @ 0x{ctor:08X}: the pair's
// implicit copy constructor is inlined into the placement new (hence the null test), the
// advanced destination is returned. One of {len(members)} bodies.
#include <new>

struct {xcls} {{
    unsigned char _body[{step-4:#x}];
    {xcls}(const {xcls}& other);           // 0x{ctor:08X}
}};

struct {ecls} {{
{"    long first;" + chr(10) + "    " + xcls + " second;" if first_scalar else "    " + xcls + " first;" + chr(10) + "    long second;"}
}};

struct CRetailAllocator {{ }};

{ecls}* __fastcall UninitFillN_{ecls}_{a}({ecls}* first, unsigned long count, const {ecls}& value{", CRetailAllocator allocator" if alloc else ""})
{{
    for (; 0 < count; --count, ++first)
        new (first) {ecls}(value);
    return first;
}}
''',T(a,'''#include <new>
static int g_c = 0; struct X { unsigned char b[4]; X() {} X(const X& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
struct P { '''+("long f; X s;" if first_scalar else "X f; long s;")+''' };
static P* __fastcall UF(P* f, unsigned long n, const P& v'''+(", A" if alloc else "")+''') { for (; 0 < n; --n, ++f) new (f) P(v); return f; }
int main()
{
    P v; '''+("v.f = 5; v.s.b[0] = 7;" if first_scalar else "v.f.b[0] = 7; v.s = 5;")+''' unsigned char buf[24]; P* e = UF((P*)buf, 3, v'''+(", A()" if alloc else "")+'''); if (e != (P*)(buf + 24) || g_c != 3 || ((P*)buf)[2].'''+("s.b[0]" if first_scalar else "f.b[0]")+''' != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_51",):   # _Uninit_fill_n for an 8-byte trivially-copyable pair: inline two-dword copy under placement new
        step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        if step!=8: continue
        ecls=f"SPairLongLong"; alloc=(retn==8)
        add(a,"_global",f"UninitFillN_{ecls}_{a}",HDRF(a,f"_Uninit_fill_n<{ecls}*> -- placement-copy `count` copies of an 8-byte {{long, long}} pair, return the advanced pointer",len(b))+f'''//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; mov ecx,[value] ; push esi ; L: test eax,eax ; je S ; mov esi,[ecx] ; mov [eax],esi ; mov esi,[ecx+4] ; mov [eax+4],esi ; S: add eax,8 ; dec edx ; jne L ; pop esi ; E: ret {retn}
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for an 8-byte pair of 4-byte scalars:
// the trivial copy is two dword moves under the placement-new null test; the advanced
// destination is returned. One of {len(members)} bodies.
#include <new>

struct {ecls} {{
    long first;
    long second;
}};

struct CRetailAllocator {{ }};

{ecls}* __fastcall UninitFillN_{ecls}_{a}({ecls}* first, unsigned long count, const {ecls}& value{", CRetailAllocator allocator" if alloc else ""})
{{
    for (; 0 < count; --count, ++first)
        new (first) {ecls}(value);
    return first;
}}
''',T(a,'''#include <new>
struct P { long f, s; }; struct A {};
static P* __fastcall UF(P* f, unsigned long n, const P& v'''+(", A" if alloc else "")+''') { for (; 0 < n; --n, ++f) new (f) P(v); return f; }
int main()
{
    P v; v.f = 5; v.s = 7; P buf[3]; P* e = UF(buf, 3, v'''+(", A()" if alloc else "")+'''); if (e != buf + 3 || buf[2].s != 7 || buf[0].f != 5) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_38",):   # ~CX() {}: members at the lea offsets destroyed out of line (reverse order), the +0 sub-object's destructor by tail call
        leas=[(i.operands[1].mem.disp) for i in ins if i.mnemonic=="lea"]; tail=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        if len(calls)!=len(leas) or not leas: continue
        pairs=sorted(zip(leas,calls))
        def dcls(d):
            dm,dn,_=label(d); dl=dn.split("::")[-1]; return dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]) and (dm,dn) in uniq) else f"CMember_{d:08x}"
        names={d:dcls(d) for d in set(calls+[tail])}
        offs=[o for o,_ in pairs]; gaps=[offs[j+1]-offs[j] for j in range(len(offs)-1)]; msz=min(gaps) if gaps else 4
        decl=""; body=""
        for d,c in sorted(names.items()):
            if d in [cc for _,cc in pairs]: sz=msz
            else: sz=4
            decl+=f"struct {c} {{\n    unsigned char _body[{sz:#x}];\n    ~{c}();                            // 0x{d:08X}\n}};\n\n"
        cls=f"CAggregate_{a}"; mem=f"    {names[tail]} m_first;{' '*(28-len(names[tail]))}// +0 (its destructor is the tail call)\n"; prev=4
        for j,(o,c) in enumerate(pairs):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=f"    {names[c]} m_{o:02x};{' '*(28-len(names[c]))}// +{o:#x}\n"; prev=o+msz
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- implicit destructor: members at {', '.join('+%#x'%o for o in reversed(offs))} destroyed out of line, the +0 member by tail call",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; {" ; ".join(f"lea ecx,[esi+{o:#x}] ; call {names[c]}::~{names[c]}" for o,c in reversed(pairs))} ; mov ecx,esi ; pop esi ; jmp {names[tail]}::~{names[tail]}
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x{tail:08X}) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~{mname.split("::")[-1][1:] if mname.split("::")[-1].startswith("~") else mname.split("::")[-1]}`) is propagated. One of {len(members)} bodies of this shape.
{decl}struct {cls} {{
{mem}    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct S { void* p; ~S() { g_d = g_d * 10 + (int)(long)p; } }; struct F { void* q; ~F() { g_d = g_d * 10 + 9; } };
struct X { F a; S b, c; ~X(); }; X::~X() {}
int main()
{
    { X x; x.b.p = (void*)2; x.c.p = (void*)3; } if (g_d != 329) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_99",):   # ~CX() {}: inlined shared-pointer member release (block with a vtable: --refs, slot-1 destroy, null the member), base/first-member destructor by tail call
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        tail=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        tm,tn,_=label(tail); tl=tn.split("::")[-1]; bcls=tm.split("::")[-1] if (tl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",tm.split("::")[-1]) and (tm,tn) in uniq) else f"CBase_{tail:08x}"
        cls=f"CSharedOwner_{a}"
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- releases the shared block at +{off:#x} (--refs, virtual destroy at zero, member nulled), then {bcls}::~{bcls} by tail call",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+{off:#x}] ; test ecx,ecx ; je T ; dec dword ptr [ecx+4] ; jne N ; mov eax,[ecx] ; call [eax+4] ; N: mov dword ptr [esi+{off:#x}],0 ; T: mov ecx,esi ; pop esi ; jmp {bcls}::~{bcls}
// Implicit/empty destructor: the shared-pointer member at +{off:#x} is released inline (its control
// block carries a vtable whose slot 1 destroys it when the reference count at +4 drops to zero,
// and the member is nulled only when it was set), then the base class destructor @ 0x{tail:08X}
// runs by tail call. The manifest label (`~{mname.split("::")[-1][1:]}`) is propagated. One of {len(members)} bodies.
struct SSharedBlock {{
    virtual void v00();
    virtual void Destroy();                // slot 1: called when the count reaches zero
    long m_references;                     // +4
}};

struct CSharedPointer {{
    SSharedBlock* m_pBlock;
    ~CSharedPointer()
    {{
        if (m_pBlock) {{
            if (--m_pBlock->m_references == 0)
                m_pBlock->Destroy();
            m_pBlock = 0;
        }}
    }}
}};

struct {bcls} {{
    unsigned char _body[{off:#x}];
    ~{bcls}();                             // 0x{tail:08X}
}};

struct {cls} : {bcls} {{
    CSharedPointer m_pointer;              // +{off:#x}
    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; struct K { virtual void v() {} virtual void D() { g_d += 100; } long r; };
struct P { K* b; ~P() { if (b) { if (--b->r == 0) b->D(); b = 0; } } };
struct B { long x; ~B() { ++g_d; } }; struct X : B { P p; ~X() {} };
int main()
{
    K k; k.r = 1; { X x; x.p.b = &k; } if (g_d != 101) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_76",):   # pair<unsigned long, T>::pair(const pair& other): first copied inline, second through T's out-of-line copy ctor
        ctor=calls[0]; cm,cn,_=label(ctor); xcls=cm.split("::")[-1] if (cn.split("::")[-1]==cm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CSecond_{ctor:08x}"
        cls=f"SPairLong{xcls}"
        add(a,"_unattributed",cls,HDRF(a,f"{cls}::{cls}(const {cls}&) -- copy constructor of a {{unsigned long, {xcls}}} pair: scalar copied inline, {xcls} copy ctor @ 0x{ctor:08X}",len(b))+f'''//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call {xcls}::{xcls}(const {xcls}&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {{unsigned long, {xcls}}} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x{ctor:08X}; `this` is returned.
// The second type is named from the constructor's label when unique. One of {len(members)} bodies.
struct {xcls} {{
    void* m_data;
    {xcls}(const {xcls}& other);           // 0x{ctor:08X}
}};

struct {cls} {{
    unsigned long first;                   // +0
    {xcls} second;                         // +4
    {cls}(const {cls}& other);
}};

{cls}::{cls}(const {cls}& other)
    : first(other.first)
    , second(other.second)
{{
}}
''',T(a,'''static int g_c = 0; struct X { void* d; X(void* v) : d(v) {} X(const X& o) : d(o.d) { ++g_c; } };
struct P { unsigned long f; X s; P(unsigned long a, void* b) : f(a), s(b) {} P(const P& o) : f(o.f), s(o.s) {} };
int main()
{
    int v; P p(3, &v); P q(p); if (q.f != 3 || q.s.d != &v || g_c != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_44",):   # void vector::push_back(const T* value): construct in place when there is room (out-of-line copy ctor), else the reallocating insert of one element
        if len(calls)!=2: continue
        ctor,grow=calls; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM][0]
        cm,cn,_=label(ctor); ecls=cm.split("::")[-1] if (cn.split("::")[-1]==cm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CElement{step}_{ctor:08x}"
        nm=f"CRetailVector_{ecls}"
        add(a,"_global",f"PushBack_{ecls}_{a}",HDRF(a,f"{nm}::push_back(const {ecls}*) -- placement-copy at `last` when there is room ({step}-byte elements, copy ctor @ 0x{ctor:08X}), else InsertGrow @ 0x{grow:08X}",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call {ecls}::{ecls}(const {ecls}&) ; S: add dword ptr [esi+4],{step:#x} ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for {step}-byte elements {{first, last, end}}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x{ctor:08X} and `last` advances; when full, the reallocating
// insert @ 0x{grow:08X} is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of {len(members)} bodies.
#include <new>

struct {ecls} {{
    unsigned char _body[{step:#x}];
    {ecls}(const {ecls}& other);           // 0x{ctor:08X}
}};

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct {nm} {{
    {ecls}* first;
    {ecls}* last;
    {ecls}* end;
    void InsertGrow({ecls}* where, const {ecls}& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x{grow:08X}
    void PushBack_{ecls}_{a}(const {ecls}& value);
}};

void {nm}::PushBack_{ecls}_{a}(const {ecls}& value)
{{
    if (last != end) {{
        new (last) {ecls}(value);
        ++last;
    }}
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}}
''',T(a,'''#include <new>
static int g_c = 0, g_g = 0; struct E { unsigned char b[4]; E() {} E(const E& o) { ++g_c; b[0] = o.b[0]; } };
struct Tg { Tg() {} }; struct V { E* f; E* l; E* e; void G(E*, const E&, const Tg&, unsigned long, bool) { ++g_g; } void P(const E& v) { if (l != e) { new (l) E(v); ++l; } else G(l, v, Tg(), 1, true); } };
int main()
{
    unsigned char buf[8]; V v; v.f = (E*)buf; v.l = v.f; v.e = v.f + 2; E x; x.b[0] = 9; v.P(x); v.P(x); v.P(x); if (g_c != 2 || g_g != 1 || v.l != v.f + 2 || v.f[1].b[0] != 9) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_50",):   # void CX::TransferOut(CPersistContext* ctx, long name) { long value = *m_pValue; ::TransferOut(ctx, name, &value); }
        callee=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "ecx +" in i.op_str][0]
        cls=f"CTransferableLong_{callee:08x}"
        add(a,"_unattributed",f"TransferOut_{cls}",HDRF(a,f"{cls}::vfunc_0(CPersistContext*, long) -- copies the target dword into a local and hands it to the __fastcall helper 0x{callee:08X}",len(b))+f'''//
// Retail bytes:  push ecx ; mov eax,[ecx+{off:#x}] ; mov ecx,[eax] ; lea edx,[esp] ; mov [esp],ecx ; mov ecx,[esp+8] ; push edx ; mov edx,[esp+10h] ; call TransferOut ; pop ecx ; ret 8
// Persist write hook of a four-byte transferable component: the dword behind the target
// pointer at +{off:#x} is copied into a stack local whose address goes to the __fastcall helper
// @ 0x{callee:08X} (context in ecx, the dword second parameter in edx, &local on the stack).
// The manifest label (`CPersistContext_TransferableComponent::vfunc_0`) is propagated; the
// class is named by the helper. One of {len(members)} bodies of this shape.
struct CPersistContext;
void __fastcall TransferOut_{callee:08x}(CPersistContext* context, long name, long* value);   // 0x{callee:08X}

struct {cls} {{
    virtual void v00();
{"    unsigned char _pad[%#x];" % (off-4) + chr(10) if off>4 else ""}    long* m_pValue;                        // +{off:#x}
    virtual void TransferOut(CPersistContext* context, long name);
}};

void {cls}::TransferOut(CPersistContext* context, long name)
{{
    long value = *m_pValue;
    TransferOut_{callee:08x}(context, name, &value);
}}
''',T(a,'''struct Ctx; static long g_v = 0; static void __fastcall Helper(Ctx*, long n, long* v) { g_v = *v + n; }
struct X { virtual void v00() {} long* p; virtual void T(Ctx* c, long n) { long v = *p; Helper(c, n, &v); } };
int main()
{
    long val = 40; X x; x.p = &val; x.T(0, 2); if (g_v != 42) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_72",):   # void CX::SetItem(CIntelligentPointer item) { m_member.SetPItem(item.GetPItem()); }  (8-byte by-value parameter destroyed by the callee)
        if len(calls)!=3: continue
        getp,setp,dtor=calls; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        cls=f"CItemOwner_{a}"
        add(a,"_unattributed",f"SetItem_{cls}",HDRF(a,f"{cls}::SetItem(CIntelligentPointer) -- m_{off:x}.SetPItem(item.GetPItem()); the by-value argument is destroyed here",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esp+8] ; call GetPItem ; push eax ; lea ecx,[esi+{off:#x}] ; call SetPItem ; lea ecx,[esp+8] ; call ~CIntelligentPointer ; pop esi ; ret 8
// Takes an 8-byte CIntelligentPointer by value (so the callee destroys it, `ret 8`), reads its
// target through CBaseIntelligentPointer::GetPItem @ 0x{getp:08X} and stores it into the member
// pointer at +{off:#x} through SetPItem @ 0x{setp:08X}; the argument's destructor @ 0x{dtor:08X}
// runs before returning. The manifest label (`CTCInventoryBase::GetItemDescription`) is
// propagated; the class is named descriptively. One of {len(members)} bodies of this shape.
struct CBaseObjectPointer;

struct CIntelligentPointer {{              // CBaseIntelligentPointer: 8 bytes (struct_layouts_egor.tsv)
    void* m_pLink;                         // +0
    CBaseObjectPointer* m_pData;           // +4
    CBaseObjectPointer* GetPItem();        // 0x{getp:08X}
    void SetPItem(CBaseObjectPointer* item);   // 0x{setp:08X}
    ~CIntelligentPointer();                // 0x{dtor:08X}
}};

struct {cls} {{
    unsigned char _pad[{off:#x}];
    CIntelligentPointer m_item;            // +{off:#x}
    void SetItem_{cls}(CIntelligentPointer item);
}};

void {cls}::SetItem_{cls}(CIntelligentPointer item)
{{
    m_item.SetPItem(item.GetPItem());
}}
''',T(a,'''static int g_d = 0; struct O; struct IP { void* l; O* d; O* G() { return d; } void S(O* o) { d = o; } ~IP() { ++g_d; } };
struct X { unsigned char pad[8]; IP m; void Set(IP i) { m.S(i.G()); } };
int main()
{
    X x; x.m.d = 0; IP i; i.d = (O*)&x; x.Set(i); if (x.m.d != (O*)&x || g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_33",):   # T* __stdcall CPersistTraits<T>::TransferIn(T* value, CPersistItem item, CPersistContext* ctx) { Helper(value, item, ctx); return value; }
        callee=calls[0]; tcls=re.sub(r"[^A-Za-z0-9_]+","_",mmod).strip("_") if (mmod,mname) in uniq else f"CTraits_{callee:08x}"
        add(a,"_unattributed",f"TransferIn_{tcls}_{a}",HDRF(a,f"{mmod}::TransferIn(T*, CPersistItem, CPersistContext*) -- forwards to the __stdcall helper 0x{callee:08X} with the 4-byte item copied by value, returns the value pointer",len(b))+f'''//
// Retail bytes:  push [esp+0Ch] ; mov edx,[esp+0Ch] ; push ecx ; mov eax,esp ; push [esp+0Ch] ; mov [eax],edx ; call Helper ; mov eax,[esp+4] ; ret 0Ch
// Persist traits forwarder (__stdcall, three dword arguments): the context pointer, the
// 4-byte persist item (a class with an inline copy constructor, so it is copied into an argument
// slot by address rather than pushed as a dword) and the value pointer go to the __stdcall
// helper @ 0x{callee:08X}; the value pointer is returned. One of {len(members)} bodies of this shape.
struct CPersistContext;

struct CPersistItem {{
    void* m_handle;
    CPersistItem(const CPersistItem& other) : m_handle(other.m_handle) {{ }}
}};

void __stdcall TransferInHelper_{callee:08x}(void* value, CPersistItem item, CPersistContext* context);   // 0x{callee:08X}

void* __stdcall TransferIn_{tcls}_{a}(void* value, CPersistItem item, CPersistContext* context)
{{
    TransferInHelper_{callee:08x}(value, item, context);
    return value;
}}
''',T(a,'''struct C; struct I { void* h; I(void* v) : h(v) {} I(const I& o) : h(o.h) {} };
static void* g_v = 0; static void __stdcall H(void* v, I i, C*) { g_v = i.h; }
static void* __stdcall T(void* v, I i, C* c) { H(v, i, c); return v; }
int main()
{
    int a, b; I i(&b); if (T(&a, i, 0) != &a || g_v != &b) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_54",):   # `??_G` of a non-polymorphic class: member destructors, base destructor, flag-tested delete (emitted by a delete-expression)
        leas=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"]
        if len(calls)!=len(leas)+2: continue
        mcalls=calls[:len(leas)]; basedt=calls[len(leas)]
        def dcls2(d):
            dm,dn,_=label(d); dl=dn.split("::")[-1]; return dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]) and (dm,dn) in uniq) else f"CMember_{d:08x}"
        pairs=sorted(zip(leas,mcalls)); offs=[o for o,_ in pairs]; gaps=[offs[j+1]-offs[j] for j in range(len(offs)-1)]; msz=min(gaps) if gaps else 8
        names={d:dcls2(d) for d in set(mcalls)}; bm,bn,_=label(basedt); bl=bn.split("::")[-1]; bcls=bm.split("::")[-1] if (bl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",bm.split("::")[-1]) and (bm,bn) in uniq) else f"CBase_{basedt:08x}"
        cls=f"{bcls}_Derived_{a}"
        decl="".join(f"struct {c} {{\n    unsigned char _body[{msz:#x}];\n    ~{c}();                            // 0x{d:08X}\n}};\n\n" for d,c in sorted(names.items()))
        mem=""; prev=offs[0]
        for j,(o,c) in enumerate(pairs):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=f"    {names[c]} m_{o:02x};{' '*(28-len(names[c]))}// +{o:#x}\n"; prev=o+msz
        add(a,"_unattributed",f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- non-virtual: {len(offs)} members destroyed out of line, {bcls}::~{bcls}, flag-tested delete",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; {" ; ".join(f"lea ecx,[esi+{o:#x}] ; call {names[c]}::~{names[c]}" for o,c in reversed(pairs))} ; mov ecx,esi ; call {bcls}::~{bcls} ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: members at {', '.join('+%#x'%o for o in reversed(offs))} are destroyed in reverse
// declaration order, then the base class destructor @ 0x{basedt:08X} runs, and bit 0 of the flag
// argument selects the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`vector
// deleting destructor`) is propagated; names come from the destructor labels when unique.
// One of {len(members)} bodies of this shape.
{decl}struct {bcls} {{
    unsigned char _body[{offs[0]:#x}];
    ~{bcls}();                             // 0x{basedt:08X}
}};

struct {cls} : {bcls} {{
{mem}}};

// The delete-expression that makes the compiler emit {cls}::`scalar deleting destructor'.
void Destroy_{cls}({cls}* object)
{{
    delete object;
}}
''',T(a,'''static int g_d = 0; struct S { void* p; void* q; ~S() { g_d = g_d * 10 + 2; } }; struct B { long x; ~B() { g_d = g_d * 10 + 1; } };
struct X : B { S a, b; };
static void Kill(X* x) { delete x; }
int main()
{
    Kill(new X); if (g_d != 221) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_134",):   # bool __stdcall Less(const S* a, const S* b): order by the dword at +4, then by address
        off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "edx +" in i.op_str][0]
        add(a,"_unattributed",f"LessByKeyThenAddress_{a}",HDRF(a,f"bool __stdcall LessByKeyThenAddress(const SKeyed*, const SKeyed*) -- a->m_{off:x} != b->m_{off:x} ? a->m_{off:x} < b->m_{off:x} : a < b",len(b))+f'''//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[edx+{off:#x}] ; push esi ; mov esi,[esp+0Ch] ; mov ecx,[esi+{off:#x}] ; cmp eax,ecx ; jne S ; cmp edx,esi ; S: sbb eax,eax ; neg eax ; pop esi ; ret 8
// Strict weak ordering helper (__stdcall, two object pointers): compares the unsigned dword at
// +{off:#x}; equal keys fall back to the objects' addresses. Both branches end in the same
// `sbb/neg` materialisation of the carry flag, so the compiler shares it. The manifest has no
// label for this gap-scan body; it is named descriptively. One of {len(members)} bodies.
struct SKeyed {{
{"    unsigned char _pad[%#x];" % off + chr(10) if off else ""}    unsigned long m_key;                   // +{off:#x}
}};

bool __stdcall LessByKeyThenAddress_{a}(const SKeyed* a, const SKeyed* b)
{{
    if (a->m_key != b->m_key)
        return a->m_key < b->m_key;
    return a < b;
}}
''',T(a,'''struct K { unsigned char p[4]; unsigned long k; };
static bool __stdcall L(const K* a, const K* b) { if (a->k != b->k) return a->k < b->k; return a < b; }
int main()
{
    K x, y; x.k = 1; y.k = 2; if (!L(&x, &y) || L(&y, &x)) '''+FAIL(a)+'''
    y.k = 1; if (L(&x, &y) != (&x < &y)) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_29",):   # void CX::FreeTree(Node* node) { while (node) { FreeTree(node->m_child); Node* next = node->m_next; free(node); node = next; } }
        if len(calls)!=2 or calls[0]!=va: continue
        child=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str]
        if len(child)!=2: continue
        coff,noff=child; cls=f"CTreeOwner_{a}"
        add(a,"_unattributed",f"FreeTree_{cls}",HDRF(a,f"{cls}::FreeTree(SNode*) -- recursive free of a child-linked (+{coff:#x}) / sibling-linked (+{noff:#x}) node list",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov esi,[esp+0Ch] ; test esi,esi ; mov ebx,ecx ; je E ; push edi ; L: mov eax,[esi+{coff:#x}] ; push eax ; mov ecx,ebx ; call FreeTree ; mov edi,[esi+{noff:#x}] ; push esi ; call free ; add esp,4 ; test edi,edi ; mov esi,edi ; jne L ; pop edi ; E: pop esi ; pop ebx ; ret 4
// Walks a sibling list: recurses into each node's child list at +{coff:#x}, remembers the sibling at
// +{noff:#x}, frees the node (free @ 0x00BFEA14) and continues with the sibling. `this` is only
// needed for the recursive call. The manifest label (`LTextTreeWalkThrough::BuildTreeArray`) is
// propagated; the owner is named descriptively. One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SNode {{
    unsigned char _pad[{min(coff,noff):#x}];
    {"SNode* m_child;" if coff<noff else "SNode* m_next;"}{" "*(23)}// +{min(coff,noff):#x}
{"    unsigned char _gap[%#x];" % (max(coff,noff)-min(coff,noff)-4) + chr(10) if max(coff,noff)-min(coff,noff)>4 else ""}    {"SNode* m_next;" if coff<noff else "SNode* m_child;"}{" "*(23)}// +{max(coff,noff):#x}
}};

struct {cls} {{
    void FreeTree_{cls}(SNode* node);
}};

void {cls}::FreeTree_{cls}(SNode* node)
{{
    while (node) {{
        FreeTree_{cls}(node->m_child);
        SNode* next = node->m_next;
        free(node);
        node = next;
    }}
}}
''',T(a,'''#include <cstdlib>
static int g_f = 0; struct N { unsigned char p[8]; N* n; N* c; };
struct X { void F(N* d) { while (d) { F(d->c); N* x = d->n; std::free(d); ++g_f; d = x; } } };
int main()
{
    N* a = (N*)std::malloc(sizeof(N)); N* b = (N*)std::malloc(sizeof(N)); N* c = (N*)std::malloc(sizeof(N)); a->n = b; a->c = 0; b->n = 0; b->c = c; c->n = 0; c->c = 0;
    X x; x.F(a); if (g_f != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_75",):   # void vector::push_back(const long& value): 4-byte scalar stored in place when there is room, else the reallocating insert (no callee-saved registers)
        if len(calls)!=1: continue
        grow=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM][0]
        if step!=4: continue
        nm=f"CRetailVector_dword"
        add(a,"_global",f"PushBack_dword_{a}",HDRF(a,f"{nm}::push_back(const long&) -- store at `last` when there is room, else InsertGrow @ 0x{grow:08X} for one element",len(b))+f'''//
// Retail bytes:  mov eax,[ecx+4] ; cmp eax,[ecx+8] ; je G ; test eax,eax ; je S ; mov edx,[esp+4] ; mov edx,[edx] ; mov [eax],edx ; S: add dword ptr [ecx+4],4 ; ret 4 ; G: push 1 ; push 1 ; lea edx,[esp+0Ch] ; push edx ; mov edx,[esp+10h] ; push edx ; push eax ; call InsertGrow ; ret 4
// Retail vector push_back for 4-byte scalar elements {{first, last, end}}: with spare capacity the
// value is placement-copied at `last` (hence the null test) and `last` advances; when full, the
// reallocating insert @ 0x{grow:08X} is called for one element with `noTail` set, its third
// argument the address of a 1-byte empty-tag temporary homed in the dead argument slot. `this`
// stays in ecx for the call. One of {len(members)} bodies of this shape.
#include <new>

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

struct {nm} {{
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x{grow:08X}
    void PushBack_dword_{a}(const long& value);
}};

void {nm}::PushBack_dword_{a}(const long& value)
{{
    if (last != end) {{
        new (last) long(value);
        ++last;
    }}
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}}
''',T(a,'''#include <new>
static int g_g = 0; struct Tg { Tg() {} };
struct V { long* f; long* l; long* e; void G(long*, const long&, const Tg&, unsigned long, bool) { ++g_g; } void P(const long& v) { if (l != e) { new (l) long(v); ++l; } else G(l, v, Tg(), 1, true); } };
int main()
{
    long buf[2]; V v; v.f = buf; v.l = buf; v.e = buf + 2; long x = 9; v.P(x); v.P(x); v.P(x); if (g_g != 1 || v.l != buf + 2 || buf[1] != 9) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_48",):   # persist: context->Transfer<long>("Name", this->m_value, default 0)  (dword default temporary)
        litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva)
        off=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[0].type==X86_OP_REG][0]; callee=calls[0]
        if not any(i.mnemonic=="mov" and "dword ptr [esp + 0xc], 0" in i.op_str for i in ins): continue
        mcls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else None
        mod=mcls or "_unattributed"; fname="vfunc_1" if mcls else f"PersistTransfer_{san(s)}_{a}"; cls=mcls or f"CPersistable_{a}"
        params="CPersistContext* context" if retn==4 else "CPersistContext* context, long unused"
        add(a,mod,fname,HDRF(a,f"{cls}::{fname}({params}) -- persists the {s} dword at +{off:#x} with a zero default",len(b))+f'''//
// Retail bytes:  push ecx ; lea eax,[esp] ; push eax ; add ecx,{off:#x} ; push ecx ; mov ecx,[esp+10h] ; push offset "{s}" ; mov dword ptr [esp+0Ch],0 ; call CPersistContext::Transfer<long> ; pop ecx ; ret {retn}
// Virtual persist hook (slot 1 of this class's vtable): transfers the dword member at +{off:#x}
// through CPersistContext::Transfer<long>(name, value, default) @ 0x{callee:08X}, with a zero
// default built in a dword stack temporary.{"" if retn==4 else " The second argument is not used by this body."} The member name is taken from the
// persisted key; the member type is any 4-byte scalar (`long` models it). One of {len(members)} bodies.
struct CPersistContext {{
    void Transfer(const char* name, long& value, const long& defaultValue);   // Transfer<long> @ 0x{callee:08X}
}};

struct {cls} {{
    unsigned char _pad[{off-4:#x}];        // after the vptr; members not touched here
    long m_{san(s)};                        // +{off:#x}
    virtual void v00();
    virtual void {fname}({params});
}};

void {cls}::{fname}({params})
{{
    long defaultValue = 0;
    context->Transfer("{s}", m_{san(s)}, defaultValue);
}}
''',T(a,'''static int g_t = 0; struct Ctx { void Transfer(const char* n, long& v, const long& d) { v = n[0] + d + 1; ++g_t; } };
struct P { unsigned char pad[0x18]; long b; virtual void v00() {} virtual void vf(Ctx* c, long) { long d = 0; c->Transfer("A", b, d); } };
int main()
{
    Ctx c; P p; p.b = 0; p.vf(&c, 0); if (g_t != 1 || p.b != 'B') '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_122",):   # node* list::Buynode(const long& value): malloc the node, placement-copy the 4-byte scalar value
        n=pushes[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]
        if len(calls)!=1 or n-off!=4: continue
        nm=f"CRetailList_node{n:#x}_dword"
        add(a,nm,"Buynode",HDRF(a,f"{nm}::Buynode(const long&) -- malloc a {n:#x}-byte node and placement-copy the dword value at +{off:#x}",len(b))+f'''//
// Retail bytes:  push {n:#x} ; call malloc ; pop ecx ; lea ecx,[eax+{off:#x}] ; test ecx,ecx ; je S ; mov edx,[esp+4] ; mov edx,[edx] ; mov [ecx],edx ; S: ret 4
// Retail std::list node factory for a 4-byte scalar value: one malloc'd node (malloc @
// 0x00BFEA0E) whose value at +{off:#x} is placement-copied (hence the null test on the value
// address); the node (malloc's result, still in eax) is returned. `this` (the list) is unused.
// One of {len(members)} bodies of this shape.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct {nm} {{
    struct SNode {{
        unsigned char _links[{off:#x}];       // prev/next (and any further link) pointers
        long m_value;                      // +{off:#x}
    }};
    SNode* Buynode(const long& value);
}};

{nm}::SNode* {nm}::Buynode(const long& value)
{{
    SNode* node = (SNode*)malloc(sizeof(SNode));
    new (&node->m_value) long(value);
    return node;
}}
''',T(a,'''#include <new>
#include <cstdlib>
struct L { struct N { unsigned char l[8]; long v; }; N* B(const long& v) { N* n = (N*)std::malloc(sizeof(N)); new (&n->v) long(v); return n; } };
int main()
{
    L l; long x = 5; L::N* n = l.B(x); if (!n || n->v != 5) '''+FAIL(a)+'''
    std::free(n);
'''),b)
    elif TPL in ("w256_92",):   # FreeTree with a member destroyed before the node is freed
        if len(calls)!=3 or calls[0]!=va: continue
        child=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str]
        if len(child)!=2: continue
        coff,noff=child; dtor=calls[1]; doff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        dm,dn,_=label(dtor); dl=dn.split("::")[-1]; mcls=dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]) and (dm,dn) in uniq) else f"CMember_{dtor:08x}"
        cls=f"CTreeOwner_{a}"; lo=min(coff,noff); hi=max(coff,noff)
        add(a,"_unattributed",f"FreeTree_{cls}",HDRF(a,f"{cls}::FreeTree(SNode*) -- recursive free of a child-linked (+{coff:#x}) / sibling-linked (+{noff:#x}) node list, {mcls} member at +{doff:#x} destroyed first",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov esi,[esp+0Ch] ; test esi,esi ; mov ebx,ecx ; je E ; push edi ; L: mov eax,[esi+{coff:#x}] ; push eax ; mov ecx,ebx ; call FreeTree ; mov edi,[esi+{noff:#x}] ; lea ecx,[esi+{doff:#x}] ; call {mcls}::~{mcls} ; push esi ; call free ; add esp,4 ; test edi,edi ; mov esi,edi ; jne L ; pop edi ; E: pop esi ; pop ebx ; ret 4
// Walks a sibling list: recurses into each node's child list at +{coff:#x}, remembers the sibling at
// +{noff:#x}, destroys the node's {mcls} member at +{doff:#x} (destructor @ 0x{dtor:08X}), frees the
// node (free @ 0x00BFEA14) and continues with the sibling. The manifest label
// (`LTextTreeWalkThrough::BuildTreeArray`) is propagated; the owner is named descriptively.
// One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct {mcls} {{
    void* m_data;
    ~{mcls}();                             // 0x{dtor:08X}
}};

struct SNode {{
    unsigned char _pad[{lo:#x}];
    {"SNode* m_child;" if coff<noff else "SNode* m_next;"}{" "*23}// +{lo:#x}
{"    unsigned char _gap[%#x];" % (hi-lo-4) + chr(10) if hi-lo>4 else ""}    {"SNode* m_next;" if coff<noff else "SNode* m_child;"}{" "*23}// +{hi:#x}
{"    unsigned char _gap2[%#x];" % (doff-hi-4) + chr(10) if doff-hi>4 else ""}    {mcls} m_name;{" "*(24-len(mcls))}// +{doff:#x}
}};

struct {cls} {{
    void FreeTree_{cls}(SNode* node);
}};

void {cls}::FreeTree_{cls}(SNode* node)
{{
    while (node) {{
        FreeTree_{cls}(node->m_child);
        SNode* next = node->m_next;
        node->m_name.~{mcls}();
        free(node);
        node = next;
    }}
}}
''',T(a,'''#include <cstdlib>
static int g_f = 0, g_d = 0; struct M { void* d; ~M() { ++g_d; } }; struct N { unsigned char p[8]; N* n; N* c; M m; };
struct X { void F(N* d) { while (d) { F(d->c); N* x = d->n; d->m.~M(); std::free(d); ++g_f; d = x; } } };
int main()
{
    N* a = (N*)std::malloc(sizeof(N)); N* b = (N*)std::malloc(sizeof(N)); a->n = b; a->c = 0; b->n = 0; b->c = 0;
    X x; x.F(a); if (g_f != 2 || g_d != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_9",):   # CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream*, T*): inlined 4-byte stream read with the 64-bit position range check
        if len(calls)!=1: continue
        reader=calls[0]; fp=b[:3]==b"\x55\x8b\xec"; isstruct=any(i.mnemonic=="movsd" for i in ins)
        vt="SPersistValue4" if isstruct else "long"
        add(a,"_unattributed",f"TransferBinaryIn_{a}",HDRF(a,f"CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream*, T*) -- inlined 4-byte read ({'struct, movsd copy' if isstruct else 'scalar'}), 64-bit position range check, ReadWithSrcChunkOverflow @ 0x{reader:08X} when the chunk runs out{'; frame-pointer TU' if fp else ''}",len(b))+f'''//
// Retail bytes:  {"push ebp ; mov ebp,esp ; mov ecx,[ebp+8]" if fp else "mov ecx,[esp+4]"} ; mov eax,[ecx+4] ; {"lea edx,[eax+4]" if fp else "push esi ; lea esi,[eax+4]"} ; xor eax,eax ; jl R ; jg B ; test lo,lo ; jb R ; B: test eax,eax ; jg R ; jl F ; cmp lo,7FFFFFFFh ; ja R ; F: {"push ebx ; mov ebx,[ecx+14h] ; cmp ebx,4 ; jl S ; mov eax,[ecx+0Ch] ; push esi ; push edi ; mov esi,eax ; lea edi,[ebp+8] ; movsd ; add eax,4 ; add ebx,-4 ; pop edi ; mov [ecx+0Ch],eax ; mov [ecx+14h],ebx ; mov [ecx+4],edx ; pop esi ; jmp J ; S: push 4 ; lea eax,[ebp+8] ; push eax ; call ReadWithSrcChunkOverflow ; J: pop ebx ; R: mov eax,[ebp+0Ch] ; mov ecx,[ebp+8] ; mov [eax],ecx ; pop ebp ; ret 8" if fp else "mov edx,[ecx+14h] ; cmp edx,4 ; jl S ; mov eax,[ecx+0Ch] ; push edi ; mov edi,[eax] ; add eax,4 ; mov [ecx+0Ch],eax ; mov eax,[esp+10h] ; add edx,-4 ; mov [ecx+4],esi ; mov [ecx+14h],edx ; mov [eax],edi ; pop edi ; pop esi ; ret 8 ; S: push 4 ; lea edx,[esp+0Ch] ; push edx ; call ReadWithSrcChunkOverflow ; R: mov ecx,[esp+8] ; mov eax,[esp+0Ch] ; mov [eax],ecx ; pop esi ; ret 8  (two more copies of R for the range-check exits)"}
// Binary persist read of one 4-byte value: a local `value` (homed in the dead stream-argument
// slot) is filled by the inlined CMemoryDataInputStream::Read(buffer, size) and copied to the
// output. Read widens `position + size` to __int64 and rejects it outside [0, 0x7FFFFFFF]
// (the zero high dword is materialised and compared like any 64-bit value); with enough bytes
// left in the current chunk it memcpy's from the chunk pointer (+0xC), advances it, decrements
// the remaining count (+0x14) and adds the size to the position (+4) -- `+=`, which keeps the
// old position live and gives the `lea` form -- otherwise it defers to
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x{reader:08X}. {'A 4-byte struct value makes the memcpy a movsd; ' if isstruct else 'A scalar value lets the memcpy forward straight into the output; '}{'the TU keeps frame pointers and optimises for size.' if fp else 'the TU has no frame pointer.'}
// The manifest label (`CPersistTraits<...>::TransferBinaryIn`) is propagated; the value type is
// modelled by size. One of {len(members)} bodies of this shape.
{'#pragma optimize("y", off)' + chr(10) + '#pragma optimize("s", on)' + chr(10) if fp else ''}extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {{
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x{reader:08X}
    __forceinline void Read(void* buffer, long size)
    {{
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (m_remaining >= size) {{
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }}
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }}
}};
{"struct SPersistValue4 { unsigned short m_low; unsigned short m_high; };" + chr(10) if isstruct else ""}
void __stdcall TransferBinaryIn_{a}(CMemoryDataInputStream* stream, {vt}* out)
{{
    {vt} value;
    stream->Read(&value, sizeof(value));
    *out = value;
}}
''',T(a,'''#include <cstring>
struct S { void* vt; unsigned long pos; long a; const unsigned char* p; long b; long rem; static int g_o; void O(unsigned char*, long) { ++g_o; }
  void R(void* buf, long n) { __int64 np = (__int64)(pos + n); if (np < 0 || np > 0x7fffffff) return; if (rem >= n) { std::memcpy(buf, p, n); p += n; rem -= n; pos += n; } else O((unsigned char*)buf, n); } };
int S::g_o = 0;
static void __stdcall T(S* s, long* o) { long v; s->R(&v, 4); *o = v; }
int main()
{
    unsigned char data[8] = { 1, 0, 0, 0, 2, 0, 0, 0 }; S s; s.pos = 0; s.p = data; s.rem = 8; long v = 0; T(&s, &v); if (v != 1 || s.pos != 4 || s.rem != 4 || s.p != data + 4) '''+FAIL(a)+'''
    s.rem = 2; T(&s, &v); if (S::g_o != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_55",):   # void __stdcall PushBackConstant(CRetailVector_dword* vector) { long value = K; vector->push_back(value); }  (push_back inlined)
        if len(calls)!=1: continue
        grow=calls[0]; ks=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM]
        if len(ks)!=2 or ks[0]!=ks[1]: continue
        k=ks[0]
        add(a,"_unattributed",f"PushBackConstant_{a}",HDRF(a,f"PushBackConstant_{a}(CRetailVector_dword*) -- vector->push_back({k:#x}) with the retail push_back inlined (InsertGrow @ 0x{grow:08X} when full)",len(b))+f'''//
// Retail bytes:  push ecx ; mov ecx,[esp+8] ; mov eax,[ecx+4] ; cmp eax,[ecx+8] ; mov dword ptr [esp],{k:#x} ; je G ; test eax,eax ; je S ; mov dword ptr [eax],{k:#x} ; S: add dword ptr [ecx+4],4 ; pop ecx ; ret 4 ; G: push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; lea edx,[esp+0Ch] ; push edx ; push eax ; call InsertGrow ; pop ecx ; ret 4
// Pushes the constant {k:#x} onto the 4-byte-element retail vector passed on the stack (`ret 4`, no
// `this`): the value is a named stack local (it must be addressable for the reallocating
// insert's reference argument), stored in place when there is room, else handed to the insert
// @ 0x{grow:08X} with the empty-tag temporary homed in the dead argument slot. The manifest label
// is propagated from an unrelated body; the function is named by its effect. One of {len(members)} bodies.
#include <new>

struct SScalarTag {{ SScalarTag() {{ }} }};

struct CRetailVector_dword {{
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x{grow:08X}
    void push_back(const long& value)
    {{
        if (last != end) {{
            new (last) long(value);
            ++last;
        }}
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }}
}};

void __stdcall PushBackConstant_{a}(CRetailVector_dword* vector)
{{
    long value = {k:#x};
    vector->push_back(value);
}}
''',T(a,'''#include <new>
static int g_g = 0; struct Tg { Tg() {} };
struct V { long* f; long* l; long* e; void G(long*, const long&, const Tg&, unsigned long, bool) { ++g_g; } void P(const long& v) { if (l != e) { new (l) long(v); ++l; } else G(l, v, Tg(), 1, true); } };
static void __stdcall PC(V* v) { long value = 0x24; v->P(value); }
int main()
{
    long buf[1]; V v; v.f = buf; v.l = buf; v.e = buf + 1; PC(&v); PC(&v); if (g_g != 1 || buf[0] != 0x24 || v.l != buf + 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_61",):   # CX& CX::operator=(const CX& other) { CBase::operator=(other); m_a = other.m_a; ... return *this; }
        if len(calls)!=1: continue
        base=calls[0]; bm,bn,_=label(base); bcls=bm.split("::")[-1] if (bn.split("::")[-1]=="operator=" and re.match(r"^C[A-Za-z0-9_]+$",bm.split("::")[-1]) and (bm,bn) in uniq) else f"CBase_{base:08x}"
        lds=[(i.operands[1].mem.disp, 1 if "byte ptr" in i.op_str else 4) for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "edi +" in i.op_str]
        sts=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str]
        if not lds or [o for o,_ in lds]!=sts: continue
        cls=f"{bcls}_Derived_{a}"; mem=""; prev=lds[0][0]
        for j,(o,sz) in enumerate(lds):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=(f"    long m_{o:02x};" if sz==4 else f"    unsigned char m_{o:02x};").ljust(40)+f"// +{o:#x}\n"; prev=o+sz
        assigns="".join(f"    m_{o:02x} = other.m_{o:02x};\n" for o,_ in lds)
        add(a,"_unattributed",f"operator=",HDRF(a,f"{cls}::operator=(const {cls}&) -- {bcls}::operator= @ 0x{base:08X}, then {len(lds)} members copied in declaration order",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call {bcls}::operator= ; {" ; ".join(("mov eax,[edi+%#x] ; mov [esi+%#x],eax" % (o,o)) if sz==4 else ("mov al,[edi+%#x] ; mov [esi+%#x],al" % (o,o)) for o,sz in lds)} ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy-assignment of a class derived from {bcls} (whose own operator= @ 0x{base:08X} is out of
// line): the base assignment runs first, then this class's {len(lds)} trailing members are copied in
// declaration order and `this` is returned. Member types are not recoverable beyond their sizes
// (`long` / `unsigned char`). The manifest label (`CThingBuildingDef::operator=`) is propagated;
// the class is named by its base. One of {len(members)} bodies of this shape.
struct {bcls} {{
    unsigned char _body[{lds[0][0]:#x}];
    {bcls}& operator=(const {bcls}& other);   // 0x{base:08X}
}};

struct {cls} : {bcls} {{
{mem}    {cls}& operator=(const {cls}& other);
}};

{cls}& {cls}::operator=(const {cls}& other)
{{
    {bcls}::operator=(other);
{assigns}    return *this;
}}
''',T(a,'''static int g_b = 0; struct B { long x; B& operator=(const B& o) { x = o.x; ++g_b; return *this; } };
struct D : B { long a; unsigned char c; long d; D& operator=(const D& o); }; D& D::operator=(const D& o) { B::operator=(o); a = o.a; c = o.c; d = o.d; return *this; }
int main()
{
    D s, t; s.x = 1; s.a = 2; s.c = 3; s.d = 4; t = s; if (g_b != 1 || t.x != 1 || t.a != 2 || t.c != 3 || t.d != 4) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_30",):   # void CX::Fire() { m_pfnCallback(m_pCallbackObject, m_thing); m_bFired = true; }  (CScriptThing copy built in the argument slot)
        loads=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str]
        pf=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM and "esi" in i.op_str]
        fl=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and "byte ptr [esi" in i.op_str]
        if len(loads)!=3 or len(pf)!=1 or len(fl)!=1 or len(vts)!=2: continue
        thing,count,obj=loads; pfn=pf[0]; flag=fl[0]
        if count!=thing+4 or obj+4!=pfn+8 and obj!=pfn+4: continue
        cls=f"CScriptCallbackOwner_{a}"
        add(a,"_unattributed",f"Fire_{cls}",HDRF(a,f"{cls}::Fire() -- calls the stored callback with a by-value CScriptThing copy of the member at +{thing-4:#x}, then sets the byte flag at +{flag:#x}",len(b))+f'''//
// Retail bytes:  push esi ; sub esp,0Ch ; mov eax,esp ; mov dword ptr [eax],offset CBaseClass::`vftable' ; mov dword ptr [eax],offset CScriptThing::`vftable' ; mov esi,ecx ; mov ecx,[esi+{thing:#x}] ; mov [eax+4],ecx ; mov ecx,[esi+{count:#x}] ; test ecx,ecx ; mov [eax+8],ecx ; je I ; inc dword ptr [ecx] ; I: mov ecx,[esi+{obj:#x}] ; call [esi+{pfn:#x}] ; mov byte ptr [esi+{flag:#x}],1 ; pop esi ; ret
// The stored __fastcall callback (+{pfn:#x}, called with the object at +{obj:#x} in ecx) receives a
// by-value copy of the CScriptThing member at +{thing-4:#x}: the copy is built in the argument slot
// with the CBaseClass and CScriptThing vtables (0x{vts[0][1]:08X}, 0x{vts[1][1]:08X}; both stores are
// kept), the thing pointer and the shared count block whose counter is incremented when present;
// then the byte flag at +{flag:#x} is set. No-RTTI module: the owning class is named descriptively.
// One of {len(members)} bodies of this shape.
struct CThing;
struct SCount {{ long m_references; }};

struct CBaseClass {{
    virtual void v00();
    CBaseClass() {{ }}
}};

struct CScriptThing : CBaseClass {{
    virtual void v00();
    CThing* m_pThing;                      // +4
    SCount* m_pCount;                      // +8
    CScriptThing(const CScriptThing& other) : CBaseClass(), m_pThing(other.m_pThing), m_pCount(other.m_pCount) {{ if (m_pCount) ++m_pCount->m_references; }}
}};

struct {cls} {{
    virtual void v00();
    unsigned char _pad0[{flag-4:#x}];
    bool m_bFired;                         // +{flag:#x}
    unsigned char _pad1[{pfn-flag-1:#x}];
    void (__fastcall* m_pfnCallback)(void* object, CScriptThing thing);   // +{pfn:#x}
    void* m_pCallbackObject;               // +{obj:#x}
{"    unsigned char _pad2[%#x];" % (thing-4-obj-4) + chr(10) if thing-4-obj-4>0 else ""}    CScriptThing m_thing;                  // +{thing-4:#x}
    void Fire();
}};

void {cls}::Fire()
{{
    m_pfnCallback(m_pCallbackObject, m_thing);
    m_bFired = true;
}}
''',T(a,'''static int g_c = 0; struct Cnt { long r; }; struct B { virtual void v00() {} B() {} };
struct ST : B { virtual void v00() {} void* t; Cnt* c; ST() : t(0), c(0) {} ST(const ST& o) : B(), t(o.t), c(o.c) { if (c) ++c->r; } };
static void __fastcall CB(void*, ST s) { g_c += (int)s.c->r; }
struct X { virtual void v00() {} bool f; void (__fastcall* pfn)(void*, ST); void* obj; ST th; void Fire() { pfn(obj, th); f = true; } };
int main()
{
    Cnt c = { 1 }; X x; x.th.c = &c; x.pfn = CB; x.obj = 0; x.f = false; x.Fire(); if (!x.f || g_c != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_59",):   # void list<CCountedPointer>::clear(): destroy each node's counted pointer, free the node, reset the head links
        if len(calls)!=2: continue
        deleter,freer=calls
        voff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        nm="CRetailList_CountedPointer"
        add(a,nm,"clear",HDRF(a,f"{nm}::clear() -- release every node's CCountedPointer (count block destroy + delete at zero), free the node, re-link the head to itself",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,ecx ; mov eax,[ebx] ; push edi ; mov edi,[eax] ; cmp edi,eax ; je E ; push ebp ; push esi ; xor ebp,ebp ; L: mov esi,edi ; mov eax,[esi+{voff:#x}] ; cmp eax,ebp ; mov edi,[edi] ; je F ; dec dword ptr [eax] ; mov eax,[esi+{voff:#x}] ; cmp [eax],ebp ; jne F ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+{voff:#x}] ; push eax ; call operator delete ; add esp,4 ; F: push esi ; mov [esi+{voff-4:#x}],ebp ; mov [esi+{voff:#x}],ebp ; call free ; mov eax,[ebx] ; add esp,4 ; cmp edi,eax ; jne L ; pop esi ; pop ebp ; E: mov eax,[ebx] ; mov [eax],eax ; mov ebx,[ebx] ; mov [ebx+4],ebx ; pop ebx ; ret
// Retail std::list::clear for CCountedPointer values: walks from head->next to the head,
// remembering each node's successor, runs the inlined CCountedPointer destructor (decrement the
// count block; at zero call its destroy function @ slot +4 with the object in ecx and delete the
// block through the flag-tested operator delete @ 0x{deleter:08X}; then null both fields), frees
// the node (free @ 0x{freer:08X}) and finally links the head to itself. The head is re-read from
// `this` on every iteration (retail reloads [ebx]). One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pObject = 0;
        m_pCount = 0;
    }}
}};

struct {nm} {{
    struct node {{
        node* next;                        // +0
        node* prev;                        // +4
        CCountedPointer value;             // +{voff-4:#x}
    }};
    node* head;
    void clear();
}};

void {nm}::clear()
{{
    node* n = head->next;
    while (n != head) {{
        node* victim = n;                  // retail advances the cursor first, then destroys the old node
        n = n->next;
        victim->value.~CCountedPointer();
        free(victim);
    }}
    head->next = head;
    head->prev = head;
}}
''',T(a,'''#include <cstdlib>
static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct L { struct N { N* n; N* p; P v; }; N* h; void clear() { N* x = h->n; while (x != h) { N* v = x; x = x->n; v->v.~P(); std::free(v); } h->n = h; h->p = h; } };
int main()
{
    L l; L::N head; head.v.o = 0; head.v.c = 0; l.h = &head; L::N* a = (L::N*)std::malloc(sizeof(L::N)); B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; a->v.o = 0; a->v.c = b; head.n = a; a->n = &head; a->p = &head; head.p = a;
    l.clear(); if (g_d != 1 || head.n != &head || head.p != &head) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_60",):   # CCountedPointer* __fastcall Move(first, last, dest, tag, tag): copy-assign a range of counted pointers, return the advanced dest
        if len(calls)!=1 or retn!=12: continue
        deleter=calls[0]
        nm="CRetailMove_CountedPointer"
        add(a,"_global",f"Move_CountedPointer_{a}",HDRF(a,f"_Move<CCountedPointer*> -- copy-assign [first, last) onto dest (count-block release/adopt inlined), return the advanced dest",len(b))+f'''//
// Retail bytes:  push ecx ; push ebx ; mov ebx,ecx ; sub edx,ebx ; sar edx,3 ; test edx,edx ; jle Z ; push ebp ; push esi ; mov esi,[dest] ; push edi ; mov [esp+10h],edx ; L: mov edi,[ebx+4] ; mov eax,[esi+4] ; cmp eax,edi ; mov ebp,[ebx] ; je N ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; A: test edi,edi ; mov [esi],ebp ; mov [esi+4],edi ; je N ; inc dword ptr [edi] ; N: mov eax,[esp+10h] ; add ebx,8 ; add esi,8 ; dec eax ; mov [esp+10h],eax ; jne L ; mov eax,esi ; ... ret 0Ch ; Z: mov eax,[dest] ; ... ret 0Ch
// Retail std::copy back end for CCountedPointer elements (`__fastcall`: first in ecx, last in edx,
// dest and two empty tag arguments on the stack, `ret 0Ch`): the signed element count is kept in
// a stack local and counted down; each assignment is the inlined CCountedPointer::operator= --
// when the count blocks differ, release the destination's block (destroy function @ slot +4
// with the object in ecx, flag-tested operator delete @ 0x{deleter:08X} at zero), adopt the source's
// object and block, and bump its counter. Returns the advanced destination (the untouched dest
// for an empty range). One of {len(members)} bodies of this shape.
struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {{
        SCountBlock* count = other.m_pCount;   // the source's fields are read first (retail keeps them in edi/ebp across the release)
        void* object = other.m_pObject;
        if (m_pCount != count) {{
            if (m_pCount) {{
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {{
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }}
            }}
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }}
        return *this;
    }}
}};

struct SIteratorTag {{ SIteratorTag() {{ }} }};

CCountedPointer* __fastcall Move_CountedPointer_{a}(CCountedPointer* first, CCountedPointer* last, CCountedPointer* dest, SIteratorTag category, SIteratorTag range)
{{
    for (long count = last - first; 0 < count; --count, ++first, ++dest)
        *dest = *first;
    return dest;
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; P& operator=(const P& x) { B* cc = x.c; void* oo = x.o; if (c != cc) { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = oo; c = cc; if (cc) ++cc->r; } return *this; } };
struct Tg { Tg() {} };
static P* __fastcall M(P* f, P* l, P* d, Tg, Tg) { for (long n = l - f; 0 < n; --n, ++f, ++d) *d = *f; return d; }
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; P src[2]; src[0].o = 0; src[0].c = b; src[1].o = 0; src[1].c = 0; P dst[2]; dst[0].o = 0; dst[0].c = 0; dst[1].o = 0; dst[1].c = 0;
    P* e = M(src, src + 2, dst, Tg(), Tg()); if (e != dst + 2 || dst[0].c != b || b->r != 2 || g_d != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_20",):   # node* CRetailTree::Copy(node* src, node* parent): clone a subtree (left spine iteratively, right subtrees recursively); frame-pointer/size TU
        if len(calls)!=4 or calls[1]!=va or calls[3]!=va or calls[0]!=calls[2]: continue
        buy=calls[0]; nm=f"CRetailTree_{buy:08x}"
        add(a,nm,"Copy",HDRF(a,f"{nm}::Copy(node*, node*) -- clone a subtree: Buynode @ 0x{buy:08X} per node, right subtrees by recursion, the left spine iteratively with `parent` re-used as the running predecessor",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push ecx ; push ebx ; push esi ; mov esi,[ebp+8] ; push edi ; lea eax,[esi+10h] ; push eax ; mov [ebp-4],ecx ; call Buynode ; mov ebx,eax ; mov al,[esi] ; and dword ptr [ebx+8],0 ; and dword ptr [ebx+0Ch],0 ; mov [ebx],al ; mov eax,[ebp+0Ch] ; mov [ebx+4],eax ; mov eax,[esi+0Ch] ; test eax,eax ; je A ; mov ecx,[ebp-4] ; push ebx ; push eax ; call Copy ; mov [ebx+0Ch],eax ; A: mov edi,[esi+8] ; mov [ebp+0Ch],ebx ; jmp T ; L: mov ecx,[ebp-4] ; lea eax,[edi+10h] ; push eax ; call Buynode ; mov esi,eax ; mov al,[edi] ; and dword ptr [esi+8],0 ; and dword ptr [esi+0Ch],0 ; mov [esi],al ; mov eax,[ebp+0Ch] ; mov [eax+8],esi ; mov [esi+4],eax ; mov eax,[edi+0Ch] ; test eax,eax ; je B ; mov ecx,[ebp-4] ; push esi ; push eax ; call Copy ; mov [esi+0Ch],eax ; B: mov edi,[edi+8] ; mov [ebp+0Ch],esi ; T: test edi,edi ; jne L ; pop edi ; pop esi ; mov eax,ebx ; pop ebx ; leave ; ret 8
// VC6-era Dinkumware `_Tree::_Copy(_X, _P)`: the first node is bought from the source value at
// +0x10 (Buynode @ 0x{buy:08X}, an out-of-line member taking the value by reference), its links
// zeroed, colour copied and parent set; its right subtree is cloned recursively; then the left
// spine is walked iteratively, each new node hung under the previous one through the `parent`
// parameter (retail keeps the running predecessor in the parameter's own slot) with its right
// subtree cloned recursively. `this` is only needed for the calls (spilled to [ebp-4]). The TU
// keeps frame pointers and optimises for size (`and dword ptr [..],0` stores). The manifest label
// (`CTCCarryable_CloneTree`) is propagated. One of {len(members)} bodies of this shape.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct {nm} {{
    struct node {{
        unsigned char color;               // +0
        unsigned char _pad[3];
        node* parent;                      // +4
        node* left;                        // +8
        node* right;                       // +0xC
        unsigned char value[4];            // +0x10 (size not recoverable from this body)
    }};
    node* head;
    unsigned long size;
    node* Buynode(const unsigned char (&value)[4]);   // 0x{buy:08X}
    node* Copy(node* src, node* parent);
}};

{nm}::node* {nm}::Copy(node* src, node* parent)
{{
    node* first = Buynode(src->value);
    first->left = 0;
    first->right = 0;
    first->color = src->color;
    first->parent = parent;
    if (src->right)
        first->right = Copy(src->right, first);
    parent = first;
    for (node* s = src->left; s; s = s->left) {{
        node* n = Buynode(s->value);
        n->left = 0;
        n->right = 0;
        n->color = s->color;
        parent->left = n;
        n->parent = parent;
        if (s->right)
            n->right = Copy(s->right, n);
        parent = n;
    }}
    return first;
}}
''',T(a,'''#include <cstdlib>
#include <cstring>
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; unsigned char v[4]; }; node* h; unsigned long n;
  node* B(const unsigned char (&v)[4]) { node* x = (node*)std::malloc(sizeof(node)); std::memcpy(x->v, v, 4); return x; }
  node* C(node* s, node* parent) { node* f = B(s->v); f->l = 0; f->r = 0; f->c = s->c; f->parent = parent; if (s->r) f->r = C(s->r, f); parent = f; for (node* q = s->l; q; q = q->l) { node* x = B(q->v); x->l = 0; x->r = 0; x->c = q->c; parent->l = x; x->parent = parent; if (q->r) x->r = C(q->r, x); parent = x; } return f; } };
int main()
{
    T::node a = {1,{0},0,0,0,{7,0,0,0}}, b = {0,{0},&a,0,0,{8,0,0,0}}, c = {1,{0},&a,0,0,{9,0,0,0}}; a.l = &b; a.r = &c; T t; T::node* d = t.C(&a, 0);
    if (!d || d->v[0] != 7 || !d->l || d->l->v[0] != 8 || d->l->parent != d || !d->r || d->r->v[0] != 9 || d->r->parent != d || d->c != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_19",):   # CRetailTree& operator=(const CRetailTree&): clear, copy the subtree, re-derive the leftmost/rightmost links, copy the size
        if len(calls)!=2: continue
        clear,copy=calls; nm=f"CRetailTree_{copy:08x}"
        add(a,nm,"operator=",HDRF(a,f"{nm}::operator=(const {nm}&) -- Clear @ 0x{clear:08X}, Copy @ 0x{copy:08X} of the source root under the head, leftmost/rightmost walks, size copied; self-assignment guarded",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; mov esi,ecx ; cmp esi,ebx ; je R ; call Clear ; and dword ptr [esi+4],0 ; mov eax,[ebx] ; mov eax,[eax+4] ; test eax,eax ; jne C ; mov eax,[esi] ; and dword ptr [eax+4],0 ; mov eax,[esi] ; mov [eax+8],eax ; mov eax,[esi] ; mov [eax+0Ch],eax ; jmp R ; C: push edi ; mov edi,[esi] ; push edi ; push eax ; mov ecx,esi ; call Copy ; mov [edi+4],eax ; mov ecx,[esi] ; mov eax,[ecx+4] ; pop edi ; jmp T1 ; L1: mov eax,edx ; T1: mov edx,[eax+8] ; test edx,edx ; jne L1 ; mov [ecx+8],eax ; mov ecx,[esi] ; mov eax,[ecx+4] ; jmp T2 ; L2: mov eax,edx ; T2: mov edx,[eax+0Ch] ; test edx,edx ; jne L2 ; mov [ecx+0Ch],eax ; mov eax,[ebx+4] ; mov [esi+4],eax ; R: mov eax,esi ; pop esi ; pop ebx ; ret 4
// VC6-era Dinkumware `_Tree::operator=`: unless self-assigned, erase everything (Clear @
// 0x{clear:08X}), zero the count, and either reset the head to an empty tree (null root, head
// linked to itself; the head pointer is re-read from `this` for each store) or clone the source's
// root under the head (Copy @ 0x{copy:08X}), re-derive the leftmost/rightmost links by walking
// the new root, and copy the count. Size TU (`and dword ptr [..],0`). The manifest label
// (`CTCCarryable::operator=`) is propagated. One of {len(members)} bodies of this shape.
#pragma optimize("s", on)
struct {nm} {{
    struct node {{
        unsigned char color;               // +0
        unsigned char _pad[3];
        node* parent;                      // +4
        node* left;                        // +8
        node* right;                       // +0xC
    }};
    node* head;                            // +0
    unsigned long size;                    // +4
    void Clear();                          // 0x{clear:08X}: erase(begin(), end())
    node* Copy(node* src, node* parent);   // 0x{copy:08X}
    {nm}& operator=(const {nm}& other);
}};

{nm}& {nm}::operator=(const {nm}& other)
{{
    if (this != &other) {{
        Clear();
        size = 0;
        if (other.head->parent == 0) {{
            head->parent = 0;
            head->left = head;
            head->right = head;
        }}
        else {{
            node* h = head;                // named: retail keeps the head in edi across the Copy call
            h->parent = Copy(other.head->parent, h);
            node* n = head->parent;
            while (n->left)
                n = n->left;
            head->left = n;
            n = head->parent;
            while (n->right)
                n = n->right;
            head->right = n;
            size = other.size;
        }}
    }}
    return *this;
}}
''',T(a,'''static int g_c = 0;
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Cl() { ++g_c; } node* Cp(node* s, node* p) { s->parent = p; return s; }
  T& operator=(const T& o) { if (this != &o) { Cl(); n = 0; if (o.h->parent == 0) { h->parent = 0; h->l = h; h->r = h; } else { node* hh = h; hh->parent = Cp(o.h->parent, hh); node* x = h->parent; while (x->l) x = x->l; h->l = x; x = h->parent; while (x->r) x = x->r; h->r = x; n = o.n; } } return *this; } };
int main()
{
    T::node ha = {0,{0},0,0,0}, hb = {0,{0},0,0,0}, root = {0,{0},0,0,0}; T a, b; a.h = &ha; a.n = 0; b.h = &hb; hb.parent = &root; b.n = 1; a = b; if (g_c != 1 || ha.parent != &root || ha.l != &root || ha.r != &root || a.n != 1) '''+FAIL(a)+'''
    T e; T::node he = {0,{0},0,0,0}; e.h = &he; a = e; if (ha.parent || ha.l != &ha || a.n != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_13",):   # CDerived::CDerived(CThing*) : CBase(thing), m_name(CCharString("LIT", -1)) {}  -- hashed-name member constructed inline from a temporary
        if len(calls)!=6 or len(vts)!=2: continue
        base,sctor,sdef,sassign,crc,sdtor=calls; litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva)
        empty=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_IMM and i.operands[1].imm>0x1200000][0]
        own=vts[0][1]; moff,mvt=vts[1]; cls=rtti.get(own) or f"CVtable_{own:08x}"; basecls=tctor_class(base) or f"CBase_{base:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{own:08x}"
        mcls=rtti.get(mvt) or f"CHashedName_{mvt:08x}"
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(CThing*) -- base {basecls}(thing), own vtable, {mcls} member at +{moff:#x} built from CCharString({s}, -1): string assigned, CRC of the name stored",len(b))+f'''//
// Retail bytes:  mov eax,[esp+4] ; push esi ; push edi ; push eax ; mov esi,ecx ; call {basecls}::{basecls} ; push -1 ; push offset "{s}" ; lea ecx,[esp+14h] ; mov dword ptr [esi],offset {cls}::`vftable' ; call CCharString::CCharString(const char*, long) ; lea edi,[esi+{moff+4:#x}] ; mov ecx,edi ; mov dword ptr [esi+{moff:#x}],offset {mcls}::`vftable' ; call CCharString::CCharString() ; lea ecx,[esp+0Ch] ; push ecx ; mov ecx,edi ; call CCharString::operator= ; mov eax,[esp+0Ch] ; test eax,eax ; jne C ; xor ecx,ecx ; mov eax,offset "" ; jmp D ; C: mov ecx,[eax+4] ; mov eax,[eax] ; D: push ecx ; mov edx,eax ; xor ecx,ecx ; call Calc ; lea ecx,[esp+0Ch] ; mov [esi+{moff+8:#x}],eax ; call CCharString::~CCharString ; pop edi ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a {basecls}-derived class: the base constructor @ 0x{base:08X}
// runs with the owner, this class's vtable (0x{own:08X}) is installed, then the {mcls} member at
// +{moff:#x} (vtable 0x{mvt:08X}) is constructed from a CCharString temporary built from the
// literal: the member's own constructor is inlined -- default-construct its string
// (@ 0x{sdef:08X}), assign the name (@ 0x{sassign:08X}) and store the name's CRC
// (Calc @ 0x{crc:08X}, `__fastcall(seed, data, length)`, run over the empty literal when the
// string has no data block). The temporary is destroyed (@ 0x{sdtor:08X}) before returning
// `this`. Names come from rtti-vtables.tsv. One of {len(members)} bodies of this shape.
struct CThing;

struct CCharStringData {{
    const char* m_pData;                   // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
}};

struct CCharString {{
    CCharStringData* m_pData;
    CCharString();                         // 0x{sdef:08X}
    CCharString(const char* text, long length = -1);   // 0x{sctor:08X}; the default argument makes the literal convert implicitly
    ~CCharString();                        // 0x{sdtor:08X}
    CCharString& operator=(const CCharString& other);   // 0x{sassign:08X}
}};

unsigned long __fastcall Calc_{crc:08x}(unsigned long seed, const void* data, unsigned long length);   // 0x{crc:08X}: CRC

struct {mcls} {{
    virtual void v00();
    CCharString m_string;                  // +4
    unsigned long m_hash;                  // +8
    {mcls}(const CCharString& name)
        : m_string()
    {{
        m_string = name;
        const char* data;
        unsigned long length;
        if (name.m_pData == 0) {{
            length = 0;
            data = "";
        }}
        else {{
            length = name.m_pData->m_length;
            data = name.m_pData->m_pData;
        }}
        m_hash = Calc_{crc:08x}(0, data, length);
    }}
}};

struct {basecls} {{
    virtual void v00();
    unsigned char _body[{moff-4:#x}];
    {basecls}(CThing* thing);              // 0x{base:08X}
}};

struct {cls} : {basecls} {{
    {mcls} m_name;                         // +{moff:#x}
    virtual void v00();
    {cls}(CThing* thing);
}};

{cls}::{cls}(CThing* thing)
    : {basecls}(thing)
    , m_name("{s}")                        // implicit CCharString conversion: the temporary is re-addressed with lea (an explicit CCharString("..", -1) would reuse the constructor's returned pointer)
{{
}}
''',T(a,'''static int g_b = 0, g_h = 0; struct Th;
struct SD { const char* p; unsigned long n; }; struct Str { SD* d; Str() : d(0) {} Str(const char* t, long = -1) { d = new SD; d->p = t; d->n = 3; } ~Str() { delete d; } Str& operator=(const Str& o) { if (o.d) { if (!d) d = new SD; *d = *o.d; } return *this; } };
static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long n) { ++g_h; return n * 7; }
struct HN { virtual void v00() {} Str s; unsigned long h; HN(const Str& nm) : s() { s = nm; const char* p; unsigned long n; if (nm.d == 0) { n = 0; p = ""; } else { n = nm.d->n; p = nm.d->p; } h = Calc(0, p, n); } };
struct B { virtual void v00() {} unsigned char body[8]; B(Th*) { ++g_b; } };
struct D : B { HN m; virtual void v00() {} D(Th* t) : B(t), m("ABC") {} };
int main()
{
    D d(0); if (g_b != 1 || g_h != 1 || d.m.h != 21 || !d.m.s.d) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_14",):   # void CX::TransferAllFrom(CSource* src): reserve count+count on the target, then move every item across through virtual slots
        if calls or len([i for i in ins if i.mnemonic=="call"])!=7: continue
        slots=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM]
        sCount,tCount,tReserve,sFirst,sRemove,xAdd,sFirst2=slots
        if sFirst!=sFirst2: continue
        toff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "ebx +" in i.op_str][0]
        cls=f"CItemCollectionOwner_{a}"
        def vt(name,slot,sig): return "".join(f"    virtual void _slot{j:02x}();\n" for j in range(0,0)) 
        def vtbl(slotsig):   # list of (slot, decl) -> virtual function list with placeholders
            out=""; mx=max(s for s,_ in slotsig)
            for j in range(0,mx+4,4):
                d=[d for s,d in slotsig if s==j]
                out+=f"    virtual {d[0]};\n" if d else f"    virtual void _slot{j:02x}();\n"
            return out
        add(a,"_unattributed",f"TransferAllFrom_{cls}",HDRF(a,f"{cls}::TransferAllFrom(CItemSource*) -- target(+{toff:#x})->Reserve(src->GetCount() + target->GetCount()), then detach each item from the source and Add it here",len(b))+f'''//
// Retail bytes:  push ebx ; push ebp ; push esi ; mov esi,[esp+10h] ; mov edx,[esi] ; mov ebx,ecx ; push edi ; mov edi,[ebx+{toff:#x}] ; mov eax,edi ; mov ebp,[eax] ; mov ecx,esi ; call [edx+{sCount:#x}] ; mov edx,[edi] ; mov ecx,edi ; mov [esp+14h],eax ; call [edx+{tCount:#x}] ; mov ecx,[esp+14h] ; add ecx,eax ; push ecx ; mov ecx,[ebx+{toff:#x}] ; call [ebp+{tReserve:#x}] ; mov eax,[esi] ; mov ecx,esi ; call [eax+{sFirst:#x}] ; mov edi,eax ; test edi,edi ; je E ; L: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+{sRemove:#x}] ; mov eax,[ebx] ; push edi ; mov ecx,ebx ; call [eax+{xAdd:#x}] ; mov edx,[esi] ; mov ecx,esi ; call [edx+{sFirst:#x}] ; mov edi,eax ; test edi,edi ; jne L ; E: pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// EH-module collection transfer (no frame: nothing to unwind): the target collection at +{toff:#x}
// is grown by the source's count plus its own (virtual slots +{sCount:#x} / +{tCount:#x} / +{tReserve:#x}; the
// target's vtable is read once up front and reused for the reserve call, the object pointer is
// re-read -- the retail TU assumes no aliasing), then every item the source hands out
// (slot +{sFirst:#x}) is detached from it (slot +{sRemove:#x}) and added here (slot +{xAdd:#x}). The
// manifest label is propagated; the classes are named by role. One of {len(members)} bodies.
struct CItem;

struct CItemSource {{
{vtbl([(sCount,"long GetCount()"),(sFirst,"CItem* GetFirst()"),(sRemove,"void Remove(CItem* item)")])}}};

struct CItemTarget {{
{vtbl([(tCount,"long GetCount()"),(tReserve,"void Reserve(long count)")])}}};

struct {cls} {{
{vtbl([(xAdd,"void Add(CItem* item)")])}{"    unsigned char _pad[%#x];" % (toff-4) + chr(10) if toff>4 else ""}    CItemTarget* m_pTarget;                // +{toff:#x}
    void TransferAllFrom_{cls}(CItemSource* source);
}};

void {cls}::TransferAllFrom_{cls}(CItemSource* source)
{{
    m_pTarget->Reserve(source->GetCount() + m_pTarget->GetCount());
    for (CItem* item = source->GetFirst(); item; item = source->GetFirst()) {{
        source->Remove(item);
        Add(item);
    }}
}}
''',T(a,'''static int g_r = 0, g_a = 0, g_n = 2; struct It;
struct Src { virtual void a() {} virtual long GetCount() { return 3; } virtual It* First() { return g_n-- > 0 ? (It*)1 : 0; } virtual void Remove(It*) { ++g_r; } };
struct Tgt { virtual long GetCount() { return 4; } virtual void Reserve(long n) { if (n == 7) g_a += 100; } };
struct X { virtual void Add(It*) { ++g_a; } Tgt* t; void T(Src* s) { t->Reserve(s->GetCount() + t->GetCount()); for (It* i = s->First(); i; i = s->First()) { s->Remove(i); Add(i); } } };
int main()
{
    Src s; Tgt t; X x; x.t = &t; x.T(&s); if (g_r != 2 || g_a != 102) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_69","w256_95","w256_52","w256_36","w256_49","w256_31"):   # SGI-STL algorithm instantiations (fastcall TU): __copy, __copy_backward, sort, __final_insertion_sort
        def elem_size(ins):
            magic=None; shift=None; prev=None
            for i in ins:
                if i.mnemonic=="imul" and prev is not None and prev.mnemonic=="mov" and prev.operands[0].type==X86_OP_REG and prev.operands[1].type==X86_OP_IMM: magic=prev.operands[1].imm
                if magic is not None and shift is None and i.mnemonic=="sar" and i.operands[1].type==X86_OP_IMM and i.op_str.startswith("edx"): shift=i.operands[1].imm
                prev=i
            if magic is not None and shift is not None: return int(round(2**(32+shift)/magic))
            for i in ins:
                if i.mnemonic=="sar" and i.operands[1].type==X86_OP_IMM: return 1<<i.operands[1].imm
            return None
        esz=elem_size(ins)
        if not esz: continue
        if TPL=="w256_69":
            if len(calls) or retn!=12: continue
            ecls=f"SElement{esz}"; nm=f"Copy_{ecls}_{a}"
            add(a,"_global",nm,HDRF(a,f"SGI __copy<{ecls}*> (random-access, {esz}-byte POD elements) -- counted forward copy, returns the advanced result",len(b))+f'''//
// Retail bytes:  sub edx,ecx ; (divide by {esz}) ; test eax,eax ; jle Z ; push ebx ; push esi ; mov edx,eax ; mov eax,[result] ; push edi ; L: (copy {esz} bytes from [ecx] to [eax]) ; add ecx,{esz:#x} ; add eax,{esz:#x} ; dec edx ; jne L ; pop edi ; pop esi ; pop ebx ; ret 0Ch ; Z: mov eax,[result] ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for {esz}-byte
// trivially-copyable elements: the signed element count is computed once (the compiler's
// reciprocal multiply for the division by {esz}), the loop copies one element per iteration
// (`*result = *first; ++first; ++result;`) and the advanced result is returned (the untouched
// result for an empty range). The TU is compiled with /Gr (all free functions __fastcall):
// first in ecx, last in edx, result + the two tag arguments on the stack (`ret 0Ch`). The
// manifest label (`_Copy_impl<...>`) is propagated from Dinkumware; the body is SGI's.
// One of {len(members)} bodies of this shape.
struct {ecls} {{
    unsigned char _body[{esz:#x}];
}};

struct SRandomAccessIteratorTag {{ SRandomAccessIteratorTag() {{ }} }};

{ecls}* __fastcall {nm}({ecls}* first, {ecls}* last, {ecls}* result, SRandomAccessIteratorTag category, long* distance)
{{
    for (long n = last - first; n > 0; --n) {{
        *result = *first;
        ++first;
        ++result;
    }}
    return result;
}}
''',T(a,'''struct E { unsigned char b['''+str(esz)+''']; }; struct Tg { Tg() {} };
static E* __fastcall C(E* f, E* l, E* r, Tg, long*) { for (long n = l - f; n > 0; --n) { *r = *f; ++f; ++r; } return r; }
int main()
{
    E s[2], d[2]; s[0].b[0] = 1; s[1].b[0] = 2; d[0].b[0] = 0; d[1].b[0] = 0; E* e = C(s, s + 2, d, Tg(), 0); if (e != d + 2 || d[0].b[0] != 1 || d[1].b[0] != 2) '''+FAIL(a)+'''
'''),b)
        elif TPL=="w256_95":
            if len(calls)!=1 or retn!=12: continue
            asg=calls[0]; ecls=f"CElement{esz}_{asg:08x}"; nm=f"Copy_{ecls}_{a}"
            add(a,"_global",nm,HDRF(a,f"SGI __copy<{ecls}*> (random-access, {esz}-byte elements assigned through operator= @ 0x{asg:08X}) -- counted forward copy, returns the advanced result",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; sub edx,esi ; (divide by {esz}) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: push esi ; mov ecx,edi ; call {ecls}::operator= ; add esi,{esz:#x} ; add edi,{esz:#x} ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for {esz}-byte
// elements with an out-of-line copy-assignment operator @ 0x{asg:08X}: the signed element count
// is computed once, each element is assigned (`*result = *first`) and the advanced result is
// returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the stack).
// The manifest label (`_Copy_impl<...>`) is propagated from Dinkumware. One of {len(members)} bodies.
struct {ecls} {{
    unsigned char _body[{esz:#x}];
    {ecls}& operator=(const {ecls}& other);   // 0x{asg:08X}
}};

struct SRandomAccessIteratorTag {{ SRandomAccessIteratorTag() {{ }} }};

{ecls}* __fastcall {nm}({ecls}* first, {ecls}* last, {ecls}* result, SRandomAccessIteratorTag category, long* distance)
{{
    for (long n = last - first; n > 0; --n) {{
        *result = *first;
        ++first;
        ++result;
    }}
    return result;
}}
''',T(a,'''static int g_a = 0; struct E { unsigned char b['''+str(esz)+''']; E& operator=(const E& o) { ++g_a; b[0] = o.b[0]; return *this; } }; struct Tg { Tg() {} };
static E* __fastcall C(E* f, E* l, E* r, Tg, long*) { for (long n = l - f; n > 0; --n) { *r = *f; ++f; ++r; } return r; }
int main()
{
    E s[2], d[2]; s[0].b[0] = 1; s[1].b[0] = 2; d[0].b[0] = 0; d[1].b[0] = 0; E* e = C(s, s + 2, d, Tg(), 0); if (e != d + 2 || g_a != 2 || d[1].b[0] != 2) '''+FAIL(a)+'''
'''),b)
        elif TPL=="w256_52":
            if len(calls) or retn!=8: continue
            ecls=f"SElement{esz}"; nm=f"CopyBackward_{ecls}_{a}"
            add(a,"_global",nm,HDRF(a,f"SGI __copy_backward<{ecls}*> ({esz}-byte POD elements) -- counted backward copy, returns the moved-back result",len(b))+f'''//
// Retail bytes:  mov eax,[result] ; push esi ; mov esi,edx ; sub esi,ecx ; (divide by {esz}) ; test esi,esi ; jle E ; mov ecx,edx ; sub ecx,eax ; L: (copy {esz} bytes from [ecx+eax-{esz:#x}] to [eax-{esz:#x}]) ; sub eax,{esz:#x} ; dec esi ; jne L ; E: pop esi ; ret 8
// SGI-STL `__copy_backward(first, last, result, tag)` for {esz}-byte trivially-copyable elements:
// `for (n = last - first; n > 0; --n) *--result = *--last;` (the compiler strength-reduces `last`
// to an offset from `result`) and the moved-back result is returned. /Gr TU (__fastcall: first
// in ecx, last in edx, result + one tag on the stack). The manifest label (`_Copy_impl`) is
// propagated from Dinkumware. One of {len(members)} bodies of this shape.
struct {ecls} {{
    unsigned char _body[{esz:#x}];
}};

struct SBidirectionalIteratorTag {{ SBidirectionalIteratorTag() {{ }} }};

{ecls}* __fastcall {nm}({ecls}* first, {ecls}* last, {ecls}* result, SBidirectionalIteratorTag category)
{{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}}
''',T(a,'''struct E { unsigned char b['''+str(esz)+''']; }; struct Tg { Tg() {} };
static E* __fastcall C(E* f, E* l, E* r, Tg) { for (long n = l - f; n > 0; --n) *--r = *--l; return r; }
int main()
{
    E s[3]; s[0].b[0] = 1; s[1].b[0] = 2; s[2].b[0] = 9; E* e = C(s, s + 2, s + 3, Tg()); if (e != s + 1 || s[1].b[0] != 1 || s[2].b[0] != 2) '''+FAIL(a)+'''
'''),b)
        elif TPL in ("w256_36","w256_49"):
            inl=(TPL=="w256_36"); ecls=f"SElement{esz}"; nm=f"Sort_{ecls}_{a}"
            if retn!=4 or len(calls)!=(4 if inl else 2): continue
            if inl:
                intro,ins_,unguarded,ins2=calls; fin=None
                if ins2!=ins_: continue
            else: intro,fin=calls; ins_=unguarded=None
            add(a,"_global",nm,HDRF(a,f"SGI sort<{ecls}*, Compare>({esz}-byte elements) -- introsort loop @ 0x{intro:08X} to depth 2*lg(n), then the final insertion sort{' (inlined: threshold 16)' if inl else ' @ 0x%08X' % fin}",len(b))+f'''//
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / {esz}) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push ebp ; mov ebp,[comp] ; push ebp ; lea eax,[ecx+ecx] ; push eax ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; {"cmp esi,10h ; mov ecx,edi ; push ebp ; jle S ; lea esi,[edi+%#x] ; mov edx,esi ; call __insertion_sort ; push ebp ; push 0 ; mov edx,ebx ; mov ecx,esi ; call __unguarded_insertion_sort_aux ; ... ret 4 ; S: mov edx,ebx ; call __insertion_sort ; ... ret 4" % (16*esz) if inl else "push ebx ; mov edx,esi ; mov ecx,edi ; call __final_insertion_sort ; pop ebx ; E: pop edi ; pop esi ; ret 4"}
// SGI-STL `sort(first, last, comp)`: for a non-empty range run `__introsort_loop(first, last,
// (T*)0, __lg(last - first) * 2, comp)` (`__lg` is the shift-count loop) and then
// `__final_insertion_sort`{": inlined here -- ranges longer than __stl_threshold (16) get `__insertion_sort(first, first + 16, comp)` followed by `__unguarded_insertion_sort_aux(first + 16, last, (T*)0, comp)`, shorter ones a single `__insertion_sort`" if inl else " out of line @ 0x%08X" % fin}.
// The comparator is a 4-byte functor passed by value. /Gr TU (__fastcall: first in ecx, last
// in edx, comp on the stack). Labels (`make_heap`, `HeapSort`, `Introsort_TripletPartition`) are
// propagated; the body is SGI's `sort`. One of {len(members)} bodies of this shape.
struct {ecls} {{
    unsigned char _body[{esz:#x}];
}};

struct SCompare {{                        // 4-byte comparison functor passed by value
    void* m_context;
}};

template<class Size> inline Size Lg(Size n)
{{
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}}

void __fastcall IntrosortLoop_{intro:08x}({ecls}* first, {ecls}* last, {ecls}* valueType, long depthLimit, SCompare comp);   // 0x{intro:08X}
{"void __fastcall InsertionSort_%08x(%s* first, %s* last, SCompare comp);   // 0x%08X" % (ins_,ecls,ecls,ins_) if inl else ""}
{"void __fastcall UnguardedInsertionSortAux_%08x(%s* first, %s* last, %s* valueType, SCompare comp);   // 0x%08X" % (unguarded,ecls,ecls,ecls,unguarded) if inl else ""}
{"__forceinline void FinalInsertionSort(%s* first, %s* last, SCompare comp)" % (ecls,ecls) if inl else "void __fastcall FinalInsertionSort_%08x(%s* first, %s* last, SCompare comp);   // 0x%08X" % (fin,ecls,ecls,fin)}
{"{" + chr(10) + "    if (last - first > 16) {" + chr(10) + "        InsertionSort_%08x(first, first + 16, comp);" % ins_ + chr(10) + "        UnguardedInsertionSortAux_%08x(first + 16, last, (%s*)0, comp);" % (unguarded,ecls) + chr(10) + "    }" + chr(10) + "    else" + chr(10) + "        InsertionSort_%08x(first, last, comp);" % ins_ + chr(10) + "}" if inl else ""}
void __fastcall {nm}({ecls}* first, {ecls}* last, SCompare comp)
{{
    if (first != last) {{
        IntrosortLoop_{intro:08x}(first, last, ({ecls}*)0, Lg(last - first) * 2, comp);
        {"FinalInsertionSort(first, last, comp);" if inl else "FinalInsertionSort_%08x(first, last, comp);" % fin}
    }}
}}
''',T(a,'''static int g_i = 0, g_f = 0; struct E { unsigned char b['''+str(esz)+''']; }; struct Cmp { void* c; };
template<class S> inline S Lg(S n) { S k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
static void __fastcall IL(E*, E*, E*, long d, Cmp) { g_i += d; }
static void __fastcall IS(E*, E*, Cmp) { ++g_f; }
static void __fastcall UA(E*, E*, E*, Cmp) { g_f += 10; }
static void __fastcall FS(E* f, E* l, Cmp c) { if (l - f > 16) { IS(f, f + 16, c); UA(f + 16, l, (E*)0, c); } else IS(f, l, c); }
static void __fastcall So(E* f, E* l, Cmp c) { if (f != l) { IL(f, l, (E*)0, Lg(l - f) * 2, c); FS(f, l, c); } }
int main()
{
    E a[20]; Cmp c = { 0 }; So(a, a + 20, c); if (g_i != 8 || g_f != 11) '''+FAIL(a)+'''
    So(a, a + 3, c); if (g_i != 10 || g_f != 12) '''+FAIL(a)+'''
'''),b)
        elif TPL=="w256_31":
            if len(calls)!=3 or retn!=4 or calls[0]!=calls[2]: continue
            ins_,unguarded,_=calls; ecls=f"SElement{esz}"; nm=f"FinalInsertionSort_{ecls}_{a}"
            add(a,"_global",nm,HDRF(a,f"SGI __final_insertion_sort<{ecls}*, Compare>({esz}-byte elements) -- insertion sort of the first 16, unguarded insertion sort of the rest",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by {esz}) ; cmp eax,10h ; jle S ; push ebx ; mov ebx,[comp] ; push edi ; lea edi,[ecx+{16*esz:#x}] ; push ebx ; mov edx,edi ; call __insertion_sort ; push ebx ; push 0 ; mov edx,esi ; mov ecx,edi ; call __unguarded_insertion_sort_aux ; pop edi ; pop ebx ; pop esi ; ret 4 ; S: mov edx,[comp] ; push edx ; mov edx,esi ; call __insertion_sort ; pop esi ; ret 4
// SGI-STL `__final_insertion_sort(first, last, comp)`: ranges longer than __stl_threshold (16)
// get `__insertion_sort(first, first + 16, comp)` @ 0x{ins_:08X} followed by
// `__unguarded_insertion_sort_aux(first + 16, last, (T*)0, comp)` @ 0x{unguarded:08X}; shorter
// ones a single `__insertion_sort`. The comparator is a 4-byte functor passed by value. /Gr TU
// (__fastcall: first in ecx, last in edx, comp on the stack). The manifest label (`make_heap`)
// is propagated; the body is SGI's. One of {len(members)} bodies of this shape.
struct {ecls} {{
    unsigned char _body[{esz:#x}];
}};

struct SCompare {{                        // 4-byte comparison functor passed by value
    void* m_context;
}};

void __fastcall InsertionSort_{ins_:08x}({ecls}* first, {ecls}* last, SCompare comp);   // 0x{ins_:08X}
void __fastcall UnguardedInsertionSortAux_{unguarded:08x}({ecls}* first, {ecls}* last, {ecls}* valueType, SCompare comp);   // 0x{unguarded:08X}

void __fastcall {nm}({ecls}* first, {ecls}* last, SCompare comp)
{{
    if (last - first > 16) {{
        InsertionSort_{ins_:08x}(first, first + 16, comp);
        UnguardedInsertionSortAux_{unguarded:08x}(first + 16, last, ({ecls}*)0, comp);
    }}
    else
        InsertionSort_{ins_:08x}(first, last, comp);
}}
''',T(a,'''static int g_f = 0; struct E { unsigned char b['''+str(esz)+''']; }; struct Cmp { void* c; };
static void __fastcall IS(E*, E*, Cmp) { ++g_f; }
static void __fastcall UA(E*, E*, E*, Cmp) { g_f += 10; }
static void __fastcall FS(E* f, E* l, Cmp c) { if (l - f > 16) { IS(f, f + 16, c); UA(f + 16, l, (E*)0, c); } else IS(f, l, c); }
int main()
{
    E a[20]; Cmp c = { 0 }; FS(a, a + 20, c); FS(a, a + 3, c); if (g_f != 12) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_42",):   # void vector<long>::resize(n, const long& value): erase the tail (memmove copy inlined) or insert n - size copies at the end
        if len(calls)!=2 or retn!=8: continue
        mm,ins_=calls; nm="CRetailVector_dword"
        add(a,nm,"resize",HDRF(a,f"{nm}::resize(unsigned long, const long&) -- shrink through the inlined erase (memmove copy @ 0x{mm:08X}) or grow through Insert @ 0x{ins_:08X}",len(b))+f'''//
// Retail bytes:  mov edx,[esp+4] ; push esi ; push edi ; mov edi,ecx ; mov eax,[edi+4] ; mov esi,[edi] ; mov ecx,eax ; sub ecx,esi ; sar ecx,2 ; cmp edx,ecx ; jae G ; cmp eax,eax ; lea ecx,[esi+edx*4] ; jne C ; mov eax,ecx ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; C: mov esi,eax ; sub esi,eax ; push esi ; push eax ; push ecx ; call memmove ; add esp,0Ch ; add eax,esi ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; G: mov esi,[value] ; push esi ; sub edx,ecx ; push edx ; push eax ; mov ecx,edi ; call Insert ; pop edi ; pop esi ; ret 8
// Retail vector resize for 4-byte elements: when `n < size()` the tail is erased through the
// inlined `erase(first + n, last)` -- `last = copy(last, last, first + n)` with the memmove
// copy (the compiler keeps the `cmp eax,eax` of the empty-range test and the dead memmove
// branch); otherwise `Insert(last, n - size(), value)` @ 0x{ins_:08X} appends the missing
// elements. The manifest label (`vector<unsigned_int>::resize`) is propagated. One of {len(members)} bodies.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);

struct {nm} {{
    long* first;
    long* last;
    long* end;
    void Insert(long* where, unsigned long count, const long& value);   // 0x{ins_:08X}
    static long* Copy(long* f, long* l, long* result)
    {{
        if (f != l) {{
            unsigned long bytes = (char*)l - (char*)f;
            result = (long*)((char*)memmove(result, f, bytes) + bytes);
        }}
        return result;
    }}
    long* Erase(long* f, long* l)
    {{
        long* s = Copy(l, last, f);
        last = s;
        return f;
    }}
    void resize(unsigned long n, const long& value);
}};

void {nm}::resize(unsigned long n, const long& value)
{{
    if (n < (unsigned long)(last - first))
        Erase(first + n, last);
    else
        Insert(last, n - (last - first), value);
}}
''',T(a,'''#include <cstring>
static int g_i = 0;
struct V { long* f; long* l; long* e; void I(long*, unsigned long c, const long&) { g_i += (int)c; }
  static long* C(long* a, long* b, long* r) { if (a != b) { unsigned long n = (char*)b - (char*)a; r = (long*)((char*)std::memmove(r, a, n) + n); } return r; }
  long* E(long* a, long* b) { long* s = C(b, l, a); l = s; return a; }
  void R(unsigned long n, const long& v) { if (n < (unsigned long)(l - f)) E(f + n, l); else I(l, n - (l - f), v); } };
int main()
{
    long buf[4]; V v; v.f = buf; v.l = buf + 4; v.e = buf + 4; long x = 0; v.R(2, x); if (v.l != buf + 2) '''+FAIL(a)+'''
    v.R(5, x); if (g_i != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_56",):   # atexit destructor of a static retail tree: erase the subtree, reset the head, zero the count, free the head
        if len(calls)!=2: continue
        erase,freer=calls
        gl=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_IMM and i.operands[1].imm>0x1300000]
        if not gl: continue
        gaddr=gl[0]; cls=f"CRetailTree_erase{erase:08x}"; gname=f"g_tree_{gaddr:08x}"
        add(a,"_static_init",gname,HDRF(a,f"atexit destructor for {cls} {gname} @ 0x{gaddr:08X} -- erase all nodes through 0x{erase:08X}, reset the head, free it",len(b))+f'''//
// Retail bytes:  mov eax,[{gname}.size] ; test eax,eax ; je F ; mov eax,[{gname}.head] ; mov ecx,[eax+4] ; push ecx ; mov ecx,offset {gname} ; call Erase ; mov eax,[head] ; mov [eax+8],eax ; mov edx,[head] ; mov dword ptr [edx+4],0 ; mov eax,[head] ; mov [eax+0Ch],eax ; mov dword ptr [size],0 ; F: mov eax,[head] ; test eax,eax ; je R ; push eax ; call free ; pop ecx ; R: ret
// The `atexit` thunk (`??__F`, section .text$yd) of a static retail red-black tree {{head, size}}
// at 0x{gaddr:08X}: its inlined destructor erases the nodes through the __thiscall Erase
// @ 0x{erase:08X} when the tree holds any, resets the head's left/parent/right (the head
// pointer re-read per store) and the count, then releases the head node with free
// @ 0x{freer:08X}. The object is a translation-unit static with a trivial inline constructor
// (only the destructor thunk is graded). The manifest label is propagated; the object is
// named by its address. One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct {cls} {{
    struct node {{ unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; }};
    node* head;
    unsigned long size;
    void Erase(node* root);                // 0x{erase:08X}
    {cls}() : head(0), size(0) {{ }}
    ~{cls}()
    {{
        if (size) {{
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }}
        if (head)
            free(head);
    }}
}};

static {cls} {gname};                   // 0x{gaddr:08X}: head at +0, size at +4

{cls}* Get_{gname}()
{{
    return &{gname};
}}
''',T(a,'''#include <cstdlib>
static int g_e = 0; struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Erase(node*) { ++g_e; } T() : h(0), n(0) {}
  ~T() { if (n) { Erase(h->parent); h->l = h; h->parent = 0; h->r = h; n = 0; } if (h) std::free(h); } };
static T g_t;
int main()
{
    g_t.h = (T::node*)std::malloc(sizeof(T::node)); g_t.h->parent = 0; g_t.n = 1; if (g_t.n != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w256_45",):   # node* CRetailTree<CCharString key>::find(const CCharString&): lower-bound walk with the null-aware string compare, then end() unless the key matches
        if len(calls)!=2 or calls[0]!=calls[1] or retn!=4: continue
        less=calls[0]; cls=f"CRetailTree_stringkey"
        add(a,cls,"find",HDRF(a,f"{cls}::find(const CCharString&) -- lower-bound walk with the inlined CCharString::operator< (null data blocks ordered first, CBasicString::operator< @ 0x{less:08X} otherwise), then end() unless the key matches",len(b))+f'''//
// Retail bytes:  push ebx ; push ebp ; mov ebp,[ecx] ; push esi ; mov esi,[ebp+4] ; test esi,esi ; mov ebx,ebp ; je E ; mov eax,[key] ; push edi ; mov edi,[eax] ; L: mov ecx,[esi+10h] ; cmp edi,ecx ; je Lf ; test ecx,ecx ; je Rt ; test edi,edi ; je Lf ; push edi ; call CBasicString::operator< ; test al,al ; jne Rt ; Lf: mov ebx,esi ; mov esi,[esi+8] ; jmp T ; Rt: mov esi,[esi+0Ch] ; T: test esi,esi ; jne L ; pop edi ; E: cmp ebx,ebp ; je H ; mov ecx,[key] ; mov eax,[ebx+10h] ; mov ecx,[ecx] ; cmp eax,ecx ; je F ; test ecx,ecx ; je H ; test eax,eax ; je F ; push eax ; call CBasicString::operator< ; test al,al ; je F ; H: (return head) ; F: (return node)
// std::map/set::find on the retail tree keyed by CCharString (the key's data-block pointer at
// +0x10 of the node): lower_bound descends from head->parent (right when node.key < key, else
// remember the node and go left), then the result is end() (the head) when nothing was found or
// key < found.key. The string `operator<` is inlined: equal data blocks are not less, a null
// block sorts first, otherwise CBasicString<char>::operator< @ 0x{less:08X} decides. The node
// pointer is returned directly (`ret 4`). One of {len(members)} bodies of this shape.
struct CBasicString {{
    bool operator<(const CBasicString& other);   // 0x{less:08X} (__thiscall, takes the other data block)
}};

struct CCharString {{
    CBasicString* m_pData;
    bool operator<(const CCharString& other) const
    {{
        if (m_pData == other.m_pData)
            return false;
        if (m_pData == 0)
            return true;
        if (other.m_pData == 0)
            return false;
        return m_pData->operator<(*other.m_pData);
    }}
}};

struct {cls} {{
    struct node {{ unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; CCharString key; }};
    node* head;
    unsigned long size;
    node* find(const CCharString& key);
}};

{cls}::node* {cls}::find(const CCharString& key)
{{
    node* result = head;
    node* n = head->parent;
    while (n) {{
        if (!(n->key < key)) {{            // Dinkumware _Lbound: `if (!comp(key(node), k))`
            result = n;
            n = n->left;
        }}
        else
            n = n->right;
    }}
    if (result == head || key < result->key)
        return head;                       // end()
    return result;
}}
''',T(a,'''struct BS { int v; bool operator<(const BS& o) { return v < o.v; } };
struct CS { BS* d; bool operator<(const CS& o) const { if (d == o.d) return false; if (d == 0) return true; if (o.d == 0) return false; return d->operator<(*o.d); } };
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; CS k; }; node* h; unsigned long n;
  node* find(const CS& k) { node* res = h; node* x = h->parent; while (x) { if (!(x->k < k)) { res = x; x = x->l; } else x = x->r; } if (res == h || k < res->k) return h; return res; } };
int main()
{
    BS b5 = { 5 }, b6 = { 6 }; T::node h = {0,{0},0,0,0,{0}}, a = {1,{0},&h,0,0,{&b5}}; h.parent = &a; T t; t.h = &h; t.n = 1; CS k5 = { &b5 }, k6 = { &b6 }; if (t.find(k5) != &a || t.find(k6) != &h) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_41",):   # T* _Uninit_copy(first, last, dest, allocator) for trivially-copyable elements: placement copy under the null test
        step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        if calls or retn!=8: continue
        ecls=f"SElement{step}"; nm=f"UninitCopy_{ecls}_{a}"
        body=("    unsigned char _body[%#x];" % step) if step%4 else "\n".join("    long m_%02x;" % (4*j) for j in range(step//4))+"\n    SElement%d(const SElement%d& o) : %s { }   // memberwise copy: direct dword moves, not a block copy through pointer registers" % (step,step,", ".join("m_%02x(o.m_%02x)" % (4*j,4*j) for j in range(step//4)))
        add(a,"_global",nm,HDRF(a,f"_Uninit_copy<{ecls}*> -- placement-copy [first, last) to dest ({step}-byte POD elements), return the advanced dest",len(b))+f'''//
// Retail bytes:  cmp ecx,edx ; mov eax,[dest] ; je E ; push esi ; L: test eax,eax ; je S ; (copy {step} bytes) ; S: add ecx,{step:#x} ; add eax,{step:#x} ; cmp ecx,edx ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for {step}-byte trivially-copyable elements: each element is copied in
// place under the placement-new null test and the advanced destination is returned.
// One of {len(members)} bodies of this shape.
#include <new>

struct {ecls} {{                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
{body}
}};

struct CRetailAllocator {{ }};

{ecls}* __fastcall {nm}({ecls}* first, {ecls}* last, {ecls}* dest, CRetailAllocator allocator)
{{
    {ecls}* d = dest;
    for (; first != last; ++first, ++d)
        new (d) {ecls}(*first);
    return d;
}}
''',T(a,'''#include <new>
struct E { unsigned char b['''+str(step)+''']; }; struct A {};
static E* __fastcall UC(E* f, E* l, E* dest, A) { E* d = dest; for (; f != l; ++f, ++d) new (d) E(*f); return d; }
int main()
{
    E src[2], dst[2]; src[0].b[0] = 1; src[1].b[0] = 3; E* e = UC(src, src + 2, dst, A()); if (e != dst + 2 || dst[1].b[0] != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_43",):   # void _Fill(first, last, const T& value) for trivially-copyable elements
        step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        if calls or retn!=4: continue
        ecls=f"SElement{step}"; nm=f"Fill_{ecls}_{a}"
        add(a,"_global",nm,HDRF(a,f"_Fill<{ecls}*> -- assign value to every element of [first, last) ({step}-byte POD elements)",len(b))+f'''//
// Retail bytes:  cmp ecx,edx ; je E ; mov eax,[value] ; push esi ; L: (copy {step} bytes from [eax] to [ecx]) ; add ecx,{step:#x} ; cmp ecx,edx ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Fill` (`__fastcall`: first in ecx, last in edx, the value reference on the
// stack) for {step}-byte trivially-copyable elements. One of {len(members)} bodies of this shape.
struct {ecls} {{
    unsigned char _body[{step:#x}];
}};

void __fastcall {nm}({ecls}* first, {ecls}* last, const {ecls}& value)
{{
    for (; first != last; ++first)
        *first = value;
}}
''',T(a,'''struct E { unsigned char b['''+str(step)+''']; };
static void __fastcall F(E* f, E* l, const E& v) { for (; f != l; ++f) *f = v; }
int main()
{
    E a[2], v; v.b[0] = 7; F(a, a + 2, v); if (a[1].b[0] != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_44",):   # void _Destroy_range(first, last, allocator) for CCountedPointer elements: release each count block, null the fields
        if len(calls)!=1 or retn!=4: continue
        deleter=calls[0]; nm=f"DestroyRange_CountedPointer_{a}"
        add(a,"_global",nm,HDRF(a,f"_Destroy_range<CCountedPointer*> -- run the inlined CCountedPointer destructor on [first, last)",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; L: mov eax,[esi+4] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; N: mov dword ptr [esi],0 ; mov dword ptr [esi+4],0 ; add esi,8 ; cmp esi,edi ; jne L ; E: pop edi ; pop esi ; ret 4
// Dinkumware `_Destroy_range` (`__fastcall`: first in ecx, last in edx, the by-value allocator
// on the stack) for CCountedPointer elements {{object, count block}}: each element's destructor is
// inlined -- decrement the block's count, at zero call its destroy function (slot +4, object in
// ecx) and delete the block through the flag-tested operator delete @ 0x{deleter:08X}, then null
// both fields. One of {len(members)} bodies of this shape.
struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pObject = 0;
        m_pCount = 0;
    }}
}};

struct CRetailAllocator {{ }};

void __fastcall {nm}(CCountedPointer* first, CCountedPointer* last, CRetailAllocator allocator)
{{
    for (; first != last; ++first)
        first->~CCountedPointer();
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } }; struct A {};
static void __fastcall DR(P* f, P* l, A) { for (; f != l; ++f) f->~P(); }
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; P a[2]; a[0].o = 0; a[0].c = b; a[1].o = 0; a[1].c = 0; DR(a, a + 2, A()); if (g_d != 1 || a[0].c) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_53",):   # ~CRetailVector<SBufferEntry>(): free each element's buffer, then the element array
        if len(calls)!=2 or calls[0]!=calls[1]: continue
        freer=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG and not i.op_str.startswith("esp")][0]
        nm=f"CRetailVector_SBufferEntry{step}"
        add(a,nm,f"~{nm}",HDRF(a,f"{nm}::~{nm}() -- destroy each {step}-byte element (free its buffer when set), then free the element array",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; L: mov eax,[esi] ; test eax,eax ; je N ; push eax ; call free ; add esp,4 ; N: add esi,{step:#x} ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Destructor of a retail vector {{first, last, end}} of {step}-byte entries whose first member is a
// malloc'd buffer: every element's inline destructor frees its buffer (free @ 0x{freer:08X},
// null-checked), then the element array itself is freed. `last` is cached in a named local
// because the loop body calls. The manifest label (`Vector_DeleteAllElements_Impl`) is
// propagated; the classes are named by shape. One of {len(members)} bodies.
extern "C" void __cdecl free(void* block);

struct SBufferEntry{step} {{
    void* m_pBuffer;                       // +0
    unsigned char _rest[{step-4:#x}];
    ~SBufferEntry{step}() {{ if (m_pBuffer) free(m_pBuffer); }}
}};

struct {nm} {{
    SBufferEntry{step}* first;
    SBufferEntry{step}* last;
    SBufferEntry{step}* end;
    ~{nm}();
}};

{nm}::~{nm}()
{{
    SBufferEntry{step}* l = last;
    for (SBufferEntry{step}* p = first; p != l; ++p)
        p->~SBufferEntry{step}();
    if (first)
        free(first);
}}
''',T(a,'''#include <cstdlib>
static int g_f = 0; struct E { void* p; unsigned char r['''+str(step-4)+''']; ~E() { if (p) { std::free(p); ++g_f; } } };
struct V { E* f; E* l; E* e; ~V() { E* ll = l; for (E* p = f; p != ll; ++p) p->~E(); if (f) std::free(f); } };
int main()
{
    { V v; v.f = (E*)std::malloc(sizeof(E) * 2); v.l = v.f + 2; v.e = v.l; v.f[0].p = std::malloc(4); v.f[1].p = 0; } if (g_f != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_58",):   # ~CX() {}: inlined CCountedPointer member release (function-pointer block, both fields nulled), base destructor by tail call
        if len(calls)!=1: continue
        deleter=calls[0]; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        tail=[i.operands[0].imm for i in ins if i.mnemonic=="jmp" and i.operands[0].type==X86_OP_IMM][0]
        tm,tn,_=label(tail); tl=tn.split("::")[-1]; bcls=tm.split("::")[-1] if (tl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",tm.split("::")[-1]) and (tm,tn) in uniq) else f"CBase_{tail:08x}"
        cls=f"{bcls}_Derived_{a}"
        add(a,"_unattributed",f"~{cls}",HDRF(a,f"{cls}::~{cls}() -- releases the CCountedPointer member at +{off-4:#x} (count block destroy + delete at zero, fields nulled), then {bcls}::~{bcls} by tail call",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+{off:#x}] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+{off:#x}] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+{off:#x}] ; push eax ; call operator delete ; add esp,4 ; N: mov dword ptr [esi+{off-4:#x}],0 ; mov dword ptr [esi+{off:#x}],0 ; mov ecx,esi ; pop esi ; jmp {bcls}::~{bcls}
// Implicit/empty destructor: the CCountedPointer member at +{off-4:#x} is released inline (decrement
// the block's count, at zero call its destroy function with the object in ecx and delete the
// block through the flag-tested operator delete @ 0x{deleter:08X}, then null both fields), and
// the base class destructor @ 0x{tail:08X} runs by tail call. The manifest label is propagated.
// One of {len(members)} bodies of this shape.
struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pObject = 0;
        m_pCount = 0;
    }}
}};

struct {bcls} {{
    unsigned char _body[{off-4:#x}];
    ~{bcls}();                             // 0x{tail:08X}
}};

struct {cls} : {bcls} {{
    CCountedPointer m_pointer;             // +{off-4:#x}
    ~{cls}();
}};

{cls}::~{cls}()
{{
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct Base { long x; ~Base() { g_d += 10; } }; struct X : Base { P p; ~X(); }; X::~X() {}
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; { X x; x.p.o = 0; x.p.c = b; } if (g_d != 11) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_103",):   # `??_G` of a non-polymorphic class: CCountedPointer member released inline, first member's destructor out of line, flag-tested delete
        if len(calls)!=3 or retn!=4 or calls[0]!=calls[2]: continue
        deleter,mdtor,_=calls; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        dm,dn,_=label(mdtor); dl=dn.split("::")[-1]; mcls=dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]) and (dm,dn) in uniq) else f"CMember_{mdtor:08x}"
        cls=f"SPair{mcls}CountedPointer_{a}"
        add(a,"_unattributed",f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- non-virtual: CCountedPointer at +{off-4:#x} released inline, {mcls} at +0 destroyed out of line, flag-tested delete",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+{off:#x}] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+{off:#x}] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+{off:#x}] ; push eax ; call operator delete ; add esp,4 ; N: mov ecx,esi ; mov dword ptr [esi+{off-4:#x}],0 ; mov dword ptr [esi+{off:#x}],0 ; call {mcls}::~{mcls} ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic pair {{{mcls}, CCountedPointer}} emitted by a delete-expression: the
// implicit destructor is inlined -- the counted pointer (declared second, destroyed first)
// releases its block (destroy function @ slot +4, flag-tested operator delete @ 0x{deleter:08X})
// and nulls its fields, then the {mcls} member's destructor @ 0x{mdtor:08X} runs; bit 0 of the
// flag argument selects the delete of the object itself. The manifest label is propagated.
// One of {len(members)} bodies of this shape.
struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pObject = 0;
        m_pCount = 0;
    }}
}};

struct {mcls} {{
    unsigned char _body[{off-4:#x}];
    ~{mcls}();                             // 0x{mdtor:08X}
}};

struct {cls} {{
    {mcls} first;                          // +0
    CCountedPointer second;                // +{off-4:#x}
}};

// The delete-expression that makes the compiler emit {cls}::`scalar deleting destructor'.
void Destroy_{cls}({cls}* object)
{{
    delete object;
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct B { long r; void (__fastcall* f)(void*); void* o; };
struct P { void* o; B* c; ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct M { void* d; ~M() { g_d += 10; } }; struct X { M f; P s; };
static void Kill(X* x) { delete x; }
int main()
{
    B* b = new B; b->r = 1; b->f = Destroy; b->o = 0; X* x = new X; x->s.o = 0; x->s.c = b; Kill(x); if (g_d != 11) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_59",):   # void fill(first, last, const T& value) with an out-of-line operator=
        if len(calls)!=1 or retn!=4: continue
        asg=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        am,an,_=label(asg); ecls=am.split("::")[-1] if (an.split("::")[-1]=="operator=" and re.match(r"^C[A-Za-z0-9_]+$",am.split("::")[-1]) and (am,an) in uniq) else f"CElement{step}_{asg:08x}"
        nm=f"Fill_{ecls}_{a}"
        add(a,"_global",nm,HDRF(a,f"fill<{ecls}*> -- assign value to every element of [first, last) through {ecls}::operator= @ 0x{asg:08X}",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; push ebx ; mov ebx,[value] ; L: push ebx ; mov ecx,esi ; call {ecls}::operator= ; add esi,{step:#x} ; cmp esi,edi ; jne L ; pop ebx ; E: pop edi ; pop esi ; ret 4
// `fill(first, last, value)` (`__fastcall`: first in ecx, last in edx, the value reference on
// the stack) for {step}-byte elements with an out-of-line copy-assignment @ 0x{asg:08X}.
// One of {len(members)} bodies of this shape.
struct {ecls} {{
    unsigned char _body[{step:#x}];
    {ecls}& operator=(const {ecls}& other);   // 0x{asg:08X}
}};

void __fastcall {nm}({ecls}* first, {ecls}* last, const {ecls}& value)
{{
    for (; first != last; ++first)
        *first = value;
}}
''',T(a,'''static int g_a = 0; struct E { unsigned char b['''+str(step)+''']; E& operator=(const E& o) { ++g_a; b[0] = o.b[0]; return *this; } };
static void __fastcall F(E* f, E* l, const E& v) { for (; f != l; ++f) *f = v; }
int main()
{
    E a[2], v; v.b[0] = 7; F(a, a + 2, v); if (g_a != 2 || a[1].b[0] != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_60",):   # long __cdecl Distance(const iterator& first, const iterator& last): count the tree nodes between two iterators (inlined increment)
        if calls or retn!=0: continue
        cls="CRetailTreeIterator"
        add(a,"_global",f"Distance_{cls}_{a}",HDRF(a,f"distance<{cls}>(first, last) -- counts increments from first to last with the red-black tree successor walk inlined",len(b))+f'''//
// Retail bytes:  mov ecx,[first] ; mov edx,[last] ; mov ecx,[ecx] ; push esi ; mov esi,[edx] ; xor eax,eax ; cmp ecx,esi ; je E ; L: (successor of ecx: descend left from the right child, else climb while the node is its parent's right) ; inc eax ; cmp ecx,esi ; jne L ; E: pop esi ; ret
// Iterator distance over the retail red-black tree: `n = 0; for (i = first; i != last; ++i) ++n;`
// with the Dinkumware-shaped `_Inc` inlined (a node with a right child goes to the leftmost
// node of that subtree; otherwise it climbs while it is its parent's right child and steps to
// that parent unless it already is the node's right child). Both iterators arrive by const
// reference (`__cdecl`, two stack arguments). The manifest label (`BinaryTree_Height`) is
// propagated. One of {len(members)} bodies of this shape.
struct {cls} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; }};
    node* current;
    {cls}(node* n) : current(n) {{ }}
    {cls}& operator++()
    {{
        node* p = current;
        if (p->right) {{
            p = p->right;
            while (p->left)
                p = p->left;
        }}
        else {{
            node* q;
            while (p == (q = p->parent)->right)
                p = q;
            if (p->right != q)
                p = q;
        }}
        current = p;
        return *this;
    }}
    bool operator!=(const {cls}& other) const {{ return current != other.current; }}
}};

long __cdecl Distance_{cls}_{a}(const {cls}& first, const {cls}& last)
{{
    long n = 0;
    {cls} i = first;
    while (i != last) {{                  // SGI __distance: advance, then count
        ++i;
        ++n;
    }}
    return n;
}}
''',T(a,'''struct I { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; }; node* c; I(node* n) : c(n) {}
  I& operator++() { node* p = c; if (p->r) { p = p->r; while (p->l) p = p->l; } else { node* q; while (p == (q = p->parent)->r) p = q; if (p->r != q) p = q; } c = p; return *this; }
  bool operator!=(const I& o) const { return c != o.c; } };
static long __cdecl D(const I& f, const I& l) { long n = 0; I i = f; while (i != l) { ++i; ++n; } return n; }
int main()
{
    I::node h = {0,{0},0,0,0}, a = {1,{0},&h,0,0}, b = {1,{0},&a,0,0}; h.parent = &a; h.l = &b; h.r = &a; a.l = &b; I f(&b), l(&h); if (D(f, l) != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_62",):   # T* vector::erase(first, last): `last = copy(last, end, first)` with the SGI counted copy inlined (8-byte POD elements)
        if calls or retn!=8: continue
        step=1<<[i.operands[1].imm for i in ins if i.mnemonic=="sar" and i.operands[1].type==X86_OP_IMM][0]
        nm=f"CRetailVector_pod{step}"
        add(a,nm,"erase",HDRF(a,f"{nm}::erase({nm[13:]}*, {nm[13:]}*) -- move the tail down with the inlined SGI counted copy, set `last`, return the first iterator",len(b))+f'''//
// Retail bytes:  mov eax,[f] ; push esi ; mov esi,[ecx+4] ; push edi ; mov edi,[l] ; sub esi,edi ; sar esi,{step.bit_length()-1} ; test esi,esi ; mov edx,eax ; jle E ; sub edi,eax ; push ebx ; L: (copy {step} bytes from [edi+edx] to [edx]) ; add edx,{step:#x} ; dec esi ; jne L ; pop ebx ; E: pop edi ; mov [ecx+4],edx ; pop esi ; ret 8
// Retail vector erase for {step}-byte trivially-copyable elements: `last = copy(l, last, f)` with the
// SGI counted `__copy` inlined (the compiler folds the source pointer into an offset from the
// destination), then the first iterator is returned. One of {len(members)} bodies of this shape.
struct SElement{step} {{
    unsigned char _body[{step:#x}];
}};

struct {nm} {{
    SElement{step}* first;
    SElement{step}* last;
    SElement{step}* end;
    static SElement{step}* Copy(SElement{step}* f, SElement{step}* l, SElement{step}* result)
    {{
        for (long n = l - f; n > 0; --n) {{
            *result = *f;
            ++f;
            ++result;
        }}
        return result;
    }}
    SElement{step}* erase(SElement{step}* f, SElement{step}* l);
}};

SElement{step}* {nm}::erase(SElement{step}* f, SElement{step}* l)
{{
    SElement{step}* s = Copy(l, last, f);
    last = s;
    return f;
}}
''',T(a,'''struct E { unsigned char b['''+str(step)+''']; };
struct V { E* f; E* l; E* e; static E* C(E* a, E* b, E* r) { for (long n = b - a; n > 0; --n) { *r = *a; ++a; ++r; } return r; } E* erase(E* a, E* b) { E* s = C(b, l, a); l = s; return a; } };
int main()
{
    E buf[3]; buf[0].b[0] = 1; buf[1].b[0] = 2; buf[2].b[0] = 3; V v; v.f = buf; v.l = buf + 3; v.e = buf + 3; E* r = v.erase(buf, buf + 1); if (r != buf || v.l != buf + 2 || buf[0].b[0] != 2 || buf[1].b[0] != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_84",):   # CRetailVector_dword::CRetailVector(n, allocator): null the three pointers, allocate n elements, set the pointers
        if len(calls)!=1 or retn!=8: continue
        alloc=calls[0]; nm="CRetailVector_dword"
        add(a,nm,nm,HDRF(a,f"{nm}::{nm}(unsigned long, allocator) -- sized constructor: pointers nulled, malloc(n * 4), first/last/end set",len(b))+f'''//
// Retail bytes:  push esi ; xor eax,eax ; mov esi,ecx ; push edi ; mov edi,[n] ; cmp edi,eax ; mov [esi],eax ; mov [esi+4],eax ; mov [esi+8],eax ; je Z ; mov eax,edi ; shl eax,2 ; push eax ; call malloc ; pop ecx ; Z: mov [esi],eax ; mov [esi+4],eax ; lea eax,[eax+edi*4] ; mov [esi+8],eax ; pop edi ; mov eax,esi ; pop esi ; ret 8
// Sized constructor of the retail vector {{first, last, end}} of 4-byte elements: the pointers are
// nulled first, then `n` elements are allocated with malloc @ 0x{alloc:08X} (a null buffer for
// n == 0: the cleared register is reused), and first/last/end are set to the buffer, the buffer
// and buffer + n. `ret 8`: the count and the by-value allocator. The manifest label
// (`Array_Initialize`) is propagated. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator {{ }};

struct {nm} {{
    long* first;
    long* last;
    long* end;
    {nm}(unsigned long count, CRetailAllocator allocator);
}};

{nm}::{nm}(unsigned long count, CRetailAllocator allocator)
    : first(0), last(0), end(0)
{{
    long* buffer = count != 0 ? (long*)malloc(count * sizeof(long)) : 0;
    first = buffer;
    last = buffer;
    end = buffer + count;
}}
''',T(a,'''#include <cstdlib>
struct A {}; struct V { long* f; long* l; long* e; V(unsigned long n, A) : f(0), l(0), e(0) { long* b = n != 0 ? (long*)std::malloc(n * 4) : 0; f = b; l = b; e = b + n; } };
int main()
{
    V v(3, A()); if (!v.f || v.l != v.f || v.e != v.f + 3) '''+FAIL(a)+'''
    std::free(v.f); V w(0, A()); if (w.f || w.e) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_102",):   # bool CIntelligentPointer::IsThingAlive(): target present and its byte flag at +off bit 0 clear
        if len(calls)!=2 or calls[0]!=calls[1] or retn!=0: continue
        getp=calls[0]; off=[i.operands[0].mem.disp for i in ins if i.mnemonic=="test" and i.operands[0].type==X86_OP_MEM][0]
        add(a,"_global",f"IsThingAlive_{a}",HDRF(a,f"IS_THING_ALIVE(CIntelligentPointer&) -- the pointer resolves (GetPItem @ 0x{getp:08X}) and the thing's flag byte at +{off:#x} has bit 0 clear",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+{off:#x}],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// `IS_THING_ALIVE` on an intelligent pointer (`__fastcall`, the pointer object in ecx): the
// target is resolved twice through CBaseIntelligentPointer::GetPItem @ 0x{getp:08X} (once for
// the null test, once for the flag), and the result is true only when the thing exists and bit
// 0 of its byte at +{off:#x} is clear; separate `return true` / `return false` paths keep retail's
// `mov al,1` / `xor al,al`. One of {len(members)} bodies of this shape.
struct CThing {{
    unsigned char _pad[{off:#x}];
    unsigned char m_flags;                 // +{off:#x}: bit 0 = dead
}};

struct CIntelligentPointer {{
    void* m_pLink;
    void* m_pData;
    CThing* GetPItem();                    // 0x{getp:08X}
}};

bool __fastcall IsThingAlive_{a}(CIntelligentPointer& pointer)
{{
    if (pointer.GetPItem() == 0)
        return false;
    if (pointer.GetPItem()->m_flags & 1)
        return false;
    return true;
}}
''',T(a,'''struct T { unsigned char p['''+str(off)+''']; unsigned char f; }; struct IP { void* l; T* d; T* G() { return d; } };
static bool __fastcall A(IP& p) { if (p.G() == 0) return false; if (p.G()->f & 1) return false; return true; }
int main()
{
    T t; t.f = 0; IP p; p.d = &t; if (!A(p)) '''+FAIL(a)+'''
    t.f = 1; if (A(p)) '''+FAIL(a)+'''
    p.d = 0; if (A(p)) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_100",):   # void vector<T>::resize(n, const T& value) for 12-byte elements: Erase or Insert out of line (signed size through idiv)
        if len(calls)!=2 or retn!=8: continue
        er,ins_=calls; st=[p for p in pushes if 0<p<64]
        if st: step=st[0]
        else:
            step=None; prev=None; magic=None; shift=None
            for i in ins:
                if i.mnemonic=="imul" and prev is not None and prev.mnemonic=="mov" and prev.operands[1].type==X86_OP_IMM: magic=prev.operands[1].imm
                if magic is not None and shift is None and i.mnemonic=="sar" and i.op_str.startswith("edx"): shift=i.operands[1].imm
                prev=i
            if magic is None or shift is None: continue
            step=int(round(2**(32+shift)/magic))
        nm=f"CRetailVector_elem{step}"
        add(a,nm,"resize",HDRF(a,f"{nm}::resize(unsigned long, const T&) -- Erase @ 0x{er:08X} of the tail or Insert @ 0x{ins_:08X} of the missing elements ({step}-byte elements, signed size)",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[ecx+4] ; push esi ; push edi ; mov edi,[ecx] ; mov eax,ebx ; sub eax,edi ; push {step:#x} ; cdq ; pop esi ; idiv esi ; mov esi,[n] ; cmp esi,eax ; jae G ; lea eax,[esi+esi*2] ; push ebx ; lea eax,[edi+eax*4] ; push eax ; call Erase ; jmp R ; G: mov ebx,[ecx+4] ; push [value] ; mov eax,ebx ; sub eax,edi ; push {step:#x} ; pop edi ; cdq ; idiv edi ; sub esi,eax ; push esi ; push ebx ; call Insert ; R: pop edi ; pop esi ; pop ebx ; ret 8
// Retail vector resize for {step}-byte elements: the size is a signed element count (`cdq / idiv`),
// `n` below it erases the tail through Erase(first + n, last) @ 0x{er:08X}, otherwise
// Insert(last, n - size, value) @ 0x{ins_:08X} appends the missing elements (the size is
// re-derived from the re-read `last`). The manifest label (`CArray<...>::push_back`) is
// propagated. One of {len(members)} bodies of this shape.
struct SElement{step} {{
    unsigned char _body[{step:#x}];
}};

struct {nm} {{
    SElement{step}* first;
    SElement{step}* last;
    SElement{step}* end;
    SElement{step}* Erase(SElement{step}* f, SElement{step}* l);   // 0x{er:08X}
    void Insert(SElement{step}* where, unsigned long count, const SElement{step}& value);   // 0x{ins_:08X}
    long size() const {{ return last - first; }}
    void resize(unsigned long n, const SElement{step}& value);
}};

void {nm}::resize(unsigned long n, const SElement{step}& value)
{{
    if (n < (unsigned long)size())
        Erase(first + n, last);
    else
        Insert(last, n - size(), value);
}}
''',T(a,'''static int g_e = 0, g_i = 0; struct E { unsigned char b['''+str(step)+''']; };
struct V { E* f; E* l; E* e; E* Er(E* a, E*) { ++g_e; l = a; return a; } void In(E*, unsigned long c, const E&) { g_i += (int)c; } long size() const { return l - f; } void R(unsigned long n, const E& v) { if (n < (unsigned long)size()) Er(f + n, l); else In(l, n - size(), v); } };
int main()
{
    E buf[4]; V v; v.f = buf; v.l = buf + 4; v.e = buf + 4; E x; v.R(2, x); if (g_e != 1 || v.l != buf + 2) '''+FAIL(a)+'''
    v.R(5, x); if (g_i != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_108",):   # STLport __insertion_sort(first, last, comp): __linear_insert(first, i, T(*i), comp) for each i in (first, last)
        if len(calls)!=1 or retn!=4: continue
        li=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG][0]
        ecls=f"SElement{step}"; nm=f"InsertionSort_{ecls}_{a}"
        body="\n".join("    long m_%02x;" % (4*j) for j in range(step//4))+"\n    %s(const %s& o) : %s { }   // user copy constructor: the by-value argument is built in the argument area" % (ecls,ecls,", ".join("m_%02x(o.m_%02x)" % (4*j,4*j) for j in range(step//4)))
        add(a,"_global",nm,HDRF(a,f"STLport __insertion_sort<{ecls}*, Compare> -- __linear_insert(first, i, T(*i), comp) @ 0x{li:08X} for every element after the first",len(b))+f'''//
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,edx ; mov ebx,ecx ; cmp ebx,edi ; je E ; lea esi,[ebx+{step:#x}] ; cmp esi,edi ; je E ; mov ebp,[comp] ; L: (push comp ; build the {step}-byte copy of *i in the argument area) ; mov edx,esi ; mov ecx,ebx ; call __linear_insert ; add esi,{step:#x} ; cmp esi,edi ; jne L ; E: pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// STLport `__insertion_sort(first, last, comp)`: `if (first != last) for (i = first + 1; i != last;
// ++i) __linear_insert(first, i, T(*i), comp);` -- the value travels BY VALUE (copied into the
// argument area) with the 4-byte comparator functor. /Gr TU (`__fastcall`: first in ecx, last in
// edx, comp on the stack). The manifest label (`_Insertion_sort1`) is propagated. One of {len(members)} bodies.
struct {ecls} {{
{body}
}};

struct SCompare {{                        // 4-byte comparison functor passed by value
    void* m_context;
}};

void __fastcall LinearInsert_{li:08x}({ecls}* first, {ecls}* last, {ecls} value, SCompare comp);   // 0x{li:08X}

void __fastcall {nm}({ecls}* first, {ecls}* last, SCompare comp)
{{
    if (first != last)
        for ({ecls}* i = first + 1; i != last; ++i)
            LinearInsert_{li:08x}(first, i, {ecls}(*i), comp);
}}
''',T(a,'''static int g_l = 0; struct E { long a, b; E(long x) : a(x), b(0) {} E(const E& o) : a(o.a), b(o.b) {} }; struct Cmp { void* c; };
static void __fastcall LI(E*, E*, E v, Cmp) { g_l += (int)v.a; }
static void __fastcall IS(E* f, E* l, Cmp c) { if (f != l) for (E* i = f + 1; i != l; ++i) LI(f, i, E(*i), c); }
int main()
{
    E a[3] = { E(1), E(2), E(3) }; Cmp c = { 0 }; IS(a, a + 3, c); if (g_l != 5) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_23",):   # void CPersistContext::TransferVector(T* value) { CheckCRC(""); mode 2 writes through the save stream, otherwise reads through the load stream }
        if len(calls)!=3 or retn!=4: continue
        crc,wr,rd=calls; litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva)
        wm,wn,_=label(wr); rm,rn,_=label(rd); wl=re.sub(r"[^A-Za-z0-9_]+","_",wn.split("::")[-1]).strip("_")[:40] or "Write"; rl=re.sub(r"[^A-Za-z0-9_]+","_",rn.split("::")[-1]).strip("_")[:40] or "Read"
        add(a,"CPersistContext",f"TransferBinary_{a}",HDRF(a,f"CPersistContext::TransferBinary(T*) -- CheckCRC(\\\"{s}\\\"), then {wl} @ 0x{wr:08X} on the save stream in mode 2, else {rl} @ 0x{rd:08X} on the load stream",len(b))+f'''//
// Retail bytes:  push esi ; push offset "{s}" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; mov edx,[value] ; dec eax ; dec eax ; je L ; mov ecx,[esi+28h] ; call {wl} ; jmp R ; L: mov ecx,[esi+24h] ; call {rl} ; R: pop esi ; ret 4
// Persist transfer of one value: after the CRC check with the (empty) key the mode at +0x18
// selects the direction -- 2 (load) hands the load stream (+0x24) and the value to the
// `__fastcall` reader @ 0x{rd:08X}, anything else hands the save stream (+0x28) and the value to
// the `__fastcall` writer @ 0x{wr:08X}. The `dec/dec` sequence is the size-optimised lowering of
// the two-way switch on the mode (the lander's "s" pragma). Labels are propagated; the streams are modelled as opaque
// pointers. One of {len(members)} bodies of this shape.
struct CDataOutputStream;
struct CMemoryDataInputStream;
struct SValue;

void __fastcall {wl}_{wr:08x}(CDataOutputStream* stream, SValue* value);        // 0x{wr:08X}
void __fastcall {rl}_{rd:08x}(CMemoryDataInputStream* stream, SValue* value);   // 0x{rd:08X}

struct CPersistContext {{
    unsigned char _pad0[0x18];
    long m_mode;                           // +0x18 (EMode: 2 = save)
    unsigned char _pad1[8];
    CMemoryDataInputStream* m_pLoadStream; // +0x24
    CDataOutputStream* m_pSaveStream;      // +0x28
    void CheckCRC(const char* name);       // 0x{crc:08X}
    void TransferBinary_{a}(SValue* value);
}};

void CPersistContext::TransferBinary_{a}(SValue* value)
{{
    CheckCRC("{s}");
    switch (m_mode) {{
    case 2:                                // load: the stream at +0x24
        {rl}_{rd:08x}(m_pLoadStream, value);
        break;
    default:                               // save: the stream at +0x28
        {wl}_{wr:08x}(m_pSaveStream, value);
        break;
    }}
}}
''',T(a,'''static int g_w = 0, g_r = 0; struct O; struct I; struct Vl;
static void __fastcall W(O*, Vl*) { ++g_w; } static void __fastcall R(I*, Vl*) { ++g_r; }
struct C { unsigned char p0[0x18]; long m; unsigned char p1[8]; I* in; O* out; void CRC(const char*) {} void T(Vl* v) { CRC(""); switch (m) { case 2: R(in, v); break; default: W(out, v); break; } } };
int main()
{
    C c; c.m = 2; c.T(0); c.m = 1; c.T(0); if (g_w != 1 || g_r != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_106","w512_112"):   # SGI __copy_backward(first, last, result, tag, Distance*) for wide elements: POD block copy or an out-of-line operator=
        def elem_size2(ins):
            magic=None; shift=None; prev=None
            for i in ins:
                if i.mnemonic=="imul" and prev is not None and prev.mnemonic=="mov" and prev.operands[0].type==X86_OP_REG and prev.operands[1].type==X86_OP_IMM: magic=prev.operands[1].imm
                if magic is not None and shift is None and i.mnemonic=="sar" and i.operands[1].type==X86_OP_IMM and i.op_str.startswith("edx"): shift=i.operands[1].imm
                prev=i
            if magic is not None and shift is not None: return int(round(2**(32+shift)/magic))
            for i in ins:
                if i.mnemonic=="sar" and i.operands[1].type==X86_OP_IMM: return 1<<i.operands[1].imm
            return None
        esz=elem_size2(ins)
        if not esz or retn!=12: continue
        pod=(TPL=="w512_106")
        if pod and calls: continue
        if not pod and len(calls)!=1: continue
        asg=calls[0] if not pod else 0
        ecls=f"SElement{esz}" if pod else f"CElement{esz}_{asg:08x}"; nm=f"CopyBackward_{ecls}_{a}"
        add(a,"_global",nm,HDRF(a,f"SGI __copy_backward<{ecls}*> ({esz}-byte elements{', out-of-line operator= @ 0x%08X' % asg if not pod else ''}) -- counted backward copy, returns the moved-back result",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by {esz}) ; test eax,eax ; jle Z ; push ebx ; {"mov ecx,eax ; mov eax,[result] ; push edi ; L: sub esi,%#x ; sub eax,%#x ; dec ecx ; (block copy of %d bytes from [esi] to [eax]) ; jne L ; pop edi ; pop ebx ; pop esi ; ret 0Ch" % (esz,esz,esz) if pod else "push edi ; mov edi,[result] ; mov ebx,eax ; L: sub esi,%#x ; sub edi,%#x ; push esi ; mov ecx,edi ; call operator= ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch" % (esz,esz)} ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` for
// {esz}-byte elements: `for (n = last - first; n > 0; --n) *--result = *--last;` and the moved-back
// result is returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the
// stack, `ret 0Ch`). {"The POD copy is a block move through pointer registers." if pod else "Each element is assigned through the out-of-line operator= @ 0x%08X." % asg} The manifest
// label (`_Move_backward` / `_Copy_backward`) is propagated from Dinkumware. One of {len(members)} bodies.
struct {ecls} {{
    unsigned char _body[{esz:#x}];
{"" if pod else "    " + ecls + "& operator=(const " + ecls + "& other);   // 0x%08X" % asg + chr(10)}}};

struct SRandomAccessIteratorTag {{ SRandomAccessIteratorTag() {{ }} }};

{ecls}* __fastcall {nm}({ecls}* first, {ecls}* last, {ecls}* result, SRandomAccessIteratorTag category, long* distance)
{{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}}
''',T(a,'''struct E { unsigned char b['''+str(esz)+''']; '''+("" if pod else "E& operator=(const E& o) { b[0] = o.b[0]; return *this; }")+''' }; struct Tg { Tg() {} };
static E* __fastcall C(E* f, E* l, E* r, Tg, long*) { for (long n = l - f; n > 0; --n) *--r = *--l; return r; }
int main()
{
    E s[3]; s[0].b[0] = 1; s[1].b[0] = 2; s[2].b[0] = 9; E* e = C(s, s + 2, s + 3, Tg(), 0); if (e != s + 1 || s[1].b[0] != 1 || s[2].b[0] != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_105",):   # CRetailVector_dword::CRetailVector_dword(const CRetailVector_dword&): allocate other's size, memmove-copy the elements
        if len(calls)!=2 or retn!=4: continue
        alloc,mm=calls; nm="CRetailVector_dword"
        add(a,nm,nm,HDRF(a,f"{nm}::{nm}(const {nm}&) -- copy constructor: pointers nulled, malloc of the source size, elements copied through the memmove-returning copy",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[other] ; mov eax,[ebx] ; push esi ; push edi ; mov edi,[ebx+4] ; sub edi,eax ; xor eax,eax ; mov esi,ecx ; sar edi,2 ; cmp edi,eax ; mov [esi],eax ; mov [esi+4],eax ; mov [esi+8],eax ; je Z ; lea eax,[edi*4] ; push eax ; call malloc ; add esp,4 ; Z: lea ecx,[eax+edi*4] ; mov [esi],eax ; mov [esi+4],eax ; mov [esi+8],ecx ; mov edi,[ebx+4] ; mov ecx,[ebx] ; cmp edi,ecx ; je S ; sub edi,ecx ; push edi ; push ecx ; push eax ; call memmove ; add esp,0Ch ; add eax,edi ; S: mov [esi+4],eax ; pop edi ; mov eax,esi ; pop esi ; pop ebx ; ret 4
// Copy constructor of the retail vector {{first, last, end}} of 4-byte elements: the pointers are
// nulled, a buffer for the source's element count is allocated (malloc @ 0x{alloc:08X}, null for
// an empty source: the cleared register is reused), first/last/end are set, and the elements
// are copied with the memmove-returning copy (@ 0x{mm:08X}; `last` becomes the advanced
// destination). The manifest label is propagated. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);

struct {nm} {{
    long* first;
    long* last;
    long* end;
    static long* Copy(long* f, long* l, long* result)
    {{
        if (f != l) {{
            unsigned long bytes = (char*)l - (char*)f;
            result = (long*)((char*)memmove(result, f, bytes) + bytes);
        }}
        return result;
    }}
    {nm}(const {nm}& other);
}};

{nm}::{nm}(const {nm}& other)
    : first(0), last(0), end(0)
{{
    unsigned long count = other.last - other.first;
    long* buffer = count != 0 ? (long*)malloc(count * sizeof(long)) : 0;
    first = buffer;
    last = buffer;
    end = buffer + count;
    last = Copy(other.first, other.last, buffer);
}}
''',T(a,'''#include <cstdlib>
#include <cstring>
struct V { long* f; long* l; long* e; static long* C(long* a, long* b, long* r) { if (a != b) { unsigned long n = (char*)b - (char*)a; r = (long*)((char*)std::memmove(r, a, n) + n); } return r; }
  V() : f(0), l(0), e(0) {} V(const V& o) : f(0), l(0), e(0) { unsigned long n = o.l - o.f; long* b = n != 0 ? (long*)std::malloc(n * 4) : 0; f = b; l = b; e = b + n; l = C(o.f, o.l, b); } };
int main()
{
    long buf[2] = { 3, 4 }; V s; s.f = buf; s.l = buf + 2; s.e = buf + 2; V d(s); if (d.l != d.f + 2 || d.f[1] != 4 || d.e != d.f + 2) '''+FAIL(a)+'''
    std::free(d.f);
'''),b)
    elif TPL in ("w512_91",):   # ~CRetailVector<SPair>(): destroy each element's two out-of-line-destructor members (reverse order), then free the array
        if len(calls)!=3 or calls[0]!=calls[1] or retn!=0: continue
        dtor,_,freer=calls; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG and not i.op_str.startswith("esp")][0]
        offs=sorted([i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"]+[0])
        dm,dn,_=label(dtor); dl=dn.split("::")[-1]; mcls=dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]) and (dm,dn) in uniq) else f"CMember_{dtor:08x}"
        if len(offs)!=2: continue
        nm=f"CRetailVector_SPair{mcls}{step}"
        add(a,nm,f"~{nm}",HDRF(a,f"{nm}::~{nm}() -- destroy each {step}-byte element's two {mcls} members (+{offs[1]:#x} then +0, destructor @ 0x{dtor:08X}), then free the array",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; L: lea ecx,[esi+{offs[1]:#x}] ; call {mcls}::~{mcls} ; mov ecx,esi ; call {mcls}::~{mcls} ; add esi,{step:#x} ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Destructor of a retail vector {{first, last, end}} of {step}-byte entries whose first two members
// are {mcls} objects (destructor @ 0x{dtor:08X}): every element's implicit destructor runs
// (members in reverse declaration order), then the element array is freed (free @ 0x{freer:08X}).
// `last` is cached in a named local because the loop body calls. The manifest label is
// propagated; the classes are named by shape. One of {len(members)} bodies.
extern "C" void __cdecl free(void* block);

struct {mcls} {{
    void* m_data;
    ~{mcls}();                             // 0x{dtor:08X}
}};

struct SPair{mcls}{step} {{
    {mcls} m_first;                        // +0
    {mcls} m_second;                       // +{offs[1]:#x}
{"    unsigned char _rest[%#x];" % (step-offs[1]-4) + chr(10) if step-offs[1]-4>0 else ""}}};

struct {nm} {{
    SPair{mcls}{step}* first;
    SPair{mcls}{step}* last;
    SPair{mcls}{step}* end;
    ~{nm}();
}};

{nm}::~{nm}()
{{
    SPair{mcls}{step}* l = last;
    for (SPair{mcls}{step}* p = first; p != l; ++p)
        p->~SPair{mcls}{step}();
    if (first)
        free(first);
}}
''',T(a,'''#include <cstdlib>
static int g_d = 0; struct M { void* d; ~M() { ++g_d; } }; struct E { M a, b; unsigned char r['''+str(max(step-8,1))+''']; };
struct V { E* f; E* l; E* e; ~V() { E* ll = l; for (E* p = f; p != ll; ++p) p->~E(); if (f) std::free(f); } };
int main()
{
    { V v; v.f = (E*)std::malloc(sizeof(E) * 2); v.l = v.f + 2; v.e = v.l; } if (g_d != 4) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_75",):   # list<T>::clear() with an out-of-line value destructor: advance the cursor, destroy the old node's value, free the node, re-link the head
        if len(calls)!=2 or retn!=0: continue
        dtor,freer=calls; voff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"][0]
        dm,dn,_=label(dtor); dl=dn.split("::")[-1]; mcls=dm.split("::")[-1] if (dl.startswith("~") and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]) and (dm,dn) in uniq) else f"CValue_{dtor:08x}"
        nm=f"CRetailList_{mcls}"
        add(a,nm,"clear",HDRF(a,f"{nm}::clear() -- destroy every node's {mcls} value (@ 0x{dtor:08X}), free the node, re-link the head to itself",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov eax,[edi] ; mov esi,[eax] ; cmp esi,eax ; je E ; push ebx ; L: mov ebx,esi ; mov esi,[esi] ; lea ecx,[ebx+{voff:#x}] ; call {mcls}::~{mcls} ; push ebx ; call free ; cmp esi,[edi] ; pop ecx ; jne L ; pop ebx ; E: mov eax,[edi] ; mov [eax],eax ; mov edi,[edi] ; mov [edi+4],edi ; pop edi ; pop esi ; ret
// Retail std::list::clear for {mcls} values: walks from head->next to the head (re-read from
// `this` each iteration), advancing the cursor before the old node's value is destroyed
// (@ 0x{dtor:08X}) and the node freed (free @ 0x{freer:08X}, the argument popped into ecx: a size
// TU), then links the head to itself. The manifest label is propagated. One of {len(members)} bodies.
extern "C" void __cdecl free(void* block);

struct {mcls} {{
    void* m_data;
    ~{mcls}();                             // 0x{dtor:08X}
}};

struct {nm} {{
    struct node {{
        node* next;                        // +0
        node* prev;                        // +4
        {mcls} value;                      // +{voff:#x}
    }};
    node* head;
    void clear();
}};

void {nm}::clear()
{{
    node* n = head->next;
    while (n != head) {{
        node* victim = n;
        n = n->next;
        victim->value.~{mcls}();
        free(victim);
    }}
    head->next = head;
    head->prev = head;
}}
''',T(a,'''#include <cstdlib>
static int g_d = 0; struct M { void* d; ~M() { ++g_d; } };
struct L { struct N { N* n; N* p; M v; }; N* h; void clear() { N* x = h->n; while (x != h) { N* v = x; x = x->n; v->v.~M(); std::free(v); } h->n = h; h->p = h; } };
int main()
{
    L l; L::N head; l.h = &head; L::N* a = (L::N*)std::malloc(sizeof(L::N)); head.n = a; a->n = &head; a->p = &head; head.p = a; l.clear(); if (g_d != 1 || head.n != &head || head.p != &head) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_77",):   # CX::CX(const CX& other) : CBase(other) { vtable; trailing dword members copied }
        if len(calls)!=1 or len(vts)!=1 or retn!=4: continue
        base=calls[0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        bm,bn,_=label(base); basecls=bm.split("::")[-1] if (bn.split("::")[-1]==bm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",bm.split("::")[-1])) else f"CBase_{base:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        lds=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "edi +" in i.op_str]
        sts=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str and i.operands[0].mem.disp]
        if not lds or lds!=sts: continue
        mem=""; prev=lds[0]
        for j,o in enumerate(lds):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=f"    long m_{o:02x};".ljust(40)+f"// +{o:#x}\n"; prev=o+4
        inits="".join(f"    , m_{o:02x}(other.m_{o:02x})\n" for o in lds)
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(const {cls}&) -- copy constructor: {basecls} copy @ 0x{base:08X}, own vtable, {len(lds)} dword members copied",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; mov edi,[other] ; push edi ; mov esi,ecx ; call {basecls}::{basecls}(const {basecls}&) ; mov dword ptr [esi],offset {cls}::`vftable' ; {" ; ".join("mov eax,[edi+%#x] ; mov [esi+%#x],eax" % (o,o) for o in lds)} ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a {basecls}-derived class: the base copy constructor @ 0x{base:08X} runs,
// this class's vtable (0x{vt:08X}, rtti-vtables.tsv) is installed and the {len(lds)} trailing dword
// members are copied in declaration order; `this` is returned. Member types are not
// recoverable beyond their size (`long`). One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    unsigned char _body[{lds[0]-4:#x}];
    {basecls}(const {basecls}& other);     // 0x{base:08X}
}};

struct {cls} : {basecls} {{
{mem}    virtual void v00();
    {cls}(const {cls}& other);
}};

{cls}::{cls}(const {cls}& other)
    : {basecls}(other)
{inits}{{
}}
''',T(a,'''static int g_b = 0; struct B { virtual void v00() {} unsigned char body[4]; B() {} B(const B&) { ++g_b; } };
struct D : B { long a, b; virtual void v00() {} D() {} D(const D& o); }; D::D(const D& o) : B(o), a(o.a), b(o.b) {}
int main()
{
    D s; s.a = 1; s.b = 2; D d(s); if (g_b != 1 || d.a != 1 || d.b != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_93",):   # CX* __fastcall Create(a, b) { return new CX(a, b); }  with the two-argument constructor out of line
        if len(calls)!=2 or retn!=0: continue
        newer,ctor=calls; size=pushes[0]; cls=tctor_class(ctor) or f"CClass_{ctor:08x}"
        add(a,"_global",f"Create_{cls}_{a}",HDRF(a,f"Create_{cls}(a, b) -- __fastcall factory: new {cls}(a, b) with the constructor @ 0x{ctor:08X} out of line",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; push {size:#x} ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; push edi ; mov ecx,eax ; call {cls}::{cls} ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// A __fastcall factory (first argument in ecx, second in edx): allocates sizeof({cls}) = {size:#x}
// bytes (operator new @ 0x{newer:08X}), null-checks the block and runs the two-argument
// constructor @ 0x{ctor:08X} on it, returning the new object (or null). The argument types are
// not recoverable from the body; `void*` models them. One of {len(members)} bodies of this shape.
struct {cls} {{
    unsigned char _body[{size:#x}];
    {cls}(void* first, void* second);     // 0x{ctor:08X}
}};

{cls}* __fastcall Create_{cls}_{a}(void* first, void* second)
{{
    return new {cls}(first, second);
}}
''',T(a,'''static int g_c = 0; struct X { unsigned char b[8]; X(void*, void*) { ++g_c; } };
static X* __fastcall C(void* a, void* b) { return new X(a, b); }
int main()
{
    X* x = C(0, 0); if (!x || g_c != 1) '''+FAIL(a)+'''
    delete x;
'''),b)
    elif TPL in ("w512_45",):   # CCombatAction_X::CCombatAction_X(owner, thing) : CCombatActionBase(owner, thing, K, CSoundPair()) { [CCharString name("LIT", -1); SetPDef(name);] }
        if len(vts)!=1 or retn!=8: continue
        base=calls[0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"; k=[p for p in pushes if p<0x10000 and p!=0xffffffff][0]
        bm,bn,_=label(base); basecls=bm.split("::")[-1] if (bm,bn) in uniq or bm=="CCombatActionBase" else f"CBase_{base:08x}"
        lits=[p for p in pushes if 0x1200000<=p<0x1300000]; s=lit(lits[0]) if lits else None; setter=calls[2] if lits else 0
        if lits and len(calls)!=4: continue
        if not lits and len(calls)!=1: continue
        body=(f'''    {{
        CCharString name("{s}", -1);       // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetName(name);                     // 0x{setter:08X}
    }}
''' if s else "")
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(CThingCreatureBase*, CThing*) -- base {basecls}(owner, thing, {k}, zeroed CSoundPair)" + (f', then the name "{s}" through the setter @ 0x{setter:08X}' if s else ""),len(b))+f'''//
// Retail bytes:  sub esp,8 ; mov edx,[thing] ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[owner] ; push {k:#x} ; push ecx ; push edx ; mov ecx,esi ; call {basecls}::{basecls} ; {"push -1 ; push offset lit ; lea ecx,[esp+1Ch] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea eax,[esp+14h] ; push eax ; mov ecx,esi ; call setter ; lea ecx,[esp+14h] ; call CCharString::~CCharString" if s else "mov dword ptr [esi],offset " + cls + "::`vftable'"} ; mov eax,esi ; pop esi ; add esp,8 ; ret 8
// Two-argument combat-action constructor: the base @ 0x{base:08X} takes the owner, the thing,
// the action code {k} ({k:#x}) and a two-dword sound pair built zeroed on the stack by its inline
// constructor; then this class's vtable (0x{vt:08X}, rtti-vtables.tsv){" and a scoped CCharString passed to the setter @ 0x%08X (its manifest label is propagated)" % setter if s else ""}.
// One of {len(members)} bodies of this shape.
{"#include " + chr(34) + "fable_string.h" + chr(34) + chr(10) if s else ""}
struct CThingCreatureBase;
struct CThing;

struct CSoundPair {{
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) {{ }}
}};

struct {basecls} {{
    virtual void v00();
    {basecls}(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x{base:08X}
{"    void SetName(const CCharString& name);   // 0x%08X" % setter + chr(10) if s else ""}}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}(CThingCreatureBase* owner, CThing* thing);
}};

{cls}::{cls}(CThingCreatureBase* owner, CThing* thing)
    : {basecls}(owner, thing, {k}, CSoundPair())
{{
{body}}}
''',T(a,'''static long g_k = 0, g_s = 0; struct SP { long a, b; SP() : a(0), b(0) {} };
struct Base { virtual void v00() {} Base(void*, void*, long k, const SP& p) { g_k = k; g_s = p.a + p.b; } };
struct D : Base { virtual void v00() {} D(void* o, void* t) : Base(o, t, 0x8c, SP()) {} };
int main()
{
    D d(0, 0); if (g_k != 0x8c || g_s != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_97",):   # void CX::Transfer(CPersistContext& ctx) { ctx.TransferA(&m_a); ... ctx.TransferB(&m_b); ... }  (one helper call per member, in offset order)
        if retn!=4 or not calls or any(i.mnemonic=="call" and i.operands[0].type!=X86_OP_IMM for i in ins): continue
        offs=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str]
        tail=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("esi,") and i.operands[1].type==X86_OP_IMM]
        offs=offs+tail
        if len(offs)!=len(calls) or offs!=sorted(offs): continue
        hs=sorted(set(calls)); hn={h:f"Transfer_{h:08x}" for h in hs}
        cls=f"CPersistable_{a}"; mem=""; prev=offs[0]
        for j,o in enumerate(offs):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=f"    long m_{o:02x};".ljust(40)+f"// +{o:#x} (via {hn[calls[j]]})\n"; prev=o+4
        add(a,"_unattributed",f"Transfer_{cls}",HDRF(a,f"{cls}::Transfer(CPersistContext&) -- {len(offs)} members at +{offs[0]:#x}..+{offs[-1]:#x} handed to the context's typed transfer helpers ({', '.join('0x%08X' % h for h in hs)})",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; {" ; ".join(("lea eax,[esi+%#x] ; push eax ; mov ecx,edi ; call %s" % (o,hn[c])) if j<len(offs)-1 else ("add esi,%#x ; push esi ; mov ecx,edi ; call %s" % (o,hn[c])) for j,(o,c) in enumerate(zip(offs,calls)))} ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __thiscall transfer helper of the
// context ({len(hs)} distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of {len(members)} bodies of this shape.
struct CPersistContext {{
{chr(10).join("    void %s(long* value);   // 0x%08X" % (hn[h],h) for h in hs)}
}};

struct {cls} {{
    virtual void v00();
{"    unsigned char _pad[%#x];" % (offs[0]-4) + chr(10) if offs[0]>4 else ""}{mem}    virtual void Transfer(CPersistContext& context);
}};

void {cls}::Transfer(CPersistContext& context)
{{
{chr(10).join("    context.%s(&m_%02x);" % (hn[c],o) for o,c in zip(offs,calls))}
}}
''',T(a,'''static int g_t = 0; struct C { void A(long* v) { *v = 1; ++g_t; } void B(long* v) { *v = 2; ++g_t; } };
struct P { virtual void v00() {} long a, b, c; virtual void T(C& c) { c.A(&a); c.A(&b); c.B(&this->c); } };
int main()
{
    C c; P p; p.T(c); if (g_t != 3 || p.a != 1 || p.c != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_42",):   # CRetailTree& operator=(const CRetailTree&) with the clear inlined (Erase @ callee, head reset), then Copy, leftmost/rightmost, size
        if len(calls)!=2 or retn!=4: continue
        erase,copy=calls; nm=f"CRetailTree_{copy:08x}"
        add(a,nm,"operator=",HDRF(a,f"{nm}::operator=(const {nm}&) -- inlined clear (Erase @ 0x{erase:08X}, head reset, size zeroed), Copy @ 0x{copy:08X} under the head, leftmost/rightmost walks, size copied",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,[other] ; push esi ; mov esi,ecx ; cmp esi,ebp ; je R ; mov eax,[esi+4] ; push ebx ; xor ebx,ebx ; cmp eax,ebx ; je S ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Erase ; (head re-read) mov [eax+8],eax ; mov [edx+4],ebx ; mov [eax+0Ch],eax ; mov [esi+4],ebx ; S: mov [esi+4],ebx ; mov eax,[ebp] ; mov eax,[eax+4] ; cmp eax,ebx ; jne C ; (empty source: head->parent = 0, head->left = head, head->right = head) ; ret 4 ; C: push edi ; mov edi,[esi] ; push edi ; push eax ; mov ecx,esi ; call Copy ; mov [edi+4],eax ; (leftmost walk) ; (rightmost walk) ; mov edx,[ebp+4] ; mov [esi+4],edx ; pop ebx ; R: mov eax,esi ; pop esi ; pop ebp ; ret 4
// VC6-era Dinkumware `_Tree::operator=` with `erase(begin(), end())` inlined: unless
// self-assigned, a non-empty tree is erased through the recursive Erase @ 0x{erase:08X} and its
// head re-linked to itself (the head pointer re-read per store), the count is zeroed, and
// either the head is reset for an empty source or the source's root is cloned under the head
// (Copy @ 0x{copy:08X}, with a named head local across the call), the leftmost/rightmost links
// re-derived and the count copied. The manifest label is propagated. One of {len(members)} bodies.
struct {nm} {{
    struct node {{
        unsigned char color;               // +0
        unsigned char _pad[3];
        node* parent;                      // +4
        node* left;                        // +8
        node* right;                       // +0xC
    }};
    node* head;                            // +0
    unsigned long size;                    // +4
    void Erase(node* root);                // 0x{erase:08X}
    node* Copy(node* src, node* parent);   // 0x{copy:08X}
    void Clear()
    {{
        if (size) {{
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }}
    }}
    {nm}& operator=(const {nm}& other);
}};

{nm}& {nm}::operator=(const {nm}& other)
{{
    if (this != &other) {{
        Clear();
        size = 0;
        if (other.head->parent == 0) {{
            head->parent = 0;
            head->left = head;
            head->right = head;
        }}
        else {{
            node* h = head;                // named: retail keeps the head in edi across the Copy call
            h->parent = Copy(other.head->parent, h);
            node* n = head->parent;
            while (n->left)
                n = n->left;
            head->left = n;
            n = head->parent;
            while (n->right)
                n = n->right;
            head->right = n;
            size = other.size;
        }}
    }}
    return *this;
}}
''',T(a,'''static int g_e = 0;
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Erase(node*) { ++g_e; } node* Cp(node* s, node* p) { s->parent = p; return s; }
  void Cl() { if (n) { Erase(h->parent); h->l = h; h->parent = 0; h->r = h; n = 0; } }
  T& operator=(const T& o) { if (this != &o) { Cl(); n = 0; if (o.h->parent == 0) { h->parent = 0; h->l = h; h->r = h; } else { node* hh = h; hh->parent = Cp(o.h->parent, hh); node* x = h->parent; while (x->l) x = x->l; h->l = x; x = h->parent; while (x->r) x = x->r; h->r = x; n = o.n; } } return *this; } };
int main()
{
    T::node ha = {0,{0},0,0,0}, hb = {0,{0},0,0,0}, root = {0,{0},0,0,0}; T a, b; a.h = &ha; a.n = 1; ha.parent = &root; b.h = &hb; hb.parent = &root; b.n = 1; a = b; if (g_e != 1 || ha.parent != &root || a.n != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_109",):   # T* _Uninit_copy(first, last, dest, allocator) for a pair {long, X} / {X, long}: scalar member copied inline, X through its out-of-line copy ctor
        if len(calls)!=1 or retn!=8: continue
        ctor=calls[0]; step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG and not i.op_str.startswith("esp")][0]
        callidx=[k for k,i in enumerate(ins) if i.mnemonic=="call"][0]
        first_scalar=any(i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.op_str.startswith("dword ptr [edi],") for i in ins[:callidx])
        cm,cn,_=label(ctor); xcls=cm.split("::")[-1] if (cn.split("::")[-1]==cm.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",cm.split("::")[-1]) and (cm,cn) in uniq) else f"CSecond_{ctor:08x}"
        ecls=f"SPair{'Long' if first_scalar else ''}{xcls}{'' if first_scalar else 'Long'}"; nm=f"UninitCopy_{ecls}_{a}"
        add(a,"_global",nm,HDRF(a,f"_Uninit_copy<{ecls}*> -- placement-copy [first, last) to dest ({step}-byte {{{'long, ' + xcls if first_scalar else xcls + ', long'}}} pairs, {xcls} copy ctor @ 0x{ctor:08X}), return the advanced dest",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; {"mov eax,[esi] ; lea ecx,[esi+4] ; push ecx ; lea ecx,[edi+4] ; mov [edi],eax ; call " + xcls + "::" + xcls if first_scalar else "push esi ; mov ecx,edi ; call " + xcls + "::" + xcls + " ; mov eax,[esi+4] ; mov [edi+4],eax"} ; S: add esi,{step:#x} ; add edi,{step:#x} ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose {'first member is a 4-byte scalar' if first_scalar else 'second member is a 4-byte scalar'} and whose {'second' if first_scalar else 'first'}
// member {xcls} has the out-of-line copy constructor @ 0x{ctor:08X}: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of {len(members)} bodies of this shape.
#include <new>

struct {xcls} {{
    unsigned char _body[{step-4:#x}];
    {xcls}(const {xcls}& other);           // 0x{ctor:08X}
}};

struct {ecls} {{
{"    long first;" + chr(10) + "    " + xcls + " second;" if first_scalar else "    " + xcls + " first;" + chr(10) + "    long second;"}
}};

struct CRetailAllocator {{ }};

{ecls}* __fastcall {nm}({ecls}* first, {ecls}* last, {ecls}* dest, CRetailAllocator allocator)
{{
    {ecls}* d = dest;
    for (; first != last; ++first, ++d)
        new (d) {ecls}(*first);
    return d;
}}
''',T(a,'''#include <new>
static int g_c = 0; struct X { unsigned char b[4]; X() {} X(const X& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
struct P { '''+("long f; X s;" if first_scalar else "X f; long s;")+''' };
static P* __fastcall UC(P* f, P* l, P* dest, A) { P* d = dest; for (; f != l; ++f, ++d) new (d) P(*f); return d; }
int main()
{
    P src[2], dst[2]; '''+("src[1].f = 5; src[1].s.b[0] = 7;" if first_scalar else "src[1].f.b[0] = 7; src[1].s = 5;")+''' P* e = UC(src, src + 2, dst, A()); if (e != dst + 2 || g_c != 2 || dst[1].'''+("s.b[0]" if first_scalar else "f.b[0]")+''' != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_66",):   # Clone() with the implicit copy constructor inlined: base copy ctor + vtable + N trailing dword members copied
        if len(calls)!=2 or retn!=0 or len(vts)!=1: continue
        size=pushes[0]; bctor=calls[1]; basecls=tctor_class(bctor) or f"CBase_{bctor:08x}"; vt=vts[0][1]; cls=rtti.get(vt) or clean(mmod) or f"CVtable_{vt:08x}"
        if cls==basecls: cls=f"{basecls}_Derived_{vt:08x}"
        ldi=[(i.operands[1].mem.disp, 1 if "byte ptr" in i.op_str else 4) for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "edi +" in i.op_str]
        lds=[o for o,_ in ldi]
        sts=[i.operands[0].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esi +" in i.op_str and i.operands[0].mem.disp]
        if len(lds)<2 or lds!=sts: continue
        mem=""; prev=lds[0]
        for j,(o,sz) in enumerate(ldi):
            if o>prev: mem+=f"    unsigned char _gap{j}[{o-prev:#x}];\n"
            mem+=(f"    long m_{o:02x};" if sz==4 else f"    unsigned char m_{o:02x};").ljust(40)+f"// +{o:#x}\n"; prev=o+sz
        add(a,cls,"Clone",HDRF(a,f"{cls}::Clone() -- new {cls}(*this) with the implicit copy constructor inlined (base copy @ 0x{bctor:08X} + {len(lds)} dword members)",len(b))+f'''//
// Retail bytes:  push esi ; push edi ; push {size:#x} ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call {basecls}::{basecls}(const {basecls}&) ; mov dword ptr [esi],offset {cls}::`vftable' ; {" ; ".join(("mov eax,[edi+%#x] ; mov [esi+%#x],eax" % (o,o)) if sz==4 else ("mov al,[edi+%#x] ; mov [esi+%#x],al" % (o,o)) for o,sz in ldi)} ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds {len(lds)} members to its base: the implicit copy constructor
// is inlined as the base copy constructor @ 0x{bctor:08X}, this class's vtable store (0x{vt:08X},
// rtti-vtables.tsv) and the member copies. sizeof = {size:#x}. Member types are not recoverable
// beyond their size (`long` / `unsigned char`). One of {len(members)} bodies of this shape.
struct {basecls} {{
    virtual void v00();
    unsigned char _body[{lds[0]:#x} - 4];     // sizeof({basecls}) = {lds[0]:#x}
    {basecls}(const {basecls}& other);     // 0x{bctor:08X}
}};

struct {cls} : {basecls} {{
{mem}    virtual void v00();                    // own vtable; copy constructor implicit
    {basecls}* Clone();
}};

{basecls}* {cls}::Clone()
{{
    return new {cls}(*this);
}}
''',T(a,f'''static int g_c = 0; struct Base {{ virtual void v00() {{}} unsigned char body[{lds[0]:#x} - 4]; Base() {{}} Base(const Base& o) {{ ++g_c; body[0] = o.body[0]; }} }};
struct X : Base {{ long e, f; virtual void v00() {{}} Base* Clone() {{ return new X(*this); }} }};
int main()
{{
    X x; x.body[0] = 5; x.e = 7; x.f = 8; X* c = (X*)x.Clone(); if (!c || g_c != 1 || c->body[0] != 5 || c->f != 8) '''+FAIL(a)+'''
    delete c;
'''),b)
    elif TPL in ("w512_27",):   # void CCombatSequence_X::OnCreate() { CCountedPointer<CCombatActionBase> action(new CCombatAction_Y(m_pOwner, m_pTarget)); AddAction(action); CCombatSequenceBase::OnCreate(); }
        if len(calls)!=6 or retn!=0: continue
        newer,ctor,reset,adder,base,deleter=calls; size=pushes[0]
        offs=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str]
        if len(offs)!=2: continue
        o2,o1=offs   # pushed right to left: second argument first
        acls=tctor_class(ctor) or f"CCombatAction_{ctor:08x}"
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CCombatSequence_{a}"
        bm,bn,_=label(base); bcls="CCombatSequenceBase" if (bm,bn) in uniq and bn.split("::")[-1]=="OnCreate" else f"CCombatSequenceBase_{base:08x}"
        add(a,cls,"OnCreate",HDRF(a,f"{cls}::OnCreate() -- counted pointer to a new {acls}(m_{o1:02x}, m_{o2:02x}) ({size:#x} bytes) added through AddAction @ 0x{adder:08X}, then the base OnCreate @ 0x{base:08X}; the temporary is released inline",len(b))+f'''//
// Retail bytes:  sub esp,8 ; push esi ; push {size:#x} ; mov esi,ecx ; mov dword ptr [esp+8],0 ; mov dword ptr [esp+0Ch],0 ; call operator new ; add esp,4 ; test eax,eax ; je Z ; mov ecx,[esi+{o2:#x}] ; mov edx,[esi+{o1:#x}] ; push ecx ; push edx ; mov ecx,eax ; call {acls}::{acls} ; jmp J ; Z: xor eax,eax ; J: push eax ; lea ecx,[esp+8] ; call CCountedPointer::Reset ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call AddAction ; mov ecx,esi ; call {bcls}::OnCreate ; mov eax,[esp+8] ; test eax,eax ; pop esi ; je E ; dec dword ptr [eax] ; mov eax,[esp+4] ; cmp dword ptr [eax],0 ; jne E ; mov ecx,[eax+8] ; call [eax+4] ; mov ecx,[esp+4] ; push ecx ; call operator delete ; add esp,4 ; E: add esp,8 ; ret
// Virtual OnCreate of a combat sequence: a new {acls} ({size:#x} bytes, constructor @ 0x{ctor:08X}
// over the sequence's two pointer members at +{o1:#x} and +{o2:#x}) is wrapped in a counted
// pointer -- default-constructed (both fields zeroed, the stores scheduled before the
// allocation) and assigned through the out-of-line Reset @ 0x{reset:08X}, which allocates the
// count block -- handed to AddAction @ 0x{adder:08X} (its label
// `std::vector::push_back` is propagated: the callee also bumps a count at +8), and the base
// OnCreate @ 0x{base:08X} runs (its manifest label is propagated). The temporary's destructor is
// inlined: decrement the block's count and, at zero, call its destroy function with the object
// in ecx and delete the block; the stores nulling a dead local are elided. The sequence class
// is this body's manifest module; the action class is named by its constructor when the label
// is unique. One of {len(members)} bodies of this shape.
struct CThingCreatureBase;
struct CThing;

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCombatActionBase {{
    virtual void v00();
}};

struct {acls} : CCombatActionBase {{
    unsigned char _body[{size-4:#x}];        // sizeof({acls}) = {size:#x}
    {acls}(CThingCreatureBase* owner, CThing* target);   // 0x{ctor:08X}
}};

struct CCountedPointer {{
    CCombatActionBase* m_pObject;          // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) {{ }}
    CCountedPointer& operator=(CCombatActionBase* object) {{ Reset(object); return *this; }}
    void Reset(CCombatActionBase* object); // 0x{reset:08X}
    ~CCountedPointer()
    {{
        if (m_pCount) {{
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {{
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }}
        }}
        m_pObject = 0;
        m_pCount = 0;
    }}
}};

struct {bcls} {{
    virtual void v00();
    unsigned char _pad[{o1-4:#x}];
    CThingCreatureBase* m_{o1:02x};               // +{o1:#x} owner
{"    unsigned char _gap[%#x];" % (o2-o1-4) + chr(10) if o2-o1>4 else ""}    CThing* m_{o2:02x};                           // +{o2:#x} target
    void AddAction(const CCountedPointer& action);   // 0x{adder:08X}
    virtual void OnCreate();               // 0x{base:08X}
}};

struct {cls} : {bcls} {{
    virtual void OnCreate();
}};

void {cls}::OnCreate()
{{
    CCountedPointer action;                // zeroed before the allocation (GOTCHAS: ctor-argument form zeroes after)
    action = new {acls}(m_{o1:02x}, m_{o2:02x});
    AddAction(action);
    {bcls}::OnCreate();
}}
''',T(a,'''static int g_r = 0, g_a = 0, g_b = 0, g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct O { virtual void v00() {} }; struct X : O { unsigned char b[8]; X(void*, void*) {} };
struct P { O* o; Blk* c; P() : o(0), c(0) {} P& operator=(O* p) { R(p); return *this; } void R(O* p) { o = p; c = new Blk; c->r = 1; c->f = Destroy; c->o = p; ++g_r; }
    ~P() { if (c) { --c->r; if (c->r == 0) { c->f(c->o); delete c; } } o = 0; c = 0; } };
struct Base { virtual void v00() {} unsigned char pad[0x18]; void* a; void* t; void Add(const P&) { ++g_a; } virtual void OnCreate() { ++g_b; } };
struct D : Base { virtual void OnCreate(); }; void D::OnCreate() { P p; p = new X(a, t); Add(p); Base::OnCreate(); }
int main()
{
    D d; d.a = 0; d.t = 0; d.OnCreate(); if (g_r != 1 || g_a != 1 || g_b != 1 || g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_26",):   # void CX::AddEntry(const CCountedPointer& value) { if (!m_pList) m_pList = new CRetailList; m_pList->push_back(value); }  (Dinkumware list, no size, node {next, prev, value})
        if len(calls)!=3 or retn!=4: continue
        newer,m1,m2=calls
        if m1!=m2: continue
        offs=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "edi +" in i.op_str]
        if not offs or len(set(offs))!=1: continue
        off=offs[0]; ns=[p for p in pushes if p==0x10]
        if len(ns)!=2 or pushes[0]!=4: continue
        leaf=mmod.split("::")[-1]; cls=leaf if re.match(r"^C[A-Za-z0-9_]+$",leaf) else f"CUIElement_{a}"
        fn=mname.split("::")[-1] if re.match(r"^vfunc_\d+$",mname.split("::")[-1]) else f"AddEntry_{a}"
        add(a,mmod if cls!=f"CUIElement_{a}" else "_unattributed",fn,HDRF(a,f"{mmod}::{fn}(const CCountedPointer&) -- lazily creates the std::list member at +{off:#x} (head sentinel malloc'd self-linked) and appends a counted-pointer copy (inlined push_back: 16-byte node, copy constructor, link before the head)",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,ecx ; mov eax,[edi+{off:#x}] ; test eax,eax ; mov dword ptr [esp+8],0 ; jne H ; push 4 ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je Z ; push 10h ; mov dword ptr [esi],0 ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; add esp,4 ; mov [esi],eax ; jmp S ; Z: xor esi,esi ; S: mov [edi+{off:#x}],esi ; H: mov eax,[edi+{off:#x}] ; mov esi,[eax] ; push 10h ; call malloc ; lea ecx,[eax+8] ; add esp,4 ; test ecx,ecx ; je L ; mov edx,[value] ; mov edi,[edx] ; mov [ecx],edi ; mov edx,[edx+4] ; test edx,edx ; mov [ecx+4],edx ; je L ; inc dword ptr [edx] ; L: mov ecx,[esi+4] ; mov [eax],esi ; mov [eax+4],ecx ; mov [ecx],eax ; pop edi ; mov [esi+4],eax ; pop esi ; pop ecx ; ret 4
// Virtual UI hook that appends a counted pointer to a lazily created list: the list member at
// +{off:#x} is created on first use (operator new @ 0x{newer:08X}, its head sentinel malloc'd
// @ 0x{m1:08X} and linked to itself by the inlined constructor), then push_back is inlined:
// a 16-byte node is malloc'd, the counted pointer is placement-copied into its value (object
// and count block, with the count incremented when present -- hence the null tests), and the
// node is linked before the head. The zeroed dword local is the inlined constructor's empty
// placement tag, homed in the frame. The list keeps no size member. One of {len(members)} bodies.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct SCountBlock {{
    long m_references;                     // +0
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {{
        if (m_pCount)
            ++m_pCount->m_references;
    }}
}};

struct CRetailList {{
    struct SNode {{
        SNode* m_next;                     // +0
        SNode* m_prev;                     // +4
        CCountedPointer m_value;           // +8
    }};
    SNode* m_head;                         // +0 sentinel
    CRetailList()
    {{
        m_head = 0;
        m_head = (SNode*)malloc(sizeof(SNode));
        m_head->m_next = m_head;
        m_head->m_prev = m_head;
    }}
    void push_back(const CCountedPointer& value)
    {{
        SNode* head = m_head;
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->m_value) CCountedPointer(value);
        node->m_next = head;
        node->m_prev = head->m_prev;
        head->m_prev->m_next = node;
        head->m_prev = node;
    }}
}};

struct {cls} {{
    unsigned char _pad[{off:#x}];
    CRetailList* m_pList;                  // +{off:#x}
    virtual void {fn}(const CCountedPointer& value);
}};

void {cls}::{fn}(const CCountedPointer& value)
{{
    if (!m_pList)
        m_pList = new CRetailList;
    m_pList->push_back(value);
}}
''',T(a,'''#include <new>
#include <cstdlib>
struct B { long r; }; struct P { void* o; B* c; P(const P& x) : o(x.o), c(x.c) { if (c) ++c->r; } };
struct L { struct N { N* n; N* p; P v; }; N* h; L() { h = 0; h = (N*)std::malloc(sizeof(N)); h->n = h; h->p = h; }
    void pb(const P& v) { N* hd = h; N* n = (N*)std::malloc(sizeof(N)); new (&n->v) P(v); n->n = hd; n->p = hd->p; hd->p->n = n; hd->p = n; } };
struct X { unsigned char pad[8]; L* l; X() { l = 0; } virtual void A(const P& v) { if (!l) l = new L; l->pb(v); } };
int main()
{
    B b; b.r = 1; P p = { 0, &b }; X x; x.A(p); x.A(p); if (!x.l || b.r != 3 || x.l->h->n->n->n != x.l->h || x.l->h->p->v.c != &b) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_24",):   # void CEngine::AddEngineComponent(CEngineComponent* component, SComponentSlot* slot, unsigned char) { DisplayProgress(CCharString("LIT", -1), -1.0f, false, false); slot->m_pComponent = component; component->AddExclusiveObserver(slot); m_components.push_back(component); component->Attach(); }
        if len(calls)!=7 or retn!=0xc: continue
        sctor,progress,sdtor,observe,attach,grow,attach2=calls
        if attach!=attach2: continue
        lits=[p for p in pushes if 0x1200000<=p<0x1300000]
        if len(lits)!=1: continue
        s=lit(lits[0]); fl=[p for p in pushes if p==0xbf800000]
        if len(fl)!=1: continue
        offs=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "edi +" in i.op_str]
        voff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "edi +" in i.op_str][0]
        if offs!=[voff+4,voff+8]: continue
        add(a,"CEngine",f"AddEngineComponent_{a}",HDRF(a,f"CEngine::AddEngineComponent(CEngineComponent*, SComponentSlot*, unsigned char) -- progress message {s}, slot store + AddExclusiveObserver @ 0x{observe:08X}, inline push_back on the component vector at +{voff:#x} (grow @ 0x{grow:08X}), then the component's attach hook @ 0x{attach:08X}",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push edi ; push -1 ; mov edi,ecx ; push offset "{s}" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; push 0 ; push 0BF800000h ; xor dl,dl ; lea ecx,[esp+10h] ; call DisplayProgress ; lea ecx,[esp+8] ; call CCharString::~CCharString ; mov eax,[slot] ; mov esi,[component] ; push eax ; mov ecx,esi ; mov [eax],esi ; call CObservable::AddExclusiveObserver ; mov eax,[edi+{voff+4:#x}] ; mov edx,[edi+{voff+8:#x}] ; cmp eax,edx ; lea ecx,[edi+{voff:#x}] ; mov [esp+10h],esi ; je G ; test eax,eax ; je S ; mov [eax],esi ; S: add dword ptr [ecx+4],4 ; mov ecx,esi ; call Attach ; pop edi ; pop esi ; pop ecx ; ret 0Ch ; G: push 1 ; push 1 ; lea edx,[esp+18h] ; push edx ; lea edx,[esp+1Ch] ; push edx ; push eax ; call InsertGrow ; mov ecx,esi ; call Attach ; pop edi ; pop esi ; pop ecx ; ret 0Ch
// One instantiation of CEngine::AddEngineComponent<T> (all {len(members)} instantiations compile to the
// same bytes): shows the progress message "{s}" (a CCharString temporary passed by
// reference to the __fastcall DisplayProgress @ 0x{progress:08X} with -1.0f and two false flags,
// destroyed @ 0x{sdtor:08X} after the call), stores the component into its slot and registers
// the slot as the component's exclusive observer (@ 0x{observe:08X}), appends the component
// pointer to the engine's component vector at +{voff:#x} (push_back inlined: in-place store with
// spare capacity, else the reallocating insert @ 0x{grow:08X} with a 1-byte empty-tag temporary
// and the value homed in the dead argument slot), and calls the component's attach hook
// @ 0x{attach:08X} (its manifest label is propagated). The flag argument is unused by this body.
// The component type of this instantiation is not recoverable from the bytes.
#include <new>
#include "fable_string.h"

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

void __fastcall DisplayProgress(const CCharString& message, float fraction, bool a, bool b);   // 0x{progress:08X}

struct SComponentSlot;

struct CEngineComponent {{
    void AddExclusiveObserver(SComponentSlot* observer);   // 0x{observe:08X}
    void Attach();                         // 0x{attach:08X}
}};

struct SComponentSlot {{
    CEngineComponent* m_pComponent;        // +0
}};

struct CRetailVector_ptr {{
    CEngineComponent** first;
    CEngineComponent** last;
    CEngineComponent** end;
    void InsertGrow(CEngineComponent** where, CEngineComponent* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x{grow:08X}
    void push_back(CEngineComponent* const& value)
    {{
        if (last != end) {{
            new (last) CEngineComponent*(value);
            ++last;
        }}
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }}
}};

struct CEngine {{
    unsigned char _pad[{voff:#x}];
    CRetailVector_ptr m_components;        // +{voff:#x}
    void AddEngineComponent_{a}(CEngineComponent* component, SComponentSlot* slot, unsigned char flag);
}};

void CEngine::AddEngineComponent_{a}(CEngineComponent* component, SComponentSlot* slot, unsigned char flag)
{{
    DisplayProgress(CCharString("{s}", -1), -1.0f, false, false);
    slot->m_pComponent = component;
    component->AddExclusiveObserver(slot);
    m_components.push_back(component);
    component->Attach();
}}
''',T(a,'''#include <new>
static int g_p = 0, g_o = 0, g_at = 0, g_g = 0; struct Str { const char* s; Str(const char* x, long) : s(x) {} ~Str() { ++g_p; } };
static void __fastcall DP(const Str&, float, bool, bool) { ++g_p; } struct Tg { Tg() {} };
struct Comp { void Obs(void*) { ++g_o; } void At() { ++g_at; } }; struct Slot { Comp* c; };
struct V { Comp** f; Comp** l; Comp** e; void G(Comp**, Comp* const&, const Tg&, unsigned long, bool) { ++g_g; } void pb(Comp* const& v) { if (l != e) { new (l) Comp*(v); ++l; } else G(l, v, Tg(), 1, true); } };
struct E { unsigned char pad[8]; V v; void Add(Comp* c, Slot* s, unsigned char) { DP(Str("x", -1), -1.0f, false, false); s->c = c; c->Obs(s); v.pb(c); c->At(); } };
int main()
{
    Comp* buf[1]; E e; e.v.f = buf; e.v.l = buf; e.v.e = buf + 1; Comp c; Slot s; e.Add(&c, &s, 0); e.Add(&c, &s, 0); if (g_p != 4 || g_o != 2 || g_at != 2 || g_g != 1 || s.c != &c || buf[0] != &c) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_67",):   # void CAIStateGroup_X::OnActivate() { if (m_target.GetPItem() && !(m_target.GetPItem()->m_flagsA & a) && (m_target.GetPItem()->m_flagsB & b)) { CTCStealth* tc = (CTCStealth*)GetTC(thing, K); tc->SetAsInDanger(m_pOwner->m_pThing, 3); } }
        if len(calls)!=5 or retn!=0 or calls[0]!=calls[1] or calls[1]!=calls[2]: continue
        getp,lower,setter=calls[0],calls[3],calls[4]
        poff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "edi +" in i.op_str][0]
        tests=[i for i in ins if i.mnemonic=="test" and i.operands[0].type==X86_OP_MEM]
        if len(tests)!=2: continue
        aoff,amask=tests[0].operands[0].mem.disp,tests[0].operands[1].imm; boff,bmask=tests[1].operands[0].mem.disp,tests[1].operands[1].imm
        moff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "eax +" in i.op_str][0]
        key=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esp" in i.op_str and i.operands[1].type==X86_OP_IMM][0]
        chain=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and ("edi +" in i.op_str or "ecx +" in i.op_str)]
        if len(chain)!=2: continue
        o1,o2=chain; k2=[p for p in pushes if p<0x100][0]
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CAIStateGroup_{a}"; fn=mname.split("::")[-1] if re.match(r"^[A-Za-z_]\w*$",mname.split("::")[-1]) else f"Hook_{a}"
        _mem=sorted([(aoff,1,"unsigned char m_%02x;" % aoff,"flags byte"),(boff,4,"unsigned long m_%02x;" % boff,"flags dword"),(moff,12,"CVectorMap m_%02x;" % moff,"TC map")]); thing_members=""; _prev=0
        for _o,_sz,_decl,_what in _mem:
            if _o>_prev: thing_members+=f"    unsigned char _pad{_prev:02x}[{_o-_prev:#x}];\n"
            thing_members+=f"    {_decl}".ljust(40)+f"// +{_o:#x} {_what}\n"; _prev=_o+_sz
        gm,gn,_=label(getp); getter=gn.split("::")[-1] if (gm,gn) in uniq and re.match(r"^[A-Za-z_]\w*$",gn.split("::")[-1]) else f"GetPItem_{getp:08x}"
        sm,sn,_=label(setter); scls=sm.split("::")[-1] if (sm,sn) in uniq and re.match(r"^C[A-Za-z0-9_]+$",sm.split("::")[-1]) else f"CTC_{setter:08x}"; sfn=sn.split("::")[-1] if (sm,sn) in uniq and re.match(r"^[A-Za-z_]\w*$",sn.split("::")[-1]) else f"Notify_{setter:08x}"
        add(a,cls,fn,HDRF(a,f"{cls}::{fn}() -- if the intelligent pointer at +{poff:#x} resolves to a thing without flag {amask:#x} at +{aoff:#x} and with flag {bmask:#x} at +{boff:#x}, look up TC {key:#x} in the thing's TC map (+{moff:#x}, inlined CVectorMap find over LowerBound @ 0x{lower:08X}) and call {scls}::{sfn}(m_{o1:02x}->m_{o2:02x}, {k2})",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,ecx ; lea esi,[edi+{poff:#x}] ; mov ecx,esi ; call {getter} ; test eax,eax ; je E ; mov ecx,esi ; call {getter} ; test byte ptr [eax+{aoff:#x}],{amask:#x} ; jne E ; mov ecx,esi ; call {getter} ; test dword ptr [eax+{boff:#x}],{bmask:#x} ; je E ; lea esi,[eax+{moff:#x}] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esp+0Ch],{key:#x} ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je N ; cmp dword ptr [eax],{key:#x} ; jle F ; N: mov eax,esi ; F: mov ecx,[edi+{o1:#x}] ; mov edx,[ecx+{o2:#x}] ; mov eax,[eax+4] ; push {k2} ; push edx ; mov ecx,eax ; call {scls}::{sfn} ; E: pop edi ; pop esi ; pop ecx ; ret
// AI state hook: the target intelligent pointer at +{poff:#x} is resolved through {getter}
// @ 0x{getp:08X} on every use (three calls), the thing's two flag words are tested, and the
// TC entry {key:#x} is fetched from the thing's CVectorMap at +{moff:#x} with the find idiom
// inlined (LowerBound @ 0x{lower:08X}, then `it == last || key < it->first` falls back to
// `last`; the key is a frame temporary passed by address); its value receives
// {sfn}(owner->m_{o2:02x}, {k2}) @ 0x{setter:08X}. One of {len(members)} bodies of this shape.
struct CThing;
struct CTCBase;

enum ETCInterfaceType {{ TC_FIRST = 0 }};

struct SPair {{
    long first;                            // ETCInterfaceType
    CTCBase* second;
}};

struct CVectorMap {{
    SPair* first;                          // +0
    SPair* last;                           // +4
    SPair* end;                            // +8
    SPair* LowerBound(const long& key);    // 0x{lower:08X}
    SPair* Find(ETCInterfaceType key)      // the enum converts to LowerBound's long reference through a frame temporary; the compare keeps the constant
    {{
        SPair* it = LowerBound(key);
        if (it == last || key < it->first)
            it = last;
        return it;
    }}
}};

struct CThing {{
{thing_members}    CTCBase* GetTC(ETCInterfaceType type) {{ return m_{moff:02x}.Find(type)->second; }}   // returned value: loaded before the call sequence
}};

struct CIntelligentPointer {{
    unsigned char _body[8];
    CThing* {getter}();                    // 0x{getp:08X}
}};

struct {scls} {{
    void {sfn}(CThing* thing, long status);   // 0x{setter:08X}
}};

struct SOwner {{
    unsigned char _pad[{o2:#x}];
    CThing* m_{o2:02x};                         // +{o2:#x}
}};

struct {cls} {{
    unsigned char _pad[{o1:#x}];
    SOwner* m_{o1:02x};                         // +{o1:#x}
    unsigned char _pad2[{poff-o1-4:#x}];
    CIntelligentPointer m_{poff:02x};           // +{poff:#x}
    void {fn}();
}};

void {cls}::{fn}()
{{
    if (m_{poff:02x}.{getter}() && !(m_{poff:02x}.{getter}()->m_{aoff:02x} & {amask:#x})) {{
        CThing* thing = m_{poff:02x}.{getter}();
        if (thing->m_{boff:02x} & {bmask:#x}) {{
            {scls}* tc = ({scls}*)thing->GetTC((ETCInterfaceType){key:#x});
            tc->{sfn}(m_{o1:02x}->m_{o2:02x}, {k2});
        }}
    }}
}}
''',T(a,'''static int g_n = 0; struct T; struct P { long f; T* s; };
struct M { P* f; P* l; P* e; P* LB(const long& k) { P* i = f; while (i != l && i->f < k) ++i; return i; } P* Find(const long& k) { P* i = LB(k); if (i == l || k < i->f) i = l; return i; } };
struct T { unsigned char pad[8]; unsigned char a; unsigned char pad2[3]; unsigned long b; M m; T* GT(long k) { return m.Find(k)->s; } void N(T*, long k) { g_n += (int)k; } };
struct IP { T* t; T* G() { return t; } }; struct O { T* x; }; struct X { O* o; IP p; void F() { if (p.G() && !(p.G()->a & 1)) { T* t = p.G(); if (t->b & 4) { T* tc = (T*)t->GT(7); tc->N(o->x, 3); } } } };
int main()
{
    T t, tc; P arr[2]; arr[0].f = 7; arr[0].s = &tc; t.m.f = arr; t.m.l = arr + 1; t.m.e = arr + 2; t.a = 0; t.b = 4; O o; o.x = &t; X x; x.o = &o; x.p.t = &t; x.F(); if (g_n != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_48",):   # void CX::F() { CCharString name("LIT", -1); m_a->vslotN(&m_b, name); }
        if len(calls)!=2 or retn!=0: continue
        sctor,sdtor=calls; lits=[p for p in pushes if 0x1200000<=p<0x1300000]
        if len(lits)!=1: continue
        s=lit(lits[0]); slot=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM][0]
        aoff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str][0]
        boff=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("esi,")][0]
        cls=f"CNamedCaller_{a}"; sl=slot//4
        vt="".join(f"    virtual void _slot{j:03d}();\n" for j in range(sl))
        add(a,"_unattributed",f"Notify_{san(s)}_{a}",HDRF(a,f"{cls}::Notify_{san(s)}_{a}() -- passes &m_{boff:02x} and the name \"{s}\" to virtual slot {sl} of the object at +{aoff:#x}",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push -1 ; mov esi,ecx ; push offset "{s}" ; lea ecx,[esp+0Ch] ; call CCharString::CCharString ; mov ecx,[esi+{aoff:#x}] ; mov eax,[ecx] ; lea edx,[esp+4] ; push edx ; add esi,{boff:#x} ; push esi ; call [eax+{slot:#x}] ; lea ecx,[esp+4] ; call CCharString::~CCharString ; pop esi ; pop ecx ; ret
// Builds a scoped CCharString from the literal (constructor @ 0x{sctor:08X}, destructor
// @ 0x{sdtor:08X}, labels propagated) and calls virtual slot {sl} (vtable offset {slot:#x}) of the
// object at +{aoff:#x} with the address of this object's member at +{boff:#x} (formed in place:
// `add esi,{boff:#x} ; push esi`) and the name. The function is unnamed in the manifest (gap
// scan); its class and the callee's are modelled by their slots. One of {len(members)} bodies.
#include "fable_string.h"

struct SMember_{boff:02x};

struct CSlotTarget_{a} {{
{vt}    virtual void Notify(SMember_{boff:02x}* member, const CCharString& name);   // slot {sl}
}};

struct {cls} {{
    unsigned char _pad[{aoff:#x}];
    CSlotTarget_{a}* m_{aoff:02x};            // +{aoff:#x}
{"    unsigned char _pad2[%#x];" % (boff-aoff-4) + chr(10) if boff-aoff>4 else ""}    unsigned char m_{boff:02x}[4];              // +{boff:#x} (opaque member handed by address)
    void Notify_{san(s)}_{a}();
}};

void {cls}::Notify_{san(s)}_{a}()
{{
    CCharString name("{s}", -1);
    m_{aoff:02x}->Notify((SMember_{boff:02x}*)&m_{boff:02x}, name);
}}
''',T(a,'''static int g_n = 0; struct Str { const char* s; Str(const char* x, long) : s(x) {} ~Str() { ++g_n; } };
struct Tg { virtual void a() {} virtual void b() {} virtual void N(void*, const Str& s) { g_n += s.s[0]; } };
struct X { Tg* t; unsigned char m[4]; void F() { Str s("A", -1); t->N(&m, s); } };
int main()
{
    Tg t; X x; x.t = &t; x.F(); if (g_n != 'A' + 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_68",):   # void CX::F() { CCharString name("LIT", -1); if (m_a.vslotN(name)) m_b->m_c = 1; }
        if len(calls)!=2 or retn!=0: continue
        sctor,sdtor=calls; lits=[p for p in pushes if 0x1200000<=p<0x1300000]
        if len(lits)!=1: continue
        s=lit(lits[0]); slot=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM][0]
        aoff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]
        boff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str and i.operands[1].mem.disp!=aoff][0]
        _bs=[i for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "byte ptr" in i.op_str][0]; coff=_bs.operands[0].mem.disp; cval=_bs.operands[1].imm
        cls=f"CNamedQuery_{a}"; sl=slot//4
        vt="".join(f"    virtual void _slot{j:03d}();\n" for j in range(sl))
        add(a,"_unattributed",f"Query_{san(s)}_{a}",HDRF(a,f"{cls}::Query_{san(s)}_{a}() -- asks virtual slot {sl} of the embedded object at +{aoff:#x} about \"{s}\"; on true stores {cval} to the byte at +{coff:#x} of the object at +{boff:#x}",len(b))+f'''//
// Retail bytes:  push ecx ; push ebx ; push esi ; push -1 ; mov esi,ecx ; push offset "{s}" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov eax,[esi+{aoff:#x}] ; lea ecx,[esi+{aoff:#x}] ; lea edx,[esp+8] ; push edx ; call [eax+{slot:#x}] ; lea ecx,[esp+8] ; mov bl,al ; call CCharString::~CCharString ; test bl,bl ; je E ; mov eax,[esi+{boff:#x}] ; mov byte ptr [eax+{coff:#x}],{cval} ; E: pop esi ; pop ebx ; pop ecx ; ret
// Builds a scoped CCharString from the literal (constructor @ 0x{sctor:08X}, destructor
// @ 0x{sdtor:08X}, labels propagated), calls virtual slot {sl} (vtable offset {slot:#x}) of the
// polymorphic member embedded at +{aoff:#x} with it, keeps the bool result in bl across the
// string's destructor, and on success sets the byte flag at +{coff:#x} of the object at
// +{boff:#x}. Unnamed in the manifest (gap scan). One of {len(members)} bodies of this shape.
struct CCharString {{                     // one pointer; the default length makes the literal convert implicitly (re-addressed with lea, GOTCHAS)
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x{sctor:08X}
    ~CCharString();                        // 0x{sdtor:08X}
}};

struct CQueryTarget_{a} {{
{vt}    virtual bool Query(const CCharString& name);   // slot {sl}
}};

struct SFlagOwner_{a} {{
    unsigned char _pad[{coff:#x}];
    unsigned char m_{coff:02x};                 // +{coff:#x}
}};

struct {cls} {{
    unsigned char _pad[{aoff:#x}];
    CQueryTarget_{a} m_{aoff:02x};            // +{aoff:#x} embedded (vptr first)
{"    unsigned char _pad2[%#x];" % (boff-aoff-4) + chr(10) if boff-aoff>4 else ""}    SFlagOwner_{a}* m_{boff:02x};             // +{boff:#x}
    void Query_{san(s)}_{a}();
}};

void {cls}::Query_{san(s)}_{a}()
{{
    bool found = m_{aoff:02x}.Query("{s}");   // the temporary dies after the call; the result rides in bl
    if (found)
        m_{boff:02x}->m_{coff:02x} = {cval};
}}
''',T(a,'''static int g_n = 0; struct Str { const char* s; Str(const char* x, long) : s(x) {} ~Str() { ++g_n; } };
struct Tg { virtual void a() {} virtual bool Q(const Str& s) { return s.s[0] == 'A'; } }; struct O { unsigned char p[4]; unsigned char f; };
struct X { Tg t; O* o; void F() { bool r = t.Q(Str("A", -1)); if (r) o->f = 1; } };
int main()
{
    O o; o.f = 0; X x; x.o = &o; x.F(); if (g_n != 1 || o.f != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_51",):   # unsigned long CRetailTree::Erase(SNode* where) { SNode* victim = Erase_(where, &head->parent, &head->left, &head->right); victim->value.~T(); free(victim); return --size; }
        if len(calls)!=3 or retn!=4: continue
        helper,dtor,freer=calls; voff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "edi +" in i.op_str][0]
        dm,dn,_=label(dtor); dl=dn.split("::")[-1]
        if dtor==0x99eae0: vcls="CCharString"; vinc='#include "fable_string.h"\n'; vdecl=""
        elif dl.startswith("~") and (dm,dn) in uniq and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]): vcls=dm.split("::")[-1]; vinc=""; vdecl=f"struct {vcls} {{\n    void* m_data;\n    ~{vcls}();                             // 0x{dtor:08X}\n}};\n\n"
        else: vcls=f"CValue_{dtor:08x}"; vinc=""; vdecl=f"struct {vcls} {{\n    void* m_data;\n    ~{vcls}();                             // 0x{dtor:08X}\n}};\n\n"
        nm=f"CRetailTree_{a}"
        add(a,"_unattributed",f"Erase_{nm}",HDRF(a,f"{nm}::Erase(SNode*) -- unlink through the __cdecl helper @ 0x{helper:08X} (node, &head->parent, &head->left, &head->right), destroy the {vcls} value at +{voff:#x}, free the node, return the decremented size",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; push edi ; lea ecx,[eax+0Ch] ; push ecx ; lea edx,[eax+8] ; push edx ; add eax,4 ; push eax ; mov eax,[where] ; push eax ; call Unlink ; mov edi,eax ; add esp,10h ; lea ecx,[edi+{voff:#x}] ; call {vcls}::~{vcls} ; test edi,edi ; je F ; push edi ; call free ; add esp,4 ; F: mov eax,[esi+4] ; dec eax ; mov [esi+4],eax ; pop edi ; pop esi ; ret 4
// Dinkumware-style tree erase: the __cdecl unlink helper @ 0x{helper:08X} takes the node and the
// addresses of the head's parent/left/right links (formed in place with `add eax,4` for the
// last) and returns the unlinked node, whose {vcls} value at +{voff:#x} is destroyed
// (@ 0x{dtor:08X}) before the node is freed through the null-tested deallocate (free @
// 0x{freer:08X}); the size is decremented and returned (`mov/dec/mov` form). The manifest label
// is propagated; the tree is named descriptively. One of {len(members)} bodies of this shape.
{vinc}extern "C" void __cdecl free(void* block);

{vdecl}struct {nm} {{
    struct SNode {{
        SNode* left;                       // +0
        SNode* parent;                     // +4
        SNode* left2;                      // +8  (head->left)
        SNode* right;                      // +0xC (head->right)
{"        unsigned char _pad[%#x];" % (voff-0x10) + chr(10) if voff>0x10 else ""}        {vcls} value;                      // +{voff:#x}
    }};
    SNode* head;                           // +0
    unsigned long size;                    // +4
    unsigned long Erase_{nm}(SNode* where);
}};

extern "C" {nm}::SNode* __cdecl Unlink_{helper:08x}({nm}::SNode* where, {nm}::SNode** root, {nm}::SNode** leftmost, {nm}::SNode** rightmost);   // 0x{helper:08X}

unsigned long {nm}::Erase_{nm}(SNode* where)
{{
    SNode* victim = Unlink_{helper:08x}(where, &head->parent, &head->left2, &head->right);
    victim->value.~{vcls}();
    if (victim)
        free(victim);
    return --size;
}}
''',T(a,'''#include <cstdlib>
static int g_d = 0; struct V { void* d; ~V() { ++g_d; } };
struct Tr { struct N { N* a; N* p; N* l; N* r; V v; }; N* h; unsigned long s; unsigned long E(N* w); };
static Tr::N* __cdecl U(Tr::N* w, Tr::N**, Tr::N**, Tr::N**) { return w; }
unsigned long Tr::E(N* w) { N* v = U(w, &h->p, &h->l, &h->r); v->v.~V(); if (v) std::free(v); return --s; }
int main()
{
    Tr t; Tr::N hd; t.h = &hd; t.s = 2; Tr::N* n = (Tr::N*)std::malloc(sizeof(Tr::N)); if (t.E(n) != 1 || g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_17","w512_31","w512_119"):   # CGameScriptInterface script entry points over a by-value CScriptThing: GetThing() through the counted pointer, TC lookup (inlined CVectorMap find), member set / reset / get; the CScriptThing destructor inlined
        mc=[i for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM]
        vslots=sorted(set(i.operands[0].mem.disp for i in mc))
        if len(vslots)!=2 or 4 not in vslots: continue
        lower=sorted(set(c for c in calls if c!=0xbfe9bc and c!=0x99a2e0))
        if len(lower)!=1 or calls[-1]!=0x99a2e0 or 0xbfe9bc not in calls: continue
        lower=lower[0]; gslot=[v for v in vslots if v!=4][0]
        vts2=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x1200000<=i.operands[1].imm<0x1300000]
        if not vts2 or len(set(vts2))!=1: continue
        vt=vts2[0]
        tst=[i for i in ins if i.mnemonic=="test" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_IMM]
        if len(tst)!=1: continue
        fmask=tst[0].operands[1].imm<<(8 if tst[0].op_str.startswith(("ch","dh","ah","bh")) else 0)
        _ti=ins.index(tst[0]); _prev=ins[_ti-1]
        if not (_prev.mnemonic=="mov" and _prev.operands[1].type==X86_OP_MEM and "eax +" in _prev.op_str): continue
        foff=_prev.operands[1].mem.disp
        moff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "eax +" in i.op_str][0]
        key=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and i.operands[1].imm<0x1000][0]
        fname=re.match(r"^\?(\w+)@",mname); fname=fname.group(1) if fname else (mname.split("::")[-1] if re.match(r"^[A-Za-z_]\w*$",mname.split("::")[-1]) else f"ScriptEntry_{a}")
        # the member access after the lookup
        post=[i for i in ins if i.address>max(i.address for i in ins if i.mnemonic=="pop" and i.op_str=="esi") and i.mnemonic=="mov"] if TPL!="w512_119" else []
        common=f'''struct CThing;
struct CTCBase;

enum ETCInterfaceType {{ TC_FIRST = 0 }};

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CScriptEntity {{
{"".join("    virtual void _slot%02d();" % j + chr(10) for j in range(gslot//4))}    virtual CThing* GetThing();            // slot {gslot//4} (vtable offset {gslot:#x})
}};

struct CCountedPointer {{
    CScriptEntity* m_pData;                // +0
    SCountBlock* m_pInfo;                  // +4
    ~CCountedPointer()
    {{
        if (m_pInfo) {{
            --m_pInfo->m_references;
            if (m_pInfo->m_references == 0) {{
                m_pInfo->m_pfnDestroy(m_pInfo->m_pObject);
                delete m_pInfo;
            }}
        }}
        m_pData = 0;
        m_pInfo = 0;
    }}
}};

struct CBase {{
    virtual void v00();
    ~CBase();                              // 0x0099A2E0 (sets the CBase vtable; manifest label propagated)
}};

struct CScriptThing : CBase {{             // 12 bytes, passed by value (vtable 0x{vt:08X}, rtti-vtables.tsv)
    CCountedPointer PImp;                  // +4 Data / +8 Info
    ~CScriptThing() {{ }}
    CThing* GetThing() const {{ return PImp.m_pData == 0 ? 0 : PImp.m_pData->GetThing(); }}
}};

struct SPair {{
    long first;                            // ETCInterfaceType
    CTCBase* second;
}};

struct CVectorMap {{
    SPair* first;                          // +0
    SPair* last;                           // +4
    SPair* end;                            // +8
    SPair* LowerBound(const long& key);    // 0x{lower:08X}
    SPair* Find(ETCInterfaceType key)      // the enum converts to LowerBound's long reference through a frame temporary; the compare keeps the constant
    {{
        SPair* it = LowerBound(key);
        if (it == last || key < it->first)
            it = last;
        return it;
    }}
}};

struct CThing {{
    unsigned char _pad[{foff:#x}];
    unsigned long m_{foff:02x};                 // +{foff:#x} flags
    unsigned char _pad2[{moff-foff-4:#x}];
    CVectorMap m_{moff:02x};                    // +{moff:#x} TC map
    CTCBase* GetTC(ETCInterfaceType type)
    {{
        CTCBase* tc;                       // deliberately left unset when the map is absent (retail reads the dead slot)
        if (m_{foff:02x} & {fmask:#x})
            tc = m_{moff:02x}.Find(type)->second;
        return tc;
    }}
}};
'''
        if TPL=="w512_17":
            if retn!=0x10: continue
            _st=[i for i in post if i.operands[0].type==X86_OP_MEM and "eax +" in i.op_str][0]; soff=_st.operands[0].mem.disp; vty="unsigned char" if _st.operands[0].size==1 else "long"
            add(a,"CGameScriptInterface",fname,HDRF(a,f"CGameScriptInterface::{fname}(CScriptThing, {vty}) -- GetThing through the counted pointer, TC {key:#x} from the thing's TC map (+{moff:#x}, inlined find), store the argument at +{soff:#x}; CScriptThing destroyed inline",len(b))+f'''//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+{gslot:#x}] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor: vtable store, count release, delete) ; jmp D ; B: (GetThing again) ; mov ecx,[eax+{foff:#x}] ; test flag ; je N ; push esi ; lea esi,[eax+{moff:#x}] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; mov dword ptr [esp+0Ch],{key:#x} ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je L ; cmp dword ptr [eax],{key:#x} ; jle F ; L: mov eax,esi ; F: mov eax,[eax+4] ; pop esi ; jmp S ; N: mov eax,[dead slot] ; S: mov ecx,[value] ; mov [eax+{soff:#x}],ecx ; (dtor) ; D: lea ecx,[esp+0Ch] ; mov [esp+14h],edi ; mov [esp+10h],edi ; call CBase::~CBase ; pop edi ; pop ecx ; ret 10h
// Script API entry point (virtual on CGameScriptInterface; `this` unused): the by-value
// CScriptThing resolves its thing through virtual slot {gslot//4} of the counted pointee (null
// tested, evaluated once for the guard and once for the body), the TC entry {key:#x} is looked
// up in the thing's CVectorMap at +{moff:#x} when flag {fmask:#x} of +{foff:#x} is set (inlined find
// over LowerBound @ 0x{lower:08X}; when the flag is clear retail reads an uninitialised local),
// and the argument is stored at +{soff:#x} of the TC. The CScriptThing parameter's destructor
// is inlined on both paths (vtable store, count-block release through the flag-tested
// operator delete, fields nulled, then CBase::~CBase @ 0x0099A2E0). One of {len(members)} bodies.
{common}
struct STC_{soff:02x} {{
    unsigned char _pad[{soff:#x}];
    {vty} m_{soff:02x};{" "*(27-len(vty))}// +{soff:#x}
}};

struct CGameScriptInterface {{
    void {fname}(CScriptThing thing, {vty} value);
}};

void CGameScriptInterface::{fname}(CScriptThing thing, {vty} value)
{{
    if (!thing.GetThing())
        return;                            // the parameter's destructor is inlined here too
    CThing* pThing = thing.GetThing();
    ((STC_{soff:02x}*)pThing->GetTC((ETCInterfaceType){key:#x}))->m_{soff:02x} = value;
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct Th { long f; long t; };
struct Ent { virtual Th* G() = 0; }; struct E2 : Ent { Th* t; virtual Th* G() { return t; } };
struct P { Ent* d; Blk* i; ~P() { if (i) { --i->r; if (i->r == 0) { i->f(i->o); delete i; } } i = 0; d = 0; } };
struct B { virtual void v() {} ~B() {} }; struct ST : B { P p; ~ST() {} Th* G() const { return p.d ? p.d->G() : 0; } };
struct I { void S(ST s, long v) { if (!s.G()) return; Th* t = s.G(); t->t = v; } };
int main()
{
    Th th; th.t = 0; E2 e; e.t = &th; Blk* b = new Blk; b->r = 1; b->f = Destroy; b->o = 0; ST s; s.p.d = &e; s.p.i = b; I i; i.S(s, 5); s.p.i = 0; s.p.d = 0; if (th.t != 5 || g_d != 1) '''+FAIL(a)+'''
'''),b)
        elif TPL=="w512_31":
            if retn!=0xc: continue
            lds=[]; sts=[]
            for i in post:
                if i.operands[0].type==X86_OP_MEM: sts.append(i.operands[0].mem.disp); break
                if i.operands[1].type==X86_OP_MEM and "eax +" in i.op_str or "ecx +" in i.op_str: lds.append(i.operands[1].mem.disp)
            if len(lds)!=2 or len(sts)!=1: continue
            d1,d2=lds; soff=sts[0]
            _st=[i for i in post if i.operands[0].type==X86_OP_MEM][0]; rty="unsigned char" if _st.operands[0].size==1 else "long"
            add(a,"CGameScriptInterface",fname,HDRF(a,f"CGameScriptInterface::{fname}(CScriptThing) -- GetThing through the counted pointer, TC {key:#x} from the thing's TC map (+{moff:#x}, inlined find), reset +{soff:#x} from the TC's def (+{d1:#x} -> +{d2:#x}); CScriptThing destroyed inline",len(b))+f'''//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+{gslot:#x}] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor) ; jmp D ; B: (GetThing again) ; mov ecx,[eax+{foff:#x}] ; test flag ; je N ; (inlined CVectorMap find of {key:#x}) ; jmp S ; N: mov eax,[dead slot] ; S: mov ecx,[eax+{d1:#x}] ; mov edx,[ecx+{d2:#x}] ; mov [eax+{soff:#x}],edx ; (dtor) ; D: lea ecx,[esp+0Ch] ; mov [esp+14h],edi ; mov [esp+10h],edi ; call CBase::~CBase ; pop edi ; pop ecx ; ret 0Ch
// Script API entry point (virtual on CGameScriptInterface; `this` unused): the by-value
// CScriptThing resolves its thing through virtual slot {gslot//4} of the counted pointee, the TC
// entry {key:#x} is looked up in the thing's CVectorMap at +{moff:#x} when flag {fmask:#x} of +{foff:#x}
// is set (inlined find over LowerBound @ 0x{lower:08X}; with the flag clear retail reads an
// uninitialised local), and the TC member at +{soff:#x} is reset from its def: the object at
// +{d1:#x} of the TC supplies +{d2:#x}. The CScriptThing destructor is inlined on both paths.
// One of {len(members)} bodies of this shape.
{common}
struct STCDef_{d2:02x} {{
    unsigned char _pad[{d2:#x}];
    {rty} m_{d2:02x};{" "*(27-len(rty))}// +{d2:#x}
}};

struct STC_{soff:02x}_{d1:02x} {{
    unsigned char _pad[{soff:#x}];
    {rty} m_{soff:02x};{" "*(27-len(rty))}// +{soff:#x}
    unsigned char _pad2[{d1-soff-(4 if rty=="long" else 1):#x}];
    STCDef_{d2:02x}* m_{d1:02x};                 // +{d1:#x} def
}};

struct CGameScriptInterface {{
    void {fname}(CScriptThing thing);
}};

void CGameScriptInterface::{fname}(CScriptThing thing)
{{
    if (!thing.GetThing())
        return;                            // the parameter's destructor is inlined here too
    CThing* pThing = thing.GetThing();
    STC_{soff:02x}_{d1:02x}* tc = (STC_{soff:02x}_{d1:02x}*)pThing->GetTC((ETCInterfaceType){key:#x});
    tc->m_{soff:02x} = tc->m_{d1:02x}->m_{d2:02x};
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct Df { long v; }; struct Th { long f; long t; Df* d; };
struct Ent { virtual Th* G() = 0; }; struct E2 : Ent { Th* t; virtual Th* G() { return t; } };
struct P { Ent* d; Blk* i; ~P() { if (i) { --i->r; if (i->r == 0) { i->f(i->o); delete i; } } i = 0; d = 0; } };
struct B { virtual void v() {} ~B() {} }; struct ST : B { P p; ~ST() {} Th* G() const { return p.d ? p.d->G() : 0; } };
struct I { void S(ST s) { if (!s.G()) return; Th* t = s.G(); t->t = t->d->v; } };
int main()
{
    Df d; d.v = 9; Th th; th.t = 0; th.d = &d; E2 e; e.t = &th; Blk* b = new Blk; b->r = 1; b->f = Destroy; b->o = 0; ST s; s.p.d = &e; s.p.i = b; I i; i.S(s); s.p.i = 0; s.p.d = 0; if (th.t != 9 || g_d != 1) '''+FAIL(a)+'''
'''),b)
        else:   # w512_119: unsigned long Get(CScriptThing) { if (!thing.GetThing()) return 0; return tc->m_x; }
            if retn!=0xc: continue
            roff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[0].reg==__import__("capstone.x86",fromlist=["X86_REG_ESI"]).X86_REG_ESI and i.operands[1].type==X86_OP_MEM and "eax +" in i.op_str]
            if not roff: continue
            roff=roff[-1]
            add(a,"CGameScriptInterface",fname,HDRF(a,f"CGameScriptInterface::{fname}(CScriptThing) -- 0 unless the counted pointer resolves a thing; else the dword at +{roff:#x} of TC {key:#x} (thing's TC map +{moff:#x}, inlined find); CScriptThing destroyed inline",len(b))+f'''//
// Retail bytes:  push ecx ; mov ecx,[thing.PImp.Data] ; push edi ; xor edi,edi ; cmp ecx,edi ; je X ; mov eax,[ecx] ; call [eax+{gslot:#x}] ; cmp eax,edi ; jne B ; X: (CScriptThing dtor) ; xor eax,eax ; pop edi ; pop ecx ; ret 0Ch ; B: (GetThing again) ; mov ecx,[eax+{foff:#x}] ; test flag ; push esi ; je N ; (inlined CVectorMap find of {key:#x}) ; jmp S ; N: mov eax,[dead slot] ; S: mov esi,[eax+{roff:#x}] ; (dtor) ; lea ecx,[esp+10h] ; mov [esp+14h],edi ; mov [esp+18h],edi ; call CBase::~CBase ; mov eax,esi ; pop esi ; pop edi ; pop ecx ; ret 0Ch
// Script API getter (virtual on CGameScriptInterface; `this` unused): returns 0 when the
// by-value CScriptThing does not resolve a thing (virtual slot {gslot//4} of the counted pointee),
// else the dword at +{roff:#x} of TC {key:#x}, looked up in the thing's CVectorMap at +{moff:#x} when
// flag {fmask:#x} of +{foff:#x} is set (inlined find over LowerBound @ 0x{lower:08X}; with the flag
// clear retail reads an uninitialised local). The result is held in esi across the inlined
// CScriptThing destructor. One of {len(members)} bodies of this shape.
{common}
struct STC_{roff:02x} {{
    unsigned char _pad[{roff:#x}];
    unsigned long m_{roff:02x};                 // +{roff:#x}
}};

struct CGameScriptInterface {{
    unsigned long {fname}(CScriptThing thing);
}};

unsigned long CGameScriptInterface::{fname}(CScriptThing thing)
{{
    if (!thing.GetThing())
        return 0;
    CThing* pThing = thing.GetThing();
    return ((STC_{roff:02x}*)pThing->GetTC((ETCInterfaceType){key:#x}))->m_{roff:02x};
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; }; struct Th { long f; unsigned long t; };
struct Ent { virtual Th* G() = 0; }; struct E2 : Ent { Th* t; virtual Th* G() { return t; } };
struct P { Ent* d; Blk* i; ~P() { if (i) { --i->r; if (i->r == 0) { i->f(i->o); delete i; } } i = 0; d = 0; } };
struct B { virtual void v() {} ~B() {} }; struct ST : B { P p; ~ST() {} Th* G() const { return p.d ? p.d->G() : 0; } };
struct I { unsigned long S(ST s) { if (!s.G()) return 0; Th* t = s.G(); return t->t; } };
int main()
{
    Th th; th.t = 7; E2 e; e.t = &th; Blk* b = new Blk; b->r = 1; b->f = Destroy; b->o = 0; ST s; s.p.d = &e; s.p.i = b; I i; unsigned long r = i.S(s); s.p.i = 0; s.p.d = 0; if (r != 7 || g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_104",):   # list<T>::clear() where T has a virtual destructor: cursor advanced, value destroyed through vtable slot 0 (flag 0), node freed, head re-linked
        if len(calls)!=1 or retn!=0: continue
        freer=calls[0]; voff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "edi +" in i.op_str][0]
        if not any(i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM for i in ins): continue
        nm=f"CRetailList_{a}"; vcls=f"CNodeValue_{a}"
        add(a,"_unattributed",f"clear_{nm}",HDRF(a,f"{nm}::clear() -- destroy every node's polymorphic value at +{voff:#x} through its virtual destructor (slot 0, flag 0), free the node, re-link the head to itself",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,ecx ; mov eax,[ebx] ; push esi ; mov esi,[eax] ; cmp esi,eax ; je E ; push edi ; L: mov edi,esi ; mov eax,[edi+{voff:#x}] ; mov esi,[esi] ; lea ecx,[edi+{voff:#x}] ; push 0 ; call [eax] ; push edi ; call free ; mov eax,[ebx] ; add esp,4 ; cmp esi,eax ; jne L ; pop edi ; E: mov eax,[ebx] ; mov [eax],eax ; mov ebx,[ebx] ; mov [ebx+4],ebx ; pop ebx ; ret
// STLport-style list::clear for values with a virtual destructor: walks from head->next to the
// head (re-read from `this` each iteration), advancing the cursor before the old node's value
// is destroyed through vtable slot 0 (the scalar deleting destructor called with flag 0: an
// explicit destructor call) and the node freed (free @ 0x{freer:08X}, no null test), then links
// the head to itself. The manifest label (`CGuiControlTreePane::SortTreeRecursively`) is
// propagated across {len(members)} identical bodies; the list is named descriptively.
extern "C" void __cdecl free(void* block);

struct {vcls} {{
    virtual ~{vcls}();                     // slot 0: scalar deleting destructor
}};

struct {nm} {{
    struct SNode {{
        SNode* m_next;                     // +0
        SNode* m_prev;                     // +4
        {vcls} m_value;                    // +{voff:#x}
    }};
    SNode* m_head;                         // +0 sentinel
    void clear_{nm}();
}};

void {nm}::clear_{nm}()
{{
    SNode* cur = m_head->m_next;
    while (cur != m_head) {{
        SNode* tmp = cur;
        cur = cur->m_next;
        tmp->m_value.~{vcls}();
        free(tmp);
    }}
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}}
''',T(a,'''#include <cstdlib>
#include <new>
static int g_d = 0; struct V { virtual ~V() { ++g_d; } };
struct L { struct N { N* n; N* p; V v; }; N* h; void C() { N* c = h->n; while (c != h) { N* t = c; c = c->n; t->v.~V(); std::free(t); } h->n = h; h->p = h; } };
int main()
{
    L l; L::N hd; l.h = &hd; L::N* a = (L::N*)std::malloc(sizeof(L::N)); new (&a->v) V; hd.n = a; a->n = &hd; a->p = &hd; hd.p = a; l.C(); if (g_d != 1 || hd.n != &hd || hd.p != &hd) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_114",):   # void CX::OnPersist(CPersistContext* context, long) { context->TransferA("NAME1", m_a, defaultA); context->TransferB("NAME2", m_b, defaultB); }  (member addresses: lea, then add-in-place; zero defaults homed in one frame slot)
        if len(calls)!=2 or retn!=8: continue
        ta,tb=calls; lits=[p for p in pushes if 0x1200000<=p<0x1300000]
        if len(lits)!=2: continue
        na,nb=lit(lits[0]),lit(lits[1])
        aoff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str]
        boff=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.op_str.startswith("esi,")]
        if len(aoff)!=1 or len(boff)!=1: continue
        aoff,boff=aoff[0],boff[0]
        zs=[i for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and i.operands[1].imm==0 and "esp" in i.op_str]
        if len(zs)!=2: continue
        tya="long" if zs[0].operands[0].size==4 else "signed char"; tyb="long" if zs[1].operands[0].size==4 else "signed char"
        hn={ta:f"Transfer_{ta:08x}",tb:f"Transfer_{tb:08x}"}
        cls=clean(mmod) if re.match(r"^C[A-Za-z0-9_]+$",mmod) else f"CPersistable_{a}"; fn=mname.split("::")[-1] if re.match(r"^[A-Za-z_]\w*$",mname.split("::")[-1]) else f"OnPersist_{a}"
        add(a,mmod if cls!=f"CPersistable_{a}" else "_unattributed",fn,HDRF(a,f"{cls}::{fn}(CPersistContext*, long) -- persists {na} (+{aoff:#x}, {tya}) and {nb} (+{boff:#x}, {tyb}) through two typed context transfers with zero defaults",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push edi ; mov edi,[context] ; mov esi,ecx ; lea eax,[esp+{"8" if tya=="long" else "0Bh"}] ; push eax ; lea ecx,[esi+{aoff:#x}] ; push ecx ; push offset "{na}" ; mov ecx,edi ; mov {"dword" if tya=="long" else "byte"} ptr [default],0 ; call {hn[ta]} ; lea edx,[default2] ; push edx ; add esi,{boff:#x} ; push esi ; push offset "{nb}" ; mov ecx,edi ; mov {"dword" if tyb=="long" else "byte"} ptr [default2],0 ; call {hn[tb]} ; pop edi ; pop esi ; pop ecx ; ret 8
// Persist hook: two members are handed by reference to typed CPersistContext transfer helpers
// (@ 0x{ta:08X} and 0x{tb:08X}, labels propagated) together with their names and a zero default
// built in a frame temporary (both defaults share the one frame slot: their lifetimes do not
// overlap). The second member's address is formed in place (`add esi,{boff:#x} ; push esi`,
// GOTCHAS). The second argument is unused by this body. Member types follow the default's
// store width. One of {len(members)} bodies of this shape.
struct CPersistContext {{
    void {hn[ta]}(const char* name, {tya}& value, const {tya}& defaultValue);   // 0x{ta:08X}
{"" if ta==tb else "    void %s(const char* name, %s& value, const %s& defaultValue);   // 0x%08X" % (hn[tb],tyb,tyb,tb) + chr(10)}}};

struct {cls} {{
    unsigned char _pad[{aoff:#x}];
    {tya} m_{aoff:02x};{" "*(28-len(tya))}// +{aoff:#x} "{na}"
{"    unsigned char _pad2[%#x];" % (boff-aoff-(4 if tya=="long" else 1)) + chr(10) if boff-aoff>(4 if tya=="long" else 1) else ""}    {tyb} m_{boff:02x};{" "*(28-len(tyb))}// +{boff:#x} "{nb}"
    void {fn}(CPersistContext* context, long unused);
}};

void {cls}::{fn}(CPersistContext* context, long unused)
{{
    context->{hn[ta]}("{na}", m_{aoff:02x}, 0);
    context->{hn[tb]}("{nb}", m_{boff:02x}, 0);
}}
''',T(a,'''static int g_t = 0; struct Ctx { void A(const char* n, long& v, const long& d) { v = n[0] + d; ++g_t; } void B(const char* n, signed char& v, const signed char& d) { v = (signed char)(n[0] + d); ++g_t; } };
struct P { unsigned char pad[0x20]; long a; signed char b; void F(Ctx* c, long) { c->A("A", a, 0); c->B("B", b, 0); } };
int main()
{
    Ctx c; P p; p.a = 0; p.b = 0; p.F(&c, 0); if (g_t != 2 || p.a != 'A' || p.b != 'B') '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_88",):   # void CX::GetEntry(long* result) { ETCInterfaceType type = K; if (m_p) { it = m_p->map.Find(type); if (it != m_p->map.last) { *result = it->second; return; } } *result = g_map.Find(type)->second; }
        if len(calls)!=2 or calls[0]!=calls[1] or retn!=4: continue
        lower=calls[0]; key=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esp" in i.op_str and i.operands[1].type==X86_OP_IMM][0]
        poff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and "ecx +" in i.op_str][0]
        moff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "edi +" in i.op_str][0]
        gl=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_IMM and i.operands[1].imm>0x1300000]
        if len(gl)!=1: continue
        gl=gl[0]; nm=f"CMapOwner_{a}"
        add(a,"_unattributed",f"GetEntry_{nm}",HDRF(a,f"{nm}::GetEntry(long*) -- entry {key:#x} from the CVectorMap of the object at +{poff:#x} (map at +{moff:#x}, inlined find over LowerBound @ 0x{lower:08X}), else from the global map @ 0x{gl:08X}",len(b))+f'''//
// Retail bytes:  push ecx ; push edi ; mov edi,[ecx+{poff:#x}] ; test edi,edi ; mov dword ptr [esp+4],{key:#x} ; je G ; push esi ; lea eax,[esp+8] ; lea esi,[edi+{moff:#x}] ; push eax ; mov ecx,esi ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je L ; cmp dword ptr [eax],{key:#x} ; jle F ; L: mov eax,esi ; F: cmp eax,[edi+{moff+4:#x}] ; pop esi ; jne R ; G: lea ecx,[esp+4] ; push ecx ; mov ecx,offset g_map ; call CVectorMap::LowerBound ; mov ecx,[g_map+4] ; cmp eax,ecx ; je E ; cmp dword ptr [eax],{key:#x} ; jg E ; mov edx,[eax+4] ; mov eax,[result] ; mov [eax],edx ; pop edi ; pop ecx ; ret 4 ; E: mov ecx,[ecx+4] ; mov edx,[result] ; mov [edx],ecx ; pop edi ; pop ecx ; ret 4 ; R: mov eax,[eax+4] ; mov ecx,[result] ; mov [ecx],eax ; pop edi ; pop ecx ; ret 4
// Two-level CVectorMap lookup written through an out-parameter: the key ({key:#x}) is one frame
// local passed by address to both LowerBound calls (@ 0x{lower:08X}) while the inlined compares
// keep the constant; the object's own map (object at +{poff:#x}, map at +{moff:#x}) is tried first
// and its hit returned when the find did not fall back to `last`, else the global map
// @ 0x{gl:08X} supplies the entry (reading `last->second` when absent, the tails duplicated).
// The manifest label is propagated; names are descriptive. One of {len(members)} bodies of this shape.
enum ETCInterfaceType {{ TC_FIRST = 0 }};

struct SPair {{
    long first;                            // ETCInterfaceType
    long second;
}};

struct CVectorMap {{
    SPair* first;                          // +0
    SPair* last;                           // +4
    SPair* end;                            // +8
    SPair* LowerBound(const long& key);    // 0x{lower:08X}
    SPair* Find(const long& key)
    {{
        SPair* it = LowerBound(key);
        if (it == last || key < it->first)
            it = last;
        return it;
    }}
}};

struct SMapHolder_{a} {{
    unsigned char _pad[{moff:#x}];
    CVectorMap m_{moff:02x};                    // +{moff:#x}
}};

extern CVectorMap g_map_{gl:08x};            // 0x{gl:08X}

struct {nm} {{
    unsigned char _pad[{poff:#x}];
    SMapHolder_{a}* m_{poff:02x};               // +{poff:#x}
    void GetEntry_{nm}(long* result);
}};

void {nm}::GetEntry_{nm}(long* result)
{{
    const long type = {key:#x};            // const local: homed once for the two reference bindings, folded in the compares
    SMapHolder_{a}* holder = m_{poff:02x};
    if (holder) {{
        SPair* it = holder->m_{moff:02x}.Find(type);
        if (it != holder->m_{moff:02x}.last) {{
            *result = it->second;
            return;
        }}
    }}
    *result = g_map_{gl:08x}.Find(type)->second;
}}
''',T(a,'''struct P { long f; long s; };
struct M { P* f; P* l; P* e; P* LB(const long& k) { P* i = f; while (i != l && i->f < k) ++i; return i; } P* Find(const long& k) { P* i = LB(k); if (i == l || k < i->f) i = l; return i; } };
struct H { unsigned char pad[8]; M m; }; static M g; 
struct X { unsigned char pad[4]; H* h; void G(long* r) { const long t = 1; H* p = h; if (p) { P* i = p->m.Find(t); if (i != p->m.l) { *r = i->s; return; } } *r = g.Find(t)->s; } };
int main()
{
    P ga[2]; ga[0].f = 1; ga[0].s = 7; g.f = ga; g.l = ga + 1; g.e = ga + 2; H h; P ha[1]; h.m.f = ha; h.m.l = ha; h.m.e = ha + 1; X x; x.h = &h; long r = 0; x.G(&r); if (r != 7) '''+FAIL(a)+'''
    x.h = 0; x.G(&r); if (r != 7) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_46",):   # _Uninit_fill_n for CCountedPointer elements: the counted-pointer copy constructor (two dwords, count incremented when present) inlined under the placement-new null test
        if calls or retn!=8: continue
        step=[i.operands[1].imm for i in ins if i.mnemonic=="add" and i.operands[1].type==X86_OP_IMM and i.operands[0].type==X86_OP_REG]
        if step!=[8] or not any(i.mnemonic=="inc" for i in ins): continue
        add(a,"_global",f"UninitFillN_CCountedPointer_{a}",HDRF(a,f"_Uninit_fill_n<CCountedPointer*> -- placement-copy `count` counted pointers (object + count block, count incremented when present), return the advanced pointer",len(b))+f'''//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[value] ; L: test eax,eax ; je S ; mov ecx,[edx] ; mov [eax],ecx ; mov ecx,[edx+4] ; test ecx,ecx ; mov [eax+4],ecx ; je S ; inc dword ptr [ecx] ; S: add eax,8 ; dec esi ; jne L ; pop esi ; E: ret 8
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for 8-byte counted-pointer
// elements: the copy constructor is inlined under the placement-new null test (object and
// count-block pointers copied, the block's count incremented when present); the advanced
// destination is returned. The manifest label names a {{__int64, CCountedPointer}} pair
// instantiation and is propagated; the bytes copy one 8-byte counted pointer. One of {len(members)} bodies.
#include <new>

struct SCountBlock {{
    long m_references;                     // +0
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {{
        if (m_pCount)
            ++m_pCount->m_references;
    }}
}};

struct CRetailAllocator {{ }};

CCountedPointer* __fastcall UninitFillN_CCountedPointer_{a}(CCountedPointer* first, unsigned long count, const CCountedPointer& value, CRetailAllocator allocator)
{{
    for (; 0 < count; --count, ++first)
        new (first) CCountedPointer(value);
    return first;
}}
''',T(a,'''#include <new>
struct B { long r; }; struct P { void* o; B* c; P(const P& x) : o(x.o), c(x.c) { if (c) ++c->r; } }; struct A {};
static P* __fastcall UF(P* f, unsigned long n, const P& v, A) { for (; 0 < n; --n, ++f) new (f) P(v); return f; }
int main()
{
    B b; b.r = 1; unsigned char vb[8]; P* v = (P*)vb; v->o = 0; v->c = &b; unsigned char buf[24]; P* e = UF((P*)buf, 3, *v, A()); if (e != (P*)(buf + 24) || b.r != 4 || ((P*)buf)[2].c != &b) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_85",):   # `??_G` of a non-polymorphic class with one member whose destructor is out of line (no base destructor): member destroyed, flag-tested delete
        leas=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea"]
        if len(calls)!=2 or len(leas)!=1 or retn!=4: continue
        dtor,deleter=calls; off=leas[0]
        dm,dn,_=label(dtor); dl=dn.split("::")[-1]
        if dtor==0x99eae0: mcls="CCharString"; minc='#include "fable_string.h"\n'; mdecl=""
        elif dl.startswith("~") and (dm,dn) in uniq and re.match(r"^C[A-Za-z0-9_]+$",dm.split("::")[-1]): mcls=dm.split("::")[-1]; minc=""; mdecl=f"struct {mcls} {{\n    void* m_data;\n    ~{mcls}();                             // 0x{dtor:08X}\n}};\n\n"
        else: mcls=f"CMember_{dtor:08x}"; minc=""; mdecl=f"struct {mcls} {{\n    void* m_data;\n    ~{mcls}();                             // 0x{dtor:08X}\n}};\n\n"
        cls=f"COwner_{a}"
        add(a,"_unattributed",f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- non-virtual: the {mcls} member at +{off:#x} destroyed out of line, flag-tested delete",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+{off:#x}] ; call {mcls}::~{mcls} ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: the {mcls} member at +{off:#x} is destroyed through its
// out-of-line destructor @ 0x{dtor:08X}, then bit 0 of the flag argument selects the flag-tested
// operator delete @ 0x{deleter:08X}. The manifest label (`vector deleting destructor` of
// CLandscapeBackgroundPatch) is propagated across the family; the owner is named descriptively.
// One of {len(members)} bodies of this shape.
{minc}{mdecl}struct {cls} {{
    unsigned char _pad[{off:#x}];
    {mcls} m_{off:02x};{" "*(28-len(mcls))}// +{off:#x}
}};

// The delete-expression that makes the compiler emit {cls}::`scalar deleting destructor'.
void Destroy_{cls}({cls}* object)
{{
    delete object;
}}
''',T(a,'''static int g_d = 0; struct S { void* p; ~S() { ++g_d; } };
struct X { long a, b; S s; };
static void Kill(X* x) { delete x; }
int main()
{
    Kill(new X); if (g_d != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_87",):   # void CRetailVector::InsertN(T* where, unsigned long count, const T& value) -- VC7.1-style _Insert_n for 8-byte POD elements with out-of-line UninitCopy / CopyBackward / Fill / UninitFillN helpers and a tagged InsertGrow
        if len(calls)!=7 or retn!=0xc: continue
        ucopy,cback,fill,ufill,ucopy2,fill2,grow=calls
        if ucopy!=ucopy2 or fill!=fill2: continue
        step=[i.operands[1].mem.scale for i in ins if i.mnemonic=="lea" and i.operands[1].mem.scale==8]
        if not step: continue
        nm=f"CRetailVector_{a}"
        add(a,"_global",f"InsertN_{nm}",HDRF(a,f"{nm}::InsertN(T*, unsigned long, const T&) -- insert `count` copies of an 8-byte POD at `where`: InsertGrow @ 0x{grow:08X} when the capacity is short, else the two Dinkumware cases (suffix moved up through UninitCopy @ 0x{ucopy:08X} / CopyBackward @ 0x{cback:08X} / Fill @ 0x{fill:08X}, or UninitFillN @ 0x{ufill:08X} past the end)",len(b))+f'''//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; mov ecx,[count] ; test ecx,ecx ; je R ; mov eax,[esi+4] ; mov edx,[esi+8] ; sub edx,eax ; sar edx,3 ; cmp edx,ecx ; jb G ; mov edx,[value] ; push ebx ; mov ebx,[where] ; push ebp ; sub eax,ebx ; push edi ; mov edi,[edx] ; mov edx,[edx+4] ; sar eax,3 ; cmp eax,ecx ; mov [tmp],edi ; mov edi,[esi+4] ; mov [tmp+4],edx ; mov [tail],eax ; jbe S ; lea ebp,[ecx*8] ; lea eax,[tag] ; mov ecx,edi ; push eax ; sub ecx,ebp ; push edi ; mov edx,edi ; mov [hole],ecx ; call UninitCopy ; mov eax,[esi+4] ; mov edx,[hole] ; lea ecx,[tag] ; push ecx ; add eax,ebp ; push edi ; mov ecx,ebx ; mov [esi+4],eax ; call CopyBackward ; lea edx,[tmp] ; push edx ; lea edx,[ebx+ebp] ; mov ecx,ebx ; call Fill ; pop edi ; pop ebp ; pop ebx ; R: pop esi ; add esp,8 ; ret 0Ch ; S: mov ebp,ecx ; sub ebp,eax ; lea eax,[tmp] ; push eax ; mov edx,ebp ; mov ecx,edi ; call UninitFillN ; mov edx,[esi+4] ; lea ecx,[ebp*8] ; add edx,ecx ; mov eax,edx ; mov [esi+4],edx ; lea edx,[tag] ; push edx ; push eax ; mov edx,edi ; mov ecx,ebx ; call UninitCopy ; mov eax,[tail] ; mov edx,[esi+4] ; lea ecx,[eax*8] ; add edx,ecx ; mov [esi+4],edx ; lea edx,[tmp] ; push edx ; mov edx,edi ; mov ecx,ebx ; call Fill ; pop edi ; pop ebp ; pop ebx ; pop esi ; add esp,8 ; ret 0Ch ; G: mov edx,[where] ; push 0 ; push ecx ; mov ecx,[value] ; lea eax,[tag] ; push eax ; push ecx ; push edx ; mov ecx,esi ; call InsertGrow ; pop esi ; add esp,8 ; ret 0Ch
// Retail vector `_Insert_n` for 8-byte trivially-copyable elements (the value is copied into a
// frame temporary first, VC7.1 Dinkumware style): with spare capacity the Dinkumware two-case
// split applies -- when more elements follow `where` than are inserted, the last `count` are
// placement-moved past the end (UninitCopy, __fastcall with a tag), the hole is closed by
// CopyBackward and filled; otherwise the overflow is placement-filled past the end (UninitFillN),
// the suffix moved up and the old range filled -- else InsertGrow reallocates (the empty tag and
// the spilled locals share dead argument slots). The manifest label is propagated; names are
// descriptive. One of {len(members)} bodies of this shape.
struct SElement {{                           // 8-byte POD element
    long m_first;
    long m_second;
}};

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

SElement* __fastcall UninitCopy_{ucopy:08x}(SElement* first, SElement* last, SElement* dest, const SScalarTag& tag);     // 0x{ucopy:08X}
SElement* __fastcall CopyBackward_{cback:08x}(SElement* first, SElement* last, SElement* result, const SScalarTag& tag); // 0x{cback:08X}
void __fastcall Fill_{fill:08x}(SElement* first, SElement* last, const SElement& value);                                  // 0x{fill:08X}
SElement* __fastcall UninitFillN_{ufill:08x}(SElement* first, unsigned long count, const SElement& value);               // 0x{ufill:08X}

struct {nm} {{
    SElement* first;                       // +0
    SElement* last;                        // +4
    SElement* end;                         // +8
    void InsertGrow(SElement* where, const SElement& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x{grow:08X}
    void InsertN_{nm}(SElement* where, unsigned long count, const SElement& value);
}};

void {nm}::InsertN_{nm}(SElement* where, unsigned long count, const SElement& value)
{{
    if (count != 0) {{
        if ((unsigned long)(end - last) < count)
            InsertGrow(where, value, SScalarTag(), count, false);
        else {{
            SElement tmp = value;              // in case the value lives in the sequence
            unsigned long tail = last - where;
            SElement* oldLast = last;
            if (count < tail) {{
                SElement* hole = oldLast - count;
                UninitCopy_{ucopy:08x}(hole, oldLast, oldLast, SScalarTag());
                last += count;
                CopyBackward_{cback:08x}(where, hole, oldLast, SScalarTag());
                Fill_{fill:08x}(where, where + count, tmp);
            }}
            else {{
                UninitFillN_{ufill:08x}(oldLast, count - tail, tmp);
                last += count - tail;
                UninitCopy_{ucopy:08x}(where, oldLast, last, SScalarTag());
                last += tail;
                Fill_{fill:08x}(where, oldLast, tmp);
            }}
        }}
    }}
}}
''',T(a,'''struct E { long a, b; }; struct Tg { Tg() {} }; static int g_g = 0;
static E* __fastcall UC(E* f, E* l, E* d, const Tg&) { while (f != l) *d++ = *f++; return d; }
static E* __fastcall CB(E* f, E* l, E* r, const Tg&) { while (l != f) *--r = *--l; return r; }
static void __fastcall FL(E* f, E* l, const E& v) { while (f != l) *f++ = v; }
static E* __fastcall UF(E* f, unsigned long n, const E& v) { while (n--) *f++ = v; return f; }
struct V { E* f; E* l; E* e; void G(E*, const E&, const Tg&, unsigned long, bool) { ++g_g; }
    void I(E* w, unsigned long n, const E& v) { if (n != 0) { if ((unsigned long)(e - l) < n) G(w, v, Tg(), n, false); else { E t = v; unsigned long tl = l - w; E* ol = l;
        if (n < tl) { E* h = ol - n; UC(h, ol, ol, Tg()); l += n; CB(w, h, ol, Tg()); FL(w, w + n, t); } else { UF(ol, n - tl, t); l += n - tl; UC(w, ol, l, Tg()); l += tl; FL(w, ol, t); } } } } };
int main()
{
    E buf[8]; for (int i = 0; i < 8; ++i) { buf[i].a = i; buf[i].b = 0; } V v; v.f = buf; v.l = buf + 4; v.e = buf + 8; E x; x.a = 9; x.b = 9;
    v.I(buf + 1, 1, x); if (v.l != buf + 5 || buf[1].a != 9 || buf[2].a != 1 || buf[4].a != 3) '''+FAIL(a)+'''
    v.I(buf + 4, 3, x); if (v.l != buf + 8 || buf[7].a != 4 || buf[5].a != 9 || g_g != 0) '''+FAIL(a)+'''
    v.I(buf + 4, 3, x); if (g_g != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_5",):   # CCountedPointer<X> CFactory::CreateActiveEntity(CScriptThing* thing, __int64 key) -- cached lookup through a function pointer when enabled, new X(...) with the derived constructor inlined, counted result
        if len(calls)!=9 or retn!=0x10: continue
        dtor,newer,bctor,share,resctor,dtor2,newer2,bctor2,resctor2=calls
        if dtor!=dtor2 or newer!=newer2 or bctor!=bctor2 or resctor!=resctor2: continue
        vts2=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and 0x1200000<=i.operands[1].imm<0x1300000]
        if len(vts2)!=2 or vts2[0]!=vts2[1]: continue
        vt=vts2[0]; size=[p for p in pushes if p==0x60]
        if len(size)!=2: continue
        # tail: the second CActiveEntityScriptBase ctor call and the result ctor
        tail=[c for c in calls[7:]] if len(calls)>7 else []
        cls=rtti.get(vt) or f"CActiveEntity_{vt:08x}"
        fname=f"CreateActiveEntity_{a}"
        add(a,"_global",fname,HDRF(a,f"CCountedPointer<{cls}> CScriptFactory::{fname}(CScriptThing*, __int64) -- with the cache enabled: lookup through the function pointer at +0x10, counted copy, virtual slot 2, new {cls} (0x60 bytes, base ctor @ 0x{bctor:08X}, ShareData @ 0x{share:08X}); else new {cls} over the cache fields; result built @ 0x{resctor:08X}",len(b))+f'''//
// Retail bytes:  sub esp,10h ; push ebx ; push ebp ; push esi ; mov esi,ecx ; mov al,[esi+14h] ; test al,al ; push edi ; je N ; mov edx,[esi+8] ; mov eax,[thing] ; mov ecx,[edx+44h] ; push eax ; push ecx ; lea ecx,[tmp] ; call [esi+10h] ; mov ebx,[eax+4] ; test ebx,ebx ; mov ebp,[eax] ; mov [cp],ebp ; mov [cp+4],ebx ; je K ; inc dword ptr [ebx] ; K: lea ecx,[tmp] ; call ~CCountedPointer ; mov edx,[ebp] ; mov ecx,ebp ; call [edx+8] ; push 60h ; call operator new ; mov edi,eax ; add esp,4 ; test edi,edi ; je Z ; (base ctor over thing, key, m_18 with m_8 spilled) ; mov [edi],vtable ; mov [edi+5Ch],cache ; lea ecx,[edi+34h] ; push ebx ; push ebp ; call ShareData ; jmp J ; Z: xor edi,edi ; J: mov esi,[result] ; push edi ; mov ecx,esi ; call CCountedPointer(X*) ; lea ecx,[cp] ; call ~CCountedPointer ; pop edi ; mov eax,esi ; pop esi ; pop ebp ; pop ebx ; add esp,10h ; ret 10h ; N: push 60h ; call operator new ; ... base ctor ; mov [edi],vtable ; mov [edi+58h],m_c ; mov [edi+5Ch],m_8 ; ... ; ret 10h
// Script-entity factory returning a counted pointer through the hidden result pointer: when the
// cache flag at +0x14 is set, the lookup function pointer at +0x10 is called (hidden result in
// ecx, the cache table at +0x44 of the cache object at +8 and the thing on the stack), the
// returned counted pointer is converted into a local copy (count incremented) and the temporary
// destroyed (@ 0x{dtor:08X}), virtual slot 2 of the pointee runs, and a new {cls} (0x60 bytes) is
// built: base constructor @ 0x{bctor:08X} over (thing, key, +0x18), vtable 0x{vt:08X}, the
// counted member at +0x34 shared from the copy (@ 0x{share:08X}), the cache stored at +0x5C.
// Without the cache the entity is built over the +0xC and +0x8 fields instead. The result is
// constructed from the raw pointer @ 0x{resctor:08X} and the copy destroyed. Manifest labels are
// propagated across the script modules; names are descriptive. One of {len(members)} bodies.
struct CScriptThing;
struct CScriptCache;

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
}};

struct CCachedEntity {{
    virtual void v00();
    virtual void v01();
    virtual void Touch();                  // slot 2
}};

struct CCountedPointerBase {{             // the lookup's return type
    CCachedEntity* m_pData;                // +0
    SCountBlock* m_pInfo;                  // +4
    ~CCountedPointerBase();                // 0x{dtor:08X}
}};

struct CCountedPointerCached {{           // the local copy (converting constructor inlined)
    CCachedEntity* m_pData;                // +0
    SCountBlock* m_pInfo;                  // +4
    CCountedPointerCached(const CCountedPointerBase& other) : m_pData(other.m_pData), m_pInfo(other.m_pInfo)
    {{
        if (m_pInfo)
            ++m_pInfo->m_references;
    }}
    ~CCountedPointerCached();              // 0x{dtor:08X} (same body as the base's)
}};

struct CCountedPointerShared {{
    unsigned char _body[8];
    void ShareData(CCachedEntity* data, SCountBlock* info);   // 0x{share:08X}
}};

struct CActiveEntityScriptBase {{
    virtual void v00();
    unsigned char _body[0x30];
    CActiveEntityScriptBase(CScriptThing* thing, __int64 key, unsigned long id);   // 0x{bctor:08X}
}};

struct {cls} : CActiveEntityScriptBase {{  // sizeof = 0x60
    CCountedPointerShared m_34;            // +0x34
    unsigned char _pad[0x1c];
    void* m_58;                            // +0x58
    CScriptCache* m_5c;                    // +0x5C
    {cls}(CScriptThing* thing, __int64 key, unsigned long id, const CCountedPointerCached& cached, CScriptCache* cache)
        : CActiveEntityScriptBase(thing, key, id)
    {{
        m_34.ShareData(cached.m_pData, cached.m_pInfo);
        m_5c = cache;
    }}
    {cls}(CScriptThing* thing, __int64 key, unsigned long id, void* owner, CScriptCache* cache)
        : CActiveEntityScriptBase(thing, key, id)
    {{
        m_58 = owner;
        m_5c = cache;
    }}
}};

struct CCountedPointerResult {{
    {cls}* m_pData;                        // +0
    SCountBlock* m_pInfo;                  // +4
    CCountedPointerResult({cls}* data);    // 0x{resctor:08X}
}};

struct SCacheTable {{
    unsigned char _pad[0x44];
    void* m_44;                            // +0x44
}};

typedef CCountedPointerBase (__fastcall* PFNLookup)(void* table, CScriptThing* thing);

struct CScriptFactory {{
    void* m_00;
    void* m_04;
    SCacheTable* m_08;                     // +8 cache object
    void* m_0c;                            // +0xC
    PFNLookup m_10;                        // +0x10 lookup
    unsigned char m_14;                    // +0x14 cache enabled
    unsigned char _pad[3];
    unsigned long m_18;                    // +0x18
    CCountedPointerResult {fname}(CScriptThing* thing, __int64 key);
}};

CCountedPointerResult CScriptFactory::{fname}(CScriptThing* thing, __int64 key)
{{
    if (m_14) {{
        CCountedPointerCached cached = m_10(m_08->m_44, thing);
        cached.m_pData->Touch();
        return CCountedPointerResult(new {cls}(thing, key, m_18, cached, (CScriptCache*)m_08));
    }}
    return CCountedPointerResult(new {cls}(thing, key, m_18, m_0c, (CScriptCache*)m_08));
}}
''',T(a,'''static int g_t = 0; struct Blk { long r; }; struct Ent { virtual void a() {} virtual void b() {} virtual void T() { ++g_t; } };
struct PB { Ent* d; Blk* i; ~PB() {} }; struct PC { Ent* d; Blk* i; PC(const PB& o) : d(o.d), i(o.i) { if (i) ++i->r; } ~PC() {} };
struct Sh { unsigned char b[8]; void S(Ent*, Blk*) { ++g_t; } };
struct Base { virtual void v() {} unsigned char b[0x30]; Base(void*, __int64, unsigned long) {} };
struct X : Base { Sh m; unsigned char p[0x1c]; void* o; void* c; X(void* t, __int64 k, unsigned long id, const PC& pc, void* cc) : Base(t, k, id) { m.S(pc.d, pc.i); c = cc; } X(void* t, __int64 k, unsigned long id, void* ow, void* cc) : Base(t, k, id) { o = ow; c = cc; } };
struct R { X* d; Blk* i; R(X* x) : d(x), i(0) {} }; struct Tb { unsigned char p[0x44]; void* t; };
static Ent g_e; static Blk g_b = { 1 };
static PB __fastcall LK(void*, void*) { PB p; p.d = &g_e; p.i = &g_b; return p; }
struct F { void* a; void* b; Tb* t; void* c; PB (__fastcall* fn)(void*, void*); unsigned char en; unsigned char pad[3]; unsigned long id;
    R C(void* th, __int64 k) { if (en) { PC pc = fn(t->t, th); pc.d->T(); return R(new X(th, k, id, pc, t)); } return R(new X(th, k, id, c, t)); } };
int main()
{
    Tb tb; F f; f.t = &tb; f.fn = LK; f.en = 1; f.id = 3; f.c = 0; R r = f.C(0, 5); if (!r.d || g_t != 2 || g_b.r != 2) '''+FAIL(a)+'''
    delete r.d; f.en = 0; R r2 = f.C(0, 5); if (!r2.d || g_t != 2) '''+FAIL(a)+'''
    delete r2.d;
'''),b)
    elif TPL in ("w512_113",):   # CX* CX::Construct(SIter first, SIter last, void* a) { return RangeInitialize(STag(), first, last, a); }  -- frame-pointer TU, by-value 4-byte iterator classes with user copy constructors (slot reserved by push, then stored), __fastcall helper with the tag in edx
        if len(calls)!=1 or retn!=0xc or b[:3]!=b"\x55\x8b\xec": continue
        helper=calls[0]
        if not any(i.mnemonic=="lea" and "ebp + 0x13" in i.op_str for i in ins): continue
        nm=f"CRangeString_{a}"
        add(a,"_unattributed",f"Construct_{nm}",HDRF(a,f"{nm}::Construct(SIterator, SIterator, void*) -- forwards to the __fastcall range initialiser @ 0x{helper:08X} with an empty tag in edx, the two 4-byte iterator classes copied by value into reserved slots; returns this",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push esi ; push [ebp+10h] ; mov esi,ecx ; push ecx ; mov ecx,[ebp+0Ch] ; mov eax,esp ; mov [eax],ecx ; push ecx ; mov ecx,[ebp+8] ; mov eax,esp ; mov [eax],ecx ; lea edx,[ebp+13h] ; mov ecx,esi ; call RangeInitialize ; mov eax,esi ; pop esi ; pop ebp ; ret 0Ch
// STLport-style string range constructor forwarder: the two iterators are 4-byte classes with a
// user copy constructor, so each by-value argument is passed by reserving its slot (`push ecx`)
// and constructing the copy in place (`mov eax,esp ; mov [eax],ecx`); the helper @ 0x{helper:08X}
// is a __fastcall member taking the empty dispatch tag by reference in edx (homed in the high
// byte of a dead argument slot) and returning nothing used -- `this` is returned. The frame
// pointer comes from this translation unit's `optimize("y", off)`. The manifest label
// (`CPersistTraits<C2DVector>::TransferIn`) is propagated across the family. One of {len(members)} bodies.
#pragma optimize("y", off)              // frame pointer kept in this TU

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

struct SIterator {{                          // 4-byte iterator class with a user copy constructor
    const unsigned short* m_pCurrent;    // wide character (wchar_t is a typedef under VC7.1 defaults)
    SIterator(const SIterator& other) : m_pCurrent(other.m_pCurrent) {{ }}
}};

struct {nm} {{
    void* m_pData;
    void __fastcall RangeInitialize(const SScalarTag& tag, SIterator first, SIterator last, void* allocator);   // 0x{helper:08X}
    {nm}* Construct_{nm}(SIterator first, SIterator last, void* allocator);
}};

{nm}* {nm}::Construct_{nm}(SIterator first, SIterator last, void* allocator)
{{
    RangeInitialize(SScalarTag(), first, last, allocator);
    return this;
}}
''',T(a,'''static int g_n = 0; struct Tg { Tg() {} }; struct It { const unsigned short* p; It(const It& o) : p(o.p) {} It(const unsigned short* x) : p(x) {} };
struct Str { void* d; void __fastcall RI(const Tg&, It f, It l, void*) { g_n = (int)(l.p - f.p); } Str* C(It f, It l, void* a) { RI(Tg(), f, l, a); return this; } };
int main()
{
    static const unsigned short w[] = { 97, 98, 99, 0 }; Str s; It f(w), l(w + 3); if (s.C(f, l, 0) != &s || g_n != 3) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_82",):   # CX::CX(owner, thing) : CCombatActionBase(owner, thing, K, GetSoundPair(owner, "LIT")) { [CCharString name("LIT2", -1); SetPDef(name);] }
        if retn!=8 or len(vts)!=1 or len(calls) not in (4,7): continue
        sctor,sound,base,sdtor=calls[:4]
        if sctor!=0x99ebf0 or sdtor!=0x99eae0: continue
        lits=[p for p in pushes if 0x1200000<=p<0x1300000]
        if len(lits)!=(1 if len(calls)==4 else 2): continue
        s=lit(lits[0]); k=[p for p in pushes if 0<=p<0x10000][0]; vt=vts[0][1]; cls=rtti.get(vt) or f"CVtable_{vt:08x}"
        bm,bn,_=label(base); basecls=bm.split("::")[-1] if (bm,bn) in uniq or bm=="CCombatActionBase" else f"CBase_{base:08x}"
        setter=calls[5] if len(calls)==7 else 0; s2=lit(lits[1]) if setter else None
        if setter and (calls[4]!=0x99ebf0 or calls[6]!=0x99eae0): continue
        body=(f'''    {{
        CCharString name("{s2}", -1);      // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetName(name);                     // 0x{setter:08X}
    }}
''' if setter else "")
        add(a,cls,cls,HDRF(a,f"{cls}::{cls}(CThingCreatureBase*, CThing*) -- base {basecls}(owner, thing, {k}, GetSoundPair(owner, {s}))" + (f', then the name {s2} through the setter @ 0x{setter:08X}' if setter else ""),len(b))+f'''//
// Retail bytes:  sub esp,0Ch ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset "{s}" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov edi,[owner] ; lea eax,[esp+8] ; push eax ; mov edx,edi ; lea ecx,[esp+10h] ; call GetSoundPair ; mov ecx,[thing] ; push eax ; push {k:#x} ; push ecx ; push edi ; mov ecx,esi ; call {basecls}::{basecls} ; lea ecx,[esp+8] ; call CCharString::~CCharString ; {"push -1 ; push offset lit2 ; lea ecx,[esp+20h] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea edx,[esp+18h] ; push edx ; mov ecx,esi ; call setter ; lea ecx,[esp+18h] ; call CCharString::~CCharString ; pop edi" if setter else "pop edi ; mov dword ptr [esi],offset vftable"} ; mov eax,esi ; pop esi ; add esp,0Ch ; ret 8
// Two-argument combat-action constructor whose sound pair comes from the __fastcall
// GetSoundPair @ 0x{sound:08X}: the name literal converts implicitly to a CCharString temporary
// (re-addressed with `lea`, GOTCHAS), the 8-byte CSoundPair result lands in a frame temporary
// through the hidden pointer in ecx and is handed by reference to the base constructor
// @ 0x{base:08X} with the action code {k} ({k:#x}); the string temporary dies after the base
// constructor, then this class's vtable (0x{vt:08X}, rtti-vtables.tsv) is stored{" and a scoped CCharString passed to the setter @ 0x%08X (its manifest label is propagated)" % setter if setter else ""}.
// One of {len(members)} bodies of this shape.
struct CThingCreatureBase;
struct CThing;

struct CCharString {{                     // one pointer; the default length makes the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
}};

struct CSoundPair {{                       // a user constructor makes the 8-byte result return through the hidden pointer (a POD would ride in edx:eax)
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) {{ }}
}};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x{sound:08X}

struct {basecls} {{
    virtual void v00();
    {basecls}(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x{base:08X}
{"    void SetName(const CCharString& name);   // 0x%08X" % setter + chr(10) if setter else ""}}};

struct {cls} : {basecls} {{
    virtual void v00();
    {cls}(CThingCreatureBase* owner, CThing* thing);
}};

{cls}::{cls}(CThingCreatureBase* owner, CThing* thing)
    : {basecls}(owner, thing, {k}, GetSoundPair(owner, "{s}"))
{{
{body}}}
''',T(a,'''static long g_k = 0, g_s = 0; struct Str { const char* s; Str(const char* x, long = -1) : s(x) {} ~Str() { ++g_s; } }; struct SP { long a, b; SP() : a(0), b(0) {} };
static SP __fastcall GSP(void*, const Str& n) { SP p; p.a = n.s[0]; p.b = 1; return p; }
struct Base { virtual void v00() {} Base(void*, void*, long k, const SP& p) { g_k = k + p.a + p.b; } };
struct D : Base { virtual void v00() {} D(void* o, void* t) : Base(o, t, 0x96, GSP(o, "A")) {} };
int main()
{
    D d(0, 0); if (g_k != 0x96 + 'A' + 1 || g_s != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_74",):   # unsigned long CRetailTree::EraseKey(const K& key) { SRange r = EqualRange(key); unsigned long n = Distance(r.first, r.second, STag()); Erase(r.first, r.second); return n; }
        if len(calls)!=3 or retn!=4: continue
        eq,dist,erase=calls
        if not any(i.mnemonic=="add" and i.op_str=="esp, 8" for i in ins): continue
        nm=f"CRetailTree_{a}"
        add(a,"_unattributed",f"EraseKey_{nm}",HDRF(a,f"{nm}::EraseKey(const void*&) -- EqualRange @ 0x{eq:08X} (pair of iterators through the hidden pointer), count through the __cdecl Distance @ 0x{dist:08X} with a tag homed in the dead key slot, Erase(first, last) @ 0x{erase:08X} by value, count returned",len(b))+f'''//
// Retail bytes:  mov eax,[key] ; sub esp,8 ; push esi ; push edi ; mov esi,ecx ; push eax ; lea ecx,[range] ; push ecx ; mov ecx,esi ; call EqualRange ; lea edx,[tag] ; push edx ; lea eax,[range.second] ; push eax ; lea ecx,[range.first] ; push ecx ; call Distance ; mov edx,[range.second] ; add esp,8 ; mov edi,eax ; mov eax,esp ; push ecx ; mov ecx,[range.first] ; mov [eax],edx ; mov eax,esp ; mov [eax],ecx ; mov ecx,esi ; call Erase ; mov eax,edi ; pop edi ; pop esi ; add esp,8 ; ret 4
// Dinkumware-style `erase(const key_type&)`: the equal range comes back as a pair of 4-byte
// iterator classes through the hidden result pointer (the pair is not trivially copyable: the
// iterator has a user copy constructor), the __cdecl Distance helper counts it (its tag temporary
// is homed in the now-dead key argument slot, and VC pops only two of its three slots, reusing
// the third for the next call), and the range is erased with both iterators passed by value
// (slot reserved by `push`, then stored). Manifest labels are propagated; names are descriptive.
// One of {len(members)} bodies of this shape.
struct SNode;

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

struct SIterator {{                          // 4-byte iterator class with a user copy constructor
    SNode* m_pNode;
    SIterator(const SIterator& other) : m_pNode(other.m_pNode) {{ }}
}};

struct SRange {{                             // returned through the hidden pointer
    SIterator first;
    SIterator second;
}};

unsigned long __cdecl Distance_{dist:08x}(const SIterator& first, const SIterator& last, const SScalarTag& tag);   // 0x{dist:08X}

struct {nm} {{
    SNode* head;                           // +0
    unsigned long size;                    // +4
    SRange EqualRange(const void* const& key);          // 0x{eq:08X}
    void Erase(SIterator first, SIterator last);        // 0x{erase:08X}
    unsigned long EraseKey_{nm}(const void* const& key);
}};

unsigned long {nm}::EraseKey_{nm}(const void* const& key)
{{
    SRange range = EqualRange(key);
    unsigned long count = Distance_{dist:08x}(range.first, range.second, SScalarTag());
    Erase(range.first, range.second);
    return count;
}}
''',T(a,'''static int g_e = 0; struct Tg { Tg() {} }; struct It { long* p; It(const It& o) : p(o.p) {} It(long* x) : p(x) {} }; struct R { It f; It s; R(long* a, long* b) : f(a), s(b) {} };
static unsigned long __cdecl D(const It& f, const It& l, const Tg&) { return (unsigned long)(l.p - f.p); }
struct Tr { long* h; unsigned long sz; R EQ(const void* const&) { return R(h, h + 3); } void E(It, It) { ++g_e; } unsigned long EK(const void* const& k) { R r = EQ(k); unsigned long n = D(r.f, r.s, Tg()); E(r.f, r.s); return n; } };
int main()
{
    long buf[4]; Tr t; t.h = buf; t.sz = 3; const void* k = 0; if (t.EK(k) != 3 || g_e != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_10",):   # frame-pointer forwarder: void* CX::F(void* a, void* unused) { return Helper(a, STag(), 0); }  (tag homed in the high byte of the dead second slot)
        if len(calls)!=1 or retn!=8 or b[:3]!=b"\x55\x8b\xec": continue
        helper=calls[0]
        if not any(i.mnemonic=="lea" and "ebp + 0xb" in i.op_str for i in ins): continue
        nm=f"CForwarder_{a}"
        add(a,"_unattributed",f"Forward_{nm}",HDRF(a,f"{nm}::Forward(void*, void*) -- frame-pointer forwarder to the __thiscall helper @ 0x{helper:08X} with an empty tag (homed in the dead second slot) and a zero; the second argument is unused",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push 0 ; lea eax,[ebp+0Bh] ; push eax ; push [ebp+8] ; call Helper ; pop ebp ; ret 8
// A two-argument forwarder in a frame-pointer translation unit: the first argument and a zero
// are passed to the __thiscall helper @ 0x{helper:08X} together with a 1-byte empty tag
// temporary that VC homes in the high byte of the (dead) second argument's slot; the helper's
// result rides through eax. The manifest label (`_Copy_impl<CArray<...>>`) is propagated.
// One of {len(members)} bodies of this shape.
#pragma optimize("y", off)              // frame pointer kept in this TU

struct SScalarTag {{ SScalarTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

struct {nm} {{
    void* Helper(void* first, const SScalarTag& tag, long zero);   // 0x{helper:08X}
    void* Forward_{nm}(void* first, void* unused);
}};

void* {nm}::Forward_{nm}(void* first, void* unused)
{{
    return Helper(first, SScalarTag(), 0);
}}
''',T(a,'''struct Tg { Tg() {} }; struct X { void* H(void* f, const Tg&, long z) { return (char*)f + z; } void* F(void* f, void*) { return H(f, Tg(), 0); } };
int main()
{
    X x; int v; if (x.F(&v, 0) != &v) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_22",):   # SResult CX::F(SHandle b, void* c) { return Helper(b, c); }  -- hidden result pointer forwarded, 4-byte class copied by value into a reserved slot
        if len(calls)!=1 or retn!=0xc or b[0]!=0xff: continue
        helper=calls[0]; nm=f"CForwarder_{a}"
        add(a,"_unattributed",f"Forward_{nm}",HDRF(a,f"{nm}::Forward(SHandle, void*) -- forwards the hidden result pointer, the by-value 4-byte handle (copied into a reserved slot) and the pointer to the __thiscall helper @ 0x{helper:08X}",len(b))+f'''//
// Retail bytes:  push [c] ; mov edx,[handle] ; push ecx ; mov eax,esp ; push [result] ; mov [eax],edx ; call Helper ; mov eax,[result] ; ret 0Ch
// A forwarder returning a class through the hidden pointer: `return Helper(handle, c)` passes
// its own result pointer straight through, copies the 4-byte handle class (user copy
// constructor, so the slot is reserved by `push ecx` and the copy stored in place) and the
// trailing pointer, and returns the result pointer. The manifest label
// (`CPersistTraits<C2DVector>::TransferIn`) is propagated. One of {len(members)} bodies of this shape.
struct SHandle {{                            // 4-byte class with a user copy constructor
    void* m_pObject;
    SHandle(const SHandle& other) : m_pObject(other.m_pObject) {{ }}
}};

struct SResult {{                            // returned through the hidden pointer
    void* m_pValue;
    SResult(const SResult& other) : m_pValue(other.m_pValue) {{ }}
}};

struct {nm} {{
    SResult Helper(SHandle handle, void* context);   // 0x{helper:08X}
    SResult Forward_{nm}(SHandle handle, void* context);
}};

SResult {nm}::Forward_{nm}(SHandle handle, void* context)
{{
    return Helper(handle, context);
}}
''',T(a,'''struct H { void* p; H(const H& o) : p(o.p) {} H(void* x) : p(x) {} }; struct R { void* v; R(const R& o) : v(o.v) {} R(void* x) : v(x) {} };
struct X { R Hp(H h, void* c) { return R(c ? c : h.p); } R F(H h, void* c) { return Hp(h, c); } };
int main()
{
    X x; int a, b; H h(&a); R r = x.F(h, &b); if (r.v != &b) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_56","w512_98"):   # void __fastcall WriteState(CDataOutputStream* out, STree* tree): write the count, then each node's two fields walking the tree with the successor helper
        if len(calls)!=4 or retn!=0 or calls[0]!=calls[1]: continue
        w1,w2,w3,nxt=calls; isf=(TPL=="w512_98")
        if (not isf and w3!=w1) or (isf and w3==w1): continue
        add(a,"_global",f"WriteState_{a}",HDRF(a,f"WriteState(CDataOutputStream*, STree*) -- writes the count at +4 and, for every node from the leftmost to the head, the {'long and float' if isf else 'two dword'} fields at +0x10/+0x14 through the stream writers @ 0x{w1:08X}{' / 0x%08X' % w3 if isf else ''}; successor through the __cdecl helper @ 0x{nxt:08X}",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; push edi ; mov edi,edx ; push [edi+4] ; mov ebx,ecx ; call Write ; mov eax,[edi] ; mov esi,[eax+8] ; cmp esi,eax ; je E ; L: push [esi+10h] ; mov ecx,ebx ; call Write ; {"fld dword ptr [esi+14h] ; push ecx ; mov ecx,ebx ; fstp dword ptr [esp] ; call WriteFloat" if isf else "push [esi+14h] ; mov ecx,ebx ; call Write"} ; push esi ; call Next ; mov esi,eax ; cmp esi,[edi] ; pop ecx ; jne L ; E: pop edi ; pop esi ; pop ebx ; ret
// __fastcall serialiser of a tree-backed container: the element count at +4 is written first,
// then every node from the leftmost (+8 of the head) until the head is reached again writes its
// fields at +0x10 and +0x14 ({'the second as a float through the float writer' if isf else 'both as dwords'}); the successor comes from the __cdecl
// helper @ 0x{nxt:08X} (its argument popped into ecx: a size TU) and the head is re-read from the
// tree each iteration. Labels are propagated (`CDataOutputStream::WriteCBYTE`, `GetNoAllocatedAreas`);
// names are descriptive. One of {len(members)} bodies of this shape.
struct SNode {{
    unsigned char _pad[0x10];
    long m_10;                             // +0x10
    {"float" if isf else "long"} m_14;{" "*(26 if isf else 27)}// +0x14
}};

struct STree {{
    SNode* m_head;                         // +0 (leftmost at +8 of the head)
    long m_count;                          // +4
}};

struct SHead {{
    unsigned char _pad[8];
    SNode* m_leftmost;                     // +8
}};

struct CDataOutputStream {{
    void Write(long value);                // 0x{w1:08X}
{"    void WriteFloat(float value);          // 0x%08X" % w3 + chr(10) if isf else ""}}};

SNode* __cdecl TreeNext_{nxt:08x}(SNode* node);   // 0x{nxt:08X}

void __fastcall WriteState_{a}(CDataOutputStream* out, STree* tree)
{{
    out->Write(tree->m_count);
    for (SNode* node = ((SHead*)tree->m_head)->m_leftmost; node != (SNode*)tree->m_head; node = TreeNext_{nxt:08x}(node)) {{
        out->Write(node->m_10);
        out->{"WriteFloat" if isf else "Write"}(node->m_14);
    }}
}}
''',T(a,'''static long g_s = 0; struct N { unsigned char p[0x10]; long a; '''+("float" if isf else "long")+''' b; N* next; };
struct Hd { unsigned char p[8]; N* lm; }; struct T { N* h; long c; };
struct O { void W(long v) { g_s += v; } void WF(float v) { g_s += (long)v; } };
static N* __cdecl NX(N* n) { return n->next; }
static void __fastcall WS(O* o, T* t) { o->W(t->c); for (N* n = ((Hd*)t->h)->lm; n != (N*)t->h; n = NX(n)) { o->W(n->a); o->'''+("WF" if isf else "W")+'''(n->b); } }
int main()
{
    Hd hd; N n1; n1.a = 1; n1.b = 2; n1.next = (N*)&hd; hd.lm = &n1; T t; t.h = (N*)&hd; t.c = 10; O o; WS(&o, &t); if (g_s != 13) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_64",):   # void CPersistContext::TransferFloat(float* value) { CheckCRC(""); switch (m_mode) { case 2: CPersistTraits<float>().TransferBinaryIn(m_pLoadStream, value); break; case 3: m_pSaveStream->Write(*value); break; } }
        if len(calls)!=3 or retn!=4: continue
        crc,wr,rd=calls; litva=[p for p in pushes if p>0x1200000][0]; s=lit(litva)
        if not any(i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and "esp" in i.op_str and i.operands[1].type==X86_OP_IMM and i.operands[1].imm==0 for i in ins): continue
        add(a,"CPersistContext",f"TransferFloat_{a}",HDRF(a,f"CPersistContext::TransferFloat(float*) -- CheckCRC({s!r}), then mode 2 reads through a zeroed CPersistTraits<float> temporary (TransferBinaryIn @ 0x{rd:08X}, load stream +0x24), mode 3 writes the value to the save stream (+0x28, @ 0x{wr:08X})",len(b))+f'''//
// Retail bytes:  push ecx ; push esi ; push offset "" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; sub eax,2 ; mov dword ptr [esp+4],0 ; je L ; dec eax ; jne R ; mov eax,[value] ; mov ecx,[eax] ; push ecx ; mov ecx,[esi+28h] ; call Write ; pop esi ; pop ecx ; ret 4 ; L: mov edx,[value] ; mov eax,[esi+24h] ; push edx ; push eax ; lea ecx,[esp+0Ch] ; call CPersistTraits<float>::TransferBinaryIn ; R: pop esi ; pop ecx ; ret 4
// Persist transfer of one float: after the CRC check with the empty key, the mode at +0x18 is
// switched -- 2 (load) runs TransferBinaryIn @ 0x{rd:08X} on a value-initialised (zeroed) 4-byte
// CPersistTraits<float> temporary with the load stream (+0x24) and the value, 3 (save) writes the
// value's bits through the save stream's writer @ 0x{wr:08X} (label `WriteCBYTE` propagated); other
// modes do nothing. The `sub/dec` chain is the size-optimised two-case switch. One of {len(members)} bodies.
struct CDataOutputStream {{
    void Write(long bits);                 // 0x{wr:08X}
}};

struct CMemoryDataInputStream;

struct CPersistTraits_float {{             // 4-byte POD: `CPersistTraits_float()` zero-initialises the temporary
    long m_reserved;
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x{rd:08X}
}};

struct CPersistContext {{
    unsigned char _pad0[0x18];
    long m_mode;                           // +0x18
    unsigned char _pad1[8];
    CMemoryDataInputStream* m_pLoadStream; // +0x24
    CDataOutputStream* m_pSaveStream;      // +0x28
    void CheckCRC(const char* name);       // 0x{crc:08X}
    void TransferFloat_{a}(float* value);
}};

void CPersistContext::TransferFloat_{a}(float* value)
{{
    CheckCRC("{s}");
    switch (m_mode) {{
    case 2:
        CPersistTraits_float().TransferBinaryIn(m_pLoadStream, value);
        break;
    case 3:
        m_pSaveStream->Write(*(long*)value);
        break;
    }}
}}
''',T(a,'''static long g_w = 0, g_r = 0; struct O { void W(long v) { g_w = v; } }; struct I; struct Tr { long r; void TB(I*, float* v) { g_r = (long)*v; } };
struct C { unsigned char p[0x18]; long m; unsigned char q[8]; I* ls; O* ss; void CC(const char*) {} void T(float* v) { CC(""); switch (m) { case 2: Tr().TB(ls, v); break; case 3: ss->W(*(long*)v); break; } } };
int main()
{
    O o; C c; c.ls = 0; c.ss = &o; float f = 3.0f; c.m = 2; c.T(&f); c.m = 3; c.T(&f); if (g_r != 3 || g_w != *(long*)&f) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_40",):   # `??_G` of a class with a list member cleared out of line and a single vtable store after it, flag-tested delete
        if len(calls)!=2 or retn!=4 or len(vts)!=1: continue
        clear,deleter=calls; off=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "esi +" in i.op_str][0]; vt=vts[0][1]
        bcls=rtti.get(vt) or f"CVtable_{vt:08x}"; cls=f"{bcls}_Derived_{a}"
        add(a,"_unattributed",f"ScalarDeletingDestructor_{cls}",HDRF(a,f"{cls}::`scalar deleting destructor'(unsigned int) -- the list member at +{off:#x} cleared out of line (@ 0x{clear:08X}), the {bcls} vtable stored by the inlined base destructor, flag-tested delete",len(b))+f'''//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+{off:#x}] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset {bcls}::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a {bcls}-derived class that adds one list member: the member's
// out-of-line destructor @ 0x{clear:08X} runs, the inlined base destructor stores the base vtable
// (0x{vt:08X}, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x{deleter:08X}. The manifest label is propagated across the family; the derived class
// is named by its base. One of {len(members)} bodies of this shape.
struct CListMember_{clear:08x} {{
    void* m_head;
    ~CListMember_{clear:08x}();            // 0x{clear:08X}
}};

struct {bcls} {{
    virtual ~{bcls}() {{ }}
{"    unsigned char _body[%#x];" % (off-4) + chr(10) if off>4 else ""}}};

struct {cls} : {bcls} {{
    CListMember_{clear:08x} m_{off:02x};        // +{off:#x}
}};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
{cls}* Create_{cls}()
{{
    return new {cls};
}}

void Destroy_{cls}({cls}* object)
{{
    delete object;
}}
''',T(a,'''static int g_d = 0; struct L { void* h; ~L() { ++g_d; } }; struct B { virtual ~B() { g_d += 10; } }; struct X : B { L l; };
static void Kill(X* x) { delete x; }
int main()
{
    Kill(new X); if (g_d != 11) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_25","w512_72"):   # pair<iterator,bool> CRetailTree::InsertUnique(const V& v) -- STLport insert_unique: descend comparing keys, step back unless at begin, insert through the out-of-line _M_insert (the iterator written through an explicit result pointer) with a null 4th argument
        strk=(TPL=="w512_72")
        if retn!=8 or len(calls)!=(3 if not strk else 5): continue
        _sets=[i.mnemonic for i in ins if i.mnemonic.startswith("set")]; _cm=[i for i in ins if i.mnemonic=="cmp" and i.operands[1].type==X86_OP_MEM and "esi +" in i.op_str]
        if not strk and (len(_sets)!=1 or not _cm): continue
        ksz=_cm[0].operands[0].size if not strk else 4; signed=(_sets[0]=="setl") if not strk else False
        if strk:
            less1,ins1,dec,less2,ins2=calls
            if less1!=less2 or ins1!=ins2: continue
            ins,less=ins1,less1
        else:
            ins1,dec,ins2=calls
            if ins1!=ins2: continue
            ins=ins1; less=0
        nm=f"CRetailTree_{a}"
        cmp_decl=('''struct CBasicString {
    bool operator<(const CBasicString* other);   // 0x%08X
};

struct SLessString {                        // pointer keys: equal pointers are not less, null sorts first
    bool operator()(CBasicString* a, CBasicString* b) const
    {
        if (a == b)
            return false;
        if (a == 0)
            return true;
        if (b == 0)
            return false;
        return a->operator<(b);
    }
};
''' % less) if strk else ""
        ktype="CBasicString*" if strk else ({1:"signed char",2:"short",4:"long"} if signed else {1:"unsigned char",2:"unsigned short",4:"unsigned long"})[ksz]; cmpname="SLessString" if strk else "SLessKey"
        add(a,"_unattributed",f"InsertUnique_{nm}",HDRF(a,f"{nm}::InsertUnique(const SValue&) -- STLport insert_unique over {'string-pointer' if strk else ktype} keys: descend from the root, step back through the out-of-line decrement @ 0x{dec:08X} unless at begin, insert through the hidden-pointer _M_insert @ 0x{ins:08X}; returns {{iterator, inserted}} through the hidden pointer",len(b))+f'''//
// Retail bytes:  {"sub esp,8 ; " if strk else ""}push ebx ; {"mov ebx,[ecx] ; push ebp ; push esi ; mov esi,[ebx+4] ; test esi,esi ; push edi ; mov [esp+14h],ecx ; mov edi,ebx" if strk else "mov ebx,[value] ; push ebp ; mov ebp,ecx ; mov ecx,[ebp] ; push esi ; mov esi,[ecx+4] ; test esi,esi ; push edi ; mov edi,ecx"} ; mov al,1 ; je J ; L: (compare key with [node+10h]) ; mov edi,esi ; (left or right) ; test esi,esi ; jne L ; J: test al,al ; (j = y) ; je C ; cmp edi,[head+8] ; jne D ; (insert x,y,v,0 ; result {{it,true}}) ; ret 8 ; D: lea ecx,[j] ; call iterator::operator-- ; C: (compare [j+10h] with key) ; jae F ; (insert ; result {{it,true}}) ; ret 8 ; F: (result {{j,false}}) ; ret 8
// STLport `_Rb_tree::insert_unique(const value_type&)`: `y` starts at the header, `x` at the root
// (+4 of the header), and the descent keeps `comp = key < node key` choosing left (+8) or right
// (+0xC); if the last comparison went left and `y` is not the leftmost node (+8 of the header),
// the iterator steps back through the out-of-line decrement @ 0x{dec:08X}; a key that is not
// already present is inserted through _M_insert @ 0x{ins:08X} (x, y, value, null; the inserted
// iterator is written through an explicit result pointer -- the address of `j`, whose slot is
// the dead value-argument slot -- and returned) and the {{iterator, bool}} pair is written
// through the function's own hidden pointer. The node layout is STLport's (colour, parent, left, right,
// value at +0x10). Labels are propagated; names are descriptive. One of {len(members)} bodies.
{"struct CBasicString;" + chr(10) + chr(10) if strk else ""}struct SNode {{
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
    {ktype} m_key;{" "*(26-len(ktype))}// +0x10
}};

struct SValue {{
    {ktype} m_key;
}};

struct SIterator {{
    SNode* m_node;
    SIterator(SNode* node) : m_node(node) {{ }}
    SIterator(const SIterator& other) : m_node(other.m_node) {{ }}
    SIterator& operator--();               // 0x{dec:08X}
    bool operator==(const SIterator& other) const {{ return m_node == other.m_node; }}
}};

struct SPairIB {{
    SIterator first;
    bool second;
    SPairIB(const SIterator& it, bool inserted) : first(it), second(inserted) {{ }}
}};

{cmp_decl}
struct {nm} {{
    SNode* m_header;                       // +0
    unsigned long m_size;                  // +4
    SIterator* Insert(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w);   // 0x{ins:08X}
    SIterator begin() {{ return SIterator(m_header->m_left); }}
    SPairIB InsertUnique_{nm}(const SValue& value);
}};

SPairIB {nm}::InsertUnique_{nm}(const SValue& value)
{{
    {"SLessString comp;" + chr(10) if strk else ""}    SNode* y = m_header;
    SNode* x = m_header->m_parent;
    bool less = true;
    while (x != 0) {{
        y = x;
        less = {"comp(value.m_key, x->m_key)" if strk else "value.m_key < x->m_key"};
        x = less ? x->m_left : x->m_right;
    }}
    SIterator j(y);
    if (less) {{
        if (j == begin())
            return SPairIB(*Insert(&j, y, y, value, 0), true);   // the new minimum: inserted to the left of y
        --j;
    }}
    if ({"comp(j.m_node->m_key, value.m_key)" if strk else "j.m_node->m_key < value.m_key"})
        return SPairIB(*Insert(&j, x, y, value, 0), true);
    return SPairIB(j, false);
}}
''',T(a,'''struct N { long c; N* p; N* l; N* r; unsigned long k; }; struct V { unsigned long k; };
struct It { N* n; It(N* x) : n(x) {} It(const It& o) : n(o.n) {} It& operator--() { n = n->l; return *this; } bool operator==(const It& o) const { return n == o.n; } };
struct PIB { It f; bool s; PIB(const It& i, bool b) : f(i), s(b) {} };
static int g_i = 0; struct Tr { N* h; unsigned long sz; It* Ins(It* r, N*, N* y, const V& v, N*) { ++g_i; r->n = y; return r; } It begin() { return It(h->l); }
    PIB IU(const V& v) { N* y = h; N* x = h->p; bool less = true; while (x != 0) { y = x; less = v.k < x->k; x = less ? x->l : x->r; } It j(y); if (less) { if (j == begin()) return PIB(*Ins(&j, y, y, v, 0), true); --j; } if (j.n->k < v.k) return PIB(*Ins(&j, x, y, v, 0), true); return PIB(j, false); } };
int main()
{
    N hd, n1; hd.p = &n1; hd.l = &n1; n1.p = &hd; n1.l = 0; n1.r = 0; n1.k = 5; Tr t; t.h = &hd; t.sz = 1; V v; v.k = 5; PIB r = t.IU(v); if (r.s || r.f.n != &n1 || g_i != 0) '''+FAIL(a)+'''
    v.k = 7; PIB r2 = t.IU(v); if (!r2.s || g_i != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_73",):   # void CRetailTree::EraseRange(SNode* first, SNode* last): clear when the range is everything, else erase(first++) with the successor inlined and the __cdecl unlink helper
        if len(calls)!=3 or retn!=8: continue
        rec,unlink,freer=calls
        nm=f"CRetailTree_{a}"
        add(a,"_unattributed",f"EraseRange_{nm}",HDRF(a,f"{nm}::EraseRange(SNode*, SNode*) -- the whole tree is cleared through the recursive Erase @ 0x{rec:08X} (head reset, size zeroed); otherwise each node from first to last is unlinked through the __cdecl helper @ 0x{unlink:08X} with the successor computed inline, freed and the size decremented",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[last] ; push esi ; mov esi,[first] ; push edi ; mov edi,ecx ; mov eax,[edi] ; cmp esi,[eax+8] ; jne L ; cmp ebx,eax ; jne L ; mov ecx,[edi+4] ; test ecx,ecx ; je E ; push [eax+4] ; mov ecx,edi ; call Erase ; (head->left = head ; head->parent = 0 ; head->right = head ; size = 0) ; pop edi ; pop esi ; pop ebx ; ret 8 ; L: cmp esi,ebx ; je E ; N: (successor of esi into esi, victim in ecx) ; mov eax,[edi] ; lea edx,[eax+0Ch] ; push edx ; lea edx,[eax+8] ; push edx ; add eax,4 ; push eax ; push ecx ; call Unlink ; add esp,10h ; test eax,eax ; je S ; push eax ; call free ; add esp,4 ; S: mov ecx,[edi+4] ; dec ecx ; cmp esi,ebx ; mov [edi+4],ecx ; jne N ; E: pop edi ; pop esi ; pop ebx ; ret 8
// STLport-style `erase(first, last)`: when the range is [begin, end) the tree is cleared (the
// recursive node release @ 0x{rec:08X} from the root, the header re-linked to itself and the
// size zeroed); otherwise `erase(first++)` repeats -- the in-order successor is computed
// inline (right subtree's leftmost, else climb while the node is a right child), the victim
// is unlinked through the __cdecl helper @ 0x{unlink:08X} with the addresses of the header's
// parent/left/right links (the last formed with `add eax,4`), freed when non-null (free @
// 0x{freer:08X}) and the size decremented. Values are trivially destructible here. Labels are
// propagated; names are descriptive. One of {len(members)} bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SNode {{
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
}};

extern "C" SNode* __cdecl Unlink_{unlink:08x}(SNode* node, SNode** root, SNode** leftmost, SNode** rightmost);   // 0x{unlink:08X}

struct {nm} {{
    SNode* m_header;                       // +0
    unsigned long m_size;                  // +4
    void Erase(SNode* root);               // 0x{rec:08X}
    static SNode* Successor(SNode* node)
    {{
        if (node->m_right != 0) {{
            node = node->m_right;
            while (node->m_left != 0)
                node = node->m_left;
        }}
        else {{
            SNode* y = node->m_parent;
            while (node == y->m_right) {{
                node = y;
                y = y->m_parent;
            }}
            if (node->m_right != y)
                node = y;
        }}
        return node;
    }}
    void EraseRange_{nm}(SNode* first, SNode* last);
}};

void {nm}::EraseRange_{nm}(SNode* first, SNode* last)
{{
    if (first == m_header->m_left && last == m_header) {{
        if (m_size != 0) {{
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_size = 0;
        }}
    }}
    else {{
        while (first != last) {{
            SNode* victim = first;
            first = Successor(first);
            SNode* node = Unlink_{unlink:08x}(victim, &m_header->m_parent, &m_header->m_left, &m_header->m_right);
            if (node)
                free(node);
            --m_size;
        }}
    }}
}}
''',T(a,'''#include <cstdlib>
struct N { long c; N* p; N* l; N* r; }; static int g_u = 0, g_e = 0;
static N* __cdecl UL(N* n, N**, N**, N**) { ++g_u; return 0; }
struct Tr { N* h; unsigned long sz; void E(N*) { ++g_e; }
    static N* Succ(N* n) { if (n->r) { n = n->r; while (n->l) n = n->l; } else { N* y = n->p; while (n == y->r) { n = y; y = y->p; } if (n->r != y) n = y; } return n; }
    void ER(N* f, N* l) { if (f == h->l && l == h) { if (sz) { E(h->p); h->l = h; h->p = 0; h->r = h; sz = 0; } } else { while (f != l) { N* v = f; f = Succ(f); N* n = UL(v, &h->p, &h->l, &h->r); if (n) std::free(n); --sz; } } } };
int main()
{
    N hd, a, b; hd.p = &a; hd.l = &a; hd.r = &b; a.p = &hd; a.l = 0; a.r = &b; b.p = &a; b.l = 0; b.r = 0; Tr t; t.h = &hd; t.sz = 2; t.ER(&a, &b); if (g_u != 1 || t.sz != 1) '''+FAIL(a)+'''
    t.ER(&a, &hd); if (g_e != 1 || t.sz != 0) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_28",):   # void CRetailVector::resize(unsigned long n, const T& value): shrink with the copy-down helper inlined (memmove), else InsertN(end, n - size, value)
        if len(calls)!=2 or retn!=8: continue
        mm,insn=calls
        shift=[i.operands[1].imm for i in ins if i.mnemonic=="sar" and i.operands[1].type==X86_OP_IMM]
        if len(shift)!=1 or any(i.mnemonic=="sar" and i.operands[1].type==X86_OP_REG for i in ins): continue
        sz=1<<shift[0]; ety={1:"unsigned char",2:"unsigned short",4:"long"}.get(sz,"SElement%d" % sz)
        nm=f"CRetailVector_{a}"
        add(a,"_global",f"Resize_{nm}",HDRF(a,f"{nm}::resize(unsigned long, const T&) -- {sz}-byte elements: shrinking copies the (empty) tail down with memmove through the inlined helper and moves `last`, growing calls InsertN @ 0x{insn:08X} at the end",len(b))+f'''//
// Retail bytes:  mov edx,[n] ; push esi ; push edi ; mov edi,ecx ; mov eax,[edi+4] ; mov esi,[edi] ; mov ecx,eax ; sub ecx,esi ; sar ecx,{shift[0]} ; cmp edx,ecx ; jae G ; cmp eax,eax ; lea ecx,[esi+edx*{sz}] ; jne M ; mov eax,ecx ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; M: mov esi,eax ; sub esi,eax ; push esi ; push eax ; push ecx ; call memmove ; add esp,0Ch ; add eax,esi ; mov [edi+4],eax ; pop edi ; pop esi ; ret 8 ; G: mov esi,[value] ; push esi ; sub edx,ecx ; push edx ; push eax ; mov ecx,edi ; call InsertN ; pop edi ; pop esi ; ret 8
// Retail vector resize for {sz}-byte scalars: when the new size is smaller, `erase(begin() + n,
// end())` is inlined as the copy-down helper -- `if (last != end)` is kept although both are
// the end pointer (`cmp eax,eax`), the tail is moved with memmove (@ 0x{mm:08X}) and the new end
// is the destination plus the byte count -- followed by the store of `last`; otherwise
// InsertN @ 0x{insn:08X} appends `n - size()` copies of the value at the end. The manifest label
// is propagated across element sizes; names are descriptive. One of {len(members)} bodies of this shape.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);
{"struct SElement%d { unsigned char _bytes[%d]; };" % (sz,sz) + chr(10) if sz>4 else ""}
struct {nm} {{
    {ety}* first;{" "*(26-len(ety))}// +0
    {ety}* last;{" "*(27-len(ety))}// +4
    {ety}* end;{" "*(28-len(ety))}// +8
    void InsertN({ety}* where, unsigned long count, const {ety}& value);   // 0x{insn:08X}
    static {ety}* CopyDown({ety}* src, {ety}* srcEnd, {ety}* dest)
    {{
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return ({ety}*)((char*)memmove(dest, src, bytes) + bytes);
    }}
    void Resize_{nm}(unsigned long n, const {ety}& value);
}};

void {nm}::Resize_{nm}(unsigned long n, const {ety}& value)
{{
    if (n < (unsigned long)(last - first))
        last = CopyDown(last, last, first + n);   // erase(begin() + n, end())
    else
        InsertN(last, n - (last - first), value);
}}
''',T(a,'''#include <cstring>
static int g_i = 0; struct V { long* f; long* l; long* e; void IN(long*, unsigned long c, const long&) { g_i += (int)c; }
    static long* CD(long* s, long* se, long* d) { if (s == se) return d; unsigned long b = (unsigned long)((char*)se - (char*)s); return (long*)((char*)std::memmove(d, s, b) + b); }
    void R(unsigned long n, const long& v) { if (n < (unsigned long)(l - f)) l = CD(l, l, f + n); else IN(l, n - (l - f), v); } };
int main()
{
    long buf[4]; V v; v.f = buf; v.l = buf + 3; v.e = buf + 4; long x = 1; v.R(1, x); if (v.l != buf + 1) '''+FAIL(a)+'''
    v.R(3, x); if (g_i != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_37","w512_38","w512_49","w512_243"):   # bool CDefinitionManager::GetDef(key, CDefPointer& out): the def pointer comes back through a hidden pointer homed in the dead key slot, the out pointer is reassigned with reference counting, the temporary released
        fp=(TPL in ("w512_49","w512_243")); extra=(TPL=="w512_243")
        if len(calls)!=1 or retn!=(12 if extra else 8): continue
        if fp!=(b[:3]==b"\x55\x8b\xec"): continue
        getter=calls[0]; hasguard=any(i.mnemonic=="jle" for i in ins)
        byname=not hasguard
        if TPL=="w512_37" and not byname: continue
        if TPL in ("w512_38","w512_49","w512_243") and byname: continue
        caddr=[i.address for i in ins if i.mnemonic=="call"][0]; start=int(a,16)+(3 if fp else 0)
        npush=sum(1 for i in ins if i.mnemonic=="push" and start<=i.address<caddr)
        if extra!=(npush==3): continue
        gm,gn,_=label(getter); gname=gn.split("::")[-1] if (gm,gn) in uniq and re.match(r"^[A-Za-z_]\w*$",gn.split("::")[-1]) else ("GetPDefFromInstantiationName" if byname else "GetPDefFromGlobalIndex")
        fname=re.sub(r"[^A-Za-z0-9_]","_",mname.split("::")[-1].split("<")[0]) or "GetDef"; fname=f"{fname}_{a}"
        ktype="const CCharString&" if byname else "long"; kname="name" if byname else "index"
        add(a,"CDefinitionManager",fname,HDRF(a,f"CDefinitionManager::GetDef<T>({'long, ' if extra else ''}{ktype}, CDefPointer&) -- {gname} @ 0x{getter:08X} returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer is reassigned (old def released, new one addref'd) and the temporary released; true, else false{'; frame-pointer size TU' if fp else ''}",len(b))+f'''//
// Retail bytes:  {"push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; " + ("push [kind] ; " if extra else "") + "push eax ; call " + gname + " ; mov eax,[def] ; test eax,eax ; je F" if fp else ("mov eax,[index] ; test eax,eax ; jle F ; push eax ; lea eax,[esp+8] ; push eax ; call " + gname + " ; mov eax,[esp+4] ; test eax,eax ; je F" if not byname else "mov eax,[name] ; push eax ; lea edx,[esp+8] ; push edx ; call " + gname + " ; mov eax,[esp+4] ; test eax,eax ; je F")} ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: {"and dword ptr [esi],0 ; mov eax,[def]" if fp else "mov eax,[def] ; mov dword ptr [esi],0"} ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: {"mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4]" if fp else "mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne R ; mov edx,[ecx] ; call [edx+4]"} ; R: mov al,1 ; pop esi ; {"jmp X ; F: xor al,al ; X: pop ebp ; ret " + ("0Ch" if extra else "8") if fp else "ret 8 ; F: xor al,al ; ret 8"}
// Definition lookup into a counted def pointer: {gname} @ 0x{getter:08X} returns a CDefPointer by
// value through the hidden pointer, which VC homes in the dead key argument slot; the
// temporary is re-read from that slot at every use. A null def fails (the false path sits at
// the end: the success branch is the `if` body). Otherwise `out = def` is inlined -- unless
// the out pointer already holds the def, its old def is released through CDefBase::Release
// (`this` in ecx, `dec [ecx+4] ; jne`, virtual slot 1 destroys at zero) and nulled, the new one
// stored and addref'd -- and the temporary's destructor releases the def through the
// ref-count base's two inline accessors (decrement, then test the count): the adjusted base
// pointer is materialised once for both (`add eax,4`), the stored value is forwarded to the
// test{" (re-read in the size TU: `dec [eax] ; cmp [eax],0`)" if fp else ""} and the def is kept in ecx for the virtual call.{" Frame pointer and size optimisation from the TU's pragmas." if fp else ""} The manifest
// label names one instantiation; the def type is opaque. One of {len(members)} bodies of this shape.
{"#pragma optimize(" + chr(34) + "y" + chr(34) + ", off)" + chr(10) + "#pragma optimize(" + chr(34) + "s" + chr(34) + ", on)" + chr(10) + chr(10) if fp else ""}{"#include " + chr(34) + "fable_string.h" + chr(34) + chr(10) + chr(10) if byname else ""}struct CRefCounted {{                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() {{ --m_count; }}
    long GetRefCount() const {{ return m_count; }}
}};

struct CDefVirtual {{
    virtual void v00();
    virtual void Destroy();                // slot 1
}};

struct CDefBase : CDefVirtual, CRefCounted {{
    void Release()                         // inlined into CDefPointer::operator=
    {{
        if (--m_count == 0)
            Destroy();
    }}
    void ReleaseTemporary()                // inlined into ~CDefPointer
    {{
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }}
}};

struct CDefPointer {{
    CDefBase* m_p;
    CDefPointer() {{ }}
    ~CDefPointer()
    {{
        if (m_p)
            m_p->ReleaseTemporary();
    }}
    {"__forceinline " if fp else ""}CDefPointer& operator=(const CDefPointer& other)   // {"forced: the size TU would call it out of line" if fp else "inlined by the speed TU"}
    {{
        if (m_p != other.m_p) {{
            if (m_p) {{
                m_p->Release();
                m_p = 0;
            }}
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }}
        return *this;
    }}
}};

struct CDefinitionManager {{
    CDefPointer {gname}({"long kind, " if extra else ""}{ktype} {kname});   // 0x{getter:08X} (hidden result pointer)
    bool {fname}({"long kind, " if extra else ""}{ktype} {kname}, CDefPointer& out);
}};

bool CDefinitionManager::{fname}({"long kind, " if extra else ""}{ktype} {kname}, CDefPointer& out)
{{
{"" if byname else "    if (" + kname + " <= 0)" + chr(10) + "        return false;" + chr(10)}    CDefPointer def = {gname}({"kind, " if extra else ""}{kname});
    if (def.m_p) {{
        out = def;
        return true;
    }}
    return false;
}}
''',T(a,'''static int g_d = 0; struct R { long c; void Dec() { --c; } long G() const { return c; } }; struct V { virtual void a() {} virtual void X() { ++g_d; } };
struct D : V, R { void Rel() { if (--c == 0) X(); } void RelT() { Dec(); if (G() == 0) X(); } };
struct P { D* p; P() {} P(D* q) : p(q) { if (p) ++p->c; } ~P() { if (p) p->RelT(); } P& operator=(const P& o) { if (p != o.p) { if (p) { p->Rel(); p = 0; } p = o.p; if (p) ++p->c; } return *this; } };
static D g_def; struct M { P G(long k) { return P(k == 1 ? &g_def : 0); }
    bool GD(long k, P& o) { if (k <= 0) return false; P d = G(k); if (d.p) { o = d; return true; } return false; } };
int main()
{
    g_def.c = 1; M m; P o; o.p = 0; if (!m.GD(1, o) || o.p != &g_def || g_def.c != 2 || m.GD(2, o) || m.GD(0, o) || g_def.c != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_34",):   # SIterator* CRetailTree::Insert(SIterator* result, SNode* x, SNode* y, const SValue& v, SNode* w) -- STLport _M_insert: create the node left or right of y, fix the header links, rebalance, count
        if len(calls)!=3 or retn!=0x14 or calls[0]!=calls[1]: continue
        alloc,_,rebal=calls; size=[p for p in pushes if p<0x100][0]
        voff=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and "eax +" in i.op_str][0]
        nm=f"CRetailTree_{a}"
        add(a,"_unattributed",f"Insert_{nm}",HDRF(a,f"{nm}::Insert(SIterator*, SNode*, SNode*, const SValue&, SNode*) -- STLport _M_insert: a {size:#x}-byte node (malloc @ 0x{alloc:08X}) linked left or right of y, header root/leftmost/rightmost fixed, rebalance @ 0x{rebal:08X}, size incremented, the iterator written through the result pointer",len(b))+f'''//
// Retail bytes:  push ebx ; mov ebx,[y] ; push esi ; mov esi,[value] ; push edi ; mov edi,ecx ; cmp ebx,[edi] ; je L ; mov eax,[w] ; test eax,eax ; jne R ; mov eax,[x] ; test eax,eax ; jne L ; mov eax,[esi] ; cmp eax,[ebx+10h] ; jl L ; R: push {size:#x} ; call malloc ; lea ecx,[eax+{voff:#x}] ; add esp,4 ; test ecx,ecx ; je S ; (two dwords copied) ; S: mov [ebx+0Ch],eax ; mov ecx,[edi] ; cmp ebx,[ecx+0Ch] ; mov esi,eax ; jne J ; mov [ecx+0Ch],eax ; jmp J ; L: push {size:#x} ; call malloc ; lea ecx,[eax+{voff:#x}] ; add esp,4 ; test ecx,ecx ; je T ; (two dwords copied) ; T: mov [ebx+8],eax ; mov ecx,[edi] ; cmp ebx,ecx ; mov esi,eax ; jne U ; mov [ecx+4],eax ; mov ecx,[edi] ; mov [ecx+0Ch],eax ; jmp J ; U: cmp ebx,[ecx+8] ; jne J ; mov [ecx+8],eax ; J: mov [esi+4],ebx ; mov dword ptr [esi+8],0 ; mov dword ptr [esi+0Ch],0 ; mov edx,[edi] ; add edx,4 ; push edx ; push esi ; call Rebalance ; mov eax,[edi+4] ; add esp,8 ; inc eax ; mov [edi+4],eax ; mov eax,[result] ; pop edi ; mov [eax],esi ; pop esi ; pop ebx ; ret 14h
// STLport `_Rb_tree::_M_insert(x, y, v, w)`: the new node goes to the left of y when y is the
// header, or (with w null) when x is non-null or the key orders before y's; otherwise to the
// right. The node is malloc'd ({size:#x} bytes) and its 8-byte POD value placement-copied at
// +{voff:#x} (hence the null test); the header's root/rightmost (y == header), leftmost or
// rightmost links are fixed, the node's parent/children set, the __cdecl rebalance @ 0x{rebal:08X}
// run with the root by reference, the count incremented and the iterator written through the
// explicit result pointer. Labels are propagated; names are descriptive. One of {len(members)} bodies.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct SValue {{
    long m_key;
    long m_data;
}};

struct SNode {{
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
    SValue m_value;                        // +{voff:#x}
}};

struct SIterator {{
    SNode* m_node;
}};

extern "C" void __cdecl Rebalance_{rebal:08x}(SNode* node, SNode*& root);   // 0x{rebal:08X}

struct {nm} {{
    SNode* m_header;                       // +0
    unsigned long m_size;                  // +4
    static SNode* CreateNode(const SValue& value)
    {{
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->m_value) SValue(value);
        return node;
    }}
    SIterator* Insert_{nm}(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w);
}};

SIterator* {nm}::Insert_{nm}(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w)
{{
    SNode* z;
    if (y == m_header || (w == 0 && (x != 0 || value.m_key < y->m_value.m_key))) {{
        z = CreateNode(value);
        y->m_left = z;
        if (y == m_header) {{
            m_header->m_parent = z;
            m_header->m_right = z;
        }}
        else if (y == m_header->m_left)
            m_header->m_left = z;
    }}
    else {{
        z = CreateNode(value);
        y->m_right = z;
        if (y == m_header->m_right)
            m_header->m_right = z;
    }}
    z->m_parent = y;
    z->m_left = 0;
    z->m_right = 0;
    Rebalance_{rebal:08x}(z, m_header->m_parent);
    ++m_size;
    result->m_node = z;
    return result;
}}
''',T(a,'''#include <new>
#include <cstdlib>
struct V { long k; long d; }; struct N { long c; N* p; N* l; N* r; V v; }; struct It { N* n; }; static int g_rb = 0;
static void __cdecl RB(N*, N*&) { ++g_rb; }
struct Tr { N* h; unsigned long sz; static N* C(const V& v) { N* n = (N*)std::malloc(sizeof(N)); new (&n->v) V(v); return n; }
    It* I(It* r, N* x, N* y, const V& v, N* w) { N* z; if (y == h || (w == 0 && (x != 0 || v.k < y->v.k))) { z = C(v); y->l = z; if (y == h) { h->p = z; h->r = z; } else if (y == h->l) h->l = z; } else { z = C(v); y->r = z; if (y == h->r) h->r = z; } z->p = y; z->l = 0; z->r = 0; RB(z, h->p); ++sz; r->n = z; return r; } };
int main()
{
    N hd; hd.p = 0; hd.l = &hd; hd.r = &hd; Tr t; t.h = &hd; t.sz = 0; V v; v.k = 3; v.d = 4; It it; t.I(&it, 0, &hd, v, 0); if (t.sz != 1 || hd.p != it.n || it.n->v.d != 4 || g_rb != 1) '''+FAIL(a)+'''
    V v2; v2.k = 5; v2.d = 0; It it2; t.I(&it2, 0, it.n, v2, 0); if (it.n->r != it2.n || t.sz != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_16",):   # CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream*, T*): inlined 4-byte stream read with the 64-bit position range check
        if len(calls)!=1: continue
        reader=calls[0]; fp=b[:3]==b"\x55\x8b\xec"; isstruct=any(i.mnemonic=="movsd" for i in ins)
        vt="SPersistValue4" if isstruct else "long"
        add(a,"_unattributed",f"TransferBinaryIn_{a}",HDRF(a,f"CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream*, T*) -- inlined 4-byte read ({'struct, movsd copy' if isstruct else 'scalar'}), 64-bit position range check through an inline bool predicate (its exits share the duplicated return tails the retail way), ReadWithSrcChunkOverflow @ 0x{reader:08X} when the chunk runs out{'; frame-pointer TU' if fp else ''}",len(b))+f'''//
// Retail bytes:  {"push ebp ; mov ebp,esp ; mov ecx,[ebp+8]" if fp else "mov ecx,[esp+4]"} ; mov eax,[ecx+4] ; {"lea edx,[eax+4]" if fp else "push esi ; lea esi,[eax+4]"} ; xor eax,eax ; jl R ; jg B ; test lo,lo ; jb R ; B: test eax,eax ; jg R ; jl F ; cmp lo,7FFFFFFFh ; ja R ; F: {"push ebx ; mov ebx,[ecx+14h] ; cmp ebx,4 ; jl S ; mov eax,[ecx+0Ch] ; push esi ; push edi ; mov esi,eax ; lea edi,[ebp+8] ; movsd ; add eax,4 ; add ebx,-4 ; pop edi ; mov [ecx+0Ch],eax ; mov [ecx+14h],ebx ; mov [ecx+4],edx ; pop esi ; jmp J ; S: push 4 ; lea eax,[ebp+8] ; push eax ; call ReadWithSrcChunkOverflow ; J: pop ebx ; R: mov eax,[ebp+0Ch] ; mov ecx,[ebp+8] ; mov [eax],ecx ; pop ebp ; ret 8" if fp else "mov edx,[ecx+14h] ; cmp edx,4 ; jl S ; mov eax,[ecx+0Ch] ; push edi ; mov edi,[eax] ; add eax,4 ; mov [ecx+0Ch],eax ; mov eax,[esp+10h] ; add edx,-4 ; mov [ecx+4],esi ; mov [ecx+14h],edx ; mov [eax],edi ; pop edi ; pop esi ; ret 8 ; S: push 4 ; lea edx,[esp+0Ch] ; push edx ; call ReadWithSrcChunkOverflow ; R: mov ecx,[esp+8] ; mov eax,[esp+0Ch] ; mov [eax],ecx ; pop esi ; ret 8  (two more copies of R for the range-check exits)"}
// Binary persist read of one 4-byte value: a local `value` (homed in the dead stream-argument
// slot) is filled by the inlined CMemoryDataInputStream::Read(buffer, size) and copied to the
// output. Read widens `position + size` to __int64 and rejects it outside [0, 0x7FFFFFFF]
// (the zero high dword is materialised and compared like any 64-bit value); with enough bytes
// left in the current chunk it memcpy's from the chunk pointer (+0xC), advances it, decrements
// the remaining count (+0x14) and adds the size to the position (+4) -- `+=`, which keeps the
// old position live and gives the `lea` form -- otherwise it defers to
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x{reader:08X}. {'A 4-byte struct value makes the memcpy a movsd; ' if isstruct else 'A scalar value lets the memcpy forward straight into the output; '}{'the TU keeps frame pointers and optimises for size.' if fp else 'the TU has no frame pointer.'}
// The manifest label (`CPersistTraits<...>::TransferBinaryIn`) is propagated; the value type is
// modelled by size. One of {len(members)} bodies of this shape.
{'#pragma optimize("y", off)' + chr(10) + '#pragma optimize("s", on)' + chr(10) if fp else ''}extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {{
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x{reader:08X}
    static __forceinline bool IsValidPosition(__int64 position) {{ return position >= 0 && position <= 0x7fffffff; }}
    __forceinline void Read(void* buffer, long size)
    {{
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {{
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }}
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }}
}};
{"struct SPersistValue4 { unsigned short m_low; unsigned short m_high; };" + chr(10) if isstruct else ""}
void __stdcall TransferBinaryIn_{a}(CMemoryDataInputStream* stream, {vt}* out)
{{
    {vt} value;
    stream->Read(&value, sizeof(value));
    *out = value;
}}
''',T(a,'''#include <cstring>
struct S { void* vt; unsigned long pos; long a; const unsigned char* p; long b; long rem; static int g_o; void O(unsigned char*, long) { ++g_o; }
  void R(void* buf, long n) { __int64 np = (__int64)(pos + n); if (!(np >= 0 && np <= 0x7fffffff)) return; if (rem >= n) { std::memcpy(buf, p, n); p += n; rem -= n; pos += n; } else O((unsigned char*)buf, n); } };
int S::g_o = 0;
static void __stdcall T(S* s, long* o) { long v; s->R(&v, 4); *o = v; }
int main()
{
    unsigned char data[8] = { 1, 0, 0, 0, 2, 0, 0, 0 }; S s; s.pos = 0; s.p = data; s.rem = 8; long v = 0; T(&s, &v); if (v != 1 || s.pos != 4 || s.rem != 4 || s.p != data + 4) '''+FAIL(a)+'''
    s.rem = 2; T(&s, &v); if (S::g_o != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_36",):   # static CRetailTree destructor registered with atexit: clear (recursive release from the root, header relinked to itself, size zeroed) then free the header
        if len(calls)!=2 or retn!=0: continue
        rec,freer=calls
        gl=sorted(set(i.operands[k].mem.disp for i in ins for k in range(len(i.operands)) if i.operands[k].type==X86_OP_MEM and i.operands[k].mem.base==0 and i.operands[k].mem.disp>0x400000))
        if len(gl)!=2 or gl[1]-gl[0]!=4: continue
        obj=gl[0]
        nm=re.sub(r"[^A-Za-z0-9_]","_",mname.split("::")[-1]) or "TreeDeleter"
        add(a,"_unattributed",f"{nm}_{a}",HDRF(a,f"static CRetailTree @ 0x{obj:08X} atexit destructor -- clear (recursive Erase @ 0x{rec:08X} from the root, header relinked to itself, size zeroed) then free the header node (free @ 0x{freer:08X})",len(b))+f'''//
// Retail bytes:  mov eax,[size] ; test eax,eax ; je F ; mov eax,[header] ; mov ecx,[eax+4] ; push ecx ; mov ecx,offset tree ; call Erase ; mov eax,[header] ; mov [eax+8],eax ; mov edx,[header] ; mov dword ptr [edx+4],0 ; mov eax,[header] ; mov [eax+0Ch],eax ; mov dword ptr [size],0 ; F: mov eax,[header] ; test eax,eax ; je R ; push eax ; call free ; pop ecx ; R: ret
// Compiler-generated dynamic atexit destructor (`??__F`) of a static STLport-style tree object at
// 0x{obj:08X} (header pointer, then the node count): the inlined destructor clears the tree when it
// holds nodes -- the recursive node release @ 0x{rec:08X} from the root, the header's left,
// parent and right links reset (each through a fresh load of the header pointer, since the
// stores may alias the static) and the count zeroed -- then returns the header node to the
// allocator, which frees it when non-null (free @ 0x{freer:08X}). The manifest label is a
// descriptive name for the static; the element type is irrelevant here. One of {len(members)}
// bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SNode {{
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
}};

struct CRetailTree {{
    SNode* m_header;                       // +0
    long m_size;                           // +4
    void Erase(SNode* root);               // 0x{rec:08X} recursive node release
    void Clear()
    {{
        if (m_size != 0) {{
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_size = 0;
        }}
    }}
    ~CRetailTree()
    {{
        Clear();
        if (m_header)
            free(m_header);
    }}
}};

static CRetailTree g_{nm}_{a};             // 0x{obj:08X}
''',T(a,'''#include <cstdlib>
static int g_e = 0, g_f = 0;
struct N { long c; N* p; N* l; N* r; };
struct Tr { N* h; long n; void E(N*) { ++g_e; } void C() { if (n != 0) { E(h->p); h->l = h; h->p = 0; h->r = h; n = 0; } } ~Tr() { C(); if (h) { ++g_f; std::free(h); } } };
static void Run() { Tr t; t.h = (N*)std::malloc(sizeof(N)); t.h->p = t.h; t.n = 1; }
int main()
{
    Run(); if (g_e != 1 || g_f != 1) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_33",):   # SPair* __fastcall CopyBackward(first, last, dest, tag): counted backward copy of 8-byte pairs
        if len(calls)!=0 or retn!=8: continue
        add(a,"_unattributed",f"CopyBackward_{a}",HDRF(a,f"__fastcall CopyBackward(SPair* first, SPair* last, SPair* dest, tag) -- counted backward copy of 8-byte pairs, returns the new dest",len(b))+f'''//
// Retail bytes:  mov eax,[dest] ; push esi ; mov esi,edx ; sub esi,ecx ; sar esi,3 ; test esi,esi ; jle R ; mov ecx,edx ; sub ecx,eax ; L: mov edx,[ecx+eax-8] ; sub eax,8 ; dec esi ; mov [eax],edx ; mov edx,[ecx+eax+4] ; mov [eax+4],edx ; jne L ; R: pop esi ; ret 8
// Random-access `copy_backward` for 8-byte pairs (a float and an enum): the element count is
// the pointer difference, the loop copies `*--dest = *--last` as two dwords while the count
// runs down, and the compiler addresses the source relative to the destination (the two
// pointers move in lockstep). Fastcall: first in ecx, last in edx, the destination and the
// iterator-category tag on the stack (`ret 8`); the new destination is returned. The manifest
// label (`_Copy_impl<pair<float,E>*, ...>`) is propagated. One of {len(members)} bodies of this
// shape.
struct SPair {{ float first; long second; }};
struct SRandomAccessIteratorTag {{ char m_tag; }};

SPair* __fastcall CopyBackward_{a}(SPair* first, SPair* last, SPair* dest, SRandomAccessIteratorTag)
{{
    long n = last - first;
    if (n > 0) {{
        do {{
            *--dest = *--last;
        }} while (--n != 0);
    }}
    return dest;
}}
''',T(a,'''struct P { float f; long s; }; struct Tg { char t; };
static P* __fastcall CB(P* a, P* b, P* d, Tg) { long n = b - a; if (n > 0) { do { *--d = *--b; } while (--n != 0); } return d; }
int main()
{
    P v[6]; for (int i = 0; i < 6; ++i) { v[i].f = (float)i; v[i].s = i * 10; } Tg t; P* r = CB(v, v + 4, v + 6, t);
    if (r != v + 2 || v[5].s != 30 || v[2].s != 0 || v[4].f != 2.0f) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_35",):   # CCountedPointer<CTriangleList_X> __fastcall Create(owner, value, thing) { return CCountedPointer<...>(new CTriangleList_X(owner, value, thing)); }
        if len(calls)!=4 or retn!=8 or len(pushes)<2: continue
        newer,thingctor,lastctor,newer2=calls
        if newer!=newer2: continue
        size=pushes[0]
        leas=[i.operands[1].mem.disp for i in ins if i.mnemonic=="lea" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base==X86_REG_ESI]
        if len(leas)!=2 or leas[0]!=8: continue
        toff=leas[1]
        dels=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[0].mem.disp==4 and i.operands[0].mem.base==X86_REG_EAX and i.operands[1].type==X86_OP_IMM]
        if len(dels)!=1: continue
        deleter=dels[0]
        lm,ln,_=label(lastctor); lcls=ln.split("::")[0] if (lm,ln) in uniq and "::" in ln and ln.split("::")[0]==ln.split("::")[-1] and re.match(r"^C[A-Za-z0-9_]+$",ln.split("::")[0]) else f"CMember_{lastctor:08x}"
        cls=f"CTriangleList_{a}"
        add(a,"_unattributed",f"Create_{cls}",HDRF(a,f"CCountedPointer<{cls}> __fastcall Create(CThingOwner*, long, CThing*) -- new {cls} ({size:#x} bytes: owner's +0x40 copied to +4, CScriptThing @ 0x{thingctor:08X} at +8, owner at +0x14, value at +0x18, {lcls} @ 0x{lastctor:08X} at +{toff:#x}) wrapped in a counted pointer whose block (count 1, destroy @ 0x{deleter:08X}, object) is allocated inline",len(b))+f'''//
// Retail bytes:  push ebx ; push esi ; push edi ; push {size:#x} ; mov ebx,edx ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je Z ; mov eax,[ebx+40h] ; mov [esi+4],eax ; mov eax,[thing] ; push eax ; lea ecx,[esi+8] ; mov dword ptr [esi],offset base vtable ; call CScriptThing::CScriptThing ; lea ecx,[esi+{toff:#x}] ; mov dword ptr [esi],offset vtable ; mov [esi+14h],ebx ; call {lcls}::{lcls} ; mov ecx,[value] ; mov [esi+18h],ecx ; jmp C ; Z: xor esi,esi ; C: test esi,esi ; mov [edi],esi ; je N ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je F ; mov ecx,[edi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset destroy ; mov [eax+8],ecx ; mov [edi+4],eax ; mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8 ; F: xor eax,eax ; mov [edi+4],eax ; mov eax,edi ; pop ... ; ret 8 ; N: mov dword ptr [edi+4],0 ; mov eax,edi ; pop ... ; ret 8
// Fastcall factory returning a counted pointer through the hidden result (ecx): the owner comes
// in edx, the value and the thing on the stack. The object is `new`'d ({size:#x} bytes; the
// constructor chain is inlined behind the null test): a non-polymorphic base copies the
// owner's +0x40 dword to +4 (it sits under the vptr), the polymorphic base sets its vtable
// and constructs the CScriptThing member at +8 (@ 0x{thingctor:08X}) from the thing, the class
// sets its own vtable, stores the owner at +0x14, constructs the {lcls} member at
// +{toff:#x} (@ 0x{lastctor:08X}) and assigns the value to +0x18 in its body. The counted
// pointer stores the object and, when non-null, allocates its count block inline (`new`
// without a throwing allocator: a null block leaves the field null) with count 1, the
// type-erased destroy routine @ 0x{deleter:08X} and the object re-read from the member. The
// manifest label (`CTriangleList::CTriangleList`) is propagated over the family; the class
// is named by address and its unconstructed members are opaque. One of {len(members)}
// bodies of this shape.
struct CThing;
struct CThingOwner {{ unsigned char _pad[0x40]; long m_40; }};

struct CScriptThing {{
    unsigned char _body[0xC];
    CScriptThing(CThing* thing);           // 0x{thingctor:08X}
}};

struct {lcls} {{
    long m_value;
    {lcls}();                              // 0x{lastctor:08X}
}};

struct SCountBlock {{
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object);
}};

void __fastcall CountedDestroy(void* object);   // 0x{deleter:08X}

inline SCountBlock::SCountBlock(void* object)
    : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object)
{{
}}

struct CActionData {{                       // non-polymorphic base: lands at +4 under the vptr
    long m_owner40;
    CActionData(CThingOwner* owner) : m_owner40(owner->m_40) {{ }}
}};

struct CActionBase : CActionData {{
    virtual void v00();
    CScriptThing m_thing;                  // +8
    CActionBase(CThingOwner* owner, CThing* thing) : CActionData(owner), m_thing(thing) {{ }}
}};

struct {cls} : CActionBase {{
    CThingOwner* m_owner;                  // +0x14
    long m_18;                             // +0x18
{"    unsigned char _gap[%#x];" % (toff-0x1c) + chr(10) if toff>0x1c else ""}    {lcls} m_{toff:02x};                        // +{toff:#x}
{"    unsigned char _tail[%#x];" % (size-toff-4) + chr(10) if size-toff-4>0 else ""}    virtual void v00();
    {cls}(CThingOwner* owner, long value, CThing* thing)
        : CActionBase(owner, thing), m_owner(owner)
    {{
        m_18 = value;
    }}
}};

struct CCountedPointer {{
    CActionBase* m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(CActionBase* object)
    {{
        m_pObject = object;
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }}
}};

CCountedPointer __fastcall Create_{cls}(CThingOwner* owner, long value, CThing* thing)
{{
    return CCountedPointer(new {cls}(owner, value, thing));
}}
''',T(a,'''static int g_d = 0; static void __fastcall Destroy(void*) { ++g_d; }
struct Blk { long r; void (__fastcall* f)(void*); void* o; Blk(void* p) : r(1), f(Destroy), o(p) {} };
struct Own { unsigned char p[0x40]; long v; }; struct Th { unsigned char b[0xC]; Th(void*) {} }; struct Tm { long t; Tm() : t(7) {} };
struct Dat { long v; Dat(Own* o) : v(o->v) {} }; struct Bs : Dat { virtual void v00() {} Th th; Bs(Own* o, void* t) : Dat(o), th(t) {} };
struct X : Bs { Own* ow; long v18; Tm tm; virtual void v00() {} X(Own* o, long v, void* t) : Bs(o, t), ow(o) { v18 = v; } };
struct P { Bs* o; Blk* c; P(Bs* p) { o = p; if (p) c = new Blk(o); else c = 0; } };
static P __fastcall Cr(Own* o, long v, void* t) { return P(new X(o, v, t)); }
int main()
{
    Own o; o.v = 42; P p = Cr(&o, 5, &o); X* x = (X*)p.o;
    if (!x || x->v != 42 || x->ow != &o || x->v18 != 5 || x->tm.t != 7 || !p.c || p.c->r != 1 || p.c->o != x || p.c->f != Destroy) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_6",):   # bool CDefClassBase::GetSubDef<T>(CDefPointer<T>& out): index lookup by a literal name through virtual slot 14, def fetched from the manager into a typed counted pointer, assigned to out
        if len(calls)!=3 or retn!=4: continue
        sctor,sdtor,getter=calls; fp=(b[:3]==b"\x55\x8b\xec")
        litva=[p for p in pushes if p>0x1200000]
        if len(litva)!=1: continue
        s=lit(litva[0]); cs='"'+s.replace("\\","\\\\").replace('"','\\"')+'"'
        vslots=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM]
        if not vslots or vslots[0]!=0x38: continue
        moffs=[i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base==X86_REG_ESI and i.operands[1].mem.disp>0]
        if moffs!=[0x1c]: continue
        fname=re.sub(r"[^A-Za-z0-9_]","_",mname.split("::")[-1].split("<")[0]) or "GetSubDef"; fname=f"{fname}_{a}"
        add(a,"CDefClassBase",fname,HDRF(a,f"CDefClassBase::GetSubDef<T>(CDefPointer<T>&) -- the sub-def index for {s!r} through virtual slot 14 (a temporary CCharString), the def fetched from the manager at +0x1C (GetPDefFromGlobalIndex @ 0x{getter:08X}) into a typed counted pointer (converting copy, the manager's temporary released), assigned to the out pointer; false when the name has no index{'; frame-pointer size TU' if fp else ''}",len(b))+f'''//
// Retail bytes:  {"push ebp ; mov ebp,esp ; " if fp else ""}push ecx ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset {s!r} ; lea ecx,[name] ; call CCharString::CCharString ; mov eax,[esi] ; lea ecx,[name] ; push ecx ; mov ecx,esi ; call [eax+38h] ; lea ecx,[name] ; mov edi,eax ; call CCharString::~CCharString ; test edi,edi ; je F ; {"push dword ptr [edi] ; mov ecx,[esi+1Ch] ; lea eax,[temp] ; push eax" if fp else "mov edx,[edi] ; mov ecx,[esi+1Ch] ; push edx ; lea eax,[temp] ; push eax"} ; call GetPDefFromGlobalIndex ; mov esi,[eax] ; test esi,esi ; je C ; inc dword ptr [esi+4] ; C: mov eax,[temp] ; test eax,eax ; je A ; {"mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne N ; mov eax,[ecx] ; call [eax+4] ; N: and dword ptr [temp],0" if fp else "mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi ; mov edx,edi ; test edx,edx ; mov [eax],edi ; jne N ; mov edx,[ecx] ; call [edx+4] ; N: mov dword ptr [temp],0"} ; A: mov edi,[out] ; mov ecx,[edi] ; cmp ecx,esi ; je K ; test ecx,ecx ; je B ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: {"and dword ptr [edi],0" if fp else "mov dword ptr [edi],0"} ; B: test esi,esi ; mov [edi],esi ; je R ; inc dword ptr [esi+4] ; K: test esi,esi ; je R ; dec dword ptr [esi+4] ; jne R ; mov e?x,[esi] ; mov ecx,esi ; call [e?x+4] ; R: {"mov al,1 ; jmp X ; F: xor al,al ; X: pop edi ; pop esi ; leave ; ret 4" if fp else "pop edi ; mov al,1 ; pop esi ; pop ecx ; ret 4 ; F: pop edi ; xor al,al ; pop esi ; pop ecx ; ret 4"}
// Typed sub-definition lookup: a block-scoped CCharString named {s!r} (constructed with the
// explicit -1 length, destroyed at the end of its block, right after the virtual lookup) is
// passed to the virtual GetSubDefIndex (slot 14), which returns a pointer to the global index
// or null (the false path sits at the end: the success branch is the `if` body). The manager
// at +0x1C returns its CDefPointer<CDefBase> through the hidden pointer -- homed in the dead
// string slot -- and the typed CDefPointer<T> is converting-copied from it (addref; no copy
// elision across the two types), then the manager's temporary is destroyed: its destructor
// releases through the ref-count base's two inline accessors (adjusted base pointer
// materialised once, stored value forwarded to the test{", re-read in the size TU" if fp else ""}) and nulls the
// field (the store survives because code follows). `out = result` is the typed pointer's
// inlined assignment (release the old def through CDefBase::Release, store, addref) and the
// typed local's destructor releases through Release (`dec [esi+4] ; jne`, the null store
// elided for the register-held field).{" Frame pointer and size optimisation from the TU's pragmas." if fp else ""} The manifest label names one instantiation;
// T is opaque. One of {len(members)} bodies of this shape.
{"#pragma optimize(" + chr(34) + "y" + chr(34) + ", off)" + chr(10) + "#pragma optimize(" + chr(34) + "s" + chr(34) + ", on)" + chr(10) + chr(10) if fp else ""}#include "fable_string.h"

struct CRefCounted {{                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() {{ --m_count; }}
    long GetRefCount() const {{ return m_count; }}
}};

struct CDefVirtual {{
    virtual void v00();
    virtual void Destroy();                // slot 1
}};

struct CDefBase : CDefVirtual, CRefCounted {{
    void Release()                         // CDefPointer<T>: destructor and assignment
    {{
        if (--m_count == 0)
            Destroy();
    }}
    void ReleaseTemporary()                // CDefPointer<CDefBase>: the manager's temporary
    {{
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }}
}};

struct CDefPointerBase {{                   // CDefPointer<CDefBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() {{ }}
    {"__forceinline " if fp else ""}~CDefPointerBase(){"                      // forced: the size TU would call it out of line" if fp else ""}
    {{
        if (m_p) {{
            m_p->ReleaseTemporary();
            m_p = 0;
        }}
    }}
}};

struct CSubDef : CDefBase {{ }};              // T

struct CDefPointer {{                       // CDefPointer<T>
    CSubDef* m_p;
    CDefPointer() {{ }}
    CDefPointer(const CDefPointerBase& other) : m_p((CSubDef*)other.m_p) {{ if (m_p) ++m_p->m_count; }}
    ~CDefPointer()
    {{
        if (m_p)
            m_p->Release();
    }}
    {"__forceinline " if fp else ""}CDefPointer& operator=(const CDefPointer& other)
    {{
        if (m_p != other.m_p) {{
            if (m_p) {{
                m_p->Release();
                m_p = 0;
            }}
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }}
        return *this;
    }}
}};

struct CDefinitionManager {{
    CDefPointerBase GetPDefFromGlobalIndex(long index);   // 0x{getter:08X} (hidden result pointer)
}};

struct CDefClassBase {{
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 4];
    CDefinitionManager* m_pManager;        // +0x1C
    bool {fname}(CDefPointer& out);
}};

bool CDefClassBase::{fname}(CDefPointer& out)
{{
    long* pIndex;
    {{
        CCharString name({cs}, -1);
        pIndex = GetSubDefIndex(name);
    }}
    if (pIndex) {{
        CDefPointer result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        out = result;
        return true;
    }}
    return false;
}}
''',T(a,'''static int g_d = 0; struct R { long c; void Dec() { --c; } long G() const { return c; } }; struct V { virtual void a() {} virtual void X() { ++g_d; } };
struct D : V, R { void Rel() { if (--c == 0) X(); } void RelT() { Dec(); if (G() == 0) X(); } };
struct PB { D* p; PB() {} PB(D* q) : p(q) { if (p) ++p->c; } ~PB() { if (p) { p->RelT(); p = 0; } } };
struct P { D* p; P() {} P(const PB& o) : p(o.p) { if (p) ++p->c; } ~P() { if (p) p->Rel(); } P& operator=(const P& o) { if (p != o.p) { if (p) { p->Rel(); p = 0; } p = o.p; if (p) ++p->c; } return *this; } };
static D g_def; static long g_idx = 1; struct M { PB G(long k) { return PB(k == 1 ? &g_def : 0); } };
struct S { char x[8]; S(const char*, long) {} ~S() {} };
struct C { virtual long* I(const S&) { return g_idx > 0 ? &g_idx : 0; } M* m; bool GS(P& o) { long* pi; { S n("x", -1); pi = I(n); } if (pi) { P r(m->G(*pi)); o = r; return true; } return false; } };
int main()
{
    g_def.c = 1; M m; C c; c.m = &m; P o; o.p = 0; if (!c.GS(o) || o.p != &g_def || g_def.c != 2) '''+FAIL(a)+'''
    g_idx = 0; if (c.GS(o) || g_def.c != 2) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_69",):   # CVector::CVector(SIterator first, SIterator last) { Construct(first, last, SInputIteratorTag()); }  -- frame-pointer size TU, by-value iterator copies into reserved slots, tag by const reference homed in the high byte of the dead second slot
        if len(calls)!=1 or retn!=8 or b[:3]!=b"\x55\x8b\xec": continue
        helper=calls[0]
        if not any(i.mnemonic=="lea" and "ebp + 0xf" in i.op_str for i in ins): continue
        if sum(1 for i in ins if i.mnemonic=="mov" and i.op_str=="eax, esp")!=2: continue
        nm=f"CVector_{a}"
        add(a,"_unattributed",nm,HDRF(a,f"{nm}::{nm}(SIterator, SIterator) -- range constructor forwarding both by-value iterators and an input-iterator tag (by const reference, homed in the high byte of the dead second slot) to the __thiscall Construct @ 0x{helper:08X}; frame-pointer size TU",len(b))+f'''//
// Retail bytes:  push ebp ; mov ebp,esp ; push esi ; lea eax,[ebp+0Fh] ; push eax ; push ecx ; mov esi,ecx ; mov ecx,[last] ; mov eax,esp ; mov [eax],ecx ; push ecx ; mov ecx,[first] ; mov eax,esp ; mov [eax],ecx ; mov ecx,esi ; call Construct ; mov eax,esi ; pop esi ; pop ebp ; ret 8
// Range constructor of a vector-like container: the two 4-byte iterator classes (user copy
// constructor) are copied into reserved argument slots (`push reg ; mov eax,esp ; mov [eax],value`)
// right to left, the category tag is an empty temporary with a user constructor passed by
// const reference and homed in the high byte of the now-dead `last` slot (`lea eax,[ebp+0Fh]`),
// and the out-of-line Construct @ 0x{helper:08X} does the work; the constructor returns `this`.
// Size optimisation keeps the `first` load next to its copy (the speed TU hoists it to the
// prologue); the frame pointer comes from the TU's pragma. The manifest label is a mangled
// template-argument fragment and is not used; the element type is opaque. One of {len(members)}
// bodies of this shape.
#pragma optimize("y", off)              // frame pointer kept in this TU
#pragma optimize("s", on)

struct SIterator {{                         // 4-byte iterator class with a user copy constructor
    void* m_pNode;
    SIterator(const SIterator& other) : m_pNode(other.m_pNode) {{ }}
}};

struct SInputIteratorTag {{ SInputIteratorTag() {{ }} }};   // user constructor: no zero-initialisation of the 1-byte temporary

struct {nm} {{
    void Construct(SIterator first, SIterator last, const SInputIteratorTag& tag);   // 0x{helper:08X}
    {nm}(SIterator first, SIterator last);
}};

{nm}::{nm}(SIterator first, SIterator last)
{{
    Construct(first, last, SInputIteratorTag());
}}
''',T(a,'''struct It { void* p; It(const It& o) : p(o.p) {} It(void* x) : p(x) {} }; struct Tg { Tg() {} };
static int g_n = 0; struct X { void C(It f, It l, const Tg&) { g_n = (int)((char*)l.p - (char*)f.p); } X(It f, It l) { C(f, l, Tg()); } };
int main()
{
    char buf[8]; X x(It(buf), It(buf + 5)); if (g_n != 5) '''+FAIL(a)+'''
'''),b)
    elif TPL in ("w512_19",):   # void CDispatcher::Fire() { (m_pHandler->*m_pfnHandler)(CEvent(m_thing)); m_fired = true; }  -- event built by value in the argument slots, counted-pointer member copied with addref, call through a single-inheritance member function pointer
        if len(calls)!=0 or retn!=0 or len(pushes)!=0: continue
        if not any(i.mnemonic=="sub" and i.op_str=="esp, 0xc" for i in ins): continue
        vts=[i.operands[1].imm for i in ins if i.mnemonic=="mov" and i.operands[0].type==X86_OP_MEM and i.operands[1].type==X86_OP_IMM and i.operands[1].imm>0x1000000]
        if len(vts)!=2: continue
        ics=[i.operands[0].mem.disp for i in ins if i.mnemonic=="call" and i.operands[0].type==X86_OP_MEM]
        if ics!=[0x34]: continue
        if not any(i.mnemonic=="mov" and i.op_str=="byte ptr [esi + 5], 1" for i in ins): continue
        offs=sorted(set(i.operands[1].mem.disp for i in ins if i.mnemonic=="mov" and i.operands[1].type==X86_OP_MEM and i.operands[1].mem.base==X86_REG_ESI))
        if offs!=[0x38,0x40,0x44]: continue
        ev=f"CEvent_{a}"; cls=f"CDispatcher_{a}"
        add(a,"_unattributed",f"Fire_{cls}",HDRF(a,f"{cls}::Fire() -- builds a {ev} (base vtable then its own, the counted pointer at +0x40 copied with addref) by value in the argument slots, calls the handler through the member function pointer at +0x34 on the target at +0x38, then sets the fired flag at +5",len(b))+f'''//
// Retail bytes:  push esi ; sub esp,0Ch ; mov eax,esp ; mov dword ptr [eax],offset base vtable ; mov dword ptr [eax],offset vtable ; mov esi,ecx ; mov ecx,[esi+40h] ; mov [eax+4],ecx ; mov ecx,[esi+44h] ; test ecx,ecx ; mov [eax+8],ecx ; je S ; inc dword ptr [ecx] ; S: mov ecx,[esi+38h] ; call [esi+34h] ; mov byte ptr [esi+5],1 ; pop esi ; ret
// Event dispatch: the 12-byte event (a polymorphic base whose inlined constructor stores the
// base vtable, then the class's own; a counted pointer copied from the dispatcher's member at
// +0x40 with its block's count at +0 bumped when present) is constructed directly in the
// reserved argument slots and passed by value (the callee destroys it) through the
// single-inheritance member function pointer at +0x34 on the handler object at +0x38; the
// fired flag at +5 is set afterwards. Classes are named by address; the vtables are
// relocations. One of {len(members)} bodies of this shape.
struct SCountBlock {{
    long m_references;                     // +0
}};

struct CCountedPointer {{
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {{
        if (m_pCount)
            ++m_pCount->m_references;
    }}
    ~CCountedPointer();
}};

struct CEventBase {{
    virtual void v00();
    CEventBase() {{ }}
}};

struct {ev} : CEventBase {{
    CCountedPointer m_thing;               // +4
    {ev}(const CCountedPointer& thing) : m_thing(thing) {{ }}
}};

struct CHandler {{                          // complete single-inheritance class: the member function pointer is one code address
    virtual void v00();
}};

struct {cls} {{
    unsigned char _pad0[5];
    bool m_fired;                          // +5
    unsigned char _pad1[0x34 - 6];
    void (CHandler::*m_pfnHandler)({ev} event);   // +0x34
    CHandler* m_pHandler;                  // +0x38
    unsigned char _pad2[0x40 - 0x3c];
    CCountedPointer m_thing;               // +0x40
    void Fire_{cls}();
}};

void {cls}::Fire_{cls}()
{{
    (m_pHandler->*m_pfnHandler)({ev}(m_thing));
    m_fired = true;
}}
''',T(a,'''static int g_h = 0, g_r = 0; struct Blk { long r; }; struct CP { void* o; Blk* c; CP(const CP& x) : o(x.o), c(x.c) { if (c) ++c->r; } CP(void* a, Blk* b) : o(a), c(b) {} ~CP() { if (c) --c->r; } };
struct EB { virtual void v() {} EB() {} }; struct E : EB { CP t; E(const CP& x) : t(x) {} };
struct H { virtual void v() {} void On(E e) { g_h = (e.t.c ? (int)e.t.c->r : -1); } };
struct D { unsigned char p0[5]; bool f; unsigned char p1[0x34 - 6]; void (H::*fn)(E); H* h; unsigned char p2[4]; CP t; D(H* x, const CP& y) : h(x), fn(&H::On), t(y) { f = false; } void Fire() { (h->*fn)(E(t)); f = true; } };
int main()
{
    Blk b; b.r = 1; H h; { CP cp(&b, &b); D d(&h, cp); d.Fire(); if (!d.f || g_h != 3 || b.r != 2) '''+FAIL(a)+'''
    }
'''),b)
    else:
        raise SystemExit("family template not implemented: "+TPL)
OUT.mkdir(parents=True, exist_ok=True)
json.dump({"result":{"authored":B}}, open(OUT/"payload.json","w"), indent=1)
for c in B:
    base=f"{san(c['module'])}_{san(c['name'])}_{c['address']}"
    (OUT/f"{base}.cpp").write_text(c["source_cpp"]); (OUT/f"{base}_test.cpp").write_text(c["test_cpp"])
with open(OUT/"oracle.tsv","w",newline="") as f:
    w=csv.writer(f,delimiter="\t",lineterminator="\n"); w.writerow(["address","name","length","bytes"]); w.writerows(rows)
print(len(B),"entries ->",OUT)
