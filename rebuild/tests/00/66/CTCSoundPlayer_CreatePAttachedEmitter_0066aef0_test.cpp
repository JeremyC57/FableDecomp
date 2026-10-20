// Behaviour test for CTCSoundPlayer::CreatePAttachedEmitter (0x0066AEF0): a live attached emitter is returned as is; otherwise a
// new emitter is created at the owner's position (when inside the map), stored and returned; out of the map the old pointer.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;
class CBaseObject;
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                                 // +0x0c
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
};
struct CThingSoundEmitter {
    static CThingSoundEmitter* __fastcall Create(const C3DVector& position, const CThing* parent, bool unattached);
    static CThingSoundEmitter* g_next; static const C3DVector* g_pos; static const CThing* g_parent; static bool g_unattached; static int g_calls;
};
struct CWorldMap {
    bool inMap;
    bool IsPosInMap(const C3DVector& pos) const { return inMap && pos.X >= 0.0f; }
};
struct CWorld {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { }
    virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { }
    virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { }
    CWorldMap* Map;
    virtual CWorldMap* GetWorldMap() { return Map; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125B1B8
    virtual void v00() { }
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.CBaseIntelligentPointer::GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
struct CTCSoundPlayer {
    virtual void v00() { }
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;   // +0x0c
    CWorld* World;
    CWorld* GetWorld() { return World; }
    CThingSoundEmitter* CreatePAttachedEmitter(bool unattached);
};

CThingSoundEmitter* CTCSoundPlayer::CreatePAttachedEmitter(bool unattached)
{
    const C3DVector& position = ParentThing->PhysicsTC->Position;
    CThing* current = PAttachedEmitter.CBaseIntelligentPointer::GetPItem();
    if (!current || (current->m_flags91 & 1)) {
        if (GetWorld()->GetWorldMap()->IsPosInMap(position)) {
            CThingSoundEmitter* emitter = CThingSoundEmitter::Create(position, ParentThing, unattached);
            if (emitter)
                PAttachedEmitter = CIntelligentPointer<CThingSoundEmitter>(emitter);
            return emitter;
        }
    }
    return PAttachedEmitter.GetPItem();
}

CThingSoundEmitter* CThingSoundEmitter::g_next = 0; const C3DVector* CThingSoundEmitter::g_pos = 0;
const CThing* CThingSoundEmitter::g_parent = 0; bool CThingSoundEmitter::g_unattached = false; int CThingSoundEmitter::g_calls = 0;
CThingSoundEmitter* __fastcall CThingSoundEmitter::Create(const C3DVector& position, const CThing* parent, bool unattached)
{
    g_pos = &position; g_parent = parent; g_unattached = unattached; ++g_calls;
    return g_next;
}
int main()
{
    int bad = 0;
    CTCPhysicsBase physics; physics.Position.X = 1.0f; physics.Position.Y = 2.0f; physics.Position.Z = 3.0f;
    CThing owner; owner.PhysicsTC = &physics;
    CThing emitterThing; emitterThing.m_flags91 = 0;
    CWorldMap map; map.inMap = true;
    CWorld world; world.Map = &map;
    CTCSoundPlayer player; player.ParentThing = &owner; player.World = &world;
    CThingSoundEmitter* fresh = (CThingSoundEmitter*)&emitterThing;
    CThingSoundEmitter::g_next = fresh;
    CThingSoundEmitter* r = player.CreatePAttachedEmitter(true);
    if (r != fresh || player.PAttachedEmitter.m_pItem != &emitterThing) bad |= 1;
    if (CThingSoundEmitter::g_calls != 1 || CThingSoundEmitter::g_pos != &physics.Position || CThingSoundEmitter::g_parent != &owner || !CThingSoundEmitter::g_unattached) bad |= 2;
    r = player.CreatePAttachedEmitter(false);                 // alive: no new emitter
    if (r != fresh || CThingSoundEmitter::g_calls != 1) bad |= 4;
    emitterThing.m_flags91 = 1; CThingSoundEmitter::g_next = 0;
    r = player.CreatePAttachedEmitter(false);                 // dead: creation fails -> null, pointer kept
    if (r != 0 || CThingSoundEmitter::g_calls != 2 || player.PAttachedEmitter.m_pItem != &emitterThing) bad |= 8;
    map.inMap = false;
    r = player.CreatePAttachedEmitter(false);                 // out of the map: the stored (dead) pointer
    if (r != fresh || CThingSoundEmitter::g_calls != 2) bad |= 16;
    if (bad) { std::printf("0066aef0_TEST FAIL\n"); return 1; }
    std::printf("0066aef0_TEST PASS\n");
    return 0;
}