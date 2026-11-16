PAIRS = [
('''#ifdef FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
    // Retail opt-in (Codex pass22,row0090E5D0): both exact original PUBLICs and method types.
    EAIActionReturn AIAction_InteractWith(EAIActionRepeat,CIntelligentPointer<CThingAICreature>,CCharString,int,bool);
#endif
''',
'''#ifdef FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
    // Retail opt-in (Codex pass22,row0090E5D0): both exact original PUBLICs and method types.
    // Also used by wfD4 (row 0x0090D030 CAIStateGroup_MinionGossip::Update, landing gn): Ego_r.pdb 0x0084C7D0
    // ?AIAction_InteractWith@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingAICreature@@@@VCCharString@@H_N@Z
    // (protected; retail 0x00981410, called at 0x0090D6F2). Declaration only.
    EAIActionReturn AIAction_InteractWith(EAIActionRepeat,CIntelligentPointer<CThingAICreature>,CCharString,int,bool);
#endif
'''),
('''#if defined(FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH) && defined(FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS)
    // Overlay opt-in (wfD4, row 0x0090D030 CAIStateGroup_MinionGossip::Update): Ego_r.pdb 0x0084C7D0
    // ?AIAction_InteractWith@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingAICreature@@@@VCCharString@@H_N@Z
    // (protected; retail 0x00981410, called at 0x0090D6F2). Declaration only.
    EAIActionReturn AIAction_InteractWith(EAIActionRepeat repeat, CIntelligentPointer<CThingAICreature> creature, CCharString interaction,
        int frames, bool initiator);                                     // 0x00981410
#endif
''', ''),
]
