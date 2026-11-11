# Identity and source evidence

Exact leaf ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z occurs at Ego_r00ba9040 and FableWin0324a980. Native00a13c74 in original C3DVector::Rotate passes the C3DVector reference and float angle to this member. Native extent259 ends at00a56093 after complete ret8.

Full shared CMatrix3x4 and C3DVector used without a local model. Existing original flat union arm E11..E43 has all12 native float slots. Full donor body0324a980 proves all element offsets and source expression order; original module1236 locals explicitly name normal, angle, cos, sin, vx2, vy2, vz2, vxvy, vxvz, vyvz, stvx, stvy, stvz. No invented owners or member names.

Original GFCos/GFSin spellings ?GFCos@@YINM@Z and ?GFSin@@YINM@Z occur in FableWin PDB at01d9e180 and01dcb5d0; calls from0324a990 and0324a99f prove float argument, double return and original use. Both helper bodies scale by the same float-two-pi promoted to double before calling CRT cos/sin. Native uses intrinsic fcos/fsin and one shared multiplication. Supplied GFCos.h used; matching full original GFSin helper added in hdr/engine/GFSin.h. Portable calling convention macro is guarded by _MSC_VER.

The earlier invented float angle temporary is removed. The original helper structure is behaviorally correct but still does not reproduce the two operand orderings and terminal schedule. Every selected source is compiled; no byte-match claim.
