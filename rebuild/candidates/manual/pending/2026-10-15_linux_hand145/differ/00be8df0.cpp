// CLandscapeBackgroundPatch::PackVertexNormal(const C3DVector& normal) -- pack a unit normal into 11:11:10 bits (x*1023 -> bits 0-10, y*1023 -> bits 11-21, z*511 -> bits 22-31, each rounded with fistp); stores the packed word in the vertex (+0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE8DF0, length 112 bytes.
// Grade: DIFFER(81v111) -- retail rounds each component with a bare dword `fistp` after a float store/reload through the dead argument slot (an inline-asm FTOI helper in the original); the C cast gives `__ftol2` calls (/QIfist gives qword fistp without the reload). Lane gap: inline-asm rounding helper.
// Callees: none. Float pool: 0x0129EF90 = 1023.0f, 0x0129EF94 = 511.0f (the z scale).
// Shape: the three rounded components land in a 3-word frame (y at [esp], z at [esp+4], x at [esp+8]) and are
// combined as ((y & 0x7ff) | (z << 11)) << 11 | (x & 0x7ff).
struct C3DVector { float X, Y, Z; };
inline long FloatToInt(float value) { return (long)value; }   // retail: `fld value ; fistp result` (inline asm)
struct CLandscapeBackgroundPatch {
    unsigned long m_packedNormal;                  // +0x00 (the packed vertex normal; `this` is the vertex)
    void PackVertexNormal(const C3DVector& normal);
};

void CLandscapeBackgroundPatch::PackVertexNormal(const C3DVector& normal)
{
    long z = FloatToInt(normal.Z * 511.0f);
    long y = FloatToInt(normal.Y * 1023.0f);
    long x = FloatToInt(normal.X * 1023.0f);
    m_packedNormal = (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}
