// CEngineLandscapeRenderer::CalcForegroundFadeFactors() -- derives the two linear fade ramps (fade-in and fade-out shader constants) from the foreground fade start/end distances
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B671E0, length 134 bytes (144 with padding).
// Globals: the 1.0f pool constant @ 0x0122DED8. No calls.
// Shape: with start = +0x614 and end = +0x618 two 4-float shader constants are rebuilt, the one at +0x62C first:
// (1/(end-start), 0, 0, end/(start-end) + 1) and then the one at +0x61C: (1/(start-end), 0, 0, start/(end-start) + 1),
// i.e. ramps that reach 1 at one distance and 0 at the other when dotted with (z, ., ., 1). Each is an inline
// `Set(x, y, z, w)`: its arguments are evaluated right to left (w is computed first and parked on the x87 stack),
// the two integer zero stores from one `xor eax,eax` register are scheduled early, then x and w are popped. A
// `C4DVector(x, y, z, w)` temporary assigned whole goes through a 16-byte frame copy (and VC rewrites the shared
// divisor as a reciprocal multiply), so it is not the form.
// Layout: the PDB places these words inside the embedded CEngineLandscapeMeshBuilder (+0x1C..+0xCF4) of the PDB
// layout, which does not fit this body; the renderer is modelled locally with descriptive names.
class C4DVector { public:
    float x, y, z, w;
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
};

class CEngineLandscapeRenderer { public:
    unsigned char _pad000[0x614];
    float         m_foregroundFadeStart;           // +0x614
    float         m_foregroundFadeEnd;             // +0x618
    C4DVector     m_fadeInFactors;                 // +0x61C
    C4DVector     m_fadeOutFactors;                // +0x62C
    void CalcForegroundFadeFactors();
};

void CEngineLandscapeRenderer::CalcForegroundFadeFactors()
{
    m_fadeOutFactors.Set(1.0f / (m_foregroundFadeEnd - m_foregroundFadeStart), 0.0f, 0.0f,
                         m_foregroundFadeEnd / (m_foregroundFadeStart - m_foregroundFadeEnd) + 1.0f);
    m_fadeInFactors.Set(1.0f / (m_foregroundFadeStart - m_foregroundFadeEnd), 0.0f, 0.0f,
                        m_foregroundFadeStart / (m_foregroundFadeEnd - m_foregroundFadeStart) + 1.0f);
}