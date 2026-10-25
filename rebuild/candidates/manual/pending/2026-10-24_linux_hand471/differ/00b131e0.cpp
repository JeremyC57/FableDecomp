// NParticleEngine::CPSCEmitterGeneric::DatabaseCopy(const CPSCEmitterGeneric& other) -- vtable slot 18: copy another generic emitter's database settings (the packed bit-field counts, sizes, speeds, times, direction and orientation flags, the position and direction parameters, the spline) and make sure a copied spline has at least two control points
// Grade: DIFFER(850v839) -- lane gap: store-to-load forwarding; retail keeps each destination bit-field word in a register across the intervening same-base stores (and forwards the zero point's constants into the inline construct), VC7.1 here reloads the word for every field (the by-value-source form forwards, so the retail alias model saw no overlap); field order, masks, the spline assignment and the top-up loop otherwise line up
// Leaf: ?DatabaseCopy@CPSCEmitterGeneric@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B131E0, 839 bytes (+ int3 padding), `ret 4`. Slot 18 of the CPSCEmitterGeneric vtable
//   0x012A002C (the slot of the landed CPSCAttractor::DatabaseCopy 0x00B18060 in its vtable 0x012A04AC).
// Callees: operator delete @ 0x00BFE9BC (the released count block), vector<C3DVector>::_M_insert_overflow(C3DVector*,
//   const C3DVector&, const __true_type&, unsigned int, bool) @ 0x0066E370 (the push_back slow path).
// Layout: as the landed CPSCEmitterGeneric::WriteBinary 0x00B12EF0 (Ego_r.pdb names and retail offsets: Spline +0x18,
//   EmitterPosParam +0x20, DirectionParamName +0x24, unsigned long bit-field words +0x30..+0x44, the unsigned short
//   NonUniformScalingZ:11 at +0x48); CCardinalSpline's control points are a 12-byte STLport vector at +0; the count block
//   {references, __fastcall destroy, object} as the landed CCreatureActionJackScream::Initialise 0x008C0D10.
// Shape: one bit-field assignment per member in retail order (each a read-modify-write of the word, the source read
//   through a `const CPSCEmitterGeneric* src` local); the spline is the inline CCountedPointer operator= (the source's
//   fields read first); the control points are topped up (signed count) with a zero point through the inline STLport
//   push_back (the __true_type tag has a user constructor, so no zero byte store).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };
struct CPointVector {                                    // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    int GetSize() const { return (int)size(); }
    void _M_insert_overflow(C3DVector* position, const C3DVector& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x0066E370
    void push_back(const C3DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DVector(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
struct CCardinalSpline {
    CPointVector ControlPoints;                          // +0x00
    float        Tension;                                // +0x0c
    bool         EndPointsConnected;                     // +0x10
};
struct SCountBlock {
    long m_references;                                   // +0
    void (__fastcall* m_pfnDestroy)(void*);              // +4
    void* m_pObject;                                     // +8
};
template <class T> struct CCountedPointer {
    T*           m_pObject;                              // +0
    SCountBlock* m_pCount;                               // +4
    T* operator->() const { return m_pObject; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        T* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;                     // operator delete @ 0x00BFE9BC
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

namespace NParticleEngine {
struct CParticleUpdateParamDesc { unsigned long Param; unsigned long Name; unsigned long Type; };
struct CPSCEmitterGeneric {
    unsigned char _base04[0xc - 0x4];                    // CPSCBase after its vptr
    CParticleUpdateParamDesc EmitterPosParamDesc;        // +0x0c
    CCountedPointer<CCardinalSpline> Spline;             // +0x18
    unsigned long EmitterPosParam;                       // +0x20
    unsigned long DirectionParamName;                    // +0x24
    long LifeTimer;                                      // +0x28
    float NextGenerateTime;                              // +0x2c
    unsigned long NoParticlesToStart : 10;               // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;       // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;                      // +0x38
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;                  // +0x3c
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;                  // +0x40
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                         // +0x44
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;              // +0x48
    unsigned long StartedTimer : 1;                      // +0x4c
    unsigned long FinishedGenerating : 1;
    virtual void DatabaseCopy(const CPSCEmitterGeneric& other);
};

void CPSCEmitterGeneric::DatabaseCopy(const CPSCEmitterGeneric& other)
{
    const CPSCEmitterGeneric* src = &other;
    EmitterType = src->EmitterType;
    EmitterPosParam = src->EmitterPosParam;
    EmitterSize = src->EmitterSize;
    Solid = src->Solid;
    RadialBias = src->RadialBias;
    UseEmitterLifeSecs = src->UseEmitterLifeSecs;
    EmitterLifeSecs = src->EmitterLifeSecs;
    EmitterStartTime = src->EmitterStartTime;
    UseEmitterTimelineSecs = src->UseEmitterTimelineSecs;
    EmitterTimelineSecs = src->EmitterTimelineSecs;
    NoParticlesToStart = src->NoParticlesToStart;
    NoParticlesToStartRand = src->NoParticlesToStartRand;
    ParticlesPerSecond = src->ParticlesPerSecond;
    OrientationXY = src->OrientationXY;
    OrientationXZ = src->OrientationXZ;
    OrientationYZ = src->OrientationYZ;
    NonUniformScalingX = src->NonUniformScalingX;
    NonUniformScalingY = src->NonUniformScalingY;
    NonUniformScalingZ = src->NonUniformScalingZ;
    MinSpeed = src->MinSpeed;
    MaxSpeed = src->MaxSpeed;
    UseCustomDirection = src->UseCustomDirection;
    UseOutwardDirection = src->UseOutwardDirection;
    UseForwardDirection = src->UseForwardDirection;
    UseRandom2DDirection = src->UseRandom2DDirection;
    UseRandom3DDirection = src->UseRandom3DDirection;
    UseParamDirection = src->UseParamDirection;
    OppositeDirection = src->OppositeDirection;
    DirectionParamName = src->DirectionParamName;
    CustomDirectionX = src->CustomDirectionX;
    CustomDirectionY = src->CustomDirectionY;
    CustomDirectionZ = src->CustomDirectionZ;
    AngularPerturbationInteger = src->AngularPerturbationInteger;
    Spline = src->Spline;
    C3DVector zero(0.0f, 0.0f, 0.0f);
    if (Spline.m_pObject) {
        while (Spline->ControlPoints.GetSize() < 2)
            Spline->ControlPoints.push_back(zero);
    }
}
} // namespace NParticleEngine
