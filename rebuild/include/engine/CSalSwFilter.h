#ifndef FABLE_CSAL_SW_FILTER_H
#define FABLE_CSAL_SW_FILTER_H
class CSalSwFilter {
private:
 struct PARAMS {
 float fF;
 float fQ;
 float fG;
 };
 PARAMS m_Params;
 PARAMS m_NewParams;
 float m_fZ0;
 float m_fZ1;
 unsigned int m_nFadeSamples;
 void ProcessEQFade(short*,unsigned int);
public:
 void ProcessEQ(short*,unsigned int);
};
#endif
