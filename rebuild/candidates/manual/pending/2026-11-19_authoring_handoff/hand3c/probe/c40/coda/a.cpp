// Leaf: ?Commit@CCodaListener@@QAEXXZ
struct _CVECTOR { float x, y, z; };
class CCodaVector : public _CVECTOR {
public:
    float Normalize();
    float Dot(const CCodaVector& v) const
        { return x * v.x + y * v.y + z * v.z; }
    void OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop)
    {
        x = vTop.y * vFront.z - vFront.y * vTop.z;
        y = vTop.z * vFront.x - vFront.z * vTop.x;
        z = vTop.x * vFront.y - vFront.x * vTop.y;
    }
};
class CCodaListener {
public:
    void Commit();
private:
    unsigned char _pad[0x24];
    CCodaVector m_vFront, m_vTop, m_vRight;
    unsigned int m_ffChanged;
    static unsigned int m_ffCommit;
};
void CCodaListener::Commit()
{
    m_ffCommit |= (m_ffChanged & 0x38000);
    if (m_ffChanged & 0x20000) {
        m_vFront.Normalize();
        m_vTop.Normalize();
        float dot = m_vFront.Dot(m_vTop);
        if (dot > 0.05f || dot < -0.05f)
            m_vTop.OrthogonalRight(m_vRight, m_vFront);
        else
            m_vRight.OrthogonalRight(m_vFront, m_vTop);
    }
    m_ffChanged = 0;
}
