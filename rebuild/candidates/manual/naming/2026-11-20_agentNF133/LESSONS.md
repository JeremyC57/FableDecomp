# NF133 lessons (one line each)
- Probe the whole REFS list once (512 rows took about 12 minutes at ~1 s per compile): the census decides which entries are cheap self-only renames.
- Multi-block July rows on candidates/*_compiled.h grade the wrong function in the probe: list the object's undefined symbols with objdump to see what they really reference.
- A test that declares the row's stand-in member without a body links the row: rewrite it as a host model when the row moves onto a header with private members.
- engine/GFGetTime.h uses FABLE_FASTCALL but includes only fable_compiler.h: include rebuild_abi.h before it, or cl stops at C2146.
- When updating STATUS from a vland log, match only `^<addr> +(MATCH|RELOCATION_MATCH)`; a bare `^<addr> ` also hits the WARN lines.
- The generic CPersistTraits<T>::TransferIn instance rows re-author as an explicit instantiation of the header's inline body (one line), RELOCATION_MATCH.
