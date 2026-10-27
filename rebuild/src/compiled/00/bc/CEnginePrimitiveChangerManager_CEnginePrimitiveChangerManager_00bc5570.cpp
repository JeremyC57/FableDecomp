// CEnginePrimitiveChangerManager::CEnginePrimitiveChangerManager(CArray<CEngineScene*>& scenes) -- base CEnginePrimitiveManager(scenes), then add the "EnableChangingPrimitives" console variable
// Leaf: ??0CEnginePrimitiveChangerManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bc5570, 62 bytes, thiscall, `ret 4`, no EH frame.
// Identity (proven): it stores vtable 0x012A65D4 (RTTI .?AVCEnginePrimitiveChangerManager@@, direct base CEnginePrimitiveManager by rtti_bases) and registers "EnableChangingPrimitives" (0x012A6608); Ego_r.pdb / FableWin.pdb have ??0CEnginePrimitiveChangerManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z; caller CEnginePrimitiveRenderer::CEnginePrimitiveRenderer 0x00B33B50.
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>&) 0x00BACDB0, CCharString::CCharString(const char*, long) 0x0099EBF0 /
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
class CEnginePrimitiveChangerManager : public CEnginePrimitiveManager { public:   // vtable 0x012A65D4
    CEnginePrimitiveChangerManager(CArray<CEngineScene*>& scenes);
    virtual ~CEnginePrimitiveChangerManager();
};

CEnginePrimitiveChangerManager::CEnginePrimitiveChangerManager(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes)
{
    AddEnableConsoleVariable("EnableChangingPrimitives");
}