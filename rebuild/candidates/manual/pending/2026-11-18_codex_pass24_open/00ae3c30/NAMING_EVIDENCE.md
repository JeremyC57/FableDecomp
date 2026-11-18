# CParticleProgram::ReadFromFile

The VC7.1 object exports `?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z`. The source-order record in Ego_d identifies `CParticleProgram::ReadFromFile`. The submitted extent is 2310 bytes.

- egor donor procedure at 00bf2ee0
- fablewin donor procedure at 033351a0

- call at 0x00AC3D70 in landed 0x00AC3BD0 C3DClothPrimitive::LoadBinary
- call at 0x00AC3E80 in landed 0x00AC3E00 C3DClothPrimitive::C3DClothPrimitive

At 00AE40AC the texture-coordinate buffer starts in EDI rather than EAX. Retail copies EAX to EDI after the count test. The later read and the remainder of the function realign.

The direct fixture covers independent empty arrays, simulation and allocation failure, exports and both groups.
