# hb308.py: exact one-occurrence line edits for NF308 batch 2 (rows/<addr>.cpp, copied from orig/)
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF308/rows/'
E={
'00967930':[("    void SetPushableByHero(bool pushable);                              protected: // 0x0097E330",
             "    protected: void SetPushableByHero(bool pushable);                   protected: // 0x0097E330 (NF308: IAE, both PDBs)")],
'00967b40':[("    void SetPushableByHero(bool pushable);      protected: // 0x0097E330",
             "    protected: void SetPushableByHero(bool pushable);      protected: // 0x0097E330 (NF308: IAE, both PDBs)")],
'00968bc0':[("    void SetPushableByHero(bool pushable);      // 0x0097E330",
             "    protected: void SetPushableByHero(bool pushable); public:      // 0x0097E330 (NF308: IAE, both PDBs)")],
'006eb390':[("    void PerformSequenceReset(CThingCreatureBase& creature);   // 0x006EA2C0",
             "    private: void PerformSequenceReset(CThingCreatureBase& creature); public:   // 0x006EA2C0 (NF308: AAE, both PDBs)")],
'006eb600':[("    void PerformSequenceReset(CThingCreatureBase&); //0x006EA2C0",
             "    private: void PerformSequenceReset(CThingCreatureBase&); public: //0x006EA2C0 (NF308: AAE, both PDBs)")],
}
import sys
for a,ed in E.items():
    if len(sys.argv)>1 and a not in sys.argv[1:]: continue
    t=open(A+a+'.cpp').read()
    for o,n in ed:
        assert t.count(o)==1,(a,o); t=t.replace(o,n)
    open(A+a+'.cpp','w').write(t); print('edited',a)
