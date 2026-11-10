// CWorldMap::LoadWorldFromBinaryFile(CWideString& filename) -- read the whole binary world file into memory, then stream in the map infos and the regions (entry 0 of each stays the default), place every map and register it with the engine manager
// Leaf: ?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00507650, 875 bytes, thiscall, `ret 4`, frame 0xA8.
// Identity: Ego_r.pdb / FableWin.pdb ?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z (non-const reference; the
//   catalog's "CWorldMap::LoadFromFile" is wrong: that name is the virtual (CAFile&, long) slot).
// Callees: ??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z 0x0099AD80 and its Read 0x0099A960 (called directly on
//   the local: the type is known), Close 0x0099A920 / ~CWideString 0x0099B510 / ~CBaseClassNonCopyable 0x0099A300 from the
//   inline ~CDiskFileWin32; malloc 0x00BFEA0E / free 0x00BFEA14 (the CArray<char> buffer); ??0CMemoryDataInputStream@@QAE@PBXK@Z
//   0x00994930 and the compiler-generated destructor's ~CDataStream 0x00993B10; ReadWithSrcChunkOverflow 0x00993CA0 (the inline
//   ReadSLONG's slow path); ?resize@?$vector@VCMapInfo@@..@@QAEXI@Z 0x0051BE20 (out of line) and ?LoadBinary@CMapInfo@@QAEXAAV
//   CDataInputStream@@@Z 0x004FB4F0; the inline vector<CRegion>::resize(n): ??0CRegion@@QAE@XZ 0x006BC410 on its default
//   temporary, erase(first, last) 0x0051CC80 / _M_fill_insert 0x0051D0A0, ??1CRegion@@QAE@XZ 0x0051A110; ?LoadBinary@CRegion@@
//   QAEXAAVCDataInputStream@@@Z 0x006BC510; ?SetMapPlacement@CWorldMap@@IAEXABVC2DBoxI@@J@Z 0x004FC9C0; the inline
//   CTBaseSingleton<CEngineManager>::Get (out of line elsewhere as 0x00436FB0): the instance CCountedPointer 0x013BA854 / 0x013BA858,
//   operator new 0x00BFEA1A, the inline CEngineManager constructor (vtable 0x01231584), the inline CCountedPointer::Reset
//   (operator delete 0x00BFE9BC, DeleteData 0x004361E0); ?AddMap@CEngineManager@@QAEXJ@Z 0x006403A0.
// Shape: the buffer is CArray<char>(length + 1) (vector(n): malloc, then STLport's char fill_n memset, 0x005076A4); Read and the
//   stream take GetAsCArray() / size() (an empty array passes its own address); the counts are ReadSLONG (the inline
//   bounds-checked chunk read); the map and region loops start at 1.
// Headers (FABLE_HDR_OVERLAY): new engine/CTBaseSingleton.h and engine/CEngineManager.h; opt-ins CWorldMap LOADWORLDFROMBINARY,
//   CRegion LOADBINARY, CMapInfo LOADBINARY, STLport FILL_CHAR (hdr/README.md).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADWORLDFROMBINARY
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_LOADBINARY
#define FABLE_RETAIL_CMAPINFO_LOADBINARY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CArray.h"
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CEngineManager.h"

void CWorldMap::LoadWorldFromBinaryFile(CWideString& filename)
{
    CDiskFileWin32 file(filename, FILE_ACCESS_READ, 2);
    CArray<char> buffer(file.GetLength() + 1);
    file.Read(buffer.GetAsCArray(), buffer.size());
    CMemoryDataInputStream stream(buffer.GetAsCArray(), buffer.size());

    long mapCount = stream.ReadSLONG();
    Maps.resize(mapCount);
    for (long i = 1; i < mapCount; ++i)
        Maps[i].LoadBinary(stream);

    long regionCount = stream.ReadSLONG();
    Regions.resize(regionCount);
    for (long r = 1; r < regionCount; ++r)
        Regions[r].LoadBinary(stream);

    for (long m = 1; m < mapCount; ++m)
    {
        SetMapPlacement(Maps[m].Area, m);
        CEngineManager::Get().AddMap(m);
    }
}