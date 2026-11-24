// Leaf: ?Clone@CObstacleNavigatorExternalInterface@@UBEPAVCNavigatorExternalInterface@@XZ
#pragma optimize("s",on)
// CObstacleNavigatorExternalInterface::Clone  @ 004df4ca
// Pattern: return new CObstacleNavigatorExternalInterface(*this);
//   -> operator new(0x14); if (p) copy-ctor(p, this); return p;

class CNavigatorExternalInterface { public: int dummy; };

class CObstacleNavigatorExternalInterface
{ public:
    char data[0x10];
    CObstacleNavigatorExternalInterface() {}
    CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& src);
    virtual CNavigatorExternalInterface* Clone() const;
};

CNavigatorExternalInterface* CObstacleNavigatorExternalInterface::Clone() const
{
    return (CNavigatorExternalInterface*)new CObstacleNavigatorExternalInterface(*this);
}