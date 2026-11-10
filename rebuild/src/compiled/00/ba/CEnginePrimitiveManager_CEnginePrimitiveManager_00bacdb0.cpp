// CEnginePrimitiveManager::CEnginePrimitiveManager(long type) -- CBase, the primitive type at +4, this class's vtable, the enabled flag at +8 set
// Leaf: ??0CEnginePrimitiveManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACDB0, length 29 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x012A53E8 = CEnginePrimitiveManager (rtti-vtables.tsv). Shape: the argument store precedes the vtable store
// (VC7.1 schedules the vtable between init-list stores), the byte is an immediate 1, `this` is returned.
// Name fix (agentNF5 2026-11-10): Ego_r.pdb / FableWin.pdb ??0CEnginePrimitiveManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z: the argument is
//   the scene list by reference, stored as the Ego_r member SceneList (CArray<CEngineScene*>*, +4; Enable +8); was (long type).
template<class T> class CArray;
class CEngineScene;
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CEnginePrimitiveManager : public CBaseClassNonCopyable { public:           // vtable 0x012A53E8
    CArray<CEngineScene*>* SceneList;             // +0x04 (Ego_r.pdb member)
    bool Enable;                                   // +0x08 (Ego_r.pdb member)
    CEnginePrimitiveManager(CArray<CEngineScene*>& scene_list);
};

CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>& scene_list)
    : CBaseClassNonCopyable()
    , SceneList(&scene_list)
    , Enable(true)
{
}