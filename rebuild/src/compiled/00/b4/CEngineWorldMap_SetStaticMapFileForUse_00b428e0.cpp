// CEngineWorldMapRetail::SetStaticMapFileForUse(file) -- opens static maps and loads water data with progress messages.
// Retail00b428e0 length245. Actual donor engine/CEngineWorldMap.h differs:
// filename+3c vs native+30; StaticMapOpenState+228 vs native+1b0 (smaller retail file object).
// Uses actual fable_string.h; no local string replica. Strings copied from retail pools.
#define CEngineWorldMap CEngineWorldMapDonorLayout
#include "engine/CEngineWorldMap.h"
#undef CEngineWorldMap
#include "fable_string.h"
void FABLE_FASTCALL ReportStaticMapProgress(const CCharString&,bool,float,unsigned); //009e9f40
struct CStaticMapPool {void EnablePoolAllocation(bool);}; //00bda070
struct CStaticMapEngineState {unsigned char UnrelatedState[0x6b0];CStaticMapPool* Pool;};
extern CStaticMapEngineState* gStaticMapEngineState; //01436ea8
struct CEngineWorldMapRetail {
 unsigned char UnrelatedMapState[0x30];CWideString StaticMapBankName;
 unsigned char StaticFileAndWorldState[0x17c];unsigned StaticMapOpenState;
 void SetStaticMapFileForUse(const wchar_t* file);
 void CloseStaticMapFile(); //00b40000
 void OpenStaticMaps(bool); //00b42750
 void LoadWaterData(); //00b41fa0
};
void CEngineWorldMapRetail::SetStaticMapFileForUse(const wchar_t* file){
 {CCharString m("SetStaticMapFileForUse: CloseStaticMapFile",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 CloseStaticMapFile();
 {CCharString m("SetStaticMapFileForUse: EnablePoolAllocation",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 gStaticMapEngineState->Pool->EnablePoolAllocation(true);
 StaticMapBankName=file;
 {CCharString m("SetStaticMapFileForUse: OpenStaticMaps",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 OpenStaticMaps(true);
 StaticMapOpenState=3;
 {CCharString m("SetStaticMapFileForUse: LoadWaterData",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 LoadWaterData();
}