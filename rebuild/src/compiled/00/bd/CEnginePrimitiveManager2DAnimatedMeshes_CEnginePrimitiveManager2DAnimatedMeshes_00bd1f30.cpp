// CEnginePrimitiveManager2DAnimatedMeshes::CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>& scenes) -- base CEnginePrimitiveManagerAnimatedMeshesBase(scenes), then add the "Enable2DAnimatedMeshes" console variable
// Leaf: ??0CEnginePrimitiveManager2DAnimatedMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bd1f30, 62 bytes, thiscall, `ret 4`, no EH frame.
// Identity (proven): it stores vtable 0x012A7964 (RTTI .?AVCEnginePrimitiveManager2DAnimatedMeshes@@, direct base CEnginePrimitiveManagerAnimatedMeshesBase by rtti_bases) and registers "Enable2DAnimatedMeshes" (0x012A7998); Ego_r.pdb / FableWin.pdb have ??0CEnginePrimitiveManager2DAnimatedMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z; caller CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D 0x00B4AC10.
// Callees: CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>&) 0x00BCFF10, CCharString::CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CEnginePrimitiveManager::AddEnableConsoleVariable(const CCharString&) 0x00BACD80 (the Ego_r.pdb
//   name; a retail no-op, `ret 4`).
// Shape (family L115): the literal goes through the implicit const char* -> const CCharString& conversion; the temporary
//   lives in the dead argument slot. Classes as in the landed CEnginePrimitiveManager2DSprites constructor 0x00BAD040.
class CCharString { public:
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    void* m_pData;
};
template <class T> class CArray;
class CEngineScene;

class CEnginePrimitiveManager { public:
    virtual ~CEnginePrimitiveManager();
    CArray<CEngineScene*>* SceneList;              // +0x04
    bool                   Enable;                 // +0x08
    CEnginePrimitiveManager(CArray<CEngineScene*>& scenes);   // 0x00BACDB0
    void AddEnableConsoleVariable(const CCharString& name);  // 0x00BACD80
};
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager { public:
    CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>& scenes);
};
class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManagerOcclusionBase { public:
    CEnginePrimitiveManagerMeshBase(CArray<CEngineScene*>& scenes);
};
class CEnginePrimitiveManagerAnimatedMeshesBase : public CEnginePrimitiveManagerMeshBase { public:
    CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>& scenes);   // 0x00BCFF10
};
class CEnginePrimitiveManager2DAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase { public:   // vtable 0x012A7964
    CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>& scenes);
    virtual ~CEnginePrimitiveManager2DAnimatedMeshes();
};

CEnginePrimitiveManager2DAnimatedMeshes::CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerAnimatedMeshesBase(scenes)
{
    AddEnableConsoleVariable("Enable2DAnimatedMeshes");
}