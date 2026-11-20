// NShape::CTrack::GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const -- sample the track at steps of 0.1 length units and return the sampled position (and its length fraction) nearest to pos
// Leaf: ?GetTrackPosNearest@CTrack@NShape@@QBEXABVC3DVector@@AAV3@AAM@Z
// Name (NF120, PRIORITY q4952): Ego_r.pdb 0x00771470 / FableWin.pdb 0x025DB750 ?GetTrackPosNearest@CTrack@NShape@@QBEXABVC3DVector@@AAV3@AAM@Z
//   (const, references); the landed callers 0x0075DC60 / 0x0087B840 bind it and 0x0087B910 is respelled with it. The callee is the const
//   ?GetTrackPosAtLengthFraction@CTrack@NShape@@QBE?AVC3DVector@@M@Z (both PDBs; the landed self row 0x00760C40 defines it). Was non-const
//   with pointers (QAEXPBV..PAV3@PAM@Z) and a non-const callee; local model respelled in place.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00760DA0, length 195 bytes (`ret 0Ch`; 208 with padding).
// Callees: NShape::CTrack::GetTrackPosAtLengthFraction(float) @ 0x00760C40 (C3DVector by hidden pointer).
// Constants: 0x0122EE58 = 0.1f, 0x0122DED8 = 1.0f; the initial best distance 1e10f is an immediate.
// Layout: NShape::CTrack TrackLength +0x24 (as the CTrack constructor 0x00761F40).
// Shape: a do/while over the fraction (`(t += step) < 1.0f`, no entry test); the squared distance is written
// dx*dx + dy*dy + dz*dz (VC7.1 evaluates the right-hand squares first: z, y, x); `d < best` keeps the copy-out in the
// taken arm.
#include "engine/C3DVector.h"
namespace NShape {
class CTrack { public:
    virtual void v00();
    unsigned char _pad04[0x20];             // Owner, Points, SelectedPoints, TrackTraverser
    float         TrackLength;              // +0x24
    C3DVector GetTrackPosAtLengthFraction(float fraction) const;   // 0x00760C40
    void GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const;
};
}

void NShape::CTrack::GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const
{
    float step = 0.1f / TrackLength;
    float best = 1e10f;
    float t = 0.0f;
    do {
        C3DVector p = GetTrackPosAtLengthFraction(t);
        float dx = p.X - pos.X;
        float dy = p.Y - pos.Y;
        float dz = p.Z - pos.Z;
        float d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            best = d;
            nearest = p;
            fraction = t;
        }
    } while ((t += step) < 1.0f);
}