# NF71 header overlay (round 7)

Every file here is a copy of the repo header taken at the moment NF71 first changed it (2026-11-17), plus `#ifdef` opt-ins only.
Without the opt-in macro each file preprocesses token-identically to the repo header (the default path is unchanged).

| Overlay file | Repo header copied | Opt-in added | Serves |
|---|---|---|---|
| stlport/fable_containers.h | rebuild/include/stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED`: `~_Vector_base() { free(_M_start); }` in place of the null-tested release (the default `#else` branch is the repo text unchanged) | 0x008F95D0 LSocket::SendBlock |
| engine/CV_TravellingHeroesScript.h | rebuild/include/engine/CV_TravellingHeroesScript.h | `FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_THUNDER_GETMASTERDATA`: public inline `GetMasterData()` in CTH_Thunder (FableWin.pdb `?GetMasterData@CTH_Thunder@CV_TravellingHeroesScript@NScript@@QAEPAVCQ_SunnyvaleMasterData@3@XZ`) | 0x00EE2490 CTH_Thunder::Main |
| engine/CAnimationEntry.h | rebuild/include/engine/CAnimationEntry.h | `FABLE_RETAIL_CANIMATIONENTRY_TRANSITION_CTOR`: declaration of Ego_r.pdb / FableWin.pdb `??0CAnimationEntry@@QAE@ABV0@J@Z` (0x005DEC70) | 0x006A0150 CCreatureAction_UnsheatheItemFromInventory ctor |

Checks: each overlay file was diffed against its saved repo original (work/hdr_orig/) at hand-back; the only differences are the
added `#ifdef` blocks above (fable_containers.h: the repo `~_Vector_base` text moved unchanged into the new `#else` branch). The repo
headers were unchanged since the copies were taken. engine/CAFile.h (a FABLE_RETAIL_CAFILE_READVAR_NOINLINE trial for the blocked
0x004FE630) is not handed back; its text is in work/CAFile_readvar_noinline.h.
