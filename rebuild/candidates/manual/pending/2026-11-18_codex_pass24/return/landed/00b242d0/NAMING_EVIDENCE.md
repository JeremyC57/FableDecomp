# 0x00B242D0 naming and relocation audit

Ego_r donor 0x009AE5F0 and FableWin donor 0x02C9F4A0 agree on `?ConvertWorldPosToScreenPos@CEngine@@UBE_NABVC3DVector@@AAVC2DVector@@@Z`. Ego_d `Engine.obj` reports `wpos`, `mpos`, `from` and `to`. Retail vtable 0x012A0F3C at slot 13 selects the exact virtual const member emitted by the final object.

- The +0x2D absolute operand resolves to 0x01436EA0, bound to `?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A`. The adjacent 0x00B24400 inverse transformation reads the same global.
- The +0xCE and +0xDB operands resolve to the binary32 constants 0.0f (0x0122DEDC) and 1.0f (0x0122DED8). The object's `__real@00000000` and `__real@3f800000` agree with the PE constant values. The 1.0f spelling is also queued in land1011cr.
- The indirect call uses slot 51 of the CIEngine interface and matches `?IsActive@CIEngine@@UBE_NXZ` from Ego_r 0x006E1350 and FableWin 0x02075B00. The inverse projection uses it in the same way.
- The 119- and 35-byte Ego_d camera helpers follow this row in Engine.obj. Their FableWin public names at 0x02C9F690 and 0x02C9F730 take two C3DVector references, the first const. The retail inline bodies use the existing camera matrix at +0x80, screen half-sizes at +0xB8/+0xBC and centers at +0xC8/+0xCC. The view-to-screen helper preserves depth and uses the Ego_d `invz` local.
