// Behaviour test for 0x00870BB0: CTCDInternalMarker::Create returns the new thing only when it has the internal-marker
// driver (type 0xA2) and activates; the driver is constructed at the position.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

class CWorldMap {
public:
    virtual ~CWorldMap() { }
    virtual long GetMapNumberAt(const C3DVector& pos) const { return pos.X > 0 ? 7 : 2; }
};

class CThingManager { public: CWorldMap* WorldMap; };
CThingManager* g_pThingManager;

class CTCDInternalMarker;

class CThing {
public:
    unsigned int UsingComponents[9];
    void* PTCDriver;
    bool  m_activates;
    long  m_def, m_player, m_map;
    bool  m_persistent;
    static CThing* s_next;
    static CThing* Create(long defIndex, long player, long map, bool levelPersistent)
    {
        CThing* t = s_next;
        if (t) { t->m_def = defIndex; t->m_player = player; t->m_map = map; t->m_persistent = levelPersistent; }
        return t;
    }
    bool DoActivationSequence() { return m_activates; }
    bool GetTCDriver(CTCDInternalMarker** out)
    {
        if (UsingComponents[5] & 0x4) {
            *out = (CTCDInternalMarker*)PTCDriver;
            return true;
        }
        return false;
    }
};
CThing* CThing::s_next = 0;

class CTCDInternalMarker {
public:
    C3DVector m_pos;
    int       m_constructed;
    static CThing* Create(long defIndex, const C3DVector& position);
    void Construct(const C3DVector& position) { m_pos = position; ++m_constructed; }
};

CThing* CTCDInternalMarker::Create(long defIndex, const C3DVector& position)
{
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(position), true);
    CThing* result = 0;
    if (thing) {
        CTCDInternalMarker* marker;
        if (thing->GetTCDriver(&marker)) {
            marker->Construct(position);
            if (thing->DoActivationSequence())
                result = thing;
        }
    }
    return result;
}

int main()
{
    CWorldMap map; CThingManager mgr; mgr.WorldMap = &map; g_pThingManager = &mgr;
    CTCDInternalMarker marker; marker.m_constructed = 0;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.UsingComponents[i] = 0;
    thing.UsingComponents[5] = 0x4; thing.PTCDriver = &marker; thing.m_activates = true;
    C3DVector pos = { 5.0f, 1.0f, 2.0f };
    int fails = 0;
    CThing::s_next = 0;
    if (CTCDInternalMarker::Create(11, pos) != 0) ++fails;
    CThing::s_next = &thing;
    if (CTCDInternalMarker::Create(11, pos) != &thing) ++fails;
    if (thing.m_def != 11 || thing.m_player != 4 || thing.m_map != 7 || !thing.m_persistent) ++fails;
    if (marker.m_constructed != 1 || marker.m_pos.X != 5.0f) ++fails;
    thing.m_activates = false;
    if (CTCDInternalMarker::Create(11, pos) != 0) ++fails;
    thing.m_activates = true; thing.UsingComponents[5] = 0;
    if (CTCDInternalMarker::Create(11, pos) != 0 || marker.m_constructed != 2) ++fails;
    if (fails) { std::printf("00870bb0_TEST FAIL\n"); return 1; }
    std::printf("00870bb0_TEST PASS\n");
    return 0;
}