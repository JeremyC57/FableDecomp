// CTesselationData::CTesselationData(CEngineMap*, long, long, unsigned short, unsigned short) -- patch tesselation record constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE59C0, length 57 bytes. Vtable 0x012A800C (RTTI: CTesselationData).
// Callees: CBase::CBase() @ 0x0099A2D0 (the polymorphic root, its vtable store is dead and dropped).
// Layout: PDB CTesselationData: PatchWorldX +0x04, PatchWorldY +0x08, Width +0x0c, Height +0x10, EngineMap +0x14
// (the PDB shows the vptr slot as padding; retail stores vtable 0x012A800C). Width/Height arrive as unsigned
// shorts (`movzx eax,word ptr [esp+..]`) and the map pointer is the FIRST argument, stored last (+0x14); the
// manifest's "CAnimateThing::GetPMeshAnims" label is propagated. `ret 14h`: five stack arguments.
// Shape: the member initialisers in declaration order; the vtable store lands between them because the
// derived constructor's own store is scheduled with the member stores.
struct CEngineMap;

struct CBase {
    virtual void v0();
    CBase();                                    // 0x0099A2D0
};

struct CTesselationData : CBase {               // vtable 0x012A800C
    long PatchWorldX;                           // +0x04
    long PatchWorldY;                           // +0x08
    long Width;                                 // +0x0c
    long Height;                                // +0x10
    CEngineMap* EngineMap;                      // +0x14
    CTesselationData(CEngineMap* map, long worldX, long worldY, unsigned short width, unsigned short height);
};

CTesselationData::CTesselationData(CEngineMap* map, long worldX, long worldY, unsigned short width, unsigned short height)
    : PatchWorldX(worldX), PatchWorldY(worldY), Width(width), Height(height), EngineMap(map)
{
}
