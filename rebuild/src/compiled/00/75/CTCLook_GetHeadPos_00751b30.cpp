// CTCLook::GetHeadPos() -- the head position: when the global head-position override flag is set ask the parent creature (GetHeadPos(false)), otherwise return the cached CachedHeadPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751B30, length 64 bytes.
// Callees: CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40 (C3DVector by hidden pointer, our own result pointer passed on).
// Global: the byte flag @ 0x013BAE1B (tested first; set = always query the creature).
// Layout (local model, no engine header): PParentCreature +0x10; TargetAcquired +0xa0 and CachedHeadPos +0xa1 (PDB +0xa4/+0xa5,
// minus 4 after the 12-byte retail vector TurningAnims; the PDB already places the C3DVector unaligned right after the bool,
// so the record is packed here to keep it at +0xa1). Shape: a C3DVector with a user constructor comes back through the
// hidden pointer (`ret 4`, eax = result); the cached copy is three dword moves through edx/ecx, which needs the
// memberwise user copy constructor (the implicit copy of the packed member is a block copy through esi).
class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};

class CThingCreatureBase { public:
public:
    C3DVector GetHeadPos(bool) const;   // 0x00661B40
};

extern bool g_bQueryCreatureHeadPos;             // 0x013BAE1B

#pragma pack(push, 1)
class CTCLook { public:
    unsigned char       _pad_0x00[0x10];
    CThingCreatureBase* PParentCreature;         // +0x10
    unsigned char       _pad_0x14[0xa0 - 0x14];
    bool                TargetAcquired;          // +0xa0 (PDB +0xa4)
    C3DVector           CachedHeadPos;           // +0xa1 (PDB +0xa5)
    C3DVector GetHeadPos();
};
#pragma pack(pop)

C3DVector CTCLook::GetHeadPos()
{
    if (g_bQueryCreatureHeadPos)
        return PParentCreature->GetHeadPos(false);
    return CachedHeadPos;
}