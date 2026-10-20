// Grade: DIFFER(259v258) -- scheduling/registers only: retail copies the first position with all three loads before the stores, interleaves the filter-pointer stores differently and keeps the found thing in esi from the first load
// CWorld::SetKickPos() -- store the hero's kick position for the current region: the nearest thing within 30 units carrying interface 0x63 or 0xDD gives the position, facing and flag
// Leaf: SetKickPos
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CTCPhysicsBase {
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
    virtual float GetAngleXY() const;              // slot 63 (+0xfc)
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PPhysics;                      // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool TestTC(long type) const { return (UsingComponents[type >> 5] >> (type & 31)) & 1; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CTCHeroStats {                              // TC 4
    void SetHeroKickPositionForCurrentRegion(const C3DVector& position, float angleXY, bool flag);   // 0x0057D990
};
struct CThingFilter_HasTC {
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
};
struct logical_or_bool { };
struct CHasEitherTC {                              // std::binary_compose<std::logical_or<bool>, HasTC, HasTC>
    logical_or_bool    Op;                         // +0x00
    CThingFilter_HasTC First;                      // +0x04
    CThingFilter_HasTC Second;                     // +0x08
    CHasEitherTC(long a, long b) : First(a), Second(b) { }
};
struct CThingFilter_In2DRange {
    C3DVector Centre;                              // +0x00
    float     Range;                               // +0x0c
    CThingFilter_In2DRange(const C3DVector& centre, float range) : Centre(centre), Range(range) { }
};
struct CFilter_Bind2 {                             // CFilter_Bind2<CHasEitherTC, CThingFilter_In2DRange>
    const CHasEitherTC*           First;
    const CThingFilter_In2DRange* Second;
    CFilter_Bind2(const CHasEitherTC& a, const CThingFilter_In2DRange& b) { Second = &b; First = &a; }
};
struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& p) : Position(p) { }
};
struct CThingSearchTools {
    bool GetPBestThing(const CFilter_Bind2& filter, const CThingCompare_Nearest& compare, CThing*& best);   // 0x004AC2C0
};
struct CPlayer { CThing* GetHeroThing(); };          // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); }; // 0x00449970
struct CWorld {
    unsigned char      _pad00[0xc];
    CPlayerManager*    PlayerManager;              // +0x0C
    unsigned char      _pad10[0x10];
    CThingSearchTools* PThingSearchTools;          // +0x20 (scoped_ptr)
    void SetKickPos();
};
void CWorld::SetKickPos()
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero->HasTC(4)) {
        CTCHeroStats* stats = hero->GetTC<CTCHeroStats>(4);
        {
            CThing* best = 0;
            CThingCompare_Nearest nearest(hero->GetPosition());
            CThingFilter_In2DRange range(hero->GetPosition(), 30.0f);
            CHasEitherTC either(0x63, 0xDD);
            CFilter_Bind2 filter(either, range);
            if (PThingSearchTools->GetPBestThing(filter, nearest, best)) {
                CThing* kick = best;
                stats->SetHeroKickPositionForCurrentRegion(kick->GetPosition(), kick->PPhysics->GetAngleXY(), kick->TestTC(0xDD));
            }
        }
    }
}
