// CConfigOptionsDefaultsDef::Transfer(CPersistContext& context) -- persist the thirteen 4-byte option defaults (+0x3c..+0x70) through three typed CPersistContext transfer instantiations
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434290, 167 bytes (176 with padding), virtual `?Transfer@CConfigOptionsDefaultsDef@@UAEXAAVCPersistContext@@@Z`.
// Callees: CPersistContext transfer instantiations 0x00431020 (+0x3c), 0x00431102 (+0x40/+0x44/+0x48/+0x4c/+0x50),
//          0x00431061 (+0x54..+0x70) (catalog "CEngineLightingManager::UpdateShadowScene" is a propagated label;
//          each starts `push ebp ; ... push 0x122d70e` = the per-type CheckCRC transfer helper).
// Shape: the def base is 0x3c bytes; the members are 4-byte values transferred by address. The order of the
// calls is the declaration order of the persist list, not the layout (+0x4c/+0x50 come last).
// Layout: local model (no engine header for this def); member types are chosen per callee only to select the overload.
struct CPersistContext {
    void TransferValue(long* p);            // 0x00431020
    void TransferValue(float* p);           // 0x00431102
    void TransferValue(unsigned long* p);   // 0x00431061
};

struct CConfigOptionsDefaultsDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x38];   // CParentDefClassBase body
    long m_3c;                     // +0x3c
    float m_40;                    // +0x40
    float m_44;                    // +0x44
    float m_48;                    // +0x48
    float m_4c;                    // +0x4c
    float m_50;                    // +0x50
    unsigned long m_54;            // +0x54
    unsigned long m_58;            // +0x58
    unsigned long m_5c;            // +0x5c
    unsigned long m_60;            // +0x60
    unsigned long m_64;            // +0x64
    unsigned long m_68;            // +0x68
    unsigned long m_6c;            // +0x6c
    unsigned long m_70;            // +0x70
};

void CConfigOptionsDefaultsDef::Transfer(CPersistContext& context)
{
    context.TransferValue(&m_3c);
    context.TransferValue(&m_40);
    context.TransferValue(&m_44);
    context.TransferValue(&m_48);
    context.TransferValue(&m_54);
    context.TransferValue(&m_58);
    context.TransferValue(&m_5c);
    context.TransferValue(&m_60);
    context.TransferValue(&m_64);
    context.TransferValue(&m_68);
    context.TransferValue(&m_6c);
    context.TransferValue(&m_70);
    context.TransferValue(&m_4c);
    context.TransferValue(&m_50);
}