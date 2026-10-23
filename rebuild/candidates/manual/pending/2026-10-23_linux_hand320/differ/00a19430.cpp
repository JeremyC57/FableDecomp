// CNavigatorFlyer::GetRandomNavigablePos(const C3DVector& pos, float radius, bool) const -- a random flyable position within radius of pos: a random direction at the radius when it lands in a live node of the same map, else a random child node or a random point inside the current node (clamped back to the radius), or the nearest navigable node's centre when pos is in a switched-off node; its height comes from the external interface
// Grade: DIFFER(1215v1165) -- control flow and callees decoded; left: frame 0x3c vs 0x40 and slot reuse of the dead pos argument, the target vector built through int copies of temporaries, x87 order of the 2D clamp and the fsqrt test
// Leaf: GetRandomNavigablePos
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A19430, 1165 bytes (+ int3 padding), `ret 0x10` (hidden C3DVector result + 3 args).
// Callees: CNavigatorManager::GetNodeAt 0x00A14F70 / IsInSameMap 0x00A14FD0 (2D temporaries), CNavigatorBase::
//   FindNearestNavigableNode 0x00A475C0, the navigator's random seed 0x00A47100 (thiscall, returns unsigned long*),
//   GFFloatRandom 0x004978A0 / GFRandom 0x004CDAE0 (__fastcall), C2DVector::Normalise 0x00A14540, C3DVector::RotateAboutZ
//   0x00A14260, CANavQuadTreeNode::GetSize 0x00A75B30 and node virtual slots 2 (+0x08 children), 3 (+0x0c has children),
//   5 (+0x14 map index); PInterface slot 4 FindPosZAt (+0x10). Globals: the probe offset 0x01397130, 3.0f, 0.5f, rsqrt table.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void RotateAboutZ(float angle, bool flag);      // 0x00A14260
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
extern const C3DVector g_FlyerProbeOffset;          // 0x01397130
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector& Normalise();                         // 0x00A14540
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

class CANavQuadTreeNode;
struct CNodeList {
    CANavQuadTreeNode** Data;                       // +0x00
    char                Count;                      // +0x04
};
class CANavQuadTreeNode {
public:
    virtual void v00(); virtual void v01();
    virtual CNodeList* GetChildren() const;         // slot 2 (+0x08)
    virtual bool HasChildren() const;               // slot 3 (+0x0c)
    virtual void v04();
    virtual long GetMapIndex() const;               // slot 5 (+0x14)
    C2DVector     Centre;                           // +0x04
    unsigned char _pad0c;
    unsigned char Layer;                            // +0x0d
    unsigned char _pad0e;
    bool          m_switchedOff;                    // +0x0f
    float GetSize() const;                          // 0x00A75B30
};
class CNavigatorManager {
public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70
    bool IsInSameMap(const C2DVector& a, const C2DVector& b);                        // 0x00A14FD0
};
class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const;      // slot 4 (+0x10)
};

class CNavigatorFlyer {
public:
    virtual void v00();
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;           // +0x14
    bool                         OnRoute;              // +0x18
    bool                         Interrupted;          // +0x19
    CNavigatorManager*           NavigationManager;    // +0x1c
    unsigned long* GetRandomSeed() const;                                          // 0x00A47100
    CANavQuadTreeNode* FindNearestNavigableNode(const CANavQuadTreeNode* from, const CANavQuadTreeNode* blocked,
                                                const C3DVector& pos, float radius, CNavigatorManager* manager) const;   // 0x00A475C0
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius, bool flag) const;
};

C3DVector CNavigatorFlyer::GetRandomNavigablePos(const C3DVector& pos, float radius, bool) const
{
    CNavigatorManager* manager = NavigationManager;
    CANavQuadTreeNode* node = manager->GetNodeAt(pos, 0);
    if (node == 0)
        return pos;
    C3DVector target;
    if (node->m_switchedOff) {
        CANavQuadTreeNode* nearest = FindNearestNavigableNode(node, node, pos, radius, manager);
        if (nearest == 0)
            return pos;
        target = C3DVector(nearest->Centre.X, nearest->Centre.Y, 0.0f);
        float dx = target.X - pos.X;
        float dy = target.Y - pos.Y;
        if (radius * radius < dy * dy + dx * dx) {
            C2DVector direction(target.X - pos.X, target.Y - pos.Y);
            direction.Normalise();
            target = C3DVector(direction.X * radius + pos.X, direction.Y * radius + pos.Y, pos.Z);
        }
    } else {
        C3DVector direction(0.0f, 1.0f, 0.0f);
        direction.RotateAboutZ(GFFloatRandom(1.0f, GetRandomSeed()), false);
        target = pos + direction * radius;
        CANavQuadTreeNode* targetNode = manager->GetNodeAt(target, node->Layer);
        if (targetNode == 0 || node->GetMapIndex() != targetNode->GetMapIndex() || targetNode->m_switchedOff ||
            !manager->IsInSameMap(pos, target)) {
            bool found = false;
            if (node->HasChildren()) {
                CNodeList* children = node->GetChildren();
                CANavQuadTreeNode* child = children->Data[GFRandom(children->Count, GetRandomSeed())];
                target = C3DVector(child->Centre.X, child->Centre.Y, pos.Z);
                found = NavigationManager->IsInSameMap(pos, target);
            }
            if (!found) {
                float randomX = GFFloatRandom(node->GetSize(), GetRandomSeed());
                float randomY = GFFloatRandom(node->GetSize(), GetRandomSeed());
                target = C3DVector(node->Centre.X - node->GetSize() + randomX, node->Centre.Y - node->GetSize() + randomY, 0.0f);
            }
            C3DVector delta = target - pos;
            if (sqrt(delta.X * delta.X + delta.Y * delta.Y) > radius) {
                delta.Normalise();
                delta = delta * radius;
                target = pos + delta;
            }
        }
    }
    return C3DVector(target.X, target.Y, PInterface->FindPosZAt(target + g_FlyerProbeOffset, true));
}
