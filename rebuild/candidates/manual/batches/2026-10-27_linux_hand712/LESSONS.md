# Chunk 712 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 712) A never-authored row whose only caller loops over an owner's pointer array is a member of the array's element type:
  0x00A23AD0 is called on each `CMeshDataBank* MeshBanks[3]` of CEngineResourceManager (+0x10), so it is CMeshDataBank::FrameEnd
  (three CResourceBank::FrameEnd calls on +0x170 / +0x268 / +0x2E4, the last as the tail); the `_Dest_val` label was propagated.
- (chunk 712) An atexit thunk in `.text$yd` registered by a landed `$E1` initializer is `_$E2` of the same text: grade the
  initializer's source with Leaf `_$E2` (0x0122A790 from 0x01224100); keep `$E1` matching from the shared text.
- (chunk 712) Entity-script vtables have no RTTI (the slot before the vtable holds code, not a COL): name them from the only ctor
  that stores the vtable and its factory's `push sizeof` (0x012EB0F8 -> CFisticuffsMember, `push 30h` = Ego_r 48).
- (chunk 712) The real STLport 4.6 list text (`~list() {}`, inline `~_List_base() { clear(); deallocate(sentinel); }`, clear()
  defined out of class) reproduces both of retail's splits in one TU: a trivially-destructible list gets an out-of-line ~_List_base
  with clear inlined (0x00718EC0, 0x005283C0), a CCountedPointer / CIntelligentPointer list an inline ~_List_base around an
  out-of-line clear (0x00719280, 0x00B0F630). Four rows (0x00719C70, 0x006A5D50, 0x00B0CA90 and the vector in 0x0055D590) matched
  on the first probe this way.
- (chunk 712) Catalog `StdList_Clear_<addr>` rows whose body ends with `mov edi,[edi]; test; push edi; call free` (the sentinel
  free) are `_List_base<T>::~_List_base`, not clear; each has a one-instruction `jmp` thunk (the ~list) elsewhere.
- (chunk 712) CIVCountedPointer member release in an implicit dtor (0x00A9D270): FableWin's `~CIVCountedPointer() { Release(); }`
  with `CIVCountedPointeeBase* object = Object; if (object) { if (object->Release() == 0) object->Delete(); Object = 0; }`; reading
  the member twice reloads it after the `dec` (46v43), as GOTCHAS chunk 656 says.
- (chunk 712) A base-class member called by a derived dtor (CTCBase::RemoveFromGlobalTCs QAE) goes into the shared header's retail
  branch, not into a local base model; a declaration-only addition inside an existing FABLE_RETAIL_ branch rechecked clean on all
  36 landed users (hdr/INCLUDERS_CTCBase.txt).
- (chunk 712) Protected PDB members show as IAE (RemoveParticles, KillAllDecalEmitters, CInitBaseClass::Initialise /
  Uninitialise): declare them `protected:` in the model; several landed bindings carry the public QAE form.

Review at landing:
- hdr/: engine/CMeshDataBank.h (new opt-in FABLE_RETAIL_CMESHDATABANK), new engine/CResourceBank.h, engine/CTCBase.h (one declaration
  in the FABLE_RETAIL_CTCBASE branch; agents 709/711 may edit CTCBase.h in parallel -- merge, do not overwrite).
- NAME_FIXES.tsv: 25 rows. Landed callers to respell (name-only): 0x00B3B870 (CResourcePool_00a23ad0::Release -> CMeshDataBank::FrameEnd),
  0x00AC6290 / 0x00719C50 / 0x00E839D0 (free __fastcall *_dtor stand-ins for 0x00A9D270 / 0x00719C70 / 0x00E839F0), 0x005297D0
  (?Destroy@CEngineSubPrimitiveGameMesh for 0x006A5D50), the 0x007B01C0 row's own leaf (QAE -> IAE), the 0x0099A360 / 0x0099A350 /
  0x00B0C730 / 0x009FC410 / 0x009FC430 bound stand-ins.
- Landed 0x00E839D0 is the scalar deleting dtor ??_GCFisticuffsMember@CV_FisticuffsClubScript@NScript@@UAEPAXI@Z, landed as
  CLandscapeBackgroundPatch `vector_deleting_destructor` (wrong class and kind).
- Landed CThingPhysical_scalardeletingdestructor_005297d0.cpp says "Retail 0x005298c0" and names its callee "Destroy @0x838a00" while
  the row is 0x005297D0 (whose callee is 0x006A5D50 ~CThingPhysical): the source text describes the CThingGameObject ??_G; recheck.
- Landed 0x01224100 ($E1 of LogFile) models CBaseClassNonCopyable as a local class without its CBaseClass base: switch it to
  engine/CBaseClassNonCopyable.h as 0x0122A790 does (the $E1 bytes stay RELOCATION_MATCH from that text, checked).
- 0x00718EC0 / 0x005283C0 are landed under `StdList_Clear_*` names; vland warns RELOC_NAME on 0x00719C70 / 0x006A5D50 until respelled.
- No row needed more than 4 grades (max 3). No catalog row holds several functions; every extent checked against the padding.
