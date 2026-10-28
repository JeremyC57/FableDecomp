// CTCInventoryStats::CreateTitle(const CWideString& text, NUISystem::CComponent* parent, bool statTitle) -- creates a personality-stats title text component (the stat title, or the viewport text) and adds it under the parent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CCF0E, length 120 bytes (`ret 0Ch`; `this` unused). Frame-pointer size TU:
// optimize("y",off) + ("s",on).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCharString::operator=(const char*) @ 0x0099EFE0,
// NUISystem::CFrontEndManager::GetInstance() @ 0x0041E5F2 (__fastcall, no arguments), NUISystem::CManager::CreateComponent(
// const CCharString&, bool) @ 0x0041DB1D, CCountedPointer<NUISystem::CComponent>(CComponent*) @ 0x00429C15 / its dtor @ 0x004291DE,
// component virtual slot 144 (+0x240, SetText(const CWideString&, bool, bool)), parent virtual slot 59 (+0xEC,
// AddChild(const CCountedPointer<CComponent>&)), ~CCharString @ 0x0099EAE0.
// Strings: "UI_STATS_PERSONALITY_STAT_TITLE" @ 0x01254A38, "UI_STATS_PERSONALITY_VIEWPORT_TEXT2" @ 0x01254A14.
// Shape: the definition name is a CCharString local reassigned when `statTitle` is false; the created component goes straight
// into a counted-pointer local whose inline operator-> reads the object word ([ebp-0Ch]).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString { public:
    void* m_pStringData;
};
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
    const CCharString& operator=(const char* text);                     // 0x0099EFE0
};
template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                                   // 0x00429C15
    ~CCountedPointer();                                           // 0x004291DE
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
class CComponent { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
    virtual void v94();
    virtual void v95();
    virtual void v96();
    virtual void v97();
    virtual void v98();
    virtual void v99();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);     // slot 144 (+0x240)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();            // 0x0041E5F2
};
}

class CTCInventoryStats { public:
    void CreateTitle(const CWideString& text, NUISystem::CComponent* parent, bool statTitle);
};

void CTCInventoryStats::CreateTitle(const CWideString& text, NUISystem::CComponent* parent, bool statTitle)
{
    CCharString definition("UI_STATS_PERSONALITY_STAT_TITLE");
    if (!statTitle)
        definition = "UI_STATS_PERSONALITY_VIEWPORT_TEXT2";
    CCountedPointer<NUISystem::CComponent> title(NUISystem::CFrontEndManager::GetInstance()->CreateComponent(definition, false));
    title->SetText(text, true, true);
    parent->AddChild(title);
}