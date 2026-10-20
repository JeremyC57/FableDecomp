// CNavigatorFlyer::GetRandomNavigablePos(const C3DVector& position, float distance, const C2DVector& direction, float) const -- step `distance` along the normalised direction; when the navigation manager has an unblocked node there and the step stays in the same map, return the step at the interface's height for (step + the global flyer offset), else the start position
// Grade: DIFFER(289v289) -- x87 operand order of the offset + step sum: retail loads offset.X / step.Y / offset.Z first, ours offset.X / offset.Y / step.Z (either source order)
// Leaf: ?GetRandomNavigablePos@CNavigatorFlyer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A198C0, 289 bytes (+ int3 padding), `ret 14h` (hidden result + 4 arguments).
// Callees: C2DVector::Normalise 0x00A14540, CNavigatorManager::GetNodeAt(const C2DVector&, unsigned char) 0x00A14F70,
//   CNavigatorManager::IsInSameMap(const C2DVector&, const C2DVector&) 0x00A14FD0, the external interface's (+0x14)
//   virtual slot 5 (+0x14, float(const C3DVector&, bool)).
// Globals: the C3DVector at 0x01397130 (.data) added to the step before the height query.
// Layout: CNavigatorBase PInterface +0x14, NavigationManager +0x1c; CANavQuadTreeNode Blocked +0x0f.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector& Normalise();                        // 0x00A14540
};
inline C2DVector operator*(const C2DVector& v, float s) { return C2DVector(v.X * s, v.Y * s); }
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
extern C3DVector g_FlyerHeightOffset;              // 0x01397130
struct CANavQuadTreeNode {
    unsigned char _pad00[0x0f];
    bool          Blocked;                         // +0x0f
};
struct CNavigatorManager {
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70
    bool IsInSameMap(const C2DVector& a, const C2DVector& b);                     // 0x00A14FD0
};
struct CNavigatorExternalInterface {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual float FindPosZAt(const C3DVector& position, bool flag) const;   // slot 5 (+0x14)
};
struct CNavigatorFlyer {
    virtual void v00();
    unsigned char                _pad04[0x14 - 0x04];
    CNavigatorExternalInterface* PInterface;        // +0x14
    bool                         OnRoute;           // +0x18
    bool                         Interrupted;       // +0x19
    CNavigatorManager*           NavigationManager; // +0x1c
    virtual C3DVector GetRandomNavigablePos(const C3DVector& position, float distance, const C2DVector& direction, float spread) const;
};

C3DVector CNavigatorFlyer::GetRandomNavigablePos(const C3DVector& position, float distance, const C2DVector& direction, float) const
{
    CNavigatorManager* manager = NavigationManager;
    C2DVector dir(direction);
    dir.Normalise();
    C2DVector step = dir * distance;
    C3DVector target(position.X + step.X, position.Y + step.Y, position.Z);
    CANavQuadTreeNode* node = manager->GetNodeAt(C2DVector(target.X, target.Y), 0);
    if (node && !node->Blocked && manager->IsInSameMap(C2DVector(position.X, position.Y), C2DVector(target.X, target.Y)))
        return C3DVector(target.X, target.Y, PInterface->FindPosZAt(g_FlyerHeightOffset + target, true));
    return position;
}
