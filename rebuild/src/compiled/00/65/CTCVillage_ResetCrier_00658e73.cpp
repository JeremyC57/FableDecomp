// Name fix (agentNF24): 0x00658DB3 respelled CTCVillage::IsCurfewTime() const (Ego_r/FableWin ?IsCurfewTime@CTCVillage@@QBE_NXZ, the landed row's own leaf) (was ComputeB1, char)
class CTCVillage { public:
    char pad[0xb0];
    char m_b0;
    char m_b1;
    char ComputeB0();
    bool IsCurfewTime() const;   // 0x00658DB3 Ego_r/FableWin ?IsCurfewTime@CTCVillage@@QBE_NXZ
    void ResetCrier();
};

void CTCVillage::ResetCrier()
{
    this->m_b0 = this->ComputeB0();
    this->m_b1 = this->IsCurfewTime();
}