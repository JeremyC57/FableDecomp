// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::SetLightingChannel (retail 0x004BFB40): model copy + stubs.
#include <cstdio>
#include <cstdlib>

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    int renderListAdds;
    void AddToRenderList() { ++renderListAdds; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~CRetailVector() { if (_M_start) std::free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    void push_back(T v) { if (!_M_start) { _M_start = _M_finish = (T*)std::malloc(sizeof(T) * 8); _M_end = _M_start + 8; } *_M_finish++ = v; }
};
class CTCCarrying { public:
    CThing* things[4]; int count;
    void GetAllCarriedThings(CRetailVector<CThing*>& out) const { for (int i = 0; i < count; ++i) out.push_back(things[i]); }
};
class CEnginePrimitiveThemeChannelDesc { public: unsigned long Channel; };

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    CThing*        ParentThing;
    CEnginePrimitiveThemeChannelDesc LightingChannel;
    unsigned char  UpdatePrimitiveReasons;
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons);
};
void CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried,
                                              bool includeWeapons)
{
    LightingChannel = channel;
    CTCCarrying* carrying;
    if (includeCarried && ParentThing->GetInterface(&carrying, 0x46)) {
        CRetailVector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (unsigned int i = 0; i < carried.size(); ++i) {
            CThing* thing = carried[i];
            CTCGraphicAppearance* appearance;
            if ((includeWeapons || !thing->HasTC(0x48)) && thing->GetInterface(&appearance, 0x5b))
                appearance->SetLightingChannel(channel, includeCarried, includeWeapons);
        }
    }
    ParentThing->AddToRenderList();
    UpdatePrimitiveReasons |= 4;
}
}

static void Init(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_interfaces.first = t.m_interfaces.last = 0; t.renderListAdds = 0; }

int main()
{
    int fail = 0;
    using NGraphicAppearance::CTCGraphicAppearance;
    CThing hero, sword, hat, rock;
    Init(hero); Init(sword); Init(hat); Init(rock);
    CTCGraphicAppearance gHero = { &hero, { 0 }, 0 }, gSword = { &sword, { 0 }, 0 }, gHat = { &hat, { 0 }, 0 };
    CTCCarrying carrying = { { &sword, &hat, &rock, 0 }, 3 };
    CInterfaceMap::SEntry heroE[1] = { { 0x46, (CTCBase*)&carrying } };
    hero.m_presentBits[2] = 1UL << 6; hero.m_interfaces.first = heroE; hero.m_interfaces.last = heroE + 1;
    CInterfaceMap::SEntry swordE[2] = { { 0x48, 0 }, { 0x5b, (CTCBase*)&gSword } };
    sword.m_presentBits[2] = (1UL << 8) | (1UL << 27); sword.m_interfaces.first = swordE; sword.m_interfaces.last = swordE + 2;
    CInterfaceMap::SEntry hatE[1] = { { 0x5b, (CTCBase*)&gHat } };
    hat.m_presentBits[2] = 1UL << 27; hat.m_interfaces.first = hatE; hat.m_interfaces.last = hatE + 1;

    CEnginePrimitiveThemeChannelDesc ch = { 7 };
    gHero.SetLightingChannel(ch, true, false);   // weapons excluded
    if (gHero.LightingChannel.Channel != 7 || gHat.LightingChannel.Channel != 7 || gSword.LightingChannel.Channel != 0) fail = 1;
    if (!(gHero.UpdatePrimitiveReasons & 4) || !(gHat.UpdatePrimitiveReasons & 4) || hero.renderListAdds != 1 || hat.renderListAdds != 1) fail = 2;
    CEnginePrimitiveThemeChannelDesc ch2 = { 9 };
    gHero.SetLightingChannel(ch2, true, true);   // weapons included
    if (gSword.LightingChannel.Channel != 9 || gHat.LightingChannel.Channel != 9 || sword.renderListAdds != 1) fail = 3;
    CEnginePrimitiveThemeChannelDesc ch3 = { 11 };
    gHero.SetLightingChannel(ch3, false, true);  // carried things left alone
    if (gHero.LightingChannel.Channel != 11 || gHat.LightingChannel.Channel != 9 || hero.renderListAdds != 3) fail = 4;
    if (fail) { std::printf("004bfb40_TEST FAIL\n"); return 1; }
    std::printf("004bfb40_TEST PASS\n");
    return 0;
}