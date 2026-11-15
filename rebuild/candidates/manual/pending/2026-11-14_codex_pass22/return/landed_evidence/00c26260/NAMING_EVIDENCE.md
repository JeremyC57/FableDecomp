# 00C26260 identity and naming

This is a whole 22-byte ordinary member, not a catch block. Both native callers C1D8F5 and C1EA6F supply the buffer as receiver and the audio manager as its one argument; its returned pool pointer feeds the sample data operation. The adjacent native C26240 adds the buffer data offset to this same pool pointer. C262E0 attaches the sample and handle, confirming the layout; C262A0 constructs the 44-byte buffer. No EH handler-table identity supports the manifest label.

## Names chosen before authoring

`CStreamBuffer::GetPoolData(CSoundEventManager*)` is DESCRIPTIVE, not a recovered PDB spelling. CStreamBuffer and CSoundEventManager follow the existing C26090/C262E0 models, with their existing struct keys; GetPoolData describes the unadjusted allocation base, unlike C26240's offset-adjusted result. The complete PDB name sets (Ego_r, Ego_d, FableWin), native RTTI table, FSE manifest, and all label files were searched. No original name exists there for this libpk member. A masked scan of all available original GameCODA release archives also found no body/symbol match. The only prior bindings for this address incorrectly call it CScriptArgument::GetValue and Thing::Compute; neither matches the two native callers or the adjacent buffer lifetime methods. These are reported in NAME_FIXES.tsv.

`CHandleBasedMemoryPool_012b4098::GetPointerFromHandle(long)` is the already-settled library-specific name, distinct from the engine pool. Its shared header explicitly records the owner decision of 2026-10-31. The bound symbol is correct and retained. The native C28380 leaf indexes Handles at +440 and returns the selected 32-byte header plus32. Ego_r's engine pool method and native library method order provide the two original-method identity sources; the library class suffix is the established owner decision, not a new invented class.

Member spellings m_handle and m_pool are DESCRIPTIVE and reused from the adjacent landed models. Native C262E0 stores the second pointer-sized field and accesses the manager's pool at +360; C26260 reads exactly those fields. Unknown bytes are explicitly opaque, not invented named engine fields. The real buffer/manager shared declarations are introduced once under include/libpk because neither class has a shared header. Manager total size is not claimed; only the evidenced prefix and existing pool subobject are represented.

No callee body is supplied; the existing pool declaration binds C28380 exactly. Neither input extent nor callee extent needs correction.
