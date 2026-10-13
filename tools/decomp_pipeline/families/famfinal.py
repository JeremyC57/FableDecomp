#!/usr/bin/env python3
"""Post-landing pass for the family batches: fix headers of landed rows (batch + compiled copy),
mark the rest, ledger DIFFERs in crawl/gen_tried.txt, write one README per batch."""
import csv, re, os, sys, glob
from pathlib import Path
ROOT=Path(sys.argv[1]); S=Path(os.environ["S"]); LOGS=[S/"wine/land_fams.log", S/"wine/land_fams2.log"]+[Path(x) for x in sys.argv[2:]]
cat={r["address"].lower() for r in csv.DictReader(open(ROOT/"rebuild/oracles/auto-re-candidates.tsv"),delimiter="\t")}
# latest verdict per address from the logs (later lines win)
verdict={}
for L in LOGS:
    if not L.exists(): continue
    for line in open(L):
        m=re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s+(.*)$", line.strip())
        if m and m.group(2) not in ("WARN",): verdict[m.group(1)]=(m.group(2),m.group(3))
DESC={
 "2":"`__fastcall Create_<Class>(owner)` factories: `return new CX(owner);` with the one-argument constructor out of line",
 "57":"`__fastcall Create_<Class>(owner)` factories (the `add esp,4` scheduling of family 2; same template)",
 "3":"`CDerived::CDerived() : CBase() {}` for two polymorphic classes (base constructor out of line, own vtable)",
 "22":"`CDerived::CDerived() : CBase() {}` (family 3 with a different scheduling; same template)",
 "19":"`CDerived::CDerived(T a) : CBase(a) {}` one-argument forwarding constructors",
 "10":"`Clone()` through an out-of-line copy constructor",
 "5":"`Clone()` with the implicit copy constructor inlined (base copy constructor + own vtable)",
 "32":"`Clone()` with the implicit copy constructor inlined (family 5 shape; same template)",
 "29":"`Clone()` with the implicit copy constructor inlined plus one extra member copied after the base",
 "6":"retail `std::list` sentinel-node constructors (`CRetailList_node<N>`), allocator by value",
 "11":"retail block constructors: one malloc'd block of N bytes, allocator by value",
 "9":"retail `std::map`/`std::set` head-node constructors (`CRetailTree_node<N>`), no argument",
 "14":"retail `std::map`/`std::set` head-node constructors with comparator + allocator by value (`ret 8`)",
 "31":"retail `std::map`/`std::set` head-node constructors with comparator + allocator by value (`ret 8`)",
 "12":"scalar deleting destructors `??_G` of classes with an empty inline virtual destructor",
 "18":"scalar deleting destructors `??_G` (family 12 shape; same template)",
 "62":"scalar deleting destructors `??_G` with the `pop ecx` cleanup (`#pragma optimize(\"s\")`; same template)",
 "16":"`__fastcall Alloc_<Class>(a, b)`: `new CX(a, b)` with the two-argument constructor inlined",
 "17":"`CDerived::CDerived() : CBase(), m_member() {}` with a polymorphic member whose own constructor is inlined",
 "20":"`void CX::Initialise()`: a scoped `CCharString name(\"<animation>\", -1)` handed to the base setter, then the base `Initialise()`",
 "26":"persist hooks: `context->Transfer<signed char>(\"<Key>\", m_b<Key>, defaultValue)` on one byte member",
 "35":"`CDerived::CDerived(T a) : CBase() { Setter(a); }`",
 "7":"dynamic initialisers `??__E` of static-storage `CCharString` objects built from a literal, plus atexit registration",
 "25":"dynamic initialisers `??__E` of static-storage `CCharString` objects built from a literal (family 7 shape)",
 "27":"dynamic initialisers `??__E` that only register an atexit destructor (object with a trivial constructor)",
 "28":"dynamic initialisers `??__E` of static-storage objects with an out-of-line default constructor, plus atexit registration",
 "21":"`__fastcall Alloc_<Class>(a, b)`: `new CX(a, b)` with `CX(a, b) : CBase(a, b) {}` inlined over an out-of-line base constructor",
 "44":"`CIntelligentPointer<T>::CIntelligentPointer(const CIntelligentPointer&)`: base default constructor, own vtable, `SetPItem(other.GetPItem())`",
 "63":"`CIntelligentPointer<T>` copy constructors (family 44 shape; same template)",
 "34":"`CDerived::CDerived() : CBase() { m_a = m_b = m_c = 0; }` (vtable store first) -- open DIFFER",
 "59":"`CDerived::CDerived() : CBase(), m_member() {}` with the member constructor out of line",
 "69":"`CDerived::CDerived(T a) : CBase(), m_a(a) {}`",
 "49":"non-polymorphic default constructors whose only work is one CCharString-like member at +0 (owner class not identifiable)",
 "54":"non-polymorphic default constructors whose only work is one CCharString-like member at +8 (owner class not identifiable)",
 "g4":"`CCountedPointer<T>::Reset(T*)`: release the shared count block (destroy + delete on the last reference), adopt the new object with a fresh block",
 "g10":"`CCountedPointer<T>::Reset(T*)` (same template as g4)",
 "g48":"`CCountedPointer<T>::CCountedPointer(T*)`: adopt the object with a null-checked count block",
 "g61":"`CCountedPointer<T>::CCountedPointer(T*)` (`pop ecx` / `and [..],0` scheduling of g48; same template)",
 "g6":"retail `std::map`/`std::set` destructors: erase the subtree, reset the head links, free the head",
 "g7":"retail tree destructors (same template as g6)",
 "g11":"retail red-black tree iterator `operator--` (Dinkumware `_Dec`)",
 "g21":"retail red-black tree iterator `operator--` (same template as g11)",
 "g13":"retail red-black tree iterator `operator++` (Dinkumware `_Inc`, static inline `_Min`)",
 "g26":"`find(const long&)` on the retail tree with a signed key at +0x10 (Dinkumware `_Lbound` form, hidden-pointer iterator)",
 "g30":"`find(const long&)` on the retail tree (same template as g26)",
 "g43":"empty destructor of three consecutive CWideString members (reverse order, tail call)",
 "g45":"one-byte persist read hook: `__fastcall TransferIn(ctx, value, &value)` then `*m_pValue = value`",
 "g52":"`CX::CX(a, b)` root polymorphic class storing the first argument, second unused",
 "g27":"retail `std::list` `_Buynode(const T&)`: malloc the node, placement-copy the value (null-tested address)",
 "g36":"`Release()` of a static CCountedPointer: `Reset(0)` force-inlined on the global",
 "g22":"`CCombatAction_X(owner) : CCombatActionBase(owner, K, CSoundPair())` plus a scoped CCharString animation name",
 "g97":"`CCombatAction_X(owner) : CCombatActionBase(owner, K, CSoundPair())` without an animation name (same template)",
 "g329":"`CCombatAction_X(owner) : CCombatActionBase(owner, K, CSoundPair())` (same template as g97)",
 "g24":"copy constructors `CX(const CX&) : CBase(other), m_target(other.m_target)` with the smart-pointer copy inlined",
 "g5":"`CActiveEntityScript<...>::SuspendableProcess()`: suspend the attached process or fire the stored callback with a by-value CScriptThing copy",
 "g31":"`Assign(CBaseObject*)`: a stack CIntelligentPointer temporary routed into a member pointer (SetPItem/GetPItem)",
 "g56":"`Assign(CBaseObject*)` through a temporary CIntelligentPointer (same template as g31)",
 "g46":"Dinkumware `_Uninit_copy` (placement-copy a range, `__fastcall`), 4-byte elements",
 "g47":"Dinkumware `_Uninit_copy`, 16/20-byte elements (size-optimised scheduling found by the sweep; same template as g46)",
 "g51":"`~vector<T>` for polymorphic elements: virtual destructor per element, free the buffer",
 "g60":"persist hooks transferring two adjacent byte members through `Transfer<signed char>` with literal zero defaults",
 "g28":"static float initialisers `g = (float)cos(extern const double)` with the x87 intrinsic",
 "g35":"static float initialisers `g = (float)sin(extern const double)` (same template as g28)",
 "g62":"non-virtual `??_G` of a non-polymorphic {unsigned long, CCountedPointer} pair: inlined member destructor, then delete (emitted by a delete-expression)",
 "g94":"non-virtual `??_G` of a non-polymorphic buffer owner: `if (m_buffer) free(m_buffer)` inlined, then delete",
 "g155":"non-virtual `??_G` buffer owner (same template as g94)",
 "g252":"non-virtual `??_G` buffer owner (same template as g94)",
 "g253":"non-virtual `??_G` buffer owner (same template as g94)",
 "g17":"EH-framed (/EHsc) `~CX() {}`: own vtable, one member destroyed out of line, inlined empty base destructor",
 "g19":"EH-framed (/EHsc) `CX(a) : CBase(&m_value), m_value(a) {}`",
 "g25":"EH-framed (/EHsc) `Call(a) { CParam p(a); m_pTarget->Slot(&p); }`",
 "g53":"`[thunk] CX::vfunc_N adjustor{N}`: second-base override entry (`sub ecx,N ; jmp`), emitted with the vtables of a class whose two bases both declare the virtual",
 "g65":"virtual getters forwarding to GetPItem on an embedded smart pointer (`add ecx,N ; jmp`)",
 "g20":"`GetInterface_<type>(CTCBase**)`: presence-bitset test then a CVectorMap lower-bound lookup (frame-pointer TU: `optimize(\"y\",off)` + `(\"s\",on)`)",
 "g38":"`GetInterface_<type>` with a register-held type constant (same template as g20)",
 "gtiny":"generic tiny-body synthesiser (≤16 bytes): dword/byte/float getters (direct and through a pointer), setters, constant stores/returns, forwarders, scaled-global static initialisers, root constructors",
 "g142":"EH-framed `OnActivate()`: a vtable-only parameter object handed to virtual slot 10 of an embedded member",
 "g212":"EH-framed `CX(a) : CBase(), m_pValue(a), m_member() {}` with the member constructed out of line",
 "g171":"EH-framed empty destructor of a non-polymorphic pair: member then base destructor, states 0/-1",
 "g348":"EH-framed empty destructor inlined on a member sub-object at +0x10 (same template as g171)",
 "g144":"EH-framed `CX(a, b) : CBase(), m_18(a), m_1c(0, b) {}`",
 "g349":"EH-framed novtable destructor: member destroyed, inlined empty intermediate-base destructor",
 "g468":"EH-framed guarded virtual query: `CGuard guard(this); return m_pObject->Slot2();`",
 "g49":"Dinkumware `_Uninit_fill_n` for {key, counted ref} pairs (placement copy, count bump, returns the advanced pointer)",
 "g50":"container removal: cdecl helper over three table fields, free the returned block, drop the count",
 "g54":"container removal (same template as g50)",
 "g41":"Dinkumware `_Find` for 4-byte elements: four-way unrolled scan plus a remainder switch",
 "15":"`CDerived::CDerived(a, b) : CBase(), m_second(b), m_first(a) {}` -- open DIFFER (retail stores the vtable after both members; VC7.1 here schedules it between them)",
}
tried=ROOT/"tools/decomp_pipeline/crawl/gen_tried.txt"; have=set(open(tried).read().split()) if tried.exists() else set(); newtried=[]
summary=[]
for D in sorted(glob.glob(str(ROOT/"rebuild/candidates/manual/batches/2026-10-13_linux_f*"))+glob.glob(str(ROOT/"rebuild/candidates/manual/batches/2026-10-13_linux_g*"))):
    D=Path(D); fam=re.search(r"_([fg]\w+)$",D.name).group(1); fam=fam[1:] if fam[0]=="f" else fam
    rows=list(csv.DictReader(open(D/"oracle.tsv"),delimiter="\t"))
    landed=[r for r in rows if r["address"].lower() in cat]; rest=[r for r in rows if r["address"].lower() not in cat]
    for r in rows:
        a=r["address"].lower(); files=list(D.glob(f"*_{a}.cpp"))
        comp=list((ROOT/"rebuild/src/compiled"/a[:2]/a[2:4]).glob(f"*_{a}.cpp"))
        for f in files+(comp if a in cat else []):
            t=f.read_text()
            if a in cat:
                t=re.sub(r"// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane \(Wine \+ DDK\n//        13\.10\.4035 cl\.exe\); the batch README records the lane verdict\.",
                         "// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK\n//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.", t)
            else:
                v=verdict.get(a,("UNGRADED","-"))
                t=re.sub(r"// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane \(Wine \+ DDK\n//        13\.10\.4035 cl\.exe\); the batch README records the lane verdict\.",
                         f"// Grade: candidate -- still {v[0]} in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a\n//        repeated body shape, see the batch README for the open question.", t)
            f.write_text(t)
        if a not in cat and verdict.get(a,("",))[0].startswith("DIFFER") and a not in have: newtried.append(a); have.add(a)
    vc={}
    for r in rest: vc.setdefault(verdict.get(r["address"].lower(),("UNGRADED","-"))[0],[]).append(r["address"].lower())
    mods=len({r["name"].split("::")[0] for r in landed})
    rd=[f"# Batch `{D.name}` — family {fam}: {len(rows)} bodies ({len(landed)} landed)\n",
        f"\n{DESC.get(fam,'repeated body shape')}.\n",
        "\nEvery un-landed body of this opcode skeleton in the manifest (bodies ≤ 64 bytes grouped by instruction\n"
        "skeleton with scratch `famscan.py`; sources generated by scratch `gen/fam.py` from the retail bytes, the\n"
        "manifest labels, `rebuild/manifest/rtti-vtables.tsv` and the literal/`.rdata` operands), graded with the\n"
        "lander sweep in the Wine lane and landed with `verify_and_land.py --land`.\n",
        f"\n## Lane grading result\n\n- Rows: {len(rows)}; landed byte-exact (RELOCATION_MATCH + PASS): {len(landed)}; modules: {mods}.\n"]
    for k,v in sorted(vc.items()): rd.append(f"- {k}: {len(v)} — " + ", ".join(v[:12]) + (" …" if len(v)>12 else "") + "\n")
    rd.append("\n## Naming and trust\n\n- Class names come from `rtti-vtables.tsv` (the vtable the body stores) where there is one; bodies in the\n"
              "  no-RTTI region (vtables without a complete-object locator, e.g. 0x012C2F64) are named `CVtable_<va>`.\n"
              "- Base/member constructor names are taken from the manifest only when the (module, name) label is unique in\n"
              "  the manifest (`label_trust.py` rule); otherwise they are named by address (`CBase_<va>`).\n"
              "- Argument types follow the callee's manifest prototype when it is a plain pointer; template or enum\n"
              "  arguments are modelled as `void*` / `long` (same codegen) and say so in the file.\n"
              "- Each file carries the retail instruction sequence and the reasoning in its header comment.\n")
    (D/"README.md").write_text("".join(rd))
    summary.append((fam,len(rows),len(landed),{k:len(v) for k,v in vc.items()}))
if newtried:
    with open(tried,"a") as f: f.write("".join(a+"\n" for a in newtried))
for s in summary: print(s)
print("ledgered DIFFER:",len(newtried))
