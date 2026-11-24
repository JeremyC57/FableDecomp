# Landing nf391 b2: 0x0051A2F0's tree erase gets the repo STLport spelling (PBV -> QAV)

0x0051A2F0 is `_Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*>>>::erase(iterator)`. Its key type is now spelled `QAV`, which is what a compile of the repo STLport emits, instead of the stale `PBV`.
- Coordinator checks: the source equals the graded source. HEAD v new (`comdat_cmp`): five renamed sections (erase, ~pair, ??_G pair, _Destroy, deallocate) and nothing else. The new ~pair and ~list copies are EQ to the landed 005029a0's. No table line, source or header uses the old spelling, and no other address carries the new self. No includers. The self matches its `// Leaf:` line. Baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- Part of the tree-spelling unification (T3 nf389 0x0051B720). 0x00513420 waits on the find-form question (R2), and the 0x005003E0 / 0x0051BF90 binders need ports (R3).
