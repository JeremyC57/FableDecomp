// CScriptDefinitionManager::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the inline user destructor (own vtable store, then ResetGlobalScriptDef()) and the out-of-line ~CDefinitionManager, then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_GCScriptDefinitionManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4330, 43 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable
//   0x012C2648 slot 0.
// Identity (proven): CScriptDefinitionManager; evidence: vtable 0x012C2648 slot 0 (no RTTI locator: the dword before the vtable is data), stored by the landed CScriptDefinitionManager::CScriptDefinitionManager 0x00CD3F00; the base destructor 0x009B02E0 is the landed ~CDefinitionManager and the clean-up is the landed ResetGlobalScriptDef 0x00CBEEB0 (the script def); FableWin.pdb names ??1CScriptDefinitionManager@@UAE@XZ and ??_GCScriptDefinitionManager@@UAEPAXI@Z.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across family H025 (seven bodies with the same shape: vtable store, one call,
//   the base destructor, the flag test; retail has no identical-code folding, so each address is one class).
// Callees: ResetGlobalScriptDef() (a __fastcall global) 0x00CBEEB0, CDefinitionManager::~CDefinitionManager 0x009B02E0, operator delete 0x00BFE9BC.
// Shape: VC7.1 resets the vptr in a user-written destructor, so the store is the class's own vtable; the destructor body is
//   one call (`this` is already in ecx), the base destructor follows. The deleting destructor is emitted with the vtable, in
//   a TU that constructs the class: MakeOne is that construction, nothing more.
void __fastcall ResetGlobalScriptDef();                // 0x00CBEEB0 (PDB ?ResetGlobalScriptDef@@YIXXZ)
class CDefinitionManager {
public:
    CDefinitionManager();
    virtual ~CDefinitionManager();                      // 0x009B02E0
};
class CScriptDefinitionManager : public CDefinitionManager {
public:
    CScriptDefinitionManager() { }
    virtual ~CScriptDefinitionManager() { ResetGlobalScriptDef(); }
};
void* MakeOne_00cd4330()
{
    return new CScriptDefinitionManager;
}