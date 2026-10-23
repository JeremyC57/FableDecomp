// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_006e71f0();
extern "C" void CGameScriptInterface_Pause() { ForwardTarget_006e71f0(); }