# 0x00697F80 naming and relocation audit

Ego_r donor 0x006BED10 and FableWin donor 0x022035D0 agree on `?Is2DAreaInViewingFrustrum@CGameCameraManager@@UBE_NABVC3DVector@@M@Z`. Ego_d reports the same member in `camera_manager.obj`, retail vtable 0x0125C754 slot 4 points here, and the final object emits that exact symbol.

- The object relocations at +0x0F, +0x6C and +0x8F resolve to `?GetFOV@CCamera@@QBEMXZ` at 0x00A0BEB0 and two calls to `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` at 0x00A14440. Both names agree with the PDBs and the binding table.
- The +0xB7 absolute operand points to `?TWO_PI@@3NB` at 0x0124F2B8. This is the established global spelling. The older `_TWO_PI` alias is recorded by the binding table from another caller.
- The +0x1A7 relative operand calls 0x00875120. The address is catalogued as `CTools::IsPosInViewingFrustrum`, and the adjacent landed 0x00697F50 passes the same camera and look-at references with the field of view to it. Ego_r donor 0x0085B8F0 and FableWin donor 0x029C2520 agree on the qualified const member `?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z`.
- The +0x1B5 and +0x1BF operands address the binary32 literals 0.1f (0x0122EE58) and 1.0f (0x0122DED8). The object emits `__real@3dcccccd` and `__real@3f800000`, which agree with the retail constant pool. The latter spelling is also queued in land1011cr.
- `GetPosition` is established by the existing CBase inline, the FableWin signature, and the +0x54 copy. `GetObservedPos` appears in the Ego_d TU next to this frustum family, in FableWin at 0x022035B0, and corresponds to the +0x10 copy from the same current-mode object. The CrossWithUp helper fits the Ego_d `perp` local, existing FableWin signature and retail Y / −X / 0 components.
