
- (agentFAM08, 2026-11-12) The whole CVtable_ family has no RTTI: settle each vtable from its storing ctor / allocator, its named slots and reloc_bindings, not vtable-4.
- (agentFAM08, 2026-11-12) reloc_bindings often already binds the right ??0 spelling to a placeholder ctor (the def-class default ctors): grep the address first.
- (agentFAM08, 2026-11-12) The script-info registration (CScriptInfoManager::AddScript calls) pairs each script's literal name with its script and master-data allocators: it names data classes and a script's CLevelScriptBase<T>.
- (agentFAM08, 2026-11-12) `new Derived` with the abstract base's constructor defined inline emits the base vtable and its ??_G (no out-of-line ctor needed).
- (agentFAM08, 2026-11-12) An implicit derived destructor whose base destructor is also implicit is NOT expanded into ??_G by VC7.1 here (CActiveEntityScript<T>), though retail did; decl-only opt-ins, /Ob2, /EHsc did not change it -- stop after two tries.
- (agentFAM08, 2026-11-12) PDB ??_E spellings can differ from retail: trust the body (bit 0 only = ??_G).
- (agentFAM08, 2026-11-12) Landed EH funclet rows #include their parent's source by path: renaming a parent row's file breaks them.
- (agentFAM08, 2026-11-12) A g++ -fsyntax-only pass (64-bit, ignoring the ptr32 static asserts) catches missing prerequisite macros before a Wine grade.
- (landing de, 2026-11-12) A generic vland --recheck-landed payload grades an EH funclet row against its whole parent (DIFFER even when untouched); recheck funclets with lane_tools/recheck.py, which reads the `funclet block offset` from the source.
