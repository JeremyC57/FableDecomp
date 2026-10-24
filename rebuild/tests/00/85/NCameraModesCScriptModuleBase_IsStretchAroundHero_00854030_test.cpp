// Behaviour test for 0x00854030 NCameraModes::CScriptModuleBase::IsStretchAroundHero: true when the live camera point
// thing has a camera-point component (interface 0x3F) whose subject is the hero, or else when the live origin thing is
// a creature (type 2) whose player is the main player; false otherwise.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
struct CTCBase;
struct CPlayer {
    bool IsMainPlayer() const;                       // 0x00487520
};
struct CInterfaceMap {                               // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
    __declspec(noinline) const SEntry* LowerBound(const long* type) const   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        const SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            const SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                            // +0x10 thing type (2 here: a creature that can be the player)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_dead : 1;                        // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool IsDead() const { return m_dead; }
    template<class T> __forceinline bool PeekTC(const T** ppOut, long interfaceType) const   // FableWin.pdb ??$PeekTC@...@CThing@@QBE_NPAPB...@Z
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* found = map->LowerBound(&type);
            const CInterfaceMap::SEntry* it;
            if (found != map->last && !(interfaceType < found->type))
                it = found;
            else
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
    CPlayer* GetPlayer();                            // 0x004C7A10 (CThingCreatureBase::GetPlayer)
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CThing* Get() const { return (CThing*)GetPItem(); }
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CTCCameraPointDefinitionBase {                // retail = Ego_r.pdb offsets for these bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType() const;         // slot 28 (+0x70)
    virtual void v29();
    virtual bool IsHeroTheSubject() const;           // slot 30 (+0x78) (FableWin.pdb virtual; slot inferred)
    CThing*       m_pThing;                          // +0x04
    unsigned char _pad08[0x0c - 0x08];
    bool          CutInto;                           // +0x0c
    bool          CutOutOf;                          // +0x0d
};
namespace NCameraModes {
struct CBase;
struct CScriptModuleBase {                           // vtable 0x01255F74, 0x4c bytes
    virtual ~CScriptModuleBase();                    // slot 0
    virtual void Update() = 0;                       // slot 1
    virtual bool IsFinished() const;                 // slot 2
    virtual bool IsShortenOnOcclusion() const;       // slot 3
    virtual bool IsStretchAroundHero() const;        // slot 4
    virtual bool GetModuleFOV(float& fov) const;     // slot 5
    virtual bool GetModuleRollAngle(float& angle) const;   // slot 6
    virtual bool ResetCameraOnEnd() const;           // slot 7
    virtual void OnInitLeaf();                       // slot 8
    CIntelligentPointer_CThing PCamPoint;            // +0x04
    CIntelligentPointer_CThing PThingOrigin;         // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    void*               PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
    CThing* GetPCameraPoint() const { return PCamPoint.Get(); }
    CThing* GetPThingOrigin() const { return PThingOrigin.Get(); }
};
bool CScriptModuleBase::IsStretchAroundHero() const
{
    if (GetPCameraPoint() && !GetPCameraPoint()->IsDead()) {
        const CTCCameraPointDefinitionBase* point;
        if (GetPCameraPoint()->PeekTC(&point, 0x3f) && point->IsHeroTheSubject())
            return true;
    }
    if (GetPThingOrigin() && !GetPThingOrigin()->IsDead() && GetPThingOrigin()->m_type == 2
        && GetPThingOrigin()->GetPlayer()->IsMainPlayer())
        return true;
    return false;
}
}


static bool g_heroSubject = false;
static bool g_mainPlayer = false;
static CPlayer g_player;
bool CPlayer::IsMainPlayer() const { return g_mainPlayer; }
CPlayer* CThing::GetPlayer() { return &g_player; }
void CBaseIntelligentPointer::v00() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
#define V(n) void CTCCameraPointDefinitionBase::v##n() { }
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(29)
long CTCCameraPointDefinitionBase::GetCameraPointType() const { return 0; }
bool CTCCameraPointDefinitionBase::IsHeroTheSubject() const { return g_heroSubject; }
namespace NCameraModes {
CScriptModuleBase::~CScriptModuleBase() { }
bool CScriptModuleBase::IsFinished() const { return false; }
bool CScriptModuleBase::IsShortenOnOcclusion() const { return false; }
bool CScriptModuleBase::GetModuleFOV(float&) const { return false; }
bool CScriptModuleBase::GetModuleRollAngle(float&) const { return false; }
bool CScriptModuleBase::ResetCameraOnEnd() const { return false; }
void CScriptModuleBase::OnInitLeaf() { }
struct TModule : CScriptModuleBase { void Update() { } };
}

static void MakeThing(CThing& t, CInterfaceMap::SEntry* entries, CTCBase* point)
{
    memset(&t, 0, sizeof(t));
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x3f; entries[1].component = point;
    t.m_interfaces.first = entries;
    t.m_interfaces.last = entries + 2;
}

int main()
{
    static CTCCameraPointDefinitionBase point;
    static CThing cam, origin;
    static CInterfaceMap::SEntry ce[2], oe[2];
    MakeThing(cam, ce, (CTCBase*)&point);
    MakeThing(origin, oe, 0);
    NCameraModes::TModule m;
    m.PCamPoint.m_pItem = 0;
    m.PThingOrigin.m_pItem = 0;

    int ok = 1;
    if (m.IsStretchAroundHero()) ok = 0;                // nothing at all
    m.PCamPoint.m_pItem = (CBaseObject*)&cam;
    g_heroSubject = true;
    if (m.IsStretchAroundHero()) ok = 0;                // camera point has no 0x3F bit
    cam.m_presentBits[0x3f >> 5] |= 1UL << (0x3f & 31);
    if (!m.IsStretchAroundHero()) ok = 0;               // hero is the subject
    cam.m_dead = 1;
    if (m.IsStretchAroundHero()) ok = 0;                // dead camera point
    cam.m_dead = 0;
    g_heroSubject = false;
    if (m.IsStretchAroundHero()) ok = 0;                // not the hero's point, no origin
    m.PThingOrigin.m_pItem = (CBaseObject*)&origin;
    origin.m_type = 2;
    g_mainPlayer = true;
    if (!m.IsStretchAroundHero()) ok = 0;               // origin is the main player's creature
    origin.m_type = 1;
    if (m.IsStretchAroundHero()) ok = 0;                // not a creature
    origin.m_type = 2;
    g_mainPlayer = false;
    if (m.IsStretchAroundHero()) ok = 0;                // another player

    printf(ok ? "00854030_TEST PASS\n" : "00854030_TEST FAIL\n");
    return ok ? 0 : 1;
}