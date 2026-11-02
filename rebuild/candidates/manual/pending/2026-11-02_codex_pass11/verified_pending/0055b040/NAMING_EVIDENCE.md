# 0055b040 identity and layout

The row is `NUISystem::CClickable::GetDataFromDef()`, public nonvirtual, `?GetDataFromDef@CClickable@NUISystem@@QAEXXZ`. The original Ego_r public and procedure record (module stream840) agree. Both native constructors0055b460(long) and0055b4c0(copy) call this exact body after initializing their click state. The manifest rumble label and old InitialiseInputDelay binding are wrong.

Original procedure locals are pdefinition, then four separate paction/nonConstFilename scopes. Native field offsets and virtual calls agree: CUIDef InputDelay+184 is assigned to CClickable InputDelay+18c; four action types+e0/e4/e8/ec select left-clicked,left-unclicked,left-held,right-clicked callbacks at slots71,80,72,73. UI_ACTION_TYPE_PLAY_MOVIE=65 and UI_ACTION_PARAM_TYPE_STRING=4 are original enum values. Its filename is CUIDef::MovieFilename+1d8.

Full classes use their original PDB keys and members: CClickable size194 derives CHoverable16c, CChangingStateComponent15c, CComponent134. CAction is16bytes after STLport list-size correction, not the donor32bytes. CActionParamString is12bytes, real polymorphic base CActionParam8, Value CWideString+8. Its constructor takes CWideString& (not const&). CCountedPointer<CAction> is the original global template with Data+0/Info+4 and original CCPPointerInfo callback type.

CUIDef is complete size228 from original PDB field order and native Transfer00631c60. Unaccessed composites keep named opaque storage at their full original retail size, as permitted by README. In particular packed C2DVector PositionOffset+142 retains8bytes without introducing any new packing directive. Real CParentDefClassBase39 and CDefClassBase25 use the pre-existing packed base headers. Actual target-read members are typed and compile-time offset checked.

The native extent is1043bytes and ends in ret before the next aligned constructor. No padding or second body is included. Every40 relocation operands is independently verified in RELOCATION_REVIEW.json; CRT allocation/deallocation distinctions and the complete CActionParamString RTTI vtable name are retained. No address annotation or catalog mutation was used.

Private evidence: newpair_types.json, newpair_original_locals.json, math_identity/UI_DEFS_PDB.json, math_identity/UI_DEFS_LAYOUTS.json, and the cut native disassembly. Those binary-derived disassembly files are review-only and should not be included in the handback.
