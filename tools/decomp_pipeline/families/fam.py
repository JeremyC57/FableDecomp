#!/usr/bin/env python3
"""Family generator: parametrised candidates for repeated body shapes.
Usage: fam.py <repo> <family-id> <out-batch-name> [limit]   (env S = scratchpad)"""
import json, sys, csv, re, os, struct
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
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
for a, mmod, mname, ln, hx in members[:LIMIT]:
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
    elif TPL in ("w26",):   # iterator CRetailTree::find(const long& key)  (signed long keys at +0x10)
        cls="CRetailTree_longkey"
        add(a,cls,"find",HDRF(a,f"{cls}::find(const long&) -- lower-bound walk, then end() unless the key matches",len(b))+f'''//
// Retail bytes:  mov ecx,[ecx] ; mov eax,[ecx+4] ; test eax,eax ; push esi ; push edi ; mov edi,[esp+10h] ; mov edx,ecx ; je E ; mov esi,[edi] ; L: cmp [eax+10h],esi ; jl R ; mov edx,eax ; mov eax,[eax+8] ; jmp T ; R: mov eax,[eax+0Ch] ; T: test eax,eax ; jne L ; E: cmp edx,ecx ; je H ; mov eax,[edi] ; cmp eax,[edx+10h] ; jge F ; H: (result = head) ; ret 8 ; F: (result = node) ; ret 8
// std::map/set::find on the retail tree with a signed 32-bit key at +0x10: lower_bound descends
// from head->parent (right when node->key < key, else remember the node and go left), then the
// result is end() (the head) when nothing was found or key < found->key. The iterator is returned
// through the hidden pointer (`ret 8`: result pointer + key reference). One of {len(members)} bodies.
struct {cls} {{
    struct node {{ unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; }};
    struct iterator {{ node* current; iterator(node* n) : current(n) {{ }} }};
    node* head;
    unsigned long size;
    iterator find(const long& key);
}};

{cls}::iterator {cls}::find(const long& key)
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
    return iterator(found);
}}
''',T(a,'''struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; long k; }; struct it { node* c; it(node* n) : c(n) {} }; node* h; unsigned long n;
  it find(const long& k) { node* res = h; node* x = h->parent; while (x) { if (!(x->k < k)) { res = x; x = x->l; } else x = x->r; } node* f = res; if (f == h || k < f->k) f = h; return it(f); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}, a = {1,{0},&h,0,0,5}; h.parent = &a; T t; t.h = &h; t.n = 1; long k = 5, m = 6; if (t.find(k).c != &a || t.find(m).c != &h) '''+FAIL(a)+'''
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
