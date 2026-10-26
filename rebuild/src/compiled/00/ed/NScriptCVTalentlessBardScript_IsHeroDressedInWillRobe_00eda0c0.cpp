// NScript::CV_TalentlessBardScript::IsHeroDressedInWillRobe() -- true when the hero wears a full evil, plain or good Will-user robe suit (shirt, gloves, trousers and boots of one set), each set checked with short-circuit
// Leaf: IsHeroDressedInWillRobe
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDA0C0, length 971 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued as CDF_ThreeSuits::DressedAsEvilMage (propagated across the byte-identical twin
//   0x00ED9CF0 (IsHeroDressedInArmour) and the CDF_ThreeSuits family); the body reads the bard script's Game interface at +0x40 and is called
//   from CV_TalentlessBardScript::ProcessSongsClothes (0x00ED9BB0, call at 0x00ED9C27); ghidra_out chunk 238 already
//   names it IsHeroDressedInWillRobe.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x12), CGameScriptInterface slot 70 (+0x118) GetHero,
//          slot 187 (+0x2ec) IsWearingClothingItem(const CScriptThing&, const CCharString&), CCharString::~CCharString
//          @ 0x0099EAE0 (x12, each under its construction flag).
// Layout (retail): Game +0x40 (landed CV_TalentlessBardScript::ProcessSongsAge 0x00EDA8C0).
// Shape: three named bools, each one `&&` chain of four implicit-literal CCharString temporaries (the landed
//   CDF_ThreeSuits::DressedAsBandit 0x00E72F00 shape: construction flags accumulate in ebx across the three chains,
//   each chain's temporaries destroyed in reverse right after it); the result is `evil || plain || good`.
class CCharString { public:
    char* m_pString;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing;
class CGameScriptInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual const CScriptThing& GetHero();                                                  // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82();
    virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94();
    virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186();
    virtual bool IsWearingClothingItem(const CScriptThing& thing, const CCharString& item);   // slot 187 (+0x2ec)
};

namespace NScript {
class CV_TalentlessBardScript { public:
    unsigned char          _base[0x40];
    CGameScriptInterface*  Game;                         // +0x40
    bool IsHeroDressedInWillRobe();
};

bool CV_TalentlessBardScript::IsHeroDressedInWillRobe()
{
    bool evil = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_WILLROBE_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_WILLROBE_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_WILLROBE_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_WILLROBE_EVIL");
    bool plain = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_WILLROBE")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_WILLROBE")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_WILLROBE")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_WILLROBE");
    bool good = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_WILLROBE_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_WILLROBE_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_WILLROBE_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_WILLROBE_GOOD");
    return evil || plain || good;
}
}