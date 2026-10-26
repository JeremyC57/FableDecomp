// CWorld::SetSaveGameMarkerPos(const C3DVector& pos) -- remember where the save-game marker is and, when the main player's hero is alive, its facing angle as the marker angle
// Leaf: ?SetSaveGameMarkerPos@CWorld@@QAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049E590, 78 bytes (+2 int3 padding), thiscall, `ret 4`.
//   Ego_r.pdb ?SetSaveGameMarkerPos@CWorld@@QAEXABVC3DVector@@@Z (0x004A77F0).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroThing 0x00487DC0, the hero's physics component
//   (+0x60) virtual slot 63 (+0xFC) GetAngleXY() const (name as landed callers of the same slot).
// Layout: local model. engine/CWorld.h (Ego_r.pdb, packed) has SaveGameMarkerPos +0x151 / SaveGameMarkerAngleXY +0x160;
//   retail writes +0x141 / +0x150 (four containers before them are 4 bytes smaller each in retail), so the header is
//   not used here. The packing (C3DVector at an odd offset, a 15-byte slot before the float) is the PDB's own. CThing: PhysicsTC +0x60 (PDB +0x70),
//   Dead +0x91 bit 0 (PDB +0xA1) -- the retail offsets landed CThing models use.
// Shape: the vector assignment through a `lea` of the unaligned member; the alive test is the inlined dead-bit check.
class C3DVector { public: float X, Y, Z; };

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;                   // slot 63 (+0xfc)
};

class CThing { public:
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad_0x64[0x2d];
    unsigned char   Dead : 1;                           // +0x91 bit 0
};

class CPlayer { public: CThing* GetHeroThing(); };             // 0x00487DC0
class CPlayerManager { public: public:
    CPlayer& GetMainPlayer();   // 0x00449970
};    // 0x00449970

#pragma pack(push, 1)
class CWorld {
public:
    void SetSaveGameMarkerPos(const C3DVector& pos);
    unsigned char   _pad_0x00[0xc];
    CPlayerManager* PlayerManager;                      // +0x0c
    unsigned char   _pad_0x10[0x131];
    C3DVector       SaveGameMarkerPos;                  // +0x141 (retail; PDB +0x151, 15-byte slot)
    unsigned char   _pad_0x14d[0x3];                    // +0x14d
    float           SaveGameMarkerAngleXY;              // +0x150 (retail; PDB +0x160)
};
#pragma pack(pop)

void CWorld::SetSaveGameMarkerPos(const C3DVector& pos)
{
    SaveGameMarkerPos = pos;
    CThing* hero = PlayerManager->GetMainPlayer().GetHeroThing();
    if (hero && !hero->Dead)
        SaveGameMarkerAngleXY = hero->PhysicsTC->GetAngleXY();
}