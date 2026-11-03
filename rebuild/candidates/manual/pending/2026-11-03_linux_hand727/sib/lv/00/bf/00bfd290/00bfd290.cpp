// CWaterSubPatchMethods::TesselatePatch(CTesselationData&) -- synchronize bridge buffers through the selected water tessellator.
// Retail00bfd290:256 bytes including switch tables. Actual subpatch fields match;
// actual water renderer donor tessellators598/5C0 vsnative564/584 after container shrinkage.
#include "engine/CEngineWaterBackgroundSubPatch.h"
#define CEngineWaterRenderer CEngineWaterRendererDonorLayout
#include "engine/CEngineWaterRenderer.h"
#undef CEngineWaterRenderer
class CEngineMap;class CPatchTesselationEdgeStrip;class CMovableResource;
class CTesselationData { public:
 void* VTable;int PatchWorldX,PatchWorldY,Width,Height;CEngineMap* EngineMap;CIndexBuffer* IndexBuffer;
 CPatchTesselationEdgeStrip* EdgeStrips[4];CVertexBufferWin32* BridgeVertexBuffers[4];CIndexBuffer* BridgeIndexBuffers[4];int BridgePolyCount[4];
};
struct CWaterTessellator {unsigned char State[0x20];bool TesselatePatch(CTesselationData*,CMovableResource*);}; //00be3910
struct CWaterRendererNative {unsigned char Prefix[0x564];CWaterTessellator Tesselator,SeaTesselator;};
extern CWaterRendererNative* gWaterRenderer; //01436e54
struct CWaterSubPatchMethods:CEngineWaterBackgroundSubPatch {bool TesselatePatch(CTesselationData&);};
bool CWaterSubPatchMethods::TesselatePatch(CTesselationData& data){
 data.IndexBuffer=IndexBuffer;
 for(int i=0;i<4;++i){data.BridgeIndexBuffers[i]=BridgeIndexBuffers[i];data.BridgeVertexBuffers[i]=BridgeVertexBuffers[i];data.BridgePolyCount[i]=BridgePolyCount[i];}
 bool result=false;
 switch(*reinterpret_cast<const int*>(WaterType)){
 case 1:case 2:case 6:case 8:result=gWaterRenderer->Tesselator.TesselatePatch(&data,reinterpret_cast<CMovableResource*>(this));break;
 case 3:case 4:case 5:result=gWaterRenderer->SeaTesselator.TesselatePatch(&data,reinterpret_cast<CMovableResource*>(this));break;
 }
 for(int j=0;j<4;++j){BridgeIndexBuffers[j]=data.BridgeIndexBuffers[j];BridgeVertexBuffers[j]=data.BridgeVertexBuffers[j];BridgePolyCount[j]=static_cast<unsigned short>(data.BridgePolyCount[j]);}
 return result;
}