// CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& filename) -- read the compiled global-things script and, for every "NEWMAP <n>" ... "ENDMAP" block, hand the block's text as an in-memory file to the thing manager for map n
// Leaf: ?LoadAllLoadableGlobalThingsFromSingleFile@CWorldMap@@QAEXABVCWideString@@@Z
// (Ego_r.pdb / FableWin.pdb spell it protected, IAE; engine/CWorldMap.h FABLE_RETAIL_CWORLDMAP_LOADFROMFILE declares it public, so the self
//  symbol stays QAE until that header block is corrected -- coordinator header change, NOTES.md.)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FE2A0, 614 bytes, `ret 4`.
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CDiskFileWin32(path, 1, 2) 0x0099AD80 (inline dtor: Close
//   0x0099A920), CCharString::LoadFromFile 0x0099F2E0, CStringParser(text, 0, "", 0) 0x00404720 (inline dtor), SkipPastString 0x009B9BA0 /
//   ReadAsInteger 0x009BA540 / ReadAsStringUntilString 0x009BA330, NProgressDisplay::DisplayProgress 0x009E9F40, operator new 0x00BFEA1A
//   (CMemoryFile, vtable 0x0122E0FC), CMemoryFile::Open(void*, long) 0x009E93C0, CCountedPointer<CAFile> (DeleteData 0x0050BE40),
//   CWorld slot 3 GetThingManager, CThingManager::LoadLoadableGlobalThingsFromFile(CCountedPointer<CAFile>, long) 0x005223F0.
// NF62 (2026-11-17): ported onto engine/CWorldMap.h (LOADFROMFILE) / CWorld.h / CThingManager.h / CMemoryFile.h / CDiskFileWin32.h /
//   CStringParser.h / CCountedPointer.h / NProgressDisplay.h / fable_string.h (every local model deleted). The memory stream the landed row
//   called CMemoryDataInputStream is CMemoryFile (RTTI vtable 0x0122E0FC, as 0x00500230) and its deleter is CCountedPointer<CAFile>::DeleteData;
//   the thing-manager call is Ego_r ?LoadLoadableGlobalThingsFromFile@CThingManager@@QAEXV?$CCountedPointer@VCAFile@@@@J@Z (the landed
//   stand-in LoadGlobalThingBatch(CDataInputStreamPointer)). Overlay opt-ins FABLE_RETAIL_CMEMORYFILE_OPENMEMORY and
//   FABLE_RETAIL_CTHINGMANAGER_LOADLOADABLEGLOBALTHINGS.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CMEMORYFILE
#define FABLE_RETAIL_CMEMORYFILE_OPENMEMORY
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADFROMFILE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_LOADLOADABLEGLOBALTHINGS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CWORLD
#include <string.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CMemoryFile.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CMapInfo.h"
#include "engine/CStringParser.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include "engine/CWorldMap.h"
#include "engine/NProgressDisplay.h"

void CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& filename)
{
    CCharString text;
    CDiskFileWin32 file(filename, FILE_ACCESS_READ, 2);
    text.LoadFromFile(file, 0, -1);
    CStringParser parser(text);
    while (parser.SkipPastString("NEWMAP"))
    {
        long map = parser.ReadAsInteger();
        CCharString block = parser.ReadAsStringUntilString("ENDMAP");
        NProgressDisplay::DisplayProgress("LoadAllLoadableGlobalThingsFromSingleFile", -1.0f, false, false);
        CMemoryFile* memory_file = new CMemoryFile;
        memory_file->Open((void*)block.GetAsCharArray(), block.GetLength());
        CCountedPointer<CAFile> file_pointer(memory_file);              // a named local: retail builds it, copies it into the argument slot, and releases it after SkipPastString
        World.GetThingManager().LoadLoadableGlobalThingsFromFile(file_pointer, map);
        parser.SkipPastString("ENDMAP");
    }
}
