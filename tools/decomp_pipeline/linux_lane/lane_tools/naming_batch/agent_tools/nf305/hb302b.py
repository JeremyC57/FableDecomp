# hb302b.py: NF305 batch 2. orig/engine/CTCInventoryItem.h -> hdr/engine/ (add-only flat opt-ins) and orig/<addr>.cpp -> rows/<addr>.cpp.
# Each old text must match exactly once; prints the line delta per file.
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF305'
H={'CTCInventoryItem':[("""    unsigned char     _pad_0x21[0x7];                        // +0x21
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(208,""","""    unsigned char     _pad_0x21[0x7];                        // +0x21
#ifdef FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETENGINEGRAPHIC
    // Add-only opt-in (naming agent NF305, 2026-11-23, binders 0x00581CFD / 0x00581FA4): Ego_r.pdb / FableWin.pdb
    // ?GetEngineGraphic@CTCInventoryItem@@SI?AVCEngineGraphic@@J@Z (static __fastcall, by value; retail 0x005D96A0), as the retail
    // branch declares it (FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC). Declaration only: no layout change.
    static class CEngineGraphic __fastcall GetEngineGraphic(long defIndex);               // 0x005D96A0
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETSTARTINGRIGHTHANDEDSET
    // Add-only opt-in (naming agent NF305, 2026-11-23, binder 0x005BCD2D): Ego_r.pdb / FableWin.pdb
    // ?GetStartingRightHandedSet@CTCInventoryItem@@SI?AVCRightHandedSet@@J@Z (static __fastcall, by value; retail 0x005D9730), as the
    // retail branch declares it. Declaration only: no layout change.
    static class CRightHandedSet __fastcall GetStartingRightHandedSet(long defIndex);     // 0x005D9730
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETCONTEXTSENSITIVETYPE
    // Add-only opt-in (naming agent NF305, 2026-11-23, binder 0x00581D8E): Ego_r.pdb / FableWin.pdb
    // ?GetContextSensitiveType@CTCInventoryItem@@SI?AW4EContextSensitiveType@@J@Z (static __fastcall; retail 0x005D9B90). The includer
    // defines enum EContextSensitiveType before it includes this header. Declaration only: no layout change.
    static EContextSensitiveType __fastcall GetContextSensitiveType(long defIndex);       // 0x005D9B90
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(208,""")]}
EG='''CEngineGraphic __fastcall GetEngineGraphic(long itemType);   // 0x005D96A0
'''
EGN='''#define FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETENGINEGRAPHIC   // NF305: ?GetEngineGraphic@CTCInventoryItem@@SI?AVCEngineGraphic@@J@Z (both PDBs), was a free stand-in
#include "engine/CTCInventoryItem.h"
'''
R={
'00581cfd':[(EG,EGN),("return GetEngineGraphic(ContextSensitiveItems[index]).BankIndex;","return CTCInventoryItem::GetEngineGraphic(ContextSensitiveItems[index]).BankIndex;")],
'00581fa4':[(EG,EGN),("return GetEngineGraphic(QuickAccessItems[index]).BankIndex;","return CTCInventoryItem::GetEngineGraphic(QuickAccessItems[index]).BankIndex;")],
'005bcd2d':[("""CRightHandedSet __fastcall GetStartingRightHandedSet(long defIndex);   // 0x005D9730
""","""#define FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETSTARTINGRIGHTHANDEDSET   // NF305: ?GetStartingRightHandedSet@CTCInventoryItem@@SI?AVCRightHandedSet@@J@Z (both PDBs), was a free stand-in
#include "engine/CTCInventoryItem.h"
"""),("ResetRHSets(GetStartingRightHandedSet(defIndex));","ResetRHSets(CTCInventoryItem::GetStartingRightHandedSet(defIndex));")],
'00581d8e':[("""EContextSensitiveType __fastcall GetContextSensitiveType(long itemType);   // 0x005D9B90
""","""#define FABLE_RETAIL_CTCINVENTORYITEM_FLAT_GETCONTEXTSENSITIVETYPE   // NF305: ?GetContextSensitiveType@CTCInventoryItem@@SI?AW4EContextSensitiveType@@J@Z (both PDBs), was a free stand-in
#include "engine/CTCInventoryItem.h"
"""),("GetContextSensitiveType(ContextSensitiveItems[i]) == CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION","CTCInventoryItem::GetContextSensitiveType(ContextSensitiveItems[i]) == CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION")],
'006412b0':[("""struct CTCGraphicAppearance {
    void RemovePrimitive(CPrimitive* primitive);        // 0x004BF920
};""","""namespace NGraphicAppearance { class CTCGraphicAppearance { public:   // NF305: the PDB class (both PDBs ?RemovePrimitive@CTCGraphicAppearance@NGraphicAppearance@@); was a global stand-in struct
    void RemovePrimitive(CPrimitive* primitive);        // 0x004BF920
}; }"""),("    CTCGraphicAppearance* PGraphicAppearance;           // +0x64","    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;           // +0x64")],
'00ed99d0':[("""struct CV_TalentlessBardScript {
    unsigned char     _pad00[0x68];""","""namespace NScript { class CV_TalentlessBardScript { public:   // NF305: the PDB owner script (both PDBs ?ProcessSongs*@CV_TalentlessBardScript@NScript@@QAEXXZ); was a global stand-in struct
    unsigned char     _pad00[0x68];"""),("""    void ProcessSongsQuestComplete();                      // 0x00ED9B40
};""","""    void ProcessSongsQuestComplete();                      // 0x00ED9B40
}; }"""),("        CV_TalentlessBardScript* ParentClass;                  // +0x14","        NScript::CV_TalentlessBardScript* ParentClass;                  // +0x14")],
}
def apply(src,pairs,name):
    t=open(src).read(); n0=t.count('\n')
    for o,nw in pairs:
        c=t.count(o)
        if c!=1: raise SystemExit('%s: old text matched %d times: %r'%(name,c,o[:70]))
        t=t.replace(o,nw)
    return t,n0
for h,p in H.items():
    t,n0=apply(A+'/orig/engine/%s.h'%h,p,h); open(A+'/hdr/engine/%s.h'%h,'w').write(t); print(h,'lines',n0,'->',t.count('\n'))
for a,p in R.items():
    t,n0=apply(A+'/orig/%s.cpp'%a,p,a); open(A+'/rows/%s.cpp'%a,'w').write(t); print(a,'lines',n0,'->',t.count('\n'))
