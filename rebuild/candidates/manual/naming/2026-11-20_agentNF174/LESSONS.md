# NF174 lessons (one line each)
- REFS lists callers, not definers: before a self rename, grep rebuild/src/compiled for out-of-line definitions of the new symbol (script dtor rows define the PDB ctor to emit the vtable); a strong owner plus any includer copy (even pick-any, selection 2) gives LNK2005.
- To emit an implicit ??1 without defining the ctor, a qualified destructor call in an anonymous-namespace helper is enough (0x00E37680); an inline ctor expanded in a helper still left a pick-any ctor COMDAT.
- reloc_bindings.tsv carries every bound spelling per target (one witness each): a target with only the correct spelling there plus a HEAD probe confirms done-already fast (q4356).
- A shared header's inline member template (Reset, DeleteData) instantiated by a respell emits a pick-any COMDAT: if retail has its own out-of-line copy with a different body (Reset<CDataBank> 0x00403E40), choose a spelling that does not instantiate it.
- A port that keeps the HEAD row's statement shapes (named CCharString locals in braces) closes size diffs from temporaries: 0x00CB8440 went 560v582 -> RELOCATION_MATCH by restoring them.
- Moving a binder onto a shared STL header can respell other callees too; check each against its owner row's HEAD-defined symbol (hd.sh) so the count only shrinks.
- In awk, compare addresses as strings (`$2==t ""` or `-v t=...` with a leading non-digit): `00e20210` is numeric 0, so `$2==t` matched every 00eDDDDD target.
