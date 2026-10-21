// Grade: DIFFER(421v419) -- register picks only (player movsx eax, hoisted lea ecx for the AddTC/key slot, radius pointer loads interleaved with fld/fadd, eax-free lea/vcall) plus the x87 pos.X add (retail fld pos.x; fadd st(2) keeping both scaled lanes)
// CTCBuyableHouse::CreateSignPost(const CCharString& defName, bool dressing) -- create a sign-post object of the named def at the house's (main or dressing) sign-post position, give it to the house's thing as owner, and push the living hero out of its way when he stands within the two radii
// Retail 0x006C0970, 419 bytes (`ret 8`; returns the sign post, 0 when the house has no sign-post position).
// Callees: CTCBuyableHouse::GetSignPostPos(bool, C3DVector&, CRightHandedSet*) @ 0x006BFCD0, CGameDefinitionManager::Get
// @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CCharString(const char*, long) @ 0x0099EBF0
// ("" @ 0x0122D70E, "CTCOwnedEntity" @ 0x0123AA74), ~CCharString @ 0x0099EAE0, CThingObject::Create @ 0x00703210
// (__fastcall), LowerBound @ 0x0040F020 (inlined GetInterface, CTCOwnedEntity = 0x33), CThing::AddTC @ 0x004C9D60,
// CTCOwnedEntity::SetPOwner @ 0x007E8590, CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0, the hero's +0x60 object's
// virtual slot 30 (+0x78, set position).
// Layout: CTCBase parent thing +4; CThing +0x60 physics object (radius +0x34), +0x90 signed player number.
struct CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
inline C3DVector operator*(const C3DVector& v, float s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const;
};
#include <math.h>
struct CRightHandedSet {
    C3DVector Up;                                   // +0x00
    C3DVector Forward;                              // +0x0c
    CRightHandedSet() {}
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThingPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& pos); // slot 30 (+0x78)
    unsigned char _pad04[0x34 - 0x04];
    float Radius;                                   // +0x34
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPhysics* Physics;                         // +0x60
    unsigned char _pad64[0x90 - 0x64];
    signed char Player;                             // +0x90
    const C3DVector& GetPosition() const;           // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingObject {
    static CThing* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                     const CRightHandedSet* orientation, unsigned long uid,
                                     const CCharString& scriptName);   // 0x00703210
};
struct CTCOwnedEntity {
    void SetPOwner(CThing* owner);                  // 0x007E8590
};
struct CPlayer {
    CThing* GetHeroCreature();                      // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
};

struct CTCBuyableHouse {
    void*   __vftable;                              // +0x00
    CThing* Thing;                                  // +0x04 CTCBase parent thing
    CPlayerManager* GetPlayerManager();             // 0x00686D40 (CTCBase)
    bool GetSignPostPos(bool dressing, C3DVector& pos, CRightHandedSet* orientation);   // 0x006BFCD0
    CThing* CreateSignPost(const CCharString& defName, bool dressing);
};

CThing* CTCBuyableHouse::CreateSignPost(const CCharString& defName, bool dressing)
{
    C3DVector pos;
    CRightHandedSet orientation;
    CThing* signPost = 0;
    if (GetSignPostPos(dressing, pos, &orientation)) {
        long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
        signPost = CThingObject::Create(defIndex, pos, Thing->Player, &orientation, 0, "");
        CTCOwnedEntity* owned;
        if (!signPost->GetInterface(&owned, 0x33))
            owned = (CTCOwnedEntity*)signPost->AddTC("CTCOwnedEntity", false, 0);
        owned->SetPOwner(Thing);
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (hero) {
            float clearance = signPost->Physics->Radius + hero->Physics->Radius;
            const C3DVector& heroPos = hero->GetPosition();
            const C3DVector& postPos = signPost->GetPosition();
            float dx = postPos.X - heroPos.X;
            float dy = postPos.Y - heroPos.Y;
            if (sqrtf(dy * dy + dx * dx) < clearance)
                hero->Physics->SetPosition(pos + orientation.Forward * clearance);
        }
    }
    return signPost;
}
