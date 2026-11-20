# NF172 lessons (one line each)
- A shared-header port that is "only missing two vptr stores" may be missing the header mode that makes the destructor inline: CSurface's
  virtual inline ~CSurface sits under FABLE_RETAIL_CSURFACE_COPYFROMMEMORY (FORCEINLINE_DTOR nests inside it); NF160's 3 forms never set it.
- Before porting a callee family, read the 08e0 / 0930-style twins: one twin may already carry the D1 member-template transformation, which
  then applies verbatim (MATCH first time) to the other.
- An extern-C /alternatename alias works for thiscall members, stubs and data pointers, but not for a ~430-character STLport symbol (LNK2019).
- A header-less local stand-in (IDirect3DVertexBuffer) may sit on a shared base (IDirect3DResource9) to reach a PDB parameter type
  without a cast (narrow rule); drop its own duplicate base slots.
- Chain the transformer and the grade with `&&`: a failed Python assert otherwise lets pdiff run on a missing file (wasted call).
- A census of every REFS row in one background probe (192 rows, about 15 minutes) settles most done-already verdicts up front.
