// Grade: DIFFER(19v1776) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CLandscapeBackgroundPatch::BuildDefaultSection() -- build the patch's default (unsubdivided) section: set the 5x4 grid
//   dimensions, allocate the vertex / index buffers from the landscape memory manager, fill the vertices (x87 positions
//   and normals, _ftol texture coordinates) and the edge strips, then lock / write / unlock the index buffer
// Leaf: BuildDefaultSection
// Flags: /GS
// Stub: x87-heavy 0xf8 frame; first-pass skeleton only, not reconstructed yet.
class CLandscapeBackgroundPatch {
public:
    unsigned char _pad00[0x104];
    unsigned short GridWidth;                          // +0x104
    unsigned short GridHeight;                         // +0x106
    void BuildDefaultSection();
};
void CLandscapeBackgroundPatch::BuildDefaultSection()
{
    GridHeight = 4;
    GridWidth = 5;
}
