// CTCInventoryStats::CreateExperienceSliders(NUISystem::CComponent* parent, long spacing, bool current) -- builds the four experience level sliders (general / strength / skill / will) under parent and returns them
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Grade: RELOCATION_MATCH (sec1b 2026-10-30) -- one `long value` and one `long level` reused by all four sliders (value = the stat, level = current ? value : the original experience), as retail homes them in the dead parent slot / [ebp-4]; per-slider named values gave fresh slots. Earlier residue: frame-slot assignment only.
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NUISystem { class CComponent; }

class CMainGameComponent;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;    // +0x14
};
namespace NUISystem {
class CComponent {
public:
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
class CManager {
public:
    static CManager* __fastcall GetInstance();  // 0x0041E5F2
public:
    CComponent* CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel,
                                  const CRGBColour& colour);   // 0x00427DC9
};
}
namespace std {
template <class T> class allocator { };
template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector& other);                        // 0x005D85DE
    ~vector() { if (_M_start) free(_M_start); }
    void push_back(const T& value);                     // 0x00486024
};
}
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
class CTCBase;
// agentNF15: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->_M_finish || interfaceType < it->first) ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
class CTCInventoryExperience {
public:
    long GetOriginalExperience(long stat);                  // 0x0056EA9A
};
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_STRENGTH, HERO_EXPERIENCE_STAT_SKILL, HERO_EXPERIENCE_STAT_WILL };   // descriptive enumerators
class CTCHeroExperience {
public:
    unsigned char _pad00[0x14];
    long ExperiencePointsAvailableToSpend;                  // +0x14 (Ego_r.pdb)
    long GetStatExperience(EHeroExperienceStatCategory stat) const;   // 0x006D6510
};
class CTCBase : public CTCBaseLayout { };
class CTCInventoryStats : public CTCBase {
public:
    std::vector<NUISystem::CComponent*> CreateExperienceSliders(NUISystem::CComponent* parent, long spacing, bool current);
};
std::vector<NUISystem::CComponent*> CTCInventoryStats::CreateExperienceSliders(NUISystem::CComponent* parent, long spacing, bool current)
{
    NGameText::CDataBank* bank = CMainGameComponent::Get()->PTextBank.Data;
    std::vector<NUISystem::CComponent*> sliders;
    CTCInventoryExperience* experience;
    ParentThing->GetInterface(&experience, 0x16);
    NUISystem::CManager* frontEnd = NUISystem::CManager::GetInstance();
    CTCHeroExperience* hero = ParentThing->GetTC<CTCHeroExperience>(0x68);
    CCountedPointer<NUISystem::CComponent> slider;
    long level;
    long value;
    value = hero->ExperiencePointsAvailableToSpend;
    level = current ? value : experience->GetOriginalExperience(0);
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_GENERAL"), C2DCoordI(0, 0), value,
                                             level, CRGBColour(0, 0xb4, 0, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    value = hero->GetStatExperience(HERO_EXPERIENCE_STAT_STRENGTH);
    level = current ? value : experience->GetOriginalExperience(1);
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_STRENGTH"), C2DCoordI(0, spacing + 0x19), value,
                                             level, CRGBColour(0xb4, 0, 0, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    value = hero->GetStatExperience(HERO_EXPERIENCE_STAT_SKILL);
    level = current ? value : experience->GetOriginalExperience(2);
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_SKILL"), C2DCoordI(0, (spacing + 0x19) * 2), value,
                                             level, CRGBColour(0xb4, 0xb4, 0, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    value = hero->GetStatExperience(HERO_EXPERIENCE_STAT_WILL);
    level = current ? value : experience->GetOriginalExperience(3);
    slider.Reset(frontEnd->CreateLevelSlider(bank->GetTextBySymbol("TEXT_GUI_STATS_EXPERIENCE_WILL"), C2DCoordI(0, (spacing + 0x19) * 3), value,
                                             level, CRGBColour(0, 0, 0xb4, 0xff)));
    parent->AddChild(slider);
    sliders.push_back(slider.Get());
    return sliders;
}