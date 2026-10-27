// CMultiStaticMeshEntryDef::Transfer(CPersistContext& context) -- persist the def's own members: Mesh, OverrideLightingChannel, OverrideRenderFadeDistance, RenderFadeDistance, LightingChannel
// Leaf: ?Transfer@CMultiStaticMeshEntryDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EB8C3, 68 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 of the CMultiStaticMeshEntryDef vtable 0x012438A4 (rtti-vtables); FableWin.pdb ?Transfer@CMultiStaticMeshEntryDef@@UAEXAAVCPersistContext@@@Z;
//   the transferred members are Ego_r.pdb's CMultiStaticMeshEntryDef fields at their retail offsets (retail = Ego_r.pdb).
// Callees: Transfer<long> 0x00431102 (Mesh), Transfer<bool> 0x0043314A (OverrideLightingChannel), Transfer<bool> 0x0043314A (OverrideRenderFadeDistance), Transfer<float> 0x00431061 (RenderFadeDistance), Transfer<ELightingChannel> 0x004735D6 (LightingChannel).
// Shape (family K094, representative 0x00457F13): one CPersistContext::Transfer<T>(T&) call per member, the context kept in edi;
//   size TU (every member address formed in eax).
#pragma optimize("s", on)
enum ELightingChannel { };                              // enumerators not needed here
class CPersistContext {
public:
    template <class T> void Transfer(T& value);          // per-type helpers (out of line)
};
class CSubDefClassBase {                                            // retail 0x28 bytes (members not touched here)
public:
    virtual ~CSubDefClassBase();
    unsigned char _body[0x28 - 4];
};
class CMultiStaticMeshEntryDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    long                Mesh;                              // +0x28
    bool                OverrideLightingChannel;           // +0x2C
    bool                OverrideRenderFadeDistance;        // +0x2D
    float               RenderFadeDistance;                // +0x30
    ELightingChannel    LightingChannel;                   // +0x34
};
void CMultiStaticMeshEntryDef::Transfer(CPersistContext& context)
{
    context.Transfer(Mesh);
    context.Transfer(OverrideLightingChannel);
    context.Transfer(OverrideRenderFadeDistance);
    context.Transfer(RenderFadeDistance);
    context.Transfer(LightingChannel);
}