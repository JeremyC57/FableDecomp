// Grade: DIFFER(745v724) -- the inline system-space transform multiplies vector-from-memory where retail loads the matrix element first (fld [E31]; fmul [z]) and the else loop keeps system in ebp not ebx; the clamped count spills to [esp+0x10] instead of staying in ebx, so the node copy reloads it
// NParticleEngine::CPSCSingleSprite::SetTrailPositionsFromSpline(CParticleSystem& system, CParticleUpdateParameters* params, float width, float time) -- rebuild the sprite trail from the component's spline: clear the trail, then append one entry (position moved into system space unless the system is local, plus the trail width) per control point (up to MaxTrailLength) when UseSplinePoints is set, otherwise MaxTrailLength + 1 evenly spaced positions along the spline
// Leaf: SetTrailPositionsFromSpline
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B08250, 724 bytes (+ int3 padding), `ret 10h`.
// Callees: free 0x00BFEA14 / malloc 0x00BFEA0E (STLport list nodes), CPSCSplineBase::GetPointAtIndex 0x00B219A0 /
//   GetPointAtPos 0x00B21940, CParticleSystem::GetInverseSystemTransform(CMatrix3x4&) 0x00AF2250. Float 1.0 0x0122DED8.
// Layout: Ego_r.pdb CPSCSingleSprite minus 4 (retail 12-byte CArray in the CPSCSplineBase part): Spline control points
//   +0x10, MaxTrailLength +0x88, UseSplinePoints +0x96, TrailUpdatePrimitive +0xac with its STLport
//   list<CSpriteTrailEntry> Trail (head node pointer) at +0xb8; CSpriteTrailEntry {C3DVector Position, float Width}.
//   CParticleSystem's local-space flag is bit 6 of +0x50 (landed CPSCLight::AddToBoundingSphere 0x00B18AF0).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
template <class T> struct CArray {                     // STLport vector<T> (12 bytes in retail)
    T* m_begin; T* m_end; T* m_capacity;
    long Size() const { return (long)(m_end - m_begin); }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;                   // +0x00
    float             Tension;                         // +0x0c
    bool              EndPointsConnected;              // +0x10
};
template <class T> struct SListNode {                  // STLport _List_node<T>
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CList {                      // STLport list<T>: the head node pointer
    SListNode<T>* _M_node;
    __forceinline void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    template <class A, class B> __forceinline void emplace_back(const A& a, const B& b)
    {
        SListNode<T>* position = _M_node;
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        new (&node->_M_data) T(a, b);
        node->_M_next = position;
        node->_M_prev = position->_M_prev;
        position->_M_prev->_M_next = node;
        position->_M_prev = node;
    }
};
struct CSpriteTrailEntry {
    C3DVector Position;
    float     Width;
    CSpriteTrailEntry(const C3DVector& position, float width) : Position(position), Width(width) {}
};
struct CEnginePrimitiveSpriteTrail {
    unsigned char           _base00[0x0c];
    CList<CSpriteTrailEntry> Trail;                    // +0x0c
};
template <class A, class B> struct CParticleUpdateParameters;
namespace NParticleEngine {
struct CParticleSystem {
    unsigned char _pad00[0x50];
    unsigned char Flags;                               // +0x50 (bit 6: local space)
    void GetInverseSystemTransform(CMatrix3x4& matrix);   // 0x00AF2250
};
__forceinline void ToSystemSpace(CParticleSystem& system, C3DVector& p)
{
    if (!(system.Flags & 0x40)) {
        CMatrix3x4 m;
        system.GetInverseSystemTransform(m);
        float x = p.X;
        float y = p.Y;
        float z = p.Z;
        p.X = m.E31 * z + m.E21 * y + m.E11 * x + m.E41;
        p.Y = m.E32 * z + m.E22 * y + m.E12 * x + m.E42;
        p.Z = m.E33 * z + m.E23 * y + m.E13 * x + m.E43;
    }
}
struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;                   // +0x04
    bool UpdateStarted : 1;                            // +0x08
    bool Initialised : 1;
};
struct CPSCSplineBase : CPSCBase {
    float           SplineAnimSpeed;                   // +0x0c
    CCardinalSpline Spline;                            // +0x10
    void*           SplineRandomOffsetData[2];         // +0x24
    void*           SplinePosParamData[2];             // +0x2c
    void GetPointAtPos(CParticleUpdateParameters<unsigned long, unsigned long>* params, C3DVector* out, float pos, float time);      // 0x00B21940
    void GetPointAtIndex(CParticleUpdateParameters<unsigned long, unsigned long>* params, C3DVector* out, long index, float time);  // 0x00B219A0
};
struct CPSCSingleSprite : CPSCSplineBase {
    unsigned char               _state34[0x88 - 0x34];
    long                        MaxTrailLength;        // +0x88
    long                        StayWithEmitterIntFactor;   // +0x8c
    bool                        UseStartColour;        // +0x90
    bool                        UseMidColour;
    bool                        UseEndColour;
    unsigned char               Flags93;
    unsigned char               Flags94;
    bool                        UsePosition;           // +0x95
    bool                        UseSplinePoints;       // +0x96
    bool                        EnteringTimelineEnd;
    unsigned long               LastUpdateFrame;       // +0x98
    long                        Lifetimer;             // +0x9c
    C3DVector                   Offset;                // +0xa0
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;  // +0xac
    void SetTrailPositionsFromSpline(CParticleSystem& system, CParticleUpdateParameters<unsigned long, unsigned long>* params, float width, float time);
};

void CPSCSingleSprite::SetTrailPositionsFromSpline(CParticleSystem& system, CParticleUpdateParameters<unsigned long, unsigned long>* params, float width, float time)
{
    TrailUpdatePrimitive.Trail.clear();
    if (UseSplinePoints) {
        long count = MaxTrailLength < Spline.ControlPoints.Size() ? MaxTrailLength : Spline.ControlPoints.Size();
        for (long i = 0; i < count; ++i) {
            C3DVector point;
            GetPointAtIndex(params, &point, i, time);
            C3DVector p = point;
            ToSystemSpace(system, p);
            TrailUpdatePrimitive.Trail.emplace_back(p, width);
        }
    } else {
        float step = 1.0f / (float)MaxTrailLength;
        for (float pos = 0.0f; pos <= 1.0f; pos += step) {
            C3DVector point;
            GetPointAtPos(params, &point, pos, time);
            C3DVector p = point;
            ToSystemSpace(system, p);
            TrailUpdatePrimitive.Trail.emplace_back(p, width);
        }
    }
}
}
