# Identity and extent

Exact leaf `?ClearGroups@CParticleProgram@@QAEXXZ` appears in Ego_r00BEF4A0 and FableWin03336E90. FableWin body clears its two CArray<C3DGroup2> members in order; native uses the corresponding retail offsets+FC/+108. Ego_r original fields SimGroups/NonSimGroups are at284/300; retail offsets follow reduction of preceding STLport containers. Native code begins after alignment padding, ends with ret atAE3546, and is followed by padding throughAE354F. Thus this is a whole ClearGroups member, not the manifest unwind label.

Both calls bind the existing exact std::vector<C3DGroup2,std::allocator<C3DGroup2> >::erase symbol at00AE7F30, confirmed from both COFF records and decoded retail REL32s. No other relocation occurs. Obj symbol confirms public thiscall void leaf.

Header changes are behind the fresh FABLE_RETAIL_CPARTICLEPROGRAM_PASS22_CLEARGROUPS flag: restore typed original SimGroups and NonSimGroups, preserve the existing accessor behavior, add the original public method declaration. All prior/default paths kept and shifted assert names pinned.

The standalone test copies the entire method body verbatim with tracing container and element stubs. Nine empty/single/multiple combinations verify both calls in order, clearing, destruction, capacity retention, unchanged unrelated state, and repeat calls. VC71 /Od /W3 /GR- without EHsc and g++03 both pass. The separate original-header build proves ABI and bytes.
