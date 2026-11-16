# NF42 header overlay (round 5)

Each file is a copy of the CURRENT repo header (`rebuild/include/<same path>`) with `#ifdef` opt-ins added only.
Nothing else in these files differs from the repo copy.

| Overlay file | Opt-in added | Used by row(s) | What it adds |
|---|---|---|---|
| stlport/fable_map.h | `FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE` | 0x0042955B | declarations of STLport `_Rb_global<bool>::_Rotate_left` / `_Rotate_right` (static `__cdecl`, `(node, root&)`) inside `_Rb_global` |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE` | 0x00583815, 0x005838DD, 0x005839A5, 0x00595356 | `FABLE_STLP_NOINLINE` in front of `explicit list(const _Alloc& = _Alloc())` (needs `FABLE_STLP_NOINLINE`, e.g. from `_LIST_DTOR_OUTOFLINE`) |
| libpk/PKCBufferedReader.h | `FABLE_RETAIL_PKCBUFFEREDREADER_RETURNALLBUFFERS` | 0x00C4B010 | declaration `void ReturnAllBuffers();` (descriptive name, see NOTES.md) |
| engine/LSocket.h | `FABLE_RETAIL_LSOCKET_THINK` | 0x008F84A0 | public `static std::list<LSocket*> GlobalList;` and `static bool FABLE_FASTCALL Think();`, private `bool TryToSendDataFromQueue();` (Ego_r.pdb spellings) |
| engine/LHNetworkTypes.h | none (unchanged copy) | 0x008F84A0 | needed only because the overlay LSocket.h includes it by relative path |
| engine/CEngine.h | `FABLE_RETAIL_CENGINE_SETSPOTLIGHTSHADOWMODE` | 0x00B23C00 | the slot-62 override declaration `virtual void SetSpotlightShadowMode(bool enable);` (Ego_r.pdb + FableWin.pdb `?SetSpotlightShadowMode@CEngine@@UAEX_N@Z`) |
| engine/CEngineLightingManager.h | `FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETSHADOWMODE` | 0x00B23C00 | inline `void SetShadowMode(ELightingManagerShadowMode mode) { ShadowMode = mode; }` (FableWin.pdb `?SetShadowMode@CEngineLightingManager@@QAEXW4ELightingManagerShadowMode@@@Z`, 0x02C9E7C0; needs `_MEMBERS`) |

To merge: add each `#ifdef` block to the repo header at the same place (`diff rebuild/include/<file> hdr/<file>` shows it).
