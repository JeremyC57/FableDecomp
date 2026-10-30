// CGameScriptInterface::TeleportAllFollowersToHeroPosition() const -- collect the live hero's followers (TC 0xBC), its live leader-like intelligent pointer (TC 0x91 +0x11) and the live entries of its TC 0x9A pointer vector; every follower more than 2 units (2D) from the hero that has creature navigation (TC 7) is moved to a nearby wander position (radius 0.8) round the hero through its physics body (slot 31)
// Grade: DIFFER(738v738) nd=96 (was 721v738 nd=117) -- lever: // Flags: /Oa (the length now matches; retail keeps hero in ebp spilled to the frame and the follower walk in registers like the /Oa TUs); tried with it: the unnamed 2D hero position (730 nd=108), the 0x9a list copied from a named const& (96); remaining: frame 0x38 v 0x30 -- every frame slot 4 bytes higher (the lookup out-pointer/key pair and the followers vector), i.e. one extra 4-byte local plus alignment
// Flags: /Oa
// Leaf: TeleportAllFollowersToHeroPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A7A70, length 738 bytes (virtual, plain `ret`).
// Callees: GetMainPlayer 0x00449970, GetHeroCreature 0x00487DC0, LowerBound 0x0040F020 (0xBC) / const 0x004365B0 (0x91, 0x9A, 7),
//   CTCFollowed::GetFollowers 0x006D5850, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
//   vector<CThing*> push_back 0x00436C80 / _M_insert_overflow 0x00436AC0, vector<CIntelligentPointer<CThing> > copy ctor 0x008AC540 /
//   dtor 0x00742210, CThing::GetPosition 0x004C73D0, CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0, free 0x00BFEA14.
//   Globals: 4.0f 0x0123088C, CIntelligentPointer<CThing> vtable 0x0123E7E0.
// Layout: CThing presence bits +0x20, component map +0x44, physics +0x60, dead flag +0x91 bit 0; the TC 0x91 pointer at packed +0x11, the TC 0x9A vector at +0x10.
#include <new>
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    explicit C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float LengthSquared() const { return X * X + Y * Y; }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);         // 0x0040F020
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* Physics;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_dead : 1;                   // +0x91 bit 0
    unsigned char m_91 : 7;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline const T* GetTCConst(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10 (slot 0)
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0123E7E0
    __forceinline CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerVector {              // STLport vector<CIntelligentPointer<CThing> > (8-byte elements)
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    CIntelligentPointerVector(const CIntelligentPointerVector& other);   // 0x008AC540
    ~CIntelligentPointerVector();                                       // 0x00742210
    long size() const { return _M_finish - _M_start; }
};
struct SScalarTag { SScalarTag() { } };
struct CThingVector {                           // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    void InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void PushBack(CThing* const& value);        // 0x00436C80 (out of line)
    __forceinline void push_back(CThing* const& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CThing*(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
__declspec(noinline) void CThingVector::InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    CThing** p = (CThing**)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
}
#pragma auto_inline(on)
struct CTCFollowed { void GetFollowers(CThingVector& out); };   // 0x006D5850
#pragma pack(push, 1)
struct CTCLeader91 {
    unsigned char _pad00[0x11];
    CIntelligentPointerThing Leader;            // +0x11
};
#pragma pack(pop)
struct CTCFollowers9A {
    unsigned char _pad00[0x10];
    CIntelligentPointerVector Followers;        // +0x10
};
struct CTCCreatureNavigation {
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector& result, bool onMap) const;   // 0x006CB6E0
};
struct CPlayer { CThing* GetHeroCreature(); };                  // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };            // 0x00449970
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CDefinitionManager;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                 World;              // +0x04
    CMainGameComponent*     Component;          // +0x08
    CDisplayEngine*         DisplayEngine;      // +0x0c
    CDefinitionManager*     DefinitionManager;  // +0x10
    CPlayerManager*         PlayerManager;      // +0x14
    virtual void TeleportAllFollowersToHeroPosition() const;
};

void CGameScriptInterface::TeleportAllFollowersToHeroPosition() const
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (!hero || hero->m_dead || !hero->HasTC(0xbc))
        return;
    CTCFollowed* followed = hero->GetTC<CTCFollowed>(0xbc);
    CThingVector followers;
    followed->GetFollowers(followers);
    if (hero->HasTC(0x91)) {
        CIntelligentPointerThing leader = hero->GetTCConst<CTCLeader91>(0x91)->Leader;
        if (leader.GetPItem() && !leader.GetPItem()->m_dead)
            followers.PushBack(leader.GetPItem());
    }
    if (hero->HasTC(0x9a)) {
        CIntelligentPointerVector others = hero->GetTCConst<CTCFollowers9A>(0x9a)->Followers;
        CIntelligentPointerThing* it = others._M_start;
        for (long i = others.size(); i > 0; --i, ++it) {
            if (it->GetPItem() && !it->GetPItem()->m_dead)
                followers.push_back(it->GetPItem());
        }
    }
    for (unsigned long i = 0; i < followers.size(); ++i) {
        CThing* follower = followers._M_start[i];
        C2DVector heroPosition(hero->GetPosition());
        if ((heroPosition - C2DVector(follower->GetPosition())).LengthSquared() > 4.0f && follower->HasTC(7)) {
            C3DVector position;
            if (follower->GetTCConst<CTCCreatureNavigation>(7)->GetNearbyWanderPosition(hero->GetPosition(), 0.8f, position, false) &&
                follower->Physics)
                follower->Physics->SetPosition(position);
        }
    }
}
