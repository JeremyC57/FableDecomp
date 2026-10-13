#!/usr/bin/env python3
"""Family generator: parametrised candidates for repeated body shapes.
Usage: fam.py <repo> <family-id> <out-batch-name> [limit]   (env S = scratchpad)"""
import json, sys, csv, re, os, struct
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
ROOT=Path(sys.argv[1]); FAM=sys.argv[2]; BATCH=sys.argv[3]; LIMIT=int(sys.argv[4]) if len(sys.argv)>4 else 10**9
S=Path(os.environ["S"]); OUT=ROOT/"rebuild/candidates/manual/batches"/BATCH
fams=json.load(open(S/"batch/families.json"))
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

key, members = fams[int(FAM)]
for a, mmod, mname, ln, hx in members[:LIMIT]:
    b=strip(hx); va=int(a,16); ins,calls,pushes,vts,retn=parse(b,va)
    if FAM in ("2","57"):   # Create: new CX(thing)
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
    elif FAM in ("3","22"):   # CDerived::CDerived() : CBase() {} with own vtable
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
    elif FAM in ("19","103"):   # CDerived::CDerived(T a) : CBase(a) {}
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
    elif FAM=="10":   # Clone with out-of-line copy ctor
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
    elif FAM in ("5","32"):    # Clone with inlined copy ctor (base copy ctor + vtable)
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
    elif FAM=="6":    # list ctor(alloc): head=0; head=malloc(N); head->next=head->prev=head
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
    elif FAM in ("11","193"):   # p=0; p=malloc(N); ret 4
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
    elif FAM in ("9","14","31"):   # tree ctor
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
    elif FAM in ("12","18","62"):   # scalar deleting destructor of a class with an empty inline virtual dtor
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
    elif FAM=="15":   # CDerived::CDerived(a, b) : CBase() { m_second = b; m_first = a; }  (stores: 2nd arg at lower offset)
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
    elif FAM=="16":   # __fastcall Alloc(a, b) = new CDerived(a, b) with the two-argument constructor inlined
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
    elif FAM=="17":   # CDerived::CDerived() : CBase(), m_member() {}  with a polymorphic member whose own ctor is inlined
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
    elif FAM=="20":   # void CDerived::Initialise() { SetAnimationName(CCharString("LIT", -1)); CBase::Initialise(); }
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
    elif FAM in ("35","246"):   # CDerived::CDerived(T arg) : CBase() { Setter(arg); }
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
    elif FAM=="26":   # persist: context->Transfer<signed char>("Name", this->m_flag, default 0)
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
    elif FAM in ("7","25","27","28","106","153","269"):   # dynamic initializer of a static-storage object (+ atexit registration of its destructor thunk)
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
    elif FAM=="21":   # __fastcall Alloc(a,b) = new CDerived(a,b); CDerived(a,b) : CBase(a,b) {} inlined, base ctor out of line
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
    elif FAM in ("44","63"):   # CIntelligentPointer<T>::CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
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
    elif FAM in ("34","119","134","192"):   # CDerived::CDerived() : CBase(), m_data() {} -- zeroed members live in an embedded struct with an inline ctor
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
    elif FAM=="59":   # CDerived::CDerived() : CBase(), m_member() {}  (member ctor out of line)
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
    elif FAM=="69":   # CDerived::CDerived(T a) : CBase(), m_a(a) {}
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
    elif FAM in ("49","54"):   # CX::CX() : m_string() {}  -- one CCharString-like member constructed out of line, nothing else
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
    elif FAM=="29":   # Clone with inlined copy ctor: base copy ctor + vtable + one extra dword member copied
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
    elif FAM=="129":   # CDerived::CDerived(a, b) : CBase(a, b) {}
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
    elif FAM in ("104","117"):   # CDerived::CDerived() : CBase(<constant>) {}
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
    elif FAM=="123":   # CX* Create_CX() { return new CX; }  with CX() : CBase() {} inlined
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
    elif FAM in ("92","108"):   # CX* Create_CX() { return new CX; }  with the constructor out of line (tail jump)
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
    elif FAM=="143":   # CX::~CX() {} : two CCharString members destroyed, vtable restored, base destructor tail-called
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
    elif FAM in ("96","147"):   # scalar deleting destructor: vtable, base destructor out of line, delete
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
    elif FAM=="73":   # scalar deleting destructor: one member destroyed out of line, vtable, delete (no base destructor)
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
    elif FAM=="98":   # ~CX() { if (m_buffer) free(m_buffer); }  with a CCharString first member destroyed by tail call
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
    elif FAM=="118":   # ~CStack() { delete m_data; m_data = 0; m_count = 0; m_capacity = 0; }
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
    elif FAM=="84":   # CCharString CX::GetName() { return CCharString("<lit>", -1); }  (hidden pointer, `this` unused)
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
    elif FAM=="127":   # bool CX::vfunc_N(long value) { return value == K ? true : false; }
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
    elif FAM in ("145","172"):   # void CX::vfunc_N() { m_flag = true; }
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
    elif FAM=="114":   # unsigned char CX::vfunc_N() { return m_pointer->m_byte; }
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
    elif FAM=="115":   # bool CX::vfunc_N() { return m_bits.flagN; }
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
    elif FAM=="116":   # void CX::vfunc_N(CY* object) { object->Method(); }
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
    elif FAM=="148":   # void CX::Method(long value) { m_pointer->Method(value); }
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
    elif FAM=="154":   # dynamic initializer of a static std::list (sentinel node allocated inline) + atexit
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
    elif FAM=="71":   # dynamic initializer of a static std::map/set (head node built inline) + atexit
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
    elif FAM=="120":   # T* allocator<T>::allocate(count, hint) { return count ? (T*)malloc(count * sizeof(T)) : 0; }
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
    elif FAM=="177":   # void CX::vfunc_N() { m_pointer->m_flag = true; }
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
    elif FAM=="174":   # CX::CX() : m_item(0) {}  (no base; vtable then one null member)
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
    elif FAM=="257":   # bool CX::vfunc_N() { return Helper() ? true : false; }
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
    elif FAM=="244":   # T CX::Get() { return g_pointer->m_field; }
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
    elif FAM=="201":   # CX* Alloc() { return new CX; }  with CX() : m_refs(1) {} inlined (root class, no base)
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
    elif FAM=="209":   # void list::Initialise(): head = 0; head = new self-linked sentinel  (no `this` return)
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
    else:
        raise SystemExit("family not implemented: "+FAM)
OUT.mkdir(parents=True, exist_ok=True)
json.dump({"result":{"authored":B}}, open(OUT/"payload.json","w"), indent=1)
for c in B:
    base=f"{san(c['module'])}_{san(c['name'])}_{c['address']}"
    (OUT/f"{base}.cpp").write_text(c["source_cpp"]); (OUT/f"{base}_test.cpp").write_text(c["test_cpp"])
with open(OUT/"oracle.tsv","w",newline="") as f:
    w=csv.writer(f,delimiter="\t",lineterminator="\n"); w.writerow(["address","name","length","bytes"]); w.writerows(rows)
print(len(B),"entries ->",OUT)
