# agent899 header overlay

## engine/CEngineInternalPrimitiveBase.h
- New nested opt-in `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO` (inside the existing
  `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS` branch): forward-declares `class CEngineInterpolationInfo;` and adds the static
  data member `static CEngineInterpolationInfo CurrentInterpolationInfo;` (retail 0x0143B8E0).
- Evidence: Ego_r.pdb 0x0101A290 and FableWin.pdb 0x04009040 `dynamic initializer for 'CEngineInternalPrimitiveBase::CurrentInterpolationInfo''
  (row 0x01224900 is that initializer: `mov ecx,0x0143B8E0 ; jmp ??0CEngineInterpolationInfo@@QAE@XZ`).
- Default path byte-identical: nothing changes unless the new macro is defined (the class branch is unchanged without it; the
  generated default branch is untouched). Used by src/01224900.cpp only.
- Repo copy at the time of the overlay: rebuild/include/engine/CEngineInternalPrimitiveBase.h (no FABLE_RETAIL_ macro dropped:
  the overlay is the repo file plus the two `#ifdef` blocks).
