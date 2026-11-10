#ifndef FABLE_ENGINE_CGAMEEVENTDATAPACKAGE_H
#define FABLE_ENGINE_CGAMEEVENTDATAPACKAGE_H
// New (chunk 1012, the CWorld EA handlers 0x0062BD80 / 0x0062BEF0 / 0x0062B070): the unpacking buffer CGameEvent::GetDataPackage
// fills. Ego_r.pdb CGameEventDataPackage, 0x2C bytes: Data uchar[32] +0x00, Length +0x20, StartPos +0x24, EndPos +0x28
// (ghidra_out/struct_layouts_egor.tsv; retail zeroes the three words before GetDataPackage, 0x0062BD9B..0x0062BDAB).
// Members: FableWin.pdb ??0CGameEventDataPackage@@QAE@XZ (inline in retail), ?GetData@CGameEventDataPackage@@QAEXJPAX@Z (Ego_r
// 0x00B454F0, out of line in retail at 0x009F18F0; the landed EA rows bind it as the invented ?Read@CMemoryBuffer@@QAEXKPAX@Z,
// NAME_FIXES) and the typed readers GetULONG / GetUBYTE / GetFloat / GetSLONG / GetUHUGE / GetUWORD (FableWin only; forwarding
// to GetData, not used by the rows that need this header yet, so not declared).
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPEDREADERS
#ifdef _MSC_VER
typedef unsigned __int64 CGameEventDataPackageUHUGE;                     // the PDB's UHUGE (mangles _K)
#else
typedef unsigned long long CGameEventDataPackageUHUGE;
#endif
#endif
class CGameEventDataPackage {
public:
    CGameEventDataPackage() : Length(0), StartPos(0), EndPos(0) { }
    void GetData(long size, void* data);                                 // 0x009F18F0
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPEDREADERS
    // (chunk 1012b) The typed readers, FableWin.pdb ?GetULONG / ?GetUBYTE / ?GetFloat / ?GetSLONG / ?GetUHUGE / ?GetUWORD
    // @CGameEventDataPackage@@QAE..XZ: inline in retail (no Ego_r.pdb / Fable.exe copy), each reads one value through GetData.
    unsigned long GetULONG() { unsigned long value; GetData(sizeof(value), &value); return value; }
    unsigned char GetUBYTE() { unsigned char value; GetData(sizeof(value), &value); return value; }
    float GetFloat() { float value; GetData(sizeof(value), &value); return value; }
    long GetSLONG() { long value; GetData(sizeof(value), &value); return value; }
    CGameEventDataPackageUHUGE GetUHUGE() { CGameEventDataPackageUHUGE value; GetData(sizeof(value), &value); return value; }
    unsigned short GetUWORD() { unsigned short value; GetData(sizeof(value), &value); return value; }
#endif
private:
    unsigned char Data[32];                                              // +0x00
    long          Length;                                                // +0x20
    long          StartPos;                                              // +0x24
    long          EndPos;                                                // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDataPackage) == 0x2c);
#else
class CGameEventDataPackage;
#endif
#endif // FABLE_ENGINE_CGAMEEVENTDATAPACKAGE_H
