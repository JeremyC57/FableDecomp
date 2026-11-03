# Original identity and native layout

CParticleProgram::ApplyTransform(const CMatrix3x4&) is public void thiscall, exact ?ApplyTransform@CParticleProgram@@QAEXABVCMatrix3x4@@@Z. Both raw PDB procedure records, original lib_particle_simulation module neighbours SetupSimulation/RelocateData and native stream-loop shape prove this identity. The manifest CCreatureActionBase label is wrong. The span is1090 bytes, final ret4 at00ae04af, padding follows00ae04b2.

Complete CParticleProgram/CParticleSimulation storage, bases, access and field types are recorded in TYPES_EGOR.json. Native offsets independently establish InitialSimulation+8, particle count+0x20/positions+0x24, Constraints+0x114 and16-byte distance-constraint records. Original TConstraintType enumerators agree with native cases2and3. Unused aggregate members remain explicitly typed opaque storage at their native offsets; no member is removed.

Two direct helpers: const CMemoryDataOutputStream::PeekData at00995e70 and const GetLength at00994b40; original PDB names plus adjacent stream implementations and existing correct bindings. Wrong Methods@X aliases are recorded for cleanup.

Matrix translation terms prove original TransformPoint, not the direction-only TransformVector. Original FableWin TPI declares both overloads; TransformPoint procedure locals independently name point,xx,yy,zz. In-place overload is public const void(C3DVector&). CParticleSimulation accessors and original vect/index parameter names are recovered from lib_particle_simulation debug procedure records.

Evidence JSON and native disassembly remain private workspace material; no retail bytes are included in hand-back. Selected candidate135 is now an exact1090-byte relocation-verified match; all prior source shapes remain in immutable history.

## Recovered final matrix overload

Original FableWin exact `?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z` is public const void(const C3DVector&,C3DVector&), with parameters pSource and destination and no extra named locals. The in-place overload has point and const-float xx/yy/zz locals. Both exact symbols are emitted by the selected object. The final target scope has only original C3DVector pos, so a getter return temporary passed directly to the two-reference helper requires no invented local. Native separate input and output stack homes plus complete byte identity independently confirm this call shape. The helper body retains only actual original fields and methods.

The original C4DVector::ToC3DVector debug procedure is53 bytes with no named locals; GetParticlePosition37 bytes and SetParticlePosition40 bytes have only their original index/vect parameters. The getter/Initialise APIs and original3D/4D layouts are retained unchanged. No game global or extra address binding is introduced.
