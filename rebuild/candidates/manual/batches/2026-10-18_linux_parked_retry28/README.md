# Renderer constructors, wide-string lifetime and bank packing

0065595c ClockSprite (194 bytes): PDB wrapper layout and neighbor006558CE establish base/theme initialization, color copies, angles and unaligned UV arrays. Main review restores ClipBox/ClipBoxIsPos/Flags/AngleStart/AngleEnd names. Callee004371A0 is a16-byte two-vector box constructor, disproving its propagated orientation label. Actual-source fixture checks initialized fields and copied inputs.

00550940 StaticMesh (192): PDB0x6c wrapper, copied orientation with real virtual destructor, invalid bounding sphere and color conversion. RTTI01239824 confirms orientation virtual slot. Fixture tests all initialized fields and all12 orientation floats/input independence.

0099bc50 SetLength (162): real fable_string.h target data, private-owner method view; clear/shared/final ownership, unique/shared resize, buffer reloading, space fill and nullable allocation tested. Host-only dependency guard adapts pointer width without altering method code; target uses actual shared header. Host -fcheck-new preserves VC7.1 nullable new.

009cd740 PackRuntimeData (270): audited12-byte retail arrays and runtime records. Fixture tests sentinel skipping, field/bit/stride/count/value arguments, strict packed-vs-capacity threshold, post-callback capacity reload and exclusive cleanup branches. Donor CBankFile's16-byte arrays are deliberately replaced with documented retail model.

All independently RELOCATION_MATCH + PASS under canonical VC7.1/Wine; host fixtures PASS and embed the exact source. Total source grades9/2/3/1 including coordinator naming/dependency integration.
