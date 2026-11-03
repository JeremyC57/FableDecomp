// CWorldMap::LoadLevelFromFiles(long, CCountedPointer<CAFile>, CCountedPointer<CAFile>, CCountedPointer<CAFile>, ELoadType) -- wrap the level's topology / navigation / thing files in a one-entry CLevelFiles vector, hand a new CLevelLoader::CObject (set loading with the world map's loadable sections) to the level loader, and for a synchronous load (type 0) run the loader until it is idle
// Leaf: ?LoadLevelFromFiles@CWorldMap@@IAEXJV?$CCountedPointer@VCAFile@@@@00W4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00501150, 756 bytes, thiscall, `ret 20h` (the three by-value CCountedPointer<CAFile>
//   arguments are released at the end, callee-destroyed).
// Identity: Ego_r.pdb ?LoadLevelFromFiles@CWorldMap@@IAEXJV?$CCountedPointer@VCAFile@@@@00W4ELoadType@1@@Z (0x004D55C0, between
//   LoadThingsWithSectionNames and GenerateRegionConnectivityGraph); only caller CWorldMap::LoadLevel 0x00502620 (Ego_r 0x004D6540).
// Callees: operator new / malloc (CObject 0x20, count block 0xc, CLevelFiles storage 0x1c), CCountedPointer<CLevelLoader::CObject>
//   ::DeleteData 0x0051AF00 (address stored), std::__uninitialized_fill_n<CLevelFiles*, unsigned, CLevelFiles> 0x00513570
//   (__fastcall, out of line in this TU), CLevelFiles::~CLevelFiles 0x0050F980, CWorldMap::GetLoadableSections (slot 26, +0x68),
//   CLevelLoader::CObject::SetAsLoading 0x006C27A0, CLevelLoader::Add 0x006C2120, CLevelLoader::IsBusy 0x006C20A0,
//   CSuspendableProcessMicrothread::RunSuspendableProcess (slot 1), set<CCharString> _M_erase 0x004ABA70, free.
// Layout: CWorldMap::PLevelLoader +0xBC; CLevelLoader / CObject / CLevelFiles from the FABLE_RETAIL_CLEVELLOADER branch (hdr/).
// Shape: vector(1) inline with the fill out of line (FABLE_STLP_OUTOFLINE_UNINIT_FILL_N, hdr/stlport); the three file members
//   assigned navigation, thing, topology (retail order); the sections set is a named local (destroyed after the loader runs);
//   `while (loadType == 0 && IsBusy())` gives retail's unrotated loop (the load-type test jump-threaded out of the back edge).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CLevelLoader.h"
#include "engine/CWorldMap.h"

void CWorldMap::LoadLevelFromFiles(long levelID, CCountedPointer<CAFile> topologyFile, CCountedPointer<CAFile> navigationFile,
                                   CCountedPointer<CAFile> thingFile, ELoadType loadType)
{
    CCountedPointer<CLevelLoader::CObject> object(new CLevelLoader::CObject);
    std::vector<CLevelLoader::CObject::CLevelFiles> levelFiles(1);
    levelFiles[0].LevelID = levelID;
    levelFiles[0].PNavigationFile = navigationFile;
    levelFiles[0].PThingFile = thingFile;
    levelFiles[0].PTopologyFile = topologyFile;
    std::set<CCharString> sections = GetLoadableSections();
    object->SetAsLoading(levelFiles, sections, 0);
    PLevelLoader->Add(object);
    while (loadType == 0 && PLevelLoader->IsBusy())
        PLevelLoader->RunSuspendableProcess();
}