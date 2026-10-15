// CEngineLandscapePatch::ClipPatch() -- tests the patch's bounding box against the first four planes of the engine camera's frustum; false as soon as the box lies wholly outside one plane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3860, length 140 bytes (144 with padding).
// Globals: the engine camera singleton @ 0x01436EA0 (its frustum planes, 16 bytes each {nx, ny, nz, d},
// start at +0x1C0; retail walks them with a pointer to the third word, edx = camera + 0x1C8), the 0.0f pool
// constant @ 0x0122DEDC. No calls.
// Shape: for each plane the box corner nearest the plane's negative side is picked per axis
// (`n > 0 ? min : max`, `fcomp ; test ah,41h ; jne` = "not greater"), the dot product is summed z, y, x
// (the z term is formed first: `pz*nz + py*ny + px*nx`) and compared with d; `dot > d` returns false.
// `this` stays in ecx (no calls); the box min sits at +0x20, the max at +0x2C (a C3DBoundingBox-style
// pair of 3-vectors inside the patch; the patch has no PDB header, so it is modelled locally).
struct C3DVector {
    float x, y, z;
};

struct CFrustumPlane {
    float nx, ny, nz, d;
};

struct CEngineCamera {
    unsigned char m_unknown[0x1c0];
    CFrustumPlane m_planes[6];                     // +0x1C0
};
extern CEngineCamera* g_pEngineCamera_01436ea0;    // 0x01436EA0

struct CEngineLandscapePatch {
    unsigned char _pad00[0x20];
    C3DVector     m_boxMin;                        // +0x20
    C3DVector     m_boxMax;                        // +0x2C
    bool ClipPatch();
};

bool CEngineLandscapePatch::ClipPatch()
{
    for (long i = 0; i < 4; ++i) {
        const CFrustumPlane& plane = g_pEngineCamera_01436ea0->m_planes[i];
        float nx = plane.nx;
        float ny = plane.ny;
        float nz = plane.nz;
        float px = nx > 0.0f ? m_boxMin.x : m_boxMax.x;
        float py = ny > 0.0f ? m_boxMin.y : m_boxMax.y;
        float pz = nz > 0.0f ? m_boxMin.z : m_boxMax.z;
        if (pz * nz + py * ny + px * nx > plane.d)
            return false;
    }
    return true;
}