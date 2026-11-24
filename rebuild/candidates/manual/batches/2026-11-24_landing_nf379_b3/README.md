# Landing nf379 b3: 0x0044C1BB joins the CThingNoiseDef S unit

`??0CTCNoiseDef@@QAE@XZ` 0x0044C1BB becomes a thin row of owner 0x0045C5F6 (held in NF379 as Q1).
- Ruling NF379-Q1 (advisor decision, owner delegated): condition (2) needs two sources. FableWin.pdb puts the out-of-line ctor in definition_manager.obj (#386); Ego_r.pdb has no out-of-line copy, but the retail neighbours 0x0044C0C0, 0x0044C108, 0x0044C1DC, 0x0044C3AA and 0x0044C6AF..0x0044C72B are all Ego_r definition_manager.obj. For a PDB with no out-of-line copy, retail-neighbour membership is the second source.
- T3 nf359 0x0044C1BB stays open (its debt is 0x00749810's pick-any copy): the line was set aside for the landing and put back in place. T3 nf355 (0x0045C5C1, the Alloc same-TU unit) is closed by hand (T3_CLOSED.tsv): one strong Alloc per link now.
