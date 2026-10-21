// NUISystem::CSlider::Update(float) -- flag a move when the thumb is a pixel or more off the value, slide it there, then run the base update
// Leaf: Update
// Retail 0x0054BA20, 390 bytes, ret 4. Callees: the component's virtual ChangePositionDelta (slot 32, +0x80, 0x0052EA30),
// STLport map<unsigned long, CUIState>::operator[] 0x0042D5B1 (out of line), CChangingStateComponent::Update
// 0x0052C7E0. Pool: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.2f pushed as an immediate.
// Layout: retail NUISystem::CSlider = Ego_r.pdb minus 0x30 (as the landed RevertValue 0x0054ABE0): Dimensions +0x184,
// Min +0x18C, Max +0x194, Step +0x19C, Value +0x1A4, Move +0x1AC; CComponent StateMap +0x24 (12-byte STLport map),
// Position +0x34, TargetPosition +0x3C (PDB-8). engine/CSprite.h is all padding at these offsets, so a local model.
// Shape: the value-to-pixel conversion is a static inline helper whose product is its own statement
// (`offset = (v - min) * dim; return offset / range;`): written as one `(v - min) * dim / range` expression VC7.1
// reassociates it into `(v - min) * (dim / range)` when dim and range are both memory operands. The X range is a
// named local (spilled, reused by the move arm); the Y range is recomputed in each arm.
#include <math.h>
struct C2DVector {
    float X, Y;
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};
struct CUIState {
    unsigned long StateChangeFlag;   // +0x00
    long          StateChangeType;   // +0x04
    C2DVector     Position;          // +0x08
};
struct CStateMap {                   // STLport map<unsigned long, CUIState> (12 bytes)
    void* Header; unsigned long NodeCount; unsigned long Compare;
    CUIState& operator[](const unsigned long& key);   // 0x0042D5B1
};
struct CChangingStateComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool linear);   // slot 32 (+0x80), 0x0052EA30
    unsigned char _pad04[0x20];      // +0x04
    CStateMap     States;            // +0x24
    float         Time;              // +0x30
    C2DVector     Position;          // +0x34
    C2DVector     TargetPosition;    // +0x3c
    void Update(float dt);           // 0x0052C7E0
};
struct CSlider : CChangingStateComponent {
    unsigned char _pad44[0x184 - 0x44];
    C2DVector Dimensions;            // +0x184
    C2DVector Min;                   // +0x18c
    C2DVector Max;                   // +0x194
    C2DVector Step;                  // +0x19c
    C2DVector Value;                 // +0x1a4
    bool      Move;                  // +0x1ac
    void Update(float dt);
};

static inline float SliderPixels(float value, float minimum, float dimension, float range)
{
    float offset = (value - minimum) * dimension;
    return offset / range;
}
void CSlider::Update(float dt)
{
    float rangeX = Max.X - Min.X;
    if ((rangeX != 0.0f && fabs(SliderPixels(Value.X, Min.X, Dimensions.X, rangeX) - TargetPosition.X) >= 1.0f) ||
        (Max.Y - Min.Y != 0.0f && fabs(SliderPixels(Value.Y, Min.Y, Dimensions.Y, Max.Y - Min.Y) - TargetPosition.Y) >= 1.0f))
        Move = true;
    if (Move) {
        C2DVector delta;
        if (rangeX != 0.0f)
            delta.X = SliderPixels(Value.X, Min.X, Dimensions.X, rangeX) - Position.X;
        else
            delta.X = 0.0f;
        float rangeY = Max.Y - Min.Y;
        if (rangeY != 0.0f)
            delta.Y = SliderPixels(Value.Y, Min.Y, Dimensions.Y, rangeY) - Position.Y;
        else
            delta.Y = 0.0f;
        ChangePositionDelta(delta, 0.2f, false);
        unsigned long key = 0;
        States[key].Position += delta;
        Move = false;
    }
    CChangingStateComponent::Update(dt);
}