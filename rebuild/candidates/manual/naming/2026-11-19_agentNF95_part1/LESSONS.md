# NF95 lessons (one line each)
- reloc_bindings.tsv is stale both ways: 0x005BDD8C already bound the right symbol, and 0x005BF91A bound a wrong one without being listed; probe-compile every caller from `xref.py` call sites (map them with a site lookup), not only the WORKLIST.
- A PASS22 TU that needs FABLE_RETAIL_CTCINVENTORYBASE_PDEF's inline GetPDef hits C2535 (the PASS22 block declares it out of line); a nested `#ifndef` around the PASS22 line fixes it with the default path unchanged.
- Data-symbol access (`@@0/1/2`) for class statics: ghidra_out/egod_carved_names.tsv (Ego_d PUB32) has it; the Ego_r / FableWin name sets carry only the `dynamic initializer for` strings.
- The 0x005BDCB4 header recipe (DEF_CLASS_BASE, CINTELLIGENTPOINTER, CTCBASE, CTCINVENTORYBASE, ..._PASS22, CONFISCATED, ALL_ITEMS_CHAIN_PASS22) moved five NInventory rows in one grade each; CONFISCATED is needed for the complete NInventory::CItem.
- A CTCInventory-subclass virtual override row moves onto its header with a declaration-only `virtual` opt-in plus engine/ETCInterfaceType.h (CTCInventory.h needs TCI_INVENTORY).
- pin_asserts.py will not pin an assert with a trailing comment; pin it by hand to its old line.
- When a header port only misses on register choice, a declaration respell inside the row's existing local model still fixes the binding in one grade; keep the port draft in work/.
