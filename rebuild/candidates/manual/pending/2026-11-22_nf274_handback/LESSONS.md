# NF274 lessons
- A pick-any ??_G that inlines the destructor comes out equal to retail in any TU that can see the destructor body and the class delete; folding the destructor's landed file into the constructor row is enough, and cmpobj against the other emitters' objects proves it without a link.
- lnkmap.py needs /OPT:NOREF or the map omits the COMDAT you are asking about; the NF272 copy of the tool had it, the one in this folder did not (lnkmap2.py).
- `inline` on same-TU member definitions changes only the COMDAT selection (NODUPLICATES -> ANY): objsec ALL_EQ, so two rows that carry the same TU-mate copies become one definition with no byte risk. It does not help against a strong owner whose body differs.
- To learn what an uncatalogued retail out-of-line copy needs, compile a held row with the callee made declaration-only and pk the copies: 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 are the helpers with FreeVirtual called, not inlined.
- A TU-mate that only matches its owner under the owner's own /d2inlT flag can be checked against the leaf cheaply: the disposal rows' leaves were unchanged at /d2inlT114, which also fits one retail TU with one setting.
- Read the self row's Leaf against both PDBs before promising "object unchanged" for a base port: a non-virtual local destructor (QAE) on a base that the shared header makes virtual turns the port into a self rename.
