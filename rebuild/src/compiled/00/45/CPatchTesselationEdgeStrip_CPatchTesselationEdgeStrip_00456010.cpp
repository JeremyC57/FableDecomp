#pragma optimize("s",on)
void __cdecl free(void* memory);
struct PatchEdgeBase456010 { unsigned char bytes[0x3c]; ~PatchEdgeBase456010(); };
class CPatchTesselationEdgeStrip : public PatchEdgeBase456010 { public: void* allocation3c; unsigned char unused40[0x10]; void* allocation50; ~CPatchTesselationEdgeStrip(); };
CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip(){if(allocation50!=0)free(allocation50);if(allocation3c!=0)free(allocation3c);}