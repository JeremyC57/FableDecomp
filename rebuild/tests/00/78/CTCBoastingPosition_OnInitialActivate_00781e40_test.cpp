// Behaviour test for CTCBoastingPosition::OnInitialActivate (0x00781E40).
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CCharString { const char* PStringData; CCharString(const char* t, long = -1) : PStringData(t) {} ~CCharString() {} };
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static bool g_hasHelper = true; static bool g_nameOk = false;
struct CTCGraphicAppearance {
    bool m_flag10;
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time)
    {
        g_nameOk = std::strcmp(name.PStringData, "PODIUM_ORB_CREATION_DUMMY") == 0 && index == 0 && flags == 0 && time == 1.0f;
        if (!g_hasHelper) return false;
        *position = C3DVector(7.0f, 8.0f, 9.0f); return true;
    }
};
static bool g_drawable = true;
class CThing {
public:
    virtual void SetAsDrawable(bool drawable) { g_drawable = drawable; }
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; CTCGraphicAppearance* PGraphicAppearance; C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
class CTCBoastingPosition {
public:
    CThing* ParentThing; C3DVector OrbPosition;
    void OnInitialActivate()
    {
        ParentThing->SetAsDrawable(false);
        CTCGraphicAppearance* graphic;
        if (ParentThing->GetInterface(&graphic, 9))
            graphic->m_flag10 = false;
        CTCGraphicAppearance* appearance = ParentThing->PGraphicAppearance;
        if (appearance) {
            bool missing;
            {
                CCharString name("PODIUM_ORB_CREATION_DUMMY");
                missing = appearance->GetHelperPoint(name, 0, 0, &OrbPosition, 1.0f) ? false : true;
            }
            if (missing) {
                const C3DVector& position = ParentThing->GetPosition();
                C3DVector orb;
                orb.X = position.X; orb.Y = position.Y; orb.Z = position.Z + 1.7f;
                OrbPosition = orb;
            }
        }
    }
};

int main()
{
    CTCGraphicAppearance app; app.m_flag10 = true;
    CInterfaceMap::SEntry entries[1] = { { 9, (CTCBaseInterface*)&app } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0] = 1UL << 9; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    thing.PGraphicAppearance = &app; thing.Pos = C3DVector(1.0f, 2.0f, 3.0f);
    CTCBoastingPosition p; p.ParentThing = &thing; p.OrbPosition = C3DVector(0.0f, 0.0f, 0.0f);
    p.OnInitialActivate();
    bool ok = !g_drawable && !app.m_flag10 && g_nameOk && p.OrbPosition.X == 7.0f && p.OrbPosition.Z == 9.0f;
    g_hasHelper = false;
    p.OnInitialActivate();
    float expectZ = 3.0f + 1.7f;
    ok = ok && p.OrbPosition.X == 1.0f && p.OrbPosition.Y == 2.0f && p.OrbPosition.Z == expectZ;
    if (!ok) { std::printf("00781e40_TEST FAIL\n"); return 1; }
    std::printf("00781e40_TEST PASS\n");
    return 0;
}