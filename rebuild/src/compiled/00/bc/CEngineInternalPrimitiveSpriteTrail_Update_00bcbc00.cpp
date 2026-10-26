// CEngineInternalPrimitiveSpriteTrail::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a sprite-trail descriptor (length cap, graphic, skip, width, head / mid / tail colours, flags, blend op/mode, world matrix); when the descriptor only extends the trail, shift the stored points by the offset vector (if it is not negligible), grow the ring to the cap and write the new points at the ring cursor, otherwise resize the ring to the descriptor's points and copy them from slot 0; then apply the game-primitive fade defaults with the descriptor's fade start
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCBC00, `ret 8`, virtual `?Update@CEngineInternalPrimitiveSpriteTrail@@UAEXABVCEnginePrimitive@@K@Z`.
// Callees: vector<CSpriteTrailEntry>::erase(first, last) @ 0x00BCBF20, ::_M_fill_insert(pos, n, x) @ 0x00BCC080,
//   ::resize(n) @ 0x00BCC3B0, CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(float) @ 0x00B8FD40.
// Layout: base = engine/CEngineInternalPrimitiveBase.h (EnginePrimitiveFlags +0x0c). Retail STLport containers are smaller
//   than the PDB's: the descriptor's list<CSpriteTrailEntry> is one node pointer (+0x0c), so every later descriptor field sits
//   8 below the PDB (UpdateExisting +0x10 .. WorldTransform +0x50); the internal Trail vector is 12 bytes (+0x68), so NextPos
//   +0x74, Flags +0x78, BlendOp +0x7c, BlendMode +0x80, WorldMat +0x84 sit 4 below the PDB.
// Shape: the offset test is `LengthSquared() > Sqr(eps)` with eps the extern GLOBAL 0x0129BA3C (a literal folds eps*eps) and the
//   length through float locals z,y,x; the shift is written per component (an operator+= flips the Z operand order); the new
//   size reads `trail.Trail.size() + points.size()` through a named vector reference (places `lea edi` inside the list walk);
//   resize(n, x) takes x by const reference over an uninitialised temporary; the list entry converts through a ctor that
//   assigns component-wise (a member-init C3DVector copy adds a `lea`).
#include "engine/CEngineInternalPrimitiveBase.h"

class C3DVector { public:
    float X, Y, Z;
    float LengthSquared() const { float z = Z, y = Y, x = X; return x * x + y * y + z * z; }
};
inline float Sqr(float x) { return x * x; }
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)
class CRGBColour { public: unsigned char B, G, R, A; };
class CMatrix3x4 { public: float m[12]; };

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveSpriteTrail : public CEnginePrimitive {
public:
    class CSpriteTrailEntry { public:
        C3DVector Position;
        float     Width;
    };
    struct _Node {
        _Node*            _M_next;
        _Node*            _M_prev;
        CSpriteTrailEntry _M_data;
    };
    struct CTrailList {                            // STLport list: the sentinel node pointer
        _Node* _M_node;
        _Node* begin() const { return _M_node->_M_next; }
        _Node* end() const { return _M_node; }
        unsigned long size() const
        {
            unsigned long n = 0;
            for (_Node* p = begin(); p != end(); p = p->_M_next)
                ++n;
            return n;
        }
    };
    CTrailList     Trail;                          // +0x0c
    bool           UpdateExisting;                 // +0x10
    unsigned long  MaxLength;                      // +0x14
    unsigned long  GraphicIndex;                   // +0x18
    unsigned long  GraphicFrame;                   // +0x1c
    float          SkipPoints;                     // +0x20
    float          Width;                          // +0x24
    CRGBColour     HeadColour;                     // +0x28
    CRGBColour     MidColour;                      // +0x2c
    CRGBColour     TailColour;                     // +0x30
    float          FadeStart;                      // +0x34
    C3DVector      OffsetVector;                   // +0x38
    unsigned short Flags;                          // +0x44
    long           BlendMode;                      // +0x48
    long           BlendOp;                        // +0x4c
    CMatrix3x4     WorldTransform;                 // +0x50
};

struct CSpriteTrailEntry {
    C3DVector Position;
    float     Width;
    CSpriteTrailEntry() {}
    CSpriteTrailEntry(const CEnginePrimitiveSpriteTrail::CSpriteTrailEntry& e) { Position.X = e.Position.X; Position.Y = e.Position.Y; Position.Z = e.Position.Z; Width = e.Width; }
};

struct CSpriteTrailVector {                        // STLport vector<CSpriteTrailEntry>, 12 bytes
    CSpriteTrailEntry* _M_start;
    CSpriteTrailEntry* _M_finish;
    CSpriteTrailEntry* _M_end_of_storage;
    CSpriteTrailEntry* begin() { return _M_start; }
    CSpriteTrailEntry* end() { return _M_finish; }
    unsigned long size() { return end() - begin(); }
    CSpriteTrailEntry& operator[](unsigned long n) { return *(begin() + n); }
    CSpriteTrailEntry* erase(CSpriteTrailEntry* first, CSpriteTrailEntry* last);                          // 0x00BCBF20
    void _M_fill_insert(CSpriteTrailEntry* pos, unsigned long n, const CSpriteTrailEntry& x);           // 0x00BCC080
    void insert(CSpriteTrailEntry* pos, unsigned long n, const CSpriteTrailEntry& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned long n, const CSpriteTrailEntry& x)
    {
        if (n < size())
            erase(begin() + n, end());
        else
            insert(end(), n - size(), x);
    }
    void resize(unsigned long n);                                                                        // 0x00BCC3B0
};

class CEngineInternalPrimitiveSpriteTrail : public CEngineInternalPrimitiveBase { public:
    unsigned long      MaxLength;                  // +0x48
    unsigned long      GraphicIndex;               // +0x4c
    unsigned long      GraphicFrame;               // +0x50
    float              SkipPoints;                 // +0x54
    float              Width;                      // +0x58
    CRGBColour         HeadColour;                 // +0x5c
    CRGBColour         MidColour;                  // +0x60
    CRGBColour         TailColour;                 // +0x64
    CSpriteTrailVector Trail;                      // +0x68
    long               NextPos;                    // +0x74
    unsigned short     Flags;                      // +0x78
    long               BlendOp;                    // +0x7c
    long               BlendMode;                  // +0x80
    CMatrix3x4         WorldMat;                   // +0x84
    void SetFadeDistanceWithGamePrimitiveDefaults(float distance);   // 0x00B8FD40
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveSpriteTrail::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveSpriteTrail& trail = static_cast<const CEnginePrimitiveSpriteTrail&>(primitive);
    EnginePrimitiveFlags = flags;
    MaxLength = trail.MaxLength;
    GraphicIndex = trail.GraphicIndex;
    GraphicFrame = trail.GraphicFrame;
    SkipPoints = trail.SkipPoints;
    Width = trail.Width;
    HeadColour = trail.HeadColour;
    MidColour = trail.MidColour;
    TailColour = trail.TailColour;
    Flags = trail.Flags;
    BlendMode = trail.BlendMode;
    BlendOp = trail.BlendOp;
    WorldMat = trail.WorldTransform;
    if (trail.UpdateExisting) {
        if (trail.OffsetVector.LengthSquared() > Sqr(g_epsilon_0129ba3c)) {
            for (CSpriteTrailEntry* it = Trail.begin(); it != Trail.end(); ++it) {
                it->Position.X += trail.OffsetVector.X;
                it->Position.Y += trail.OffsetVector.Y;
                it->Position.Z += trail.OffsetVector.Z;
            }
        }
        CSpriteTrailVector& points = Trail;
        long newSize = trail.Trail.size() + points.size();
        if (newSize > MaxLength)
            newSize = MaxLength;
        Trail.resize(newSize, CSpriteTrailEntry());
        NextPos = NextPos % MaxLength;
        for (CEnginePrimitiveSpriteTrail::_Node* p = trail.Trail.begin(); p != trail.Trail.end(); p = p->_M_next) {
            if (NextPos >= newSize)
                NextPos = 0;
            Trail[NextPos] = p->_M_data;
            ++NextPos;
        }
    } else {
        Trail.resize(trail.Trail.size());
        NextPos = 0;
        for (CEnginePrimitiveSpriteTrail::_Node* p = trail.Trail.begin(); p != trail.Trail.end(); p = p->_M_next) {
            Trail[NextPos] = p->_M_data;
            ++NextPos;
        }
    }
    SetFadeDistanceWithGamePrimitiveDefaults(trail.FadeStart);
}