// CThing::FrameUpdate() -- run the thing's bullet-time update while bullet time is active, this is not a full-update frame and the thing is affected by bullet time; otherwise its normal world update
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9D20, 64 bytes. Callees: CBulletTimeManager::IsTimeForFullUpdate
// @ 0x004C6060, CThing::IsAffectedByBulletTime @ 0x004C9920, CThing virtual slots 20 (+0x50,
// WorldUpdateBulletTime 0x004C8BA0) and 19 (+0x4c, the world update 0x004C8050), both tail jumps.
// Shape: the bullet-time manager is reached through the thing manager @ 0x013B8A1C (+0x24 -> +0x68) and its
// "active" test is an inline `Override || !BulletTimeInfoMap.empty()` (the bool at +0x0c, PDB +0x10; the
// map's element count at +0x04 -- the retail STLport map is 12 bytes, not the PDB's 16).
class CBulletTimeManager { public:
    unsigned char _pad00[0x04];
    long          m_bulletTimeInfoCount;             // +0x04 BulletTimeInfoMap size
    unsigned char _pad08[0x0c - 0x08];
    bool          Override;                          // +0x0c (PDB +0x10)
    bool IsActive() const { return Override || m_bulletTimeInfoCount; }
    bool IsTimeForFullUpdate();                      // 0x004C6060
};

struct CWorldTime {
    unsigned char        _pad00[0x68];
    CBulletTimeManager*  m_pBulletTimeManager;       // +0x68
};

class CThingManager { public:
    unsigned char _pad00[0x24];
    CWorldTime*   m_pWorldTime;                      // +0x24
};

extern CThingManager* g_pThingManager;              // 0x013B8A1C

class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual void WorldUpdate();                      // slot 19 (+0x4c)
    virtual void WorldUpdateBulletTime();            // slot 20 (+0x50)
    bool IsAffectedByBulletTime();                   // 0x004C9920
    void FrameUpdate();
};

void CThing::FrameUpdate()
{
    CBulletTimeManager* bulletTime = g_pThingManager->m_pWorldTime->m_pBulletTimeManager;
    if (bulletTime->IsActive() && !bulletTime->IsTimeForFullUpdate() && IsAffectedByBulletTime())
        WorldUpdateBulletTime();
    else
        WorldUpdate();
}