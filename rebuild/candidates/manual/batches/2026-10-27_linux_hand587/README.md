# Chunk 587 (2026-10-30): 7 of 10 engine-range released pass 9 rows landed

1 MATCH (mdct_butterfly_generic) + 6 RELOCATION_MATCH, all PASS (g++ and VC7.1). Rows: libvorbis mdct_butterfly_generic,
DirectShow CEnumPins::Next (strmbase model), std::vector<C3DGroup2>::_M_fill_insert, NScript::CQ_AwakeningTheOracleScript::
OnPersist (moved onto engine/CLevelScriptBase.h after vland's shared-header warning; still matches),
std::__merge_without_buffer<CSortElement*,...>, CAudioObject::Update (descriptive, libpk), the default ctor of the landed
placeholder class CActivationReceptor_00c38b20 (/EHsc; the parts at offset 0 are members, not bases).
Parked (../../pending/2026-10-27_linux_hand587/differ/, 4 grades each, tests pass): 0x00C5DC60 CDSoundAccelerationQuery::
OpenDeviceRegistryKey (265v267, loop register allocation + inline -1 exit), 0x00C89D70 libvorbis _get_prev_page (265v269:
two `mov eax,esi` missing -- the same-TU static _get_next_page chooses its own first-parameter register; lane gap),
0x00AE3920 CParticleProgram::RemapBoneIndices (266v270, key temporary stored earlier + dtor registers; try an explicit
iterator from find).
NAME_FIXES.tsv: 14 rows queued (C3DGroup2 vector family spellings, C3DGroup2::SetBoneIndex landed as __EH_epilog3, CAudioObject
::ReportQueueInfo, six CActivationReceptor part ctors bound as CThingNoise*), plus the part dtors 0x00C39BA0..0x00C39CE0
carrying two spellings each. Header gaps (NOTES.md): no headers for C3DGroup2, CArray, CSortElement (CEngineSubPrimitiveRenderer.h
lacks the nested class), the libpk audio classes, strmbase, OggVorbis_File; CParticleProgram.h has no member functions and a
flattened vftable (layout-rename include used); engine/CBaseObject.h is Fable's, so the SHARED_HEADER warning on the
strmbase row 0x00CAAFC0 is a false positive; the landed CActivationReceptor dtor model splits the +0xCC part wrongly
(one 0x2C part) and its head models declare virtual dtors that retail has trivial.
