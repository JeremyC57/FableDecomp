# T21: CCharString::LoadFromFile takes CAFile& (Ego_r/FableWin ?LoadFromFile@CCharString@@QAEXAAVCAFile@@PAVCASuspendableProcess@@J@Z), cx12 4352
A = "rebuild/src/compiled/00/4f/CWorldMap_LoadAllLoadableGlobalThingsFromSingleFile_004fe2a0.cpp"
B = "rebuild/src/compiled/00/50/CWorldMap_LoadRegionGraph_00506d40.cpp"
EDITS = [
    (A, "void LoadFromFile(CAFile* file, CASuspendableProcess* process, long length);", "void LoadFromFile(CAFile& file, CASuspendableProcess* process, long length);"),
    (A, "text.LoadFromFile((CAFile*)&file, 0, -1);", "text.LoadFromFile(*(CAFile*)&file, 0, -1);"),
    (B, "void LoadFromFile(CAFile* file, CASuspendableProcess* process, long length);", "void LoadFromFile(CAFile& file, CASuspendableProcess* process, long length);"),
    (B, "CCharString::LoadFromFile(CAFile*, CASuspendableProcess*, long)", "CCharString::LoadFromFile(CAFile&, CASuspendableProcess*, long)"),
    (B, "text.LoadFromFile(&file, 0, -1);", "text.LoadFromFile(file, 0, -1);"),
]
