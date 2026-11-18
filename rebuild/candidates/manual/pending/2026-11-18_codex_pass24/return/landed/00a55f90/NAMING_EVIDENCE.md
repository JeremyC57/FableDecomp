# Naming evidence

- `CMatrix3x4::SetFromVectorAndAngle` is the exact Ego_r symbol at donor 0x00BA9040 and FableWin symbol at donor 0x0324A980. Ego_d places it in lib_matrix.obj. The retail calls in the landed C3DVector::Rotate and CSplinePosParamData::TransformPosToMaintainShape and the native 0x00A55F90 axis-angle matrix stores provide independent evidence.
- Ego_d records the parameter names `normal`, `angle` and locals `cos`, `sin`, `vx2`, `vy2`, `vz2`, `vxvy`, `vxvz`, `vyvz`, `stvx`, `stvy`, and `stvz`. The retail disassembly independently identifies their scalar roles through the matched products and matrix assignments. The additional `axis` reference is descriptive. It is a fitted source spelling that changes x87 operand selection without producing a runtime instruction.
- The shared matrix header uses the Ego_r field order. The retail offsets and landed C3DVector::Rotate corroborate the layout. FableWin has `GFCos(float)` at donor 0x01D9E180 and `GFSin(float)` at donor 0x01DCB5D0. Native expands them as `fcos` and `fsin` after a common angle scale.
- `?TWO_PI@@3NB` is the established project binding for 0x0124F2B8. The retail double and the sole COFF relocation agree. See RELOCATIONS.md.

## Behavioral fixture

The exact selected method uses the shared matrix and trig headers. The harness supplies the retail TWO_PI value and checks 1,125 input combinations against an independent axis-angle matrix calculation. VC7.1 RTM /Od /W3 and GNU C++03 both passed.
