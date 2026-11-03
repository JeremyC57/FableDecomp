import sys
D=sys.argv[1]
# addr, derived vt, base class, base dtor addr, coda?, ctor addr, deleting dtor addr, base ctor
rows=[
("00c1d8b0","012b32a0","CVtable_012b3404","00C1EB10",False,"00C1D880","00C1DD30","00C1EAA0"),
("00c1e080","012b33a4","CVtable_012b3404","00C1EB10",False,"00C1E050","00C1E0F0","00C1EAA0"),
("00c3c5c0","012b5860","CVtable_012B8778","00C4CF40",True,"00C3C590","00C3C600","00C4CF20"),
("00c56d60","012b8d98","CVtable_012b8df8","00C57310",True,"00C56D30","00C56E40","00C572F0"),
("00c57410","012b8e20","CVtable_012b8ed4","00C577B0",True,"00C573E0","00C57440","00C57780"),
("00c6cac0","012bed7c","CVtable_012beda4","00C6E630",True,"00C6CA20","00C6D350","00C6E6A0"),
("00c6da30","012bed90","CVtable_012beda4","00C6E630",True,"00C6D9D0","00C6E1D0","00C6E6A0"),
("00c6e780","012bedb8","CVtable_012beda4","00C6E630",True,"00C6E760","00C6ECB0","00C6E6A0"),
]
for a,vt,base,bd,coda,ctor,gdt,bctor in rows:
    cls="CVtable_"+vt
    lib = "GameCODA" if coda else "libpk"
    root = ("\n//   The base destructor ends by restoring the GameCODA root vtable 0x012B5868 (CCodaObjectBase, owner decision D3), and the\n//   class's deleting destructor 0x%s frees through CodaFree 0x00C3C370, so the hierarchy roots at CCodaObjectBase." % gdt) if coda else ("\n//   The class's deleting destructor 0x%s calls this row, then operator delete 0x00BFE9BC." % gdt)
    inc = '#include "engine/CCodaObjectBase.h"   // CCodaObjectBase (vtable 0x012B5868), CodaMalloc / CodaFree\n' if coda else ''
    bdecl = ("class %s : public CCodaObjectBase {" % base) if coda else ("class %s {" % base)
    src=f"""// {cls}::~{cls}() -- empty virtual destructor of the {lib} class with vtable 0x{vt.upper()}: own vtable, then the base {base} destructor (tail jump)
// Leaf: ??1{cls}@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x{a.upper()}, 11 bytes: `mov dword ptr [ecx],offset vtable 0x{vt.upper()} ; jmp 0x{bd}`.
// Identity: a destructor is named by the vtable it stores first. 0x{vt.upper()} has no complete-object locator (no-RTTI
//   {lib} module, no PDB), so the class is named by its vtable as its landed constructor 0x{ctor} is (CVtable_<va>, the
//   catalog's ??_7 convention). That constructor calls the base constructor 0x{bctor} first, and the base destructor
//   0x{bd} ({base}, named by its own stored vtable) is the jump target here.{root}
//   The catalog label "CChunkedFileChunk::~CChunkedFileChunk" is propagated across this byte-twin shape and is wrong.
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the out-of-line
//   base destructor.
{inc}
{bdecl}
public:
    virtual ~{base}();                  // 0x{bd}
}};

class {cls} : public {base} {{
public:
    virtual ~{cls}();
}};

{cls}::~{cls}()
{{
}}
"""
    open(f"{D}/src/{a}.cpp","w").write(src)
