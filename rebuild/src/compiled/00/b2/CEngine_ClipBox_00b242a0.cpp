// CEngine::ClipBox(const C3DBoundingBox&) -- build the box's world matrix and ask the engine camera whether the box is clipped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B242A0, length 35 bytes.
// Callees: C3DBoundingBox::CalculateMatrix(CMatrix3x4*) @ 0x00ADEBC0 (on the argument), CEngineCamera::ClipBox(CMatrix3x4*) @ 0x00B30610 (on the engine camera
// singleton @ 0x01436EA0, bool result). Shape: the 48-byte matrix local is uninitialised (`sub esp,30h`), `this` is never read (`mov ecx,[esp+4]` at once);
// `ret 4`.
class CMatrix3x4 { public:
    float m_rows[12];
};

class C3DBoundingBox { public:
    void CalculateMatrix(CMatrix3x4* matrix) const;   // 0x00ADEBC0
};

class CEngineCamera { public:
    bool ClipBox(CMatrix3x4* matrix);              // 0x00B30610
};
extern CEngineCamera* g_pEngineCamera_01436ea0;    // 0x01436EA0

class CEngine { public:
    bool ClipBox(const C3DBoundingBox& box);
};

bool CEngine::ClipBox(const C3DBoundingBox& box)
{
    CMatrix3x4 matrix;
    box.CalculateMatrix(&matrix);
    return g_pEngineCamera_01436ea0->ClipBox(&matrix);
}