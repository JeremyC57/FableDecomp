# r1fix_nf291.py: NF294 entry 1 -- add-only opt-in FABLE_RETAIL_CTCQUESTCARD_FLAT_ISVIGNETTE in hdr/engine/CTCQuestCard.h's flat branch
# (declaration only) and binder 0x00896A30 onto it (its QuestCardMethods stand-in member dropped; line count kept).
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF294/'
h=A+'hdr/engine/CTCQuestCard.h'; t=open(h,encoding='utf-8').read()
o='''    long             RenownReward;                           // +0x30
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(137, sizeof(CTCQuestCard) == 0x34);'''
assert t.count(o)==1
t=t.replace(o,'''    long             RenownReward;                           // +0x30
#ifdef FABLE_RETAIL_CTCQUESTCARD_FLAT_ISVIGNETTE
    // Add-only opt-in (naming agent NF294, 2026-11-23, binder 0x00896A30 CGameScriptInterface::SetQuestCardObjective): Ego_r.pdb /
    // FableWin.pdb ?IsVignette@CTCQuestCard@@QBE_NXZ (retail 0x007022E0, called at 0x00896AC4). Declaration only: no layout change.
    bool IsVignette() const;                                             // 0x007022E0
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(137, sizeof(CTCQuestCard) == 0x34);''')
open(h,'w',encoding='utf-8').write(t)
f=A+'rows/00896a30.cpp'; t=open(f,encoding='utf-8').read(); n0=t.count('\n')
o='// Retail00896A30,294 bytes.\n'
assert t.count(o)==1
t=t.replace(o,'#define FABLE_RETAIL_CTCQUESTCARD_FLAT_ISVIGNETTE   // NF294: IsVignette 0x007022E0 from CTCQuestCard.h (Ego_r + FableWin QBE). Retail00896A30,294 bytes.\n')
o='struct QuestCardMethods:CTCQuestCard{bool IsVignette()const;}; //007022E0 reads native Def+59; donor+5D differs due vector stride\n'
assert t.count(o)==1
t=t.replace(o,'struct QuestCardMethods:CTCQuestCard{}; //007022E0 IsVignette: the CTCQuestCard.h member (NF294; was declared on this stand-in); reads native Def+59, donor+5D differs due vector stride\n')
assert t.count('\n')==n0,(t.count('\n'),n0); open(f,'w',encoding='utf-8').write(t); print('ok')
