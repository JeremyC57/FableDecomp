# r4fix.py: NF311 entry 22 -- add-only opt-in FABLE_RETAIL_CPLAYERMANAGER_FLAT_CREATEPLAYERS in hdr/engine/CPlayerManager.h's flat branch
# (declaration only) and binder 0x004166A8 onto it (its CPlayerManager_Methods stand-in member dropped; line count kept).
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311/'
h=A+'hdr/engine/CPlayerManager.h'; t=open(h,encoding='utf-8').read()
o='''    void UnfreezePlayerControls(long number);                        // 0x00449A50
#endif
};
#pragma pack(pop)
'''
assert t.count(o)==1
t=t.replace(o,'''    void UnfreezePlayerControls(long number);                        // 0x00449A50
#endif
#ifdef FABLE_RETAIL_CPLAYERMANAGER_FLAT_CREATEPLAYERS
    // Add-only opt-in (naming agent NF311, 2026-11-23, binder 0x004166A8 CMainGameComponent::CreatePlayers): Ego_r.pdb / FableWin.pdb
    // ?CreatePlayers@CPlayerManager@@QAEXXZ (out of line at 0x0044A530, the landed self row). Declaration only: no layout change.
    void CreatePlayers();                                            // 0x0044A530
#endif
};
#pragma pack(pop)
''')
open(h,'w',encoding='utf-8').write(t)
f=A+'rows/004166a8.cpp'; t=open(f,encoding='utf-8').read(); n0=t.count('\\n')
o='// Opt: size TU (unaligned entry).\n#pragma optimize("s", on)\n'
assert t.count(o)==1
t=t.replace(o,'#pragma optimize("s", on)   // Opt: size TU (unaligned entry).\n')
o='#define FABLE_RETAIL_CNETWORKCLIENT_UNINITIALISE\n'
assert t.count(o)==1
t=t.replace(o,o+'#define FABLE_RETAIL_CPLAYERMANAGER_FLAT_CREATEPLAYERS   // NF311: CreatePlayers 0x0044A530 from CPlayerManager.h (Ego_r + FableWin QAE)\n')
o='    void CreatePlayers();                                // 0x0044A530\n'
assert t.count(o)==1
t=t.replace(o,'    // CreatePlayers 0x0044A530: the CPlayerManager.h member (NF311; it was declared on this stand-in)\n')
assert t.count('\\n')==n0,(t.count('\\n'),n0); open(f,'w',encoding='utf-8').write(t); print('ok')
