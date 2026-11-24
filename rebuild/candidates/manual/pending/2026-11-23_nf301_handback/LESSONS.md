# NF301 lessons (2026-11-23)

- **A free `__fastcall` stand-in for a static member is an in-place fix when the binder already models the class.** Declare it `static`
  in that model and qualify the call: the bytes are the same (0x006778A0, 0x00717C50). Put the declaration on an existing class line, so
  no line below moves.
- **A member-object type in the wrong namespace changes only the ctor call symbol** when the type has no dtor. Wrap the local struct in
  its PDB namespace (0x00B81BC0, 0x006B84B0). Check first for a user dtor: EH funclets would rename too.
- **Stale witnesses are common where a row was later ported onto a header or a local NScript model:** 12 of 13 candidates were stale.
  A cheap filter is tri4.py IDMISSING plus "the class in the binder already sits in the PDB namespace".
- **A doubled stand-in (one witness, global and NScript copies of the same stand-in) is half stale.** The object binds one of them.
- **A stale witness is not enough to delete a line** whose stand-in spelling other rows can also produce (`CPlayer* GetPlayer(long)` on
  many local models). Delete only when the stand-in token appears in no other landed code, or when every caller has been read.
- **The tracker can carry a VC10 STL spelling as pdb_spelling** (`_List_val`). STLport cannot emit it; check it before classifying the
  entry as a split.
