#ifndef FABLE_CSALCPUINFO_H
#define FABLE_CSALCPUINFO_H
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
// SDK SalCpuInfo CodeView and release relocations; native globals143bc78/7a/7d.
class CSalCpuInfo {
public:
 CSalCpuInfo(){if(!m_bInitialized)Initialize();}
#ifdef FABLE_RETAIL_SAL_FRAME_FLOAT_TO_INT_PASS29
 static bool HasSSE(){return m_bHasSSE;}
#endif
 static bool HasMMX(){return m_bHasMMX;}
 static bool HasSSE2(){return m_bHasSSE2;}
private:
 static void Initialize();
 static bool m_bInitialized,m_bHasMMX,m_bHasSSE2;
#ifdef FABLE_RETAIL_SAL_FRAME_FLOAT_TO_INT_PASS29
 static bool m_bHasSSE;
#endif
};
#endif
#endif
