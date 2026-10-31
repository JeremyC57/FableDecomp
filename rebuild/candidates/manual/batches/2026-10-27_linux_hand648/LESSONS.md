# Chunk 648 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The frame size follows the return-temporary type. A virtual that returns a 0x38-byte CCamera by value, read as `.WorldPosition`, gives retail's 0x7c frame. A model returning C3DVector packs the frame to 0x44. Respell the callee to its PDB return type (0x006B1F30).
- Escape analysis follows a same-TU stand-in. A `GetAllThingsInRoughArea<..., CThingFilter_None>` stand-in that never reads its empty filter lets VC7.1 home the filter on an existing byte local, as retail does. A stand-in that only fills `def->Object` fixes the loop's CCharString/CDefPointer slots. Stand-ins are levers only, never landed as reconstructions (0x006B1F30, and 0x009552c0 with a GetSubDef stand-in).
- Reading a member through an inline method on a subobject leaves a dead `add eax,<off>` after the call. This happens with `region.PDef.Get()` and with a CDefPointer copy. Read `region.PDef.Object` directly when retail has no add (0x006B1F30).
- The shared engine/CDefPointer.h Release() gives `mov edx,[eax+4]; dec edx; mov [eax],edx`. Retail's `dec dword ptr [ecx+4]; jne` comes from `if (--Object->IVRefCount == 0) Object->Delete();` on the member itself (0x006B1F30).
- A C3DVector without a user copy ctor turns a struct copy into `rep movsd`. A user copy ctor gives paired dword copies, but not retail's software-pipelined order (0x00912840, still open).

Review at landing:
- 0x006B1F30 took more than 4 grades.
- 0x006B1F30 keeps a local CDefPointer template over the shared CDefPointeeBase.h (header debt in CDefPointer.h).
- 0x006B1F30 keeps local CWorld and CAIGameCameraBase models, because their headers flatten the vptr.
- 0x006B1F30 has local CWorldMap, CThingSearchTools, CDefinitionManager and CRegion next to their layout-define includes.
- Already-landed 0x006B2720 is spelled non-const (QAEX); the PDB has QBE. Respell it.
- Already-landed 0x00B33010 binds 0x01436E3C as g_pWaterRenderer. It is the CEngineScreenEffectOutlineGlow global.
- Callers of 0x006B1600 under `LoadMainTheme_006b1600` (CAtmosProcess::Update 0x006B19FE) need respelling to ApplyReverbSettings.
