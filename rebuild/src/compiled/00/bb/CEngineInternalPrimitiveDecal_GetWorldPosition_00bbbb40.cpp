class C3DVector { public:
    float x;
    float y;
    float z;
};

class CEngineInternalPrimitiveDecal { public:
    char pad[0x54];
    C3DVector m_worldPosition;

    bool GetWorldPosition(C3DVector& out);
};

bool CEngineInternalPrimitiveDecal::GetWorldPosition(C3DVector& out)
{
    out = m_worldPosition;
    return true;
}