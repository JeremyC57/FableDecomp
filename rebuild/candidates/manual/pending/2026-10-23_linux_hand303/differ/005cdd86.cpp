// CTCInventoryStats::CreateExperienceSliders(NUISystem::CComponent* parent, long spacing, bool current) -- builds the four experience level sliders (general / strength / skill / will) under parent and returns them
// Grade: DIFFER(860v864) -- slider-construction block order and register allocation differ across the six slider entries (265 masked lines; scheduling around the CCharString temporaries)
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
struct CDataBank { CWideString GetTextBySymbol(const CCharString& symbol); };   // 0x009C95E0
struct CMainGameComponent { unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NUISystem { struct CComponent; }
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    ~CCountedPointer();                                 // 0x004291DE
    void Reset(T* object);                              // 0x00429C6F
    T* Get() const { return m_pObject; }
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
};
struct CManager {
    CComponent* CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel,
                                  const CRGBColour& colour);   // 0x00427DC9
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
namespace std {
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector& other);                        // 0x005D85DE
    ~vector() { if (_M_start) free(_M_start); }
    void push_back(const T& value);                     // 0x00486024
};
}
struct CPresenceBits { bool test(long kind) const; };      // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
struct CTCInventoryExperience { long GetOriginalExperience(long stat); };   // 0x0056EA9A
struct CTCHeroExperience {
    unsigned char _pad00[0x14];
    long GeneralExperience;                                 // +0x14
    long GetStatExperience(long stat);                      // 0x006D6510
};
struct CTCInventoryStats {
    void* _vf;
    CThing* ParentThing;                                    // +0x04
    std::vector<NUISystem::CComponent*> CreateExperienceSliders(NUISystem::CComponent* parent, long spacing, bool current);
};
std::vector<NUISystem::CComponent*> CTCInventoryStats::CreateExperienceSliders(NUISystem::CComponent* parent, long spacing, bool current)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    std::vector<NUISystem::CComponent*> sliders;
    CTCInventoryExperience* experience;
    ParentThing->GetInterface(&experience, 0x16);
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    CTCHeroExperience* hero = ParentThing->GetTC<CTCHeroExperience>(0x68);
    CCountedPointer<NUISystem::CComponent> slider;
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_GENERAL"), C2DCoordI(0, 0), hero->GeneralExperience,
                                             current ? hero->GeneralExperience : experience->GetOriginalExperience(0), CRGBColour(0, 0xb4, 0, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    long strength = hero->GetStatExperience(0);
    long step = spacing + 0x19;
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_STRENGTH"), C2DCoordI(0, step), strength,
                                             current ? strength : experience->GetOriginalExperience(1), CRGBColour(0xb4, 0, 0, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    long skill = hero->GetStatExperience(1);
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_SKILL"), C2DCoordI(0, step * 2), skill,
                                             current ? skill : experience->GetOriginalExperience(2), CRGBColour(0xb4, 0xb4, 0, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    long will = hero->GetStatExperience(2);
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_WILL"), C2DCoordI(0, step * 3), will,
                                             current ? will : experience->GetOriginalExperience(3), CRGBColour(0, 0, 0xb4, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    return sliders;
}
