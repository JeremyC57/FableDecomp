W='${S}/w22/chunk734/hdr/'
p=W+'engine/CWorld.h'; s=open(p).read()
cut=s.index('#else // flattened Ego_r layout')
ret,rest=s[:cut],s[cut:]
pairs=[
('''#include "stlport/fable_containers.h"                        // chunk 734: WeatherMaskingPrimitiveHandles' real type
''','''#include "stlport/fable_containers.h"                        // chunk 734: WeatherMaskingPrimitiveHandles' real type
#include "fable_string.h"                                    // chunk 734: SaveGamePathName / MostRecentManualSaveName' real type
'''),
('''enum EPlayerSpawnStatus;
''','''enum EPlayerSpawnStatus;
enum ESaveType;
'''),
('''    void OpenHeroInformationScreen(EPlayerMode mode);            // 0x0049F3F0 ?OpenHeroInformationScreen@CWorld@@QAEXW4EPlayerMode@@@Z
''','''    void OpenHeroInformationScreen(EPlayerMode mode);            // 0x0049F3F0 ?OpenHeroInformationScreen@CWorld@@QAEXW4EPlayerMode@@@Z
    // Chunk 734 (row 0x004A5E90 AutoLoadRespawn): the save-game load entry points (QAE / QBE in both PDBs).
    CWideString GetPathNameForFileName(const CWideString& fileName) const;   // 0x0049EF50 ?GetPathNameForFileName@CWorld@@QBE?AVCWideString@@ABV2@@Z
    bool LoadGameState(const CWideString& name, bool loadNow);  // 0x004A3200 ?LoadGameState@CWorld@@QAE_NABVCWideString@@_N@Z
    bool AutoLoadRespawn();                                      // 0x004A5E90 ?AutoLoadRespawn@CWorld@@QAE_NXZ
'''),
('''    void UpdateEnvironmentCameraPos(const C3DVector& pos);       // 0x0049E370 ?UpdateEnvironmentCameraPos@CWorld@@IAEXABVC3DVector@@@Z
''','''    void UpdateEnvironmentCameraPos(const C3DVector& pos);       // 0x0049E370 ?UpdateEnvironmentCameraPos@CWorld@@IAEXABVC3DVector@@@Z
    bool IsHeroStateFileValid(const CWideString& name) const;    // 0x0049D4E0 ?IsHeroStateFileValid@CWorld@@IBE_NABVCWideString@@@Z
    bool LoadGameStateInternal(const CWideString& pathName, bool b);   // 0x004A21F0 ?LoadGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z
    bool LoadHeroStateInternal(const CWideString& name);         // 0x004A2D70 ?LoadHeroStateInternal@CWorld@@IAE_NABVCWideString@@@Z
    bool AutoLoadInternal(const CWideString& name, bool loadNow);   // 0x004A3730 ?AutoLoadInternal@CWorld@@IAE_NABVCWideString@@_N@Z
'''),
('CWideStringData*            SaveGamePathName_PStringData; // +0xfc CWideString::PStringData','CWideString                 SaveGamePathName;            // +0xfc (chunk 734: typed)'),
('unsigned char               MostRecentSaveType[0x4];     // +0x134 ESaveType','ESaveType                   MostRecentSaveType;          // +0x134 (chunk 734: typed)'),
('unsigned char               MostRecentSaveTypeBeforeManualSave[0x4]; // +0x138 ESaveType','ESaveType                   MostRecentSaveTypeBeforeManualSave; // +0x138 (chunk 734: typed)'),
('CWideStringData*            MostRecentManualSaveName_PStringData; // +0x13c CWideString::PStringData','CWideString                 MostRecentManualSaveName;    // +0x13c (chunk 734: typed)'),
]
for old,new in pairs:
    assert ret.count(old)==1,(old[:60],ret.count(old))
    ret=ret.replace(old,new)
open(p,'w').write(ret+rest)
