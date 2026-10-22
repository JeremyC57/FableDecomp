// Grade: DIFFER(664v654) -- the parameter key is reloaded through &PositionParam after LowerBound where retail keeps the value tested in ebp, and the inline system-space transform evaluates E21*y before E31*z and loads the vector component first (retail: fld [E31]; fmul [z])
// NParticleEngine::CPSCSingleSprite::Update(CParticleSystem& system, CParticleUpdateParameters<ulong,ulong>* params, bool& alive) -- per-frame trail update: report the component alive unless it is entering the timeline end with no time left, on the first frame of a position-tracking sprite restart the trail at the tracked position at time 0, then append the tracked position at time 1 (moved into system space unless the system is local); a spline sprite instead rebuilds its trail from the spline when the trail is empty or only position parameters drive it; finally count the frame
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B08530, 654 bytes (+ int3 padding), `ret 0Ch`.
// Callees: the component's virtual slot 31 (+0x7c, remaining time), the trail list clear 0x00B09D50 (label
//   "SortTreeRecursively" propagated), CVectorMap<ulong, CParticleUpdateParamDesc>::LowerBound 0x00AF2C50, the parameter
//   callback's virtual slot 1 (+0x04), CParticleSystem::GetInverseSystemTransform 0x00AF2250 and its scaled twin 0x00B05B10,
//   CPSCSingleSprite::AddTrailPoint 0x00B081A0 (label "EventQueue_PushEvent" propagated), SetTrailPositionsFromSpline
//   0x00B08250. Floats 0.0 0x0122DEDC.
// Layout: Ego_r.pdb CPSCSingleSprite minus 4 (retail 12-byte CArray in the CPSCSplineBase part): SplineRandomOffsetData
//   +0x24, SplinePosParamData +0x2c, PositionParam +0x6c, TrailWidth +0x84, UsePosition +0x95, EnteringTimelineEnd +0x97,
//   Lifetimer +0x9c, TrailUpdatePrimitive.Trail (STLport list head) +0xb8. CParticleUpdateParameters<ulong,ulong>:
//   VectorParams +0x04, UpdateParams +0x14 (CVectorMap: 12-byte vector of pair<ulong, CParticleUpdateParamDesc>, 16-byte
//   entries: key, desc vtable, Callback +0x08, Index +0x0c).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
template <class T> struct SListNode {                  // STLport _List_node<T>
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CList {                      // STLport list<T>: the head node pointer
    SListNode<T>* _M_node;
    void clear();                                      // 0x00B09D50
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SListNode<T>* cur = _M_node->_M_next; cur != _M_node; cur = cur->_M_next)
            ++n;
        return n;
    }
};
struct CSpriteTrailEntry {
    C3DVector Position;
    float     Width;
};
struct CEnginePrimitiveSpriteTrail {
    unsigned char            _base00[0x0c];
    CList<CSpriteTrailEntry> Trail;                    // +0x0c
};
namespace NParticleEngine {
template <class A, class B> struct CParticleUpdateParameters;
template <class A, class B> struct CParticleUpdateParamCallback {
    virtual void v00();
    virtual bool GetValue(C3DVector* out, B index, float time, void* context, CParticleUpdateParameters<A, B>* params, bool interpolate);   // slot 1 (+0x04)
};
template <class A, class B> struct CParticleUpdateParamDesc {
    virtual void v00();
    CParticleUpdateParamCallback<A, B>* Callback;      // +0x04
    B                                   Index;         // +0x08
};
template <class K, class V> struct CVectorMapEntry {
    K first;
    V second;
};
template <class K, class V> struct CVectorMap {        // sorted vector + compare + dirty flag (retail 16 bytes)
    CVectorMapEntry<K, V>* _M_start;
    CVectorMapEntry<K, V>* _M_finish;
    CVectorMapEntry<K, V>* _M_end_of_storage;
    char                   Compare;
    bool                   Dirty;
    CVectorMapEntry<K, V>* LowerBound(const K& key);   // 0x00AF2C50
    CVectorMapEntry<K, V>* end() { return _M_finish; }
    CVectorMapEntry<K, V>* find(const K& key)
    {
        CVectorMapEntry<K, V>* it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
};
template <class A, class B> struct CParticleUpdateParameters {
    void*                                        __vftable;      // +0x00
    CVectorMap<A, C3DVector>                     VectorParams;   // +0x04
    CVectorMap<A, CParticleUpdateParamDesc<A, B> > UpdateParams; // +0x14
};
typedef CParticleUpdateParameters<unsigned long, unsigned long> CSpriteUpdateParameters;
struct CParticleSystem {
    unsigned char _pad00[0x50];
    unsigned char Flags;                               // +0x50 (bit 6: local space)
    void GetInverseSystemTransform(CMatrix3x4& matrix);         // 0x00AF2250
    void GetScaledInverseSystemTransform(CMatrix3x4& matrix);   // 0x00B05B10
};
__forceinline void ToSystemSpace(CParticleSystem& system, C3DVector& p)
{
    if (!(system.Flags & 0x40)) {
        CMatrix3x4 m;
        system.GetInverseSystemTransform(m);
        float x = p.X;
        float y = p.Y;
        p.X = m.E31 * p.Z + m.E21 * y + m.E11 * x + m.E41;
        p.Y = m.E22 * y + m.E32 * p.Z + m.E12 * x + m.E42;
        p.Z = m.E33 * p.Z + m.E23 * y + m.E13 * x + m.E43;
    }
}
__forceinline void ToScaledSystemSpace(CParticleSystem& system, C3DVector& p)
{
    if (!(system.Flags & 0x40)) {
        CMatrix3x4 m;
        system.GetScaledInverseSystemTransform(m);
        float x = p.X;
        float y = p.Y;
        p.X = m.E31 * p.Z + m.E21 * y + m.E11 * x + m.E41;
        p.Y = m.E32 * p.Z + m.E22 * y + m.E12 * x + m.E42;
        p.Z = m.E33 * p.Z + m.E23 * y + m.E13 * x + m.E43;
    }
}
struct CPSCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual float GetTimeRemaining();                  // slot 31 (+0x7c)
    CParticleSystem* ParticleSystem;                   // +0x04
    bool UpdateStarted : 1;                            // +0x08
    bool Initialised : 1;
};
struct CPSCSplineBase : CPSCBase {
    float         SplineAnimSpeed;                     // +0x0c
    unsigned char Spline[0x14];                        // +0x10 CCardinalSpline
    void*         SplineRandomOffsetData;              // +0x24 CCountedPointer data
    void*         SplineRandomOffsetInfo;
    void*         SplinePosParamData;                  // +0x2c CCountedPointer data
    void*         SplinePosParamInfo;
};
struct CPSCSingleSprite : CPSCSplineBase {
    unsigned char               _state34[0x6c - 0x34];
    unsigned long               PositionParam;         // +0x6c
    long                        BlendMode;             // +0x70
    long                        BlendOp;
    long                        TrailBlendMode;
    long                        TrailBlendOp;
    long                        TrailBankIndex;        // +0x80
    float                       TrailWidth;            // +0x84
    long                        MaxTrailLength;        // +0x88
    long                        StayWithEmitterIntFactor;   // +0x8c
    bool                        UseStartColour;        // +0x90
    bool                        UseMidColour;
    bool                        UseEndColour;
    unsigned char               Flags93;
    unsigned char               Flags94;
    bool                        UsePosition;           // +0x95
    bool                        UseSplinePoints;       // +0x96
    bool                        EnteringTimelineEnd;   // +0x97
    unsigned long               LastUpdateFrame;       // +0x98
    long                        Lifetimer;             // +0x9c
    C3DVector                   Offset;                // +0xa0
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;  // +0xac
    void AddTrailPoint(const C3DVector& position, float width);   // 0x00B081A0
    void SetTrailPositionsFromSpline(CParticleSystem& system, CSpriteUpdateParameters* params, float width, float time);   // 0x00B08250
    void Update(CParticleSystem& system, CSpriteUpdateParameters* params, bool& alive);
};

void CPSCSingleSprite::Update(CParticleSystem& system, CSpriteUpdateParameters* params, bool& alive)
{
    if (!EnteringTimelineEnd || GetTimeRemaining() > 0.0f)
        alive = true;
    if (Lifetimer == 0 && UsePosition) {
        TrailUpdatePrimitive.Trail.clear();
        if (PositionParam) {
            CVectorMapEntry<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* it = params->UpdateParams.find(PositionParam);
            if (it != params->UpdateParams.end()) {
                C3DVector position;
                if (it->second.Callback->GetValue(&position, it->second.Index, 0.0f, 0, params, true)) {
                    ToSystemSpace(system, position);
                    AddTrailPoint(position, TrailWidth);
                }
            }
        }
    }
    if (UsePosition) {
        if (PositionParam) {
            CVectorMapEntry<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* it = params->UpdateParams.find(PositionParam);
            if (it != params->UpdateParams.end()) {
                C3DVector position;
                if (it->second.Callback->GetValue(&position, it->second.Index, 1.0f, 0, params, true)) {
                    ToScaledSystemSpace(system, position);
                    AddTrailPoint(position, TrailWidth);
                }
            }
        }
    } else if ((!SplineRandomOffsetData && SplinePosParamData) || TrailUpdatePrimitive.Trail.size() == 0)
        SetTrailPositionsFromSpline(system, params, TrailWidth, 0.0f);
    ++Lifetimer;
}
}
