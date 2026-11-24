# fix301.py: NF305 in-place edits (zero line delta), orig/<addr>.cpp -> rows/<addr>.cpp; each old text must match exactly once.
import sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF305'
E={
'006778a0':[
 ("""    virtual void TransferBinaryIn(CDataInputStream* stream);            // +0x0C
};
CReactionFrequencyTraits* __fastcall CreateFromType(EReactionFrequencyType type);   // 0x006FF930
""","""    virtual void TransferBinaryIn(CDataInputStream* stream);            // +0x0C
    static CReactionFrequencyTraits* __fastcall CreateFromType(EReactionFrequencyType type);   // 0x006FF930 (both PDBs: static member)
};
"""),
 ("traits = CreateFromType((EReactionFrequencyType)type);","traits = CReactionFrequencyTraits::CreateFromType((EReactionFrequencyType)type);")],
'006b84b0':[
 ("""struct CScreenEffectPriorityStack {
    unsigned char _body[0x10];
    CScreenEffectPriorityStack();                   // 0x00881370
};""","""namespace NScreenEffect { struct CScreenEffectPriorityStack {   // both PDBs: NScreenEffect::CScreenEffectPriorityStack
    unsigned char _body[0x10];
    CScreenEffectPriorityStack();                   // 0x00881370
}; }"""),
 ("    CScreenEffectPriorityStack m_priorityStack;     // +0x90","    NScreenEffect::CScreenEffectPriorityStack m_priorityStack;     // +0x90")],
'00dc21f0':[
 ("""struct CQ_BanditRaidScript {
    unsigned char _pad[0x4a];""","""namespace NScript { class CQ_OakValeBanditRaidScript { public:   // the PDB owner script (NF305: was the stand-in struct CQ_BanditRaidScript)
    unsigned char _pad[0x4a];"""),
 ("""    bool PopulateLevel();                               // 0x00DC0300
};
namespace NScript {
class CQ_OakValeBanditRaidScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CM_RestartMarker;
};
}
""","""    bool PopulateLevel();                               // 0x00DC0300 (both PDBs: ?PopulateLevel@CQ_OakValeBanditRaidScript@NScript@@QAE_NXZ)
    class CM_RestartMarker;
}; }
// The members above are the parent fields Main reads (Flag4a, RestartRequested, Flag60) and the
// parent call it makes (PopulateLevel); the nested entity class follows.
//
//
//
"""),
 ("    CQ_BanditRaidScript* ParentClass;                   // +0x14","    CQ_OakValeBanditRaidScript* ParentClass;            // +0x14")],
'00717c50':[
 ("""    CSubtitledMessage(const CSubtitledMessage& other);
""","""    CSubtitledMessage(const CSubtitledMessage& other); static CSubtitledMessage __fastcall GetMessageFromTextIndex(unsigned long textIndex, const char* groupName);   // 0x004CEA60 (both PDBs: static member)
"""),
 ("""CSubtitledMessage __fastcall GetMessageFromTextIndex(unsigned long textIndex, const char* groupName);   // 0x004CEA60
""","""// CSubtitledMessage::GetMessageFromTextIndex @ 0x004CEA60 is declared in the class above (NF305: was a free-function stand-in).
"""),
 ("messages.push_back(GetMessageFromTextIndex(textIndex, groupName));","messages.push_back(CSubtitledMessage::GetMessageFromTextIndex(textIndex, groupName));")],
'0092bcb0':[
 ("struct CTCAvoidRegionExitView {","struct CTCAvoidRegionExit {   // NF305: was the view stand-in CTCAvoidRegionExitView (both PDBs: CTCAvoidRegionExit::GetPosition)"),
 ("CTCAvoidRegionExitView* avoid=(CTCAvoidRegionExitView*)creature->GetTC(0x105);","CTCAvoidRegionExit* avoid=(CTCAvoidRegionExit*)creature->GetTC(0x105);")],
'00b81bc0':[
 ('''struct CPerlinNoise {
    unsigned long IndexTable[256];                 // +0x000
    C3DVector     VectorTable[256];                // +0x400
    CPerlinNoise(unsigned long seed);              // 0x00F36BE0
};''','''namespace NFractal { struct CPerlinNoise {     // both PDBs: NFractal::CPerlinNoise
    unsigned long IndexTable[256];                 // +0x000
    C3DVector     VectorTable[256];                // +0x400
    CPerlinNoise(unsigned long seed);              // 0x00F36BE0
}; }'''),
 ('    CPerlinNoise  PerlinNoise;                     // +0x28','    NFractal::CPerlinNoise PerlinNoise;            // +0x28')],
}
for a in (sys.argv[1:] or E):
    t=open(A+'/orig/%s.cpp'%a,encoding='utf-8').read(); n0=t.count('\n')
    for o,nw in E[a]:
        c=t.count(o)
        if c!=1: print('FAIL',a,c,o[:60]); sys.exit(1)
        t=t.replace(o,nw)
    open(A+'/rows/%s.cpp'%a,'w',encoding='utf-8').write(t)
    print(a,'lines',n0,'->',t.count('\n'))
