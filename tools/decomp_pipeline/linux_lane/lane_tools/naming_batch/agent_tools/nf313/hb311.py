# hb311.py: the exact NF313 edits (one replace per file, each must match exactly once)
import sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313'
E=[
 ('rows/006d5050.cpp','// Leaf: ?RemoveEmoteIcon@\n','// Leaf: ?RemoveEmoteIcon@CTCFollowed@@IAEXAAVCThing@@@Z\n'),
 ('rows/006d5050.cpp','    void RemoveEmoteIcon(CThing& follower);\n};','protected: void RemoveEmoteIcon(CThing& follower);      // NF313: IAE (Ego_r.pdb + FableWin.pdb)\n};'),
 ('rows/006d5180.cpp','    void RemoveEmoteIcon(CThing& thing);            // 0x006D5050\n','    protected: void RemoveEmoteIcon(CThing& thing); public: // 0x006D5050 (NF313: IAE, both PDBs)\n'),
 ('evidence/held_18/0097e330.cpp','    void SetPushableByHero(bool pushable);\n};','protected: void SetPushableByHero(bool pushable);      // NF313: IAE (Ego_r.pdb + FableWin.pdb)\n};'),
 ('evidence/held_18/00967930.cpp','    void SetPushableByHero(bool pushable);                              protected: // 0x0097E330','    protected: void SetPushableByHero(bool pushable);                   protected: // 0x0097E330 (NF308: IAE, both PDBs)'),
 ('evidence/held_18/00967b40.cpp','    void SetPushableByHero(bool pushable);      protected: // 0x0097E330','    protected: void SetPushableByHero(bool pushable);      protected: // 0x0097E330 (NF308: IAE, both PDBs)'),
 ('evidence/held_18/00968bc0.cpp','    void SetPushableByHero(bool pushable);      // 0x0097E330','    protected: void SetPushableByHero(bool pushable); public:      // 0x0097E330 (NF308: IAE, both PDBs)'),
]
for f,o,n in (E if len(sys.argv)==1 else []):
    p=A+'/'+f; t=open(p,encoding='utf-8').read(); c=t.count(o)
    if c!=1: print('MISS',c,f,repr(o[:60])); continue
    open(p,'w',encoding='utf-8').write(t.replace(o,n)); print('OK',f)
# plus: evidence/held_18/0097e330.cpp gains line 2 `// Leaf: ?SetPushableByHero@CAIStateGroupBase@@IAEX_N@Z` (inserted by a one-off python call)
# --- batch 2 (entries 119 / 138: virtual on the local base, NF299 Q3 / NF303 Q1) ---
E2=[
 ('rows/00701990.cpp','// Leaf: TransferBinaryIn@CReactionMatch_Sphere\n','// Leaf: ?TransferBinaryIn@CReactionMatch_Sphere@@UAEXAAVCMemoryDataInputStream@@@Z\n'),
 ('rows/00701990.cpp','    void* __vftable;                                    // +0x00\n','    // +0x00: the vptr (NF313: TransferBinaryIn is virtual, Ego_r.pdb + FableWin.pdb UAE)\n'),
 ('rows/00701990.cpp','    void TransferBinaryIn(CMemoryDataInputStream& stream);  // 0x006FEFB0\n','    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);  // 0x006FEFB0\n'),
 ('rows/00701990.cpp','    float       SqRadius;                               // +0x14\n    void TransferBinaryIn(CMemoryDataInputStream& stream);\n','    float       SqRadius;                               // +0x14\n    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);\n'),
 ('rows/00835ac0.cpp','// Leaf: DrawPreparePrimitivesForRendering\n','// Leaf: ?DrawPreparePrimitivesForRendering@CThingDeadCreature@@MAE_NABVCEnginePrimitivePrepareInfo@@@Z\n'),
 ('rows/00835ac0.cpp','    unsigned char         _pad00[0x64];\n','    unsigned char         _pad04[0x60];             // +0x04 (NF313: the vptr is +0x00; the member below is virtual)\n'),
 ('rows/00835ac0.cpp','    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0\n','    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0 (UAE, both PDBs)\n'),
 ('rows/00835ac0.cpp','class CThingDeadCreature : public CThingPhysical { public:\n    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);\n','class CThingDeadCreature : public CThingPhysical { protected:   // NF313: MAE (Ego_r.pdb + FableWin.pdb)\n    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);\n'),
]
if __name__=='__main__' and len(sys.argv)>1 and sys.argv[1]=='b2':
    for f,o,n in E2:
        p=A+'/'+f; t=open(p,encoding='utf-8').read(); c=t.count(o)
        if c!=1: print('MISS',c,f,repr(o[:60])); continue
        open(p,'w',encoding='utf-8').write(t.replace(o,n)); print('OK',f)
# --- batch 3 (entry 85): rows/00709b10.cpp and rows/00708a30.cpp, local C3DAnimationInterpolator `_data/_body[0x14]` -> [0x10] + `virtual ~C3DAnimationInterpolator();` (applied by a one-off python call; see the diff in evidence/diff_rows.txt)
