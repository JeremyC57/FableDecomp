// CEngineInternalPrimitiveSpriteTrail::GetWorldPosition(C3DVector& position) -- the centre of the box bounding every trail point grown by the trail width; false for an empty trail
// Grade: DIFFER(334v334) -- frame-slot order only: retail puts the box at [esp+8] and the point copy above it ([esp+20h]); this build puts the point copy at [esp+8] and the box above it (every other instruction matches) -- this body is probe grade 3 variant NB | tried (4 grades): GetCenter via operator+/operator* (temporaries, 374), GetCenter by components (358), std-style min/max templates (434), no user copy ctor (388), the point declared outside the loop / before the box (348), a forced-inline AddSphereToBox(box, C3DVector, float) helper (graded file, 354) | next: VC7.1 frame-slot ordering (declaration order of the box vs the point copy; perhaps the box is a member-wise pair of C3DVector locals)
// Leaf: ?GetWorldPosition@CEngineInternalPrimitiveSpriteTrail@@UAE_NAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCC1C0, length 334 bytes, `ret 4`.
// Name: FableWin.pdb `?GetWorldPosition@CEngineInternalPrimitiveSpriteTrail@@UAE_NAAVC3DVector@@@Z` (virtual). No callees.
// Layout: Ego_r.pdb CEngineInternalPrimitiveSpriteTrail Width +0x58, Trail +0x68 (CArray<CSpriteTrailEntry>, retail 12 bytes:
//   data +0x68, last +0x6C); CSpriteTrailEntry {C3DVector Position, float Width} (16 bytes); C3DBoundingBox {MinCoord, MaxCoord}.
//   No engine/ header exists for the class.
// Constants: -FLT_MAX @ 0x012A6ABC (the enregistered MaxCoord.Z), 0.5f @ 0x0122F59C; the other extremes are immediates.
// Shape: `size() == 0` is `test eax,0FFFFFFF0h` on the byte difference; each point is copied (memberwise C3DVector copy
//   constructor: X stays on the x87 stack, Y/Z go to the frame); the min tests keep the box value when it is smaller
//   (`test ah,5 ; jnp`), the max tests when it is larger (`test ah,41h ; je`); the centre is written component by component.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C3DBoundingBox { public:
    C3DVector MinCoord;                                    // +0x00
    C3DVector MaxCoord;                                    // +0x0c
    C3DBoundingBox()                                       // FableWin ??0C3DBoundingBox@@QAE@XZ: an inverted (empty) box
        : MinCoord(3.402823466e+38f, 3.402823466e+38f, 3.402823466e+38f),
          MaxCoord(-3.402823466e+38f, -3.402823466e+38f, -3.402823466e+38f) { }
};
template <class T> class CArray { public:                 // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pData; }
    T* begin() const { return m_pData; }
    T* end() const { return m_pLast; }
};
class CEngineInternalPrimitiveSpriteTrail { public:
    class CSpriteTrailEntry { public:
        C3DVector Position;                                // +0x00
        float     Width;                                   // +0x0c
    };
    virtual bool GetWorldPosition(C3DVector& position);
    unsigned char             _pad04[0x58 - 0x04];
    float                     Width;                       // +0x58
    unsigned char             _pad5c[0x68 - 0x5c];
    CArray<CSpriteTrailEntry> Trail;                       // +0x68
};

bool CEngineInternalPrimitiveSpriteTrail::GetWorldPosition(C3DVector& position)
{
    if (Trail.size() == 0)
        return false;
    C3DBoundingBox box;
    for (CSpriteTrailEntry* it = Trail.begin(); it != Trail.end(); ++it) {
        const C3DVector pos = it->Position;
        box.MinCoord.X = box.MinCoord.X < pos.X - Width ? box.MinCoord.X : pos.X - Width;
        box.MinCoord.Y = box.MinCoord.Y < pos.Y - Width ? box.MinCoord.Y : pos.Y - Width;
        box.MinCoord.Z = box.MinCoord.Z < pos.Z - Width ? box.MinCoord.Z : pos.Z - Width;
        box.MaxCoord.X = box.MaxCoord.X > pos.X + Width ? box.MaxCoord.X : pos.X + Width;
        box.MaxCoord.Y = box.MaxCoord.Y > pos.Y + Width ? box.MaxCoord.Y : pos.Y + Width;
        box.MaxCoord.Z = box.MaxCoord.Z > pos.Z + Width ? box.MaxCoord.Z : pos.Z + Width;
    }
    position.X = (box.MaxCoord.X + box.MinCoord.X) * 0.5f;
    position.Y = (box.MaxCoord.Y + box.MinCoord.Y) * 0.5f;
    position.Z = (box.MaxCoord.Z + box.MinCoord.Z) * 0.5f;
    return true;
}
