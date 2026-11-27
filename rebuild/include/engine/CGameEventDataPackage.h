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
class CGameEventDataPackage {
public:
    CGameEventDataPackage() : Length(0), StartPos(0), EndPos(0) { }
    void GetData(long size, void* data);                                 // 0x009F18F0
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETULONG_PASS26
    // FableWin 0x018A8690 and the inlined 4-byte read in retail CWorld::EAOpenHeroInformationScreen.
    unsigned long GetULONG() { unsigned long value; GetData(sizeof(value), &value); return value; }
#endif
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE
    // Opt-in (NF95 q11320, row 0x00628E80): FableWin.pdb 0x023D5E10 ?GetUHUGE@CGameEventDataPackage@@QAE_KXZ (the only 8-byte typed
    // reader: GetData(8, &value), the 64-bit result in edx:eax), out of line in retail at 0x00628E80. Declaration only.
    unsigned __int64 GetUHUGE();                                         // 0x00628E80
#endif
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS
    // Opt-in (NF97, the CWorld EA handlers 0x00629420 / 0x00629B10 / 0x00629C40 / 0x00629DE0 / 0x0062ABA0 / 0x0062CE60): FableWin.pdb
    // ?GetUBYTE@CGameEventDataPackage@@QAEEXZ / ?GetUWORD@..QAEGXZ / ?GetFloat@..QAEMXZ / ?GetSLONG@..QAEJXZ (out of line only in
    // FableWin; no copy in Ego_r.pdb: expanded in place there). Those rows differ with a direct GetData read into the named
    // local and match with the inline reader.
    // Inferred: the bodies (one GetData of the value's size, as GetUHUGE 0x00628E80 does out of line); the rows' bytes need an inline
    // reader wrapping the GetData call, the exact original wording is not in the PDBs.
    unsigned char  GetUBYTE() { unsigned char value; GetData(sizeof(value), &value); return value; }
    unsigned short GetUWORD() { unsigned short value; GetData(sizeof(value), &value); return value; }
    float          GetFloat() { float value; GetData(sizeof(value), &value); return value; }
    long           GetSLONG() { long value; GetData(sizeof(value), &value); return value; }
#endif
private:
    unsigned char Data[32];                                              // +0x00
    long          Length;                                                // +0x20
    long          StartPos;                                              // +0x24
    long          EndPos;                                                // +0x28
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CGameEventDataPackage) == 0x2c);
#ifdef FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE_INLINE_PASS29
// FableWin GetUHUGE and retail 0062bdbd..0062bdda: the typed reader expands its eight-byte GetData call.
inline unsigned __int64 CGameEventDataPackage::GetUHUGE()
{
    unsigned __int64 value;
    GetData(sizeof(value), &value);
    return value;
}
#endif
#else
class CGameEventDataPackage;
#endif
#endif // FABLE_ENGINE_CGAMEEVENTDATAPACKAGE_H
