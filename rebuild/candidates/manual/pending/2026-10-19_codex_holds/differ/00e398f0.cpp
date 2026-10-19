// Grade: HELD(bytes match retail) -- test checks an unrelated local struct; header comment describes another function; names CBaseObject ctor 0x99a380 as CBaseIntelligentPointer and vtable 0x127094c (CScriptGameResourceObjectScriptedThingBase) as CIntelligentThing | Codex source 00/e3/CQHangingTreeGoodScript_CQHangingTreeGoodScript_00e398f0.cpp, flags , pass pattern 00e398f0_TEST PASS
// CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* data, CGameScriptInterface* game) -- construct its script handles and map storage
// Retail: Fable.exe @ 0x00E398F0, 224-byte catalog span, ret 8. One CBase-backed script thing is followed by four 16-byte intelligent-pointer things and an empty map.
// Callees: CScriptBase @ 0x00CB8110, CBase @ 0x0099A2D0, CBaseIntelligentPointer @ 0x0099A380, malloc @ 0x00BFEA0E.
// CQ_WizardBattleScript::CQ_WizardBattleScript(void* data, CGameScriptInterface* game) -- construct the script base, one script thing, three intelligent pointers and the quest map
// Retail: Fable.exe @ 0x00EDEBA0, 176-byte catalog span, ret 8. The three 16-byte members use CBaseIntelligentPointer's constructor; the first 12-byte member uses CBase.
// Callees: CScriptBase @ 0x00CB8110, CBase @ 0x0099A2D0, CBaseIntelligentPointer @ 0x0099A380, malloc @ 0x00BFEA0E.
struct CGameScriptInterface {
    virtual long v00();
    virtual long v01();
    virtual long v02();
    virtual long v03();
    virtual long v04();
    virtual long v05();
    virtual long v06();
    virtual long v07();
    virtual long v08();
    virtual long v09();
    virtual long v10();
    virtual long v11();
    virtual long v12();
    virtual long v13();
    virtual long v14();
    virtual long v15();
    virtual long v16();
    virtual long v17();
    virtual long v18();
    virtual long v19();
    virtual long v20();
    virtual long v21();
    virtual long v22();
    virtual long v23();
    virtual long v24();
    virtual long v25();
    virtual long v26();
    virtual long v27();
    virtual long v28();
    virtual long v29();
    virtual long v30();
    virtual long v31();
    virtual long v32();
    virtual long v33();
    virtual long v34();
    virtual long v35();
    virtual long v36();
    virtual long v37();
    virtual long v38();
    virtual long v39();
    virtual long v40();
    virtual long v41();
    virtual long v42();
    virtual long v43();
    virtual long v44();
    virtual long v45();
    virtual long v46();
    virtual long v47();
    virtual long v48();
    virtual long v49();
    virtual long v50();
    virtual long v51();
    virtual long v52();
    virtual long v53();
    virtual long v54();
    virtual long v55();
    virtual long v56();
    virtual long v57();
    virtual long v58();
    virtual long v59();
    virtual long v60();
    virtual long v61();
    virtual long v62();
    virtual long v63();
    virtual long v64();
    virtual long v65();
    virtual long v66();
    virtual long v67();
    virtual long v68();
    virtual long v69();
    virtual long v70();
    virtual long v71();
    virtual long v72();
    virtual long v73();
    virtual long v74();
    virtual long v75();
    virtual long v76();
    virtual long v77();
    virtual long v78();
    virtual long v79();
    virtual long v80();
    virtual long v81();
    virtual long v82();
    virtual long v83();
    virtual long v84();
    virtual long v85();
    virtual long v86();
    virtual long RegisterTimer();
};
extern CGameScriptInterface* g_pGameScriptInterface;
struct CScriptBase { virtual void v(); unsigned char body[0x3c]; CScriptBase(); // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase { void* Game; void* Data; CQuestScriptObject(void* data,CGameScriptInterface* game):CScriptBase(),Game(game),Data(data){} };
struct CBase { virtual void v(); CBase(); // 0x0099A2D0
};
struct CPointerWords { void* Data; void* Info; CPointerWords():Data(0),Info(0){} };
struct CScriptThing : CBase { CPointerWords P; CScriptThing():CBase(),P(){} virtual void w(); };
struct CBaseIntelligentPointer { virtual void v(); void* Item; CBaseIntelligentPointer(); // 0x0099A380
};
struct CIntelligentThing : CBaseIntelligentPointer { void* Data; void* Info; CIntelligentThing():CBaseIntelligentPointer(),Data(0),Info(0){} virtual void w(); };
extern "C" void* malloc(unsigned int); // 0x00BFEA0E
struct Rb { unsigned char color; void* parent; void* left; void* right; };
struct Map { Rb* h; unsigned long count; unsigned char pad[4]; Map():h(0){ h=(Rb*)malloc(0x18); count=0; h->color=0;h->parent=0;h->left=h;h->right=h;} };
struct CQ_HangingTreeGoodScript : CQuestScriptObject { unsigned char pad[0x14]; CScriptThing first; unsigned char gap[0x0c]; CIntelligentThing a,b,c,d; Map map; virtual void v(); CQ_HangingTreeGoodScript(void* data,CGameScriptInterface* game); };
CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* data,CGameScriptInterface* game):CQuestScriptObject(data,game),first(),a(),b(),c(),d(),map(){}