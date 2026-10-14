// CLandscapeTesselatorBase::TesselatePatch(SLandscapePatch* patch, void* context) -- remember the patch (+0x10) and tesselate each of its four edges (+0x1c..+0x28 of the patch) that exists and is not already done (byte +0x28 of the edge) through TesselateEdgeGeneric (0x00BE37E0 with (edge index, edge, context)); false as soon as one edge fails, else true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE3910, length 96 bytes.
// Callees: 0x00BE37E0 (bool, thiscall). Shape: the edge index in ebx, the edge offset in esi stepping by 4 from 0x1c to 0x2c, the patch re-read from `this` each iteration (the call may change it), `ret 8`.
struct SLandscapeEdge_00be3910 {
    unsigned char _pad00[0x28];
    bool          m_tesselated;                    // +0x28
};
struct SLandscapePatch_00be3910 {
    unsigned char            _pad00[0x1c];
    SLandscapeEdge_00be3910* m_edges[4];           // +0x1c
};

struct CLandscapeTesselatorBase {
    unsigned char             _pad00[0x10];
    SLandscapePatch_00be3910* m_pPatch;            // +0x10
    bool TesselateEdgeGeneric(long edge, SLandscapeEdge_00be3910* edgeData, void* context);   // 0x00BE37E0
    bool TesselatePatch(SLandscapePatch_00be3910* patch, void* context);
};

bool CLandscapeTesselatorBase::TesselatePatch(SLandscapePatch_00be3910* patch, void* context)
{
    m_pPatch = patch;
    for (long edge = 0; edge < 4; ++edge) {
        SLandscapeEdge_00be3910* edgeData = m_pPatch->m_edges[edge];
        if (edgeData && !edgeData->m_tesselated) {
            if (!TesselateEdgeGeneric(edge, edgeData, context))
                return false;
        }
    }
    return true;
}