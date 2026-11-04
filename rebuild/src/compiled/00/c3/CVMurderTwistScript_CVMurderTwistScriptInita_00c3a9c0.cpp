// Vendor naming wave V1 (one address, one name): 0x00C3A9C0 ?Reset@CReaderBase_00c4b300@@QAEXXZ (was ?Init@CV_MurderTwistScript@@QAEXXZ)
class CReaderBase_00c4b300
{
public:
    char pad[0x70];
    int m_field70;
    int m_field74;
    int m_field78;

    void Reset();
};

void CReaderBase_00c4b300::Reset()
{
    m_field70 = 0;
    m_field74 = 0;
    m_field78 = 0;
}