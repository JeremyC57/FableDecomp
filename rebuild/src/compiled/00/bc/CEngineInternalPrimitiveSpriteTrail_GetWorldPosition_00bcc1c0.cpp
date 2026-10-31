// CEngineInternalPrimitiveSpriteTrail::GetWorldPosition(C3DVector& position) -- the centre of the box bounding every trail point grown by the trail width; false for an empty trail
// Leaf: ?GetWorldPosition@CEngineInternalPrimitiveSpriteTrail@@UAE_NAAVC3DVector@@@Z
// Grade: RELOCATION_MATCH (retry wave 2) -- the bounds are two C3DVector locals (min at +FLT_MAX, max at -FLT_MAX)
//   instead of one C3DBoundingBox, which puts them below the point copy in the frame as retail
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
    C3DVector minCoord(3.402823466e+38f, 3.402823466e+38f, 3.402823466e+38f);
    C3DVector maxCoord(-3.402823466e+38f, -3.402823466e+38f, -3.402823466e+38f);
    for (CSpriteTrailEntry* it = Trail.begin(); it != Trail.end(); ++it) {
        const C3DVector pos = it->Position;
        minCoord.X = minCoord.X < pos.X - Width ? minCoord.X : pos.X - Width;
        minCoord.Y = minCoord.Y < pos.Y - Width ? minCoord.Y : pos.Y - Width;
        minCoord.Z = minCoord.Z < pos.Z - Width ? minCoord.Z : pos.Z - Width;
        maxCoord.X = maxCoord.X > pos.X + Width ? maxCoord.X : pos.X + Width;
        maxCoord.Y = maxCoord.Y > pos.Y + Width ? maxCoord.Y : pos.Y + Width;
        maxCoord.Z = maxCoord.Z > pos.Z + Width ? maxCoord.Z : pos.Z + Width;
    }
    position.X = (maxCoord.X + minCoord.X) * 0.5f;
    position.Y = (maxCoord.Y + minCoord.Y) * 0.5f;
    position.Z = (maxCoord.Z + minCoord.Z) * 0.5f;
    return true;
}