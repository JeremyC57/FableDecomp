// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
extern "C" void ForwardTarget_00b4b830();
extern "C" void CEngineShadowRenderer_OnReleaseDefaultPoolResources() { ForwardTarget_00b4b830(); }