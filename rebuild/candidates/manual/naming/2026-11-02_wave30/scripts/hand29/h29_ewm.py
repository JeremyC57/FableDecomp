# h29_ewm.py: naming wave 29, family ewm (scope 3, c698 queue rows 49 / 50): CEngineWorldMap's own rows take their Ego_r.pdb / FableWin.pdb
#  names -- 0x00B40000 ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ (was the catalog CloseStaticMap, a CEngineMap member name), 0x00B42C70
#  the private overload ?DeleteMap@CEngineWorldMap@@AAEXXZ (was the catalog DeleteAllMaps; it calls the public DeleteMap(long) 0x00B42B10)
#  -- and the binder 0x00B428E0 (?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z) drops its invented CEngineWorldMapRetail
#  class: private OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE) 0x00B42750, private LoadWaterData() 0x00B41FA0, public
#  CloseStaticMapFile() 0x00B40000 (all three PDB spellings, both PDBs).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
a = '00b40000'
he.edit(a, 'src', '// CEngineWorldMap::CloseStaticMap() --', '// CEngineWorldMap::CloseStaticMapFile() --')
he.edit(a, 'src', 'StaticMapBuildStatus;// +0x1b0\n    void CloseStaticMap();\n};', 'StaticMapBuildStatus;// +0x1b0\n    void CloseStaticMapFile();\n};')
he.edit(a, 'src', 'void CEngineWorldMap::CloseStaticMap()', 'void CEngineWorldMap::CloseStaticMapFile()')
note(a, 'ewm', 'the own row is ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ (Ego_r.pdb / FableWin.pdb; bound by 0x00B42E70 / 0x00B428E0).\n'
     '//   Was the catalog name CloseStaticMap (the CEngineMap / CEngineWaterRenderer member it calls).', leaf='?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ')
a = '00b42c70'
he.edit(a, 'src', '// CEngineWorldMap::DeleteAllMaps() --', '// CEngineWorldMap::DeleteMap() --')
he.edit(a, 'src', '    void DeleteMap(long index);                  // 0x00B42B10\n    void DeleteAllMaps();\n};',
        '    void DeleteMap(long index);                  // 0x00B42B10\nprivate:\n    void DeleteMap();\n};')
he.edit(a, 'src', 'void CEngineWorldMap::DeleteAllMaps()', 'void CEngineWorldMap::DeleteMap()')
note(a, 'ewm', 'the own row is the private overload ?DeleteMap@CEngineWorldMap@@AAEXXZ (Ego_r.pdb / FableWin.pdb; bound by\n'
     '//   0x00B42E70 / 0x00B42ED0). Was the catalog name DeleteAllMaps.', leaf='?DeleteMap@CEngineWorldMap@@AAEXXZ')
a = '00b428e0'
he.edit(a, 'src', '// CEngineWorldMapRetail::SetStaticMapFileForUse(file) --', '// CEngineWorldMap::SetStaticMapFileForUse(file) --')
he.edit(a, 'src', '''struct CEngineWorldMapRetail {
 unsigned char UnrelatedMapState[0x30];CWideString StaticMapBankName;
 unsigned char StaticFileAndWorldState[0x17c];unsigned StaticMapOpenState;
 void SetStaticMapFileForUse(const wchar_t* file);
 void CloseStaticMapFile(); //00b40000
 void OpenStaticMaps(bool); //00b42750
 void LoadWaterData(); //00b41fa0
};
void CEngineWorldMapRetail::SetStaticMapFileForUse(const wchar_t* file){''', '''class CEngineMap { public: enum STATIC_MAP_OPEN_STATE { }; };   // the PDB's nested enum (enumerators not in the name sets)
class CEngineWorldMap { public:
 unsigned char UnrelatedMapState[0x30];CWideString StaticMapBankName;
 unsigned char StaticFileAndWorldState[0x17c];unsigned StaticMapOpenState;
 void SetStaticMapFileForUse(const CWideString& file);
 void CloseStaticMapFile(); //00b40000
private:
 void OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE); //00b42750
 void LoadWaterData(); //00b41fa0
};
void CEngineWorldMap::SetStaticMapFileForUse(const CWideString& file){''')
he.edit(a, 'src', ' StaticMapBankName=*reinterpret_cast<const CWideString*>(file); // retail calls the copy operator= 0x0099B7D0 with the argument as a CWideString&',
        ' StaticMapBankName=file; // the copy operator= 0x0099B7D0')
he.edit(a, 'src', ' OpenStaticMaps(true);', ' OpenStaticMaps((CEngineMap::STATIC_MAP_OPEN_STATE)1);')
note(a, 'ewm', 'the class is CEngineWorldMap (was the invented CEngineWorldMapRetail): own row\n'
     '//   ?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z, private ?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z\n'
     '//   0x00B42750 and ?LoadWaterData@CEngineWorldMap@@AAEXXZ 0x00B41FA0, ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ 0x00B40000 (Ego_r.pdb / FableWin.pdb).',
     leaf='?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z')
