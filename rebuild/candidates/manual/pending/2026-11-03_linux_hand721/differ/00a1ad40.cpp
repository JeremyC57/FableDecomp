// CNavigatorAStar::GetPosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const
// Leaf: ?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z
// Grade: DIFFER(366v370) -- the current/last C3DVector frame slots are swapped, ebx is pushed late (VC shrink-wraps it) where retail pushes it in the prologue, the loop-break `fstp st(0)` block sits inline instead of after the ret, and last + g_Offset (0x013D27D0, unnamed zero .bss C3DVector) loads its x87 operands in the other order (14 probes: init orders, inline distance squared, commuted adds).
// Flags: none
class C2DVector;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    const C3DVector& operator+=(const C2DVector& v);
};

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
};
inline const C3DVector& C3DVector::operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
class CANavQuadTreeNode { public:
    virtual void v00();
    C2DVector     Position;
    unsigned char QuadTreeLevel;
    unsigned char NavLayer;
    bool          LeafNode;
    bool          Blocked;
    float GetSize() const;                                   // 0x00A75B30
};
class CNavigatorManager { public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70
};
class CNavigatorExternalInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const;   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual unsigned char GetLayerAt(const C3DVector& position) const;          // slot 16 (+0x40)
};
extern C3DVector g_Offset;
class CNavigatorBase { public:
    virtual void v00();
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;
    bool                         Interrupted;
    CNavigatorManager*           NavigationManager;   // +0x1c
    void*                        SteeringAgent;
};
class CNavigatorAStar : public CNavigatorBase { private:
    C3DVector GetPosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const;
};

C3DVector CNavigatorAStar::GetPosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const
{
    float distanceSquared = distance * distance;
    C3DVector current(pos.X, pos.Y, pos.Z);
    C3DVector last(current);
    const CANavQuadTreeNode* node = NavigationManager->GetNodeAt(pos, PInterface->GetLayerAt(pos));
    while (node && !node->Blocked) {
        last = current;
        current += direction * (node->GetSize() * 0.5f);
        if (C2DVector(current.X - pos.X, current.Y - pos.Y).GetSquaredMagnitude() > distanceSquared)
            break;
        node = NavigationManager->GetNodeAt(current, PInterface->GetLayerAt(current));
    }
    return C3DVector(last.X, last.Y, PInterface->FindPosZAt(last + g_Offset, true));
}
