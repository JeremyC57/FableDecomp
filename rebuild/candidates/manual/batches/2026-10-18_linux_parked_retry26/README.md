# Three parked sprite constructors

0041bf60 and0043fa20 (167 bytes each) and0041beb0 (171) independently MATCH + behavior PASS. PDB CEnginePrimitive2DSprite/ThemeChannelDesc and landed006558CE restore Type, InterpolationSet, theme channels, ClipBox, Layer, BankIndex/Frame, angles, BlendMode, Colour, ClipBoxIsPos, Flags, Texture and UV offsets. Texture overloads clear BankIndex/Frame through the inline texture-frame subobject; channel subobject initialization reproduces retail store scheduling. Position overload copies the point into both box corners.

Coordinator integration changes names only; total grades10/10/4. No wrapper/composite headers exist; the generated internal renderer is a distinct object. Actual-source fixtures check all fields for zero/nonzero/high-bit values, color ordering, null texture, geometry/UV copying and input independence, and target sizeof/offsets. Host g++ and canonical VC7.1/Wine PASS.

0041beb0 input oracle overcaptured five trailing int3 bytes. Disassembly proves ret44 at0041bf58 ends171-byte body; only those five padding bytes were removed before successful independent verification.
