// C2DBoxI::ClipLineTo(C2DLineF&) const -- clip a line to the box: the first edge crossing replaces the start point, later crossings the end point
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39FE0, length 355 bytes.
// Callees: C2DLineF::IntersectsWith(const C2DLineF&, C2DVector*, float) @ 0x00A56C80.
// Globals: intersection tolerance @ 0x0129BA3C (0.0001f, pushed as a dword; same symbol as the landed
//          C2DLineF::IntersectsWith(C2DBoxI*) @ 0x00A57AD0).
// Shape: each box edge is a named function-scope C2DLineF built from the int corners just before its call (VC
//          schedules the fild/fstp between the tolerance and &hit pushes; unnamed temporaries would share one slot),
//          so the four edges get four 16-byte frame slots (0x48 frame with the hit point). A `first` flag in bl
//          picks start or end; the first edge needs no test (the flag is still true there).
// Layout: C2DBoxI {minX, minY, maxX, maxY} longs; C2DLineF {x1, y1, x2, y2} floats (as the landed 0x00A57AD0).
struct C2DVector { float x; float y; };
struct C2DLineF {
    float x1; float y1; float x2; float y2;
    C2DLineF(float ax, float ay, float bx, float by) : x1(ax), y1(ay), x2(bx), y2(by) {}
    bool IntersectsWith(const C2DLineF& other, C2DVector* point, float tolerance);   // 0x00A56C80
};
struct C2DBoxI {
    long minX; long minY; long maxX; long maxY;
    void ClipLineTo(C2DLineF& line) const;
};
extern float g_C2DIntersectionTolerance;   // 0x0129BA3C

void C2DBoxI::ClipLineTo(C2DLineF& line) const
{
    C2DVector hit;
    bool first = true;
    C2DLineF top((float)minX, (float)minY, (float)maxX, (float)minY);
    if (line.IntersectsWith(top, &hit, g_C2DIntersectionTolerance)) {
        if (first) { line.x1 = hit.x; line.y1 = hit.y; }
        else { line.x2 = hit.x; line.y2 = hit.y; }
        first = false;
    }
    C2DLineF right((float)maxX, (float)minY, (float)maxX, (float)maxY);
    if (line.IntersectsWith(right, &hit, g_C2DIntersectionTolerance)) {
        if (first) { line.x1 = hit.x; line.y1 = hit.y; }
        else { line.x2 = hit.x; line.y2 = hit.y; }
        first = false;
    }
    C2DLineF bottom((float)minX, (float)maxY, (float)maxX, (float)maxY);
    if (line.IntersectsWith(bottom, &hit, g_C2DIntersectionTolerance)) {
        if (first) { line.x1 = hit.x; line.y1 = hit.y; }
        else { line.x2 = hit.x; line.y2 = hit.y; }
        first = false;
    }
    C2DLineF left((float)minX, (float)minY, (float)minX, (float)maxY);
    if (line.IntersectsWith(left, &hit, g_C2DIntersectionTolerance)) {
        if (first) { line.x1 = hit.x; line.y1 = hit.y; }
        else { line.x2 = hit.x; line.y2 = hit.y; }
    }
}