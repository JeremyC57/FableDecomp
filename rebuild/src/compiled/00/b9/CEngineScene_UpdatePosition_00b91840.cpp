// CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags) -- re-file a moved primitive: unless flag 0x40 is set, unlink it from its grid cell / render list and hand it back to its type's primitive manager
// Leaf: UpdatePosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B91840, 39 bytes, static __fastcall (primitive ecx, flags edx), `ret`.
// Identity: Ego_r.pdb ?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z (static, __fastcall); ten
//   callers in the primitive SetPosition/SetObjectTransform family (0x00B31EB4 .., 0x00BC65C3, several as tail jumps).
// Callees: CEngineInternalPrimitiveBase::RemoveFromList 0x00B8FE60 (landed), CEnginePrimitiveManager::AddPrimitive(
//   CEngineInternalPrimitiveBase&, unsigned long) 0x00BACDD0 (Ego_r name) on the manager registered for the primitive's
//   Type, from the primitive type table at 0x01436E84 (managers from +0x10, as the landed 0x00BBCF70 models it).
// Layout: CEnginePersistentPrimitiveBase::Type at +0x08 (Ego_r.pdb). engine/CEngineInternalPrimitiveBase.h is a
//   member-less layout struct with +0x08 unnamed, so the primitive is a local model (as the landed RemoveFromList).
class CEngineInternalPrimitiveBase;

class CEnginePrimitiveManager {
public:
    void AddPrimitive(CEngineInternalPrimitiveBase& primitive, unsigned long flags);   // 0x00BACDD0
};

class CEnginePrimitiveTypeInfo { public:
    unsigned char            _pad[0x10];
    CEnginePrimitiveManager* Managers[1];          // +0x10, indexed by primitive type
};
extern CEnginePrimitiveTypeInfo* g_pPrimitiveTypeInfo;   // 0x01436E84

class CEngineInternalPrimitiveBase {
public:
    void* __vftable;                               // +0x00
    long  RefCount;                                // +0x04 CEnginePersistentPrimitiveBase
    long  Type;                                    // +0x08 CEnginePersistentPrimitiveBase::Type
    void RemoveFromList();                         // 0x00B8FE60
};

class CEngineScene {
public:
    static void __fastcall UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags);
};

void __fastcall CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase& primitive, unsigned long flags)
{
    if (!(flags & 0x40)) {
        primitive.RemoveFromList();
        g_pPrimitiveTypeInfo->Managers[primitive.Type]->AddPrimitive(primitive, flags);
    }
}