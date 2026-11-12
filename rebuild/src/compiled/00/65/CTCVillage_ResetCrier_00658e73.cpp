// CTCVillage::ResetCrier() -- note whether it is work time (+0xB0) and curfew time (+0xB1) as the crier last announced
// Leaf: ?ResetCrier@CTCVillage@@QAEXXZ
// Name fix (agentNF24): 0x00658DB3 respelled CTCVillage::IsCurfewTime() const (Ego_r/FableWin ?IsCurfewTime@CTCVillage@@QBE_NXZ, the landed row's own leaf) (was ComputeB1, char)
class CTCVillage { public:
    char pad[0xb0];
    char m_b0;
    char m_b1;
    bool IsWorkTime() const;     // 0x00658B20 Ego_r/FableWin ?IsWorkTime@CTCVillage@@QBE_NXZ (agentNF25b; was ComputeB0, char)
    bool IsCurfewTime() const;   // 0x00658DB3 Ego_r/FableWin ?IsCurfewTime@CTCVillage@@QBE_NXZ
    void ResetCrier();
};

void CTCVillage::ResetCrier()
{
    this->m_b0 = this->IsWorkTime();
    this->m_b1 = this->IsCurfewTime();
}