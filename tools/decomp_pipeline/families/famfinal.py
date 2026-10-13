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
 "h1":"script-module factories returning `CCountedPointer<CX>` by value: `new CX(owner, thing, arg)` (root/base/derived constructors inlined) wrapped in a fresh count block",
 "h22":"script-module counted-pointer factories with an extra out-of-line member constructor (same template as h1)",
 "h16":"retail (VC6-era Dinkumware) `_Tree::insert` with signed keys, frame-pointer/size TU: descent, begin() check, out-of-line `--iterator`, `_Insert(X, Y, V, 0)`",
 "h139":"retail `_Tree::insert` with unsigned keys (same template as h16)",
 "h15":"retail `_Tree::_Insert(trynode, wherenode, value, forceRight)`: link left/right, fix the head links, rebalance, count (signed keys)",
 "h138":"retail `_Tree::_Insert` with unsigned keys (same template as h15)",
 "h28":"Dinkumware `_Uninit_fill_n` (`__fastcall`): placement-copy `count` elements through an out-of-line copy constructor, return the advanced pointer",
 "h91":"Dinkumware `_Uninit_fill_n` for 8-byte elements with an out-of-line copy constructor (same template as h28)",
 "h82":"Dinkumware `_Uninit_fill_n` for {long, X} pairs: scalar first member copied inline, X copy constructor out of line",
 "h101":"Dinkumware `_Uninit_fill_n` for {X, long} pairs: X copy constructor out of line, scalar second member copied inline",
 "h51":"Dinkumware `_Uninit_fill_n` for 8-byte {long, long} pairs: two dword moves under the placement-new null test",
 "h38":"implicit destructors of non-polymorphic aggregates: members destroyed out of line in reverse order, the +0 member by tail call",
 "h78":"implicit destructors of three-member aggregates (same template as h38)",
 "h99":"`~CX() {}` releasing a shared-pointer member (control block with a vtable: --refs, virtual destroy, member nulled) before the base destructor tail call",
 "h76":"`pair<unsigned long, X>::pair(const pair&)`: scalar first member inline, X copy constructor out of line",
 "h44":"retail vector `push_back(const T&)`: placement copy at `last` when there is room, else the reallocating insert with an empty-tag temporary homed in the dead argument slot",
 "h108":"retail vector `push_back` for 8-byte elements (same template as h44)",
 "h75":"retail vector `push_back(const long&)` for 4-byte scalars (no callee-saved registers)",
 "h126":"retail vector `push_back` for 4-byte scalars (same template as h75)",
 "h50":"persist write hooks: copy the target dword into a local and hand it to the `__fastcall` TransferOut helper",
 "h72":"`SetItem(CIntelligentPointer)`: 8-byte by-value argument, `m_item.SetPItem(item.GetPItem())`, the argument destroyed by the callee",
 "h54":"non-virtual `??_G` of non-polymorphic classes: two CIntelligentPointer members and the base destructor, flag-tested delete (emitted by a delete-expression)",
 "h134":"`bool __stdcall Less(a, b)`: order by the dword at +4, equal keys by address (shared `sbb/neg`)",
 "h29":"recursive `FreeTree(node)`: child list at +0xC, sibling at +8, free the node",
 "h35":"recursive `FreeTree(node)` (same template as h29)",
 "h92":"recursive `FreeTree(node)` with a CIntelligentPointer member destroyed before the free",
 "h26":"`CDerived::CDerived() : CBase(), m_data() {}` with three zeroed members in an embedded struct (template 34)",
 "h121":"`CDerived::CDerived() : CBase(), m_data() {}` with one zeroed member (template 34)",
 "h48":"persist hooks `context->Transfer<long>(\"Name\", m_value, 0)` with a dword default temporary",
 "h122":"retail `list::Buynode(const long&)`: malloc a 12-byte node, placement-copy the dword value, return malloc's result",
 "h9":"`CPersistTraits<T>::TransferBinaryIn(stream, out)`: inlined CMemoryDataInputStream::Read with the __int64 position range check, memcpy fast path (movsd for struct values), ReadWithSrcChunkOverflow fallback; frame-pointer/size TU",
 "h64":"retail tree iterator increment on a node-pointer reference (template w13)",
 "h66":"retail `_Tree::find` with unsigned 32-bit keys (template w26, signedness detected from the compare)",
 "h89":"retail vector `push_back` in a frame-pointer/size TU (template h44)",
 "h55":"`PushBackConstant(vector*)`: `long value = K; vector->push_back(value)` with the retail push_back inlined",
 "h61":"`CX::operator=`: the base class's operator= out of line, then the trailing members copied in declaration order",
 "h59":"retail `list<CCountedPointer>::clear()`: destroy each value inline, free the node, re-link the head",
 "h60":"`_Move<CCountedPointer*>`: range copy-assignment with the counted-pointer operator= inlined (source fields read first), signed count in a stack local",
 "h20":"VC6-era `_Tree::_Copy(src, parent)`: Buynode per node, right subtrees recursively, the left spine iteratively through the `parent` parameter; frame-pointer/size TU",
 "h19":"VC6-era `_Tree::operator=`: Clear, Copy under a named head local, leftmost/rightmost walks, size copied; size TU",
 "h13":"`CDerived::CDerived(CThing*) : CBase(thing), m_name(\"LIT\") {}` -- hashed-name member built inline from an implicitly converted CCharString temporary (string assigned, CRC of the name)",
 "h14":"EH-module collection transfer: reserve count+count through virtual slots, then detach each item from the source and add it here",
 "h69":"SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 12-byte POD elements (counted loop, /Gr fastcall TU)",
 "h86":"SGI `__copy` for 28-byte POD elements (`rep movsd`; same template as h69)",
 "h114":"SGI `__copy` for 36..48-byte POD elements (`rep movsd`; same template as h69)",
 "h95":"SGI `__copy` with an out-of-line element operator=",
 "h36":"SGI `sort(first, last, comp)`: `__introsort_loop` to depth 2*lg(n), `__final_insertion_sort` inlined (threshold 16); 8-byte elements",
 "h98":"SGI `sort` with the final insertion sort inlined; 4/8-byte elements (same template as h36)",
 "h49":"SGI `sort` with `__final_insertion_sort` out of line; 12-byte elements",
 "h129":"SGI `sort` with `__final_insertion_sort` out of line; 20..44-byte elements (same template as h49)",
 "h31":"SGI `__final_insertion_sort(first, last, comp)`: `__insertion_sort` of the first 16, `__unguarded_insertion_sort_aux` of the rest",
 "i41":"Dinkumware `_Uninit_copy` for 8-byte POD elements (placement copy under the null test, `__fastcall`)",
 "i55":"Dinkumware `_Uninit_copy` for 16-byte POD elements (same template as i41)",
 "i79":"Dinkumware `_Uninit_copy` for 8-byte POD elements (same template as i41)",
 "i43":"Dinkumware `_Fill` for 8-byte POD elements",
 "i89":"Dinkumware `_Fill` for 40-byte POD elements (`rep movsd`; same template as i43)",
 "i44":"Dinkumware `_Destroy_range` over CCountedPointer elements (inlined release, fields nulled)",
 "i52":"Dinkumware `_Destroy_range` over CCountedPointer elements (same template as i44)",
 "i53":"`~CRetailVector<SBufferEntry>`: free each element's buffer, then the array (`last` cached)",
 "i58":"`~CX() {}` releasing a CCountedPointer member (function-pointer block) before the base destructor tail call",
 "i103":"non-virtual `??_G` of {member, CCountedPointer} pairs: counted pointer released inline, member destructor out of line, flag-tested delete",
 "i59":"`fill(first, last, value)` through an out-of-line operator=",
 "i60":"`distance(first, last)` over retail tree iterators with the successor walk inlined",
 "i86":"`distance` over retail tree iterators (same template as i60)",
 "i61":"retail `_Tree::find` with unsigned keys, hidden-pointer iterator result (template w26)",
 "i83":"retail `_Tree::find` with signed keys returning the node pointer directly (template w26)",
 "i62":"retail vector `erase(first, last)` for 8-byte PODs with the SGI counted copy inlined",
 "i96":"retail vector `erase(first, last)` for 8-byte PODs (same template as i62)",
 "i84":"`CRetailVector_dword(n, allocator)`: pointers nulled, malloc(n*4), first/last/end set",
 "i102":"`IS_THING_ALIVE(CIntelligentPointer&)`: target resolved twice, flag byte bit 0 clear",
 "i100":"retail vector `resize` for 12-byte elements with Erase/Insert out of line (signed size through idiv)",
 "i107":"retail tree constructor with a 0x1c-byte head node (template 9)",
 "i108":"STLport `__insertion_sort(first, last, comp)`: `__linear_insert(first, i, T(*i), comp)` per element",
 "i110":"Dinkumware `_Uninit_fill_n` through an out-of-line copy constructor (template h28)",
 "i23":"`CPersistContext::TransferBinary(T*)`: CheckCRC, then the `__fastcall` reader on the load stream in mode 2, else the writer on the save stream (size TU)",
 "i65":"`_Move<CCountedPointer*>` range copy-assignment (template h60)",
 "i63":"Dinkumware `_Uninit_copy` through an out-of-line copy constructor (template g46)",
 "i111":"SGI `__copy` with an out-of-line element operator= (template h95)",
 "i71":"`CX::operator=` over an out-of-line base assignment (template h61)",
 "i106":"SGI `__copy_backward` for 12-byte POD elements (block copy, two tags, `ret 0Ch`)",
 "i112":"SGI `__copy_backward` through an out-of-line element operator= (same template as i106)",
 "i91":"`~CRetailVector<SPair>`: both members of every element destroyed out of line, then the array freed",
 "i75":"retail `list<T>::clear()` with an out-of-line value destructor (cursor advanced first; size TU)",
 "i77":"`CX::CX(const CX&)`: base copy constructor out of line, own vtable, trailing dword members copied",
 "i93":"`__fastcall Create(a, b)`: `new CX(a, b)` with the constructor out of line",
 "i45":"`CCombatAction_X(owner, thing) : CCombatActionBase(owner, thing, K, CSoundPair())` (zeroed sound pair on the stack)",
 "i54":"two-argument combat-action constructors (same template as i45)",
 "i80":"two-argument combat-action constructors with a scoped CCharString passed to a setter (same template as i45)",
 "i97":"persist `Transfer(CPersistContext&)`: one typed context helper per member, last address formed in place",
 "i99":"persist `Transfer(CPersistContext&)` (same template as i97)",
 "i42":"VC6-era `_Tree::operator=` with the clear inlined (Erase, head reset) before the Copy",
 "i115":"retail vector `resize` for 28/124-byte elements, Erase/Insert out of line, element size from the reciprocal multiply (template i100)",
 "i118":"`CX::CX(const CX&) : CBase(other), m_target(other.m_target)` with the smart-pointer copy constructor inlined (template g24)",
 "i50":"`AssignTarget(thing)`: temporary CIntelligentPointer SetPItem/GetPItem into the member (template g31)",
 "i109":"Dinkumware `_Uninit_copy` for {long, X} pairs (scalar inline, X copy constructor out of line)",
 "i117":"Dinkumware `_Uninit_copy` for {X, long} pairs (same template as i109)",
 "i66":"`Clone()` with the implicit copy constructor inlined: base copy, vtable, two trailing dword members",
 "15":"`CDerived::CDerived(a, b) : CBase(), m_second(b), m_first(a) {}` -- open DIFFER (retail stores the vtable after both members; VC7.1 here schedules it between them)",
}
tried=ROOT/"tools/decomp_pipeline/crawl/gen_tried.txt"; have=set(open(tried).read().split()) if tried.exists() else set(); newtried=[]
summary=[]
for D in sorted(glob.glob(str(ROOT/"rebuild/candidates/manual/batches/2026-10-13_linux_f*"))+glob.glob(str(ROOT/"rebuild/candidates/manual/batches/2026-10-13_linux_g*"))+glob.glob(str(ROOT/"rebuild/candidates/manual/batches/2026-10-13_linux_h*"))+glob.glob(str(ROOT/"rebuild/candidates/manual/batches/2026-10-13_linux_i*"))):
    D=Path(D); fam=re.search(r"_([fghi]\w+)$",D.name).group(1); fam=fam[1:] if fam[0]=="f" else fam
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
