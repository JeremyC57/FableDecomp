#ifndef FABLE_CSALCPUINFO_H
#define FABLE_CSALCPUINFO_H
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
// SDK SalCpuInfo CodeView and release relocations; native globals143bc78/7a/7d.
class CSalCpuInfo {
public:
 CSalCpuInfo(){if(!m_bInitialized)Initialize();}
 static bool HasMMX(){return m_bHasMMX;}
 static bool HasSSE2(){return m_bHasSSE2;}
private:
 static void Initialize();
 static bool m_bInitialized,m_bHasMMX,m_bHasSSE2;
};
#endif
#endif
