// Leaf: ?IsDead@CObstacleNavigatorExternalInterface@@UBE_NXZ
#pragma optimize("s",on)
struct Inner {
    char pad[0x91];
    unsigned char flag; // +0x91
};

class CObstacleNavigatorExternalInterface { public:
        Inner* inner;    // +4
    virtual bool IsDead() const;
};

bool CObstacleNavigatorExternalInterface::IsDead() const
{
    CObstacleNavigatorExternalInterface* self = const_cast<CObstacleNavigatorExternalInterface*>(this);
    Inner* p = self->inner;
    bool alive = (p != 0) && ((p->flag & 1) == 0);
    return !alive;
}
// Flatten route (flatten_pilot v4, NF324): CObstacleNavigatorExternalInterface_IsDead -> CObstacleNavigatorExternalInterface::IsDead, the RTTI-placed PDB member; class model: the row's local class model.
