D="rebuild/src/compiled/00/b3/"
A=D+"CEngineResourceManager_Render_00b3b7c0.cpp"; B=D+"CEngineResourceManager_FrameEnd_00b3b870.cpp"; C=D+"CEngineResourceManager_ResizeHiresTextureMemory_00b3b4f0.cpp"
N="void SetReduceSizeFrameDelay(unsigned long delay);   // 0x009FD100 (Ego_r / FableWin ?SetReduceSizeFrameDelay@CGraphicDataBank@@QAEXK@Z)"
EDITS=[
(A,"void Reload(long flags);                       // 0x009FD100",N),(A,"m_pDataBank->Reload(1);","m_pDataBank->SetReduceSizeFrameDelay(1);"),
(B,"void Reload(long flags);                       // 0x009FD100",N),(B,"m_pDataBank->Reload(3);","m_pDataBank->SetReduceSizeFrameDelay(3);"),
(C,"void SetLevel(long level);                           // 0x009FD100",N),(C,"m_pMainGraphicBank->SetLevel(","m_pMainGraphicBank->SetReduceSizeFrameDelay(",2),
]
