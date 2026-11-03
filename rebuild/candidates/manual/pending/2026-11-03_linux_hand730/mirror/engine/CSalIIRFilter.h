#ifndef FABLE_ENGINE_CSALIIRFILTER_H
#define FABLE_ENGINE_CSALIIRFILTER_H
// GameCODA debug CodeView: class V, size12, private int members +0/+4/+8.
// Named release object Process overloads are exact retail byte twins.
class CSalIIRFilter {
public:
    CSalIIRFilter();
    void SetLowPass(int iLevelHF,float fFrequencyRef);
    void Clear();
    bool IsSilent() const;
    void Process(short* pOutput,const short* pInput);
    void Process(short* pBuffer);
private:
    int m_iA;
    int m_iB;
    int m_iZ;
};
#endif
