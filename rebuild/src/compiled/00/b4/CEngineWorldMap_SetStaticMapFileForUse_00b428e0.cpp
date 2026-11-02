// CEngineWorldMap::SetStaticMapFileForUse(file) -- opens static maps and loads water data with progress messages.
// Leaf: ?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z
// Naming wave 29 (family ewm): the class is CEngineWorldMap (was the invented CEngineWorldMapRetail): own row
//   ?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z, private ?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z
//   0x00B42750 and ?LoadWaterData@CEngineWorldMap@@AAEXXZ 0x00B41FA0, ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ 0x00B40000 (Ego_r.pdb / FableWin.pdb).
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
class CEngineMap { public: enum STATIC_MAP_OPEN_STATE { }; };   // the PDB's nested enum (enumerators not in the name sets)
class CEngineWorldMap { public:
 unsigned char UnrelatedMapState[0x30];CWideString StaticMapBankName;
 unsigned char StaticFileAndWorldState[0x17c];unsigned StaticMapOpenState;
 void SetStaticMapFileForUse(const CWideString& file);
 void CloseStaticMapFile(); //00b40000
private:
 void OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE); //00b42750
 void LoadWaterData(); //00b41fa0
};
void CEngineWorldMap::SetStaticMapFileForUse(const CWideString& file){
 {CCharString m("SetStaticMapFileForUse: CloseStaticMapFile",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 CloseStaticMapFile();
 {CCharString m("SetStaticMapFileForUse: EnablePoolAllocation",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 gStaticMapEngineState->Pool->EnablePoolAllocation(true);
 StaticMapBankName=file; // the copy operator= 0x0099B7D0
 {CCharString m("SetStaticMapFileForUse: OpenStaticMaps",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 OpenStaticMaps((CEngineMap::STATIC_MAP_OPEN_STATE)1);
 StaticMapOpenState=3;
 {CCharString m("SetStaticMapFileForUse: LoadWaterData",-1);ReportStaticMapProgress(m,false,-1.0f,0);}
 LoadWaterData();
}