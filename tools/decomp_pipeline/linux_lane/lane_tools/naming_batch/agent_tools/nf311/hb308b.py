# hb308b.py: NF311 batch 3 exact edits (free-function declarations moved into their PDB namespaces; owner 0x009E9F20 self re-scope)
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311/rows/'
E={
'008cc8e0':[("void __fastcall SetLungedThisFrame();           // 0x009054F0",
             "namespace NBalverineCombat { void __fastcall SetLungedThisFrame(); } using NBalverineCombat::SetLungedThisFrame;   // 0x009054F0 (NF311: PDB scope)")],
'007fd580':[("C2DBoxI __fastcall GetLineThingSearchArea(const C3DVector& start, const C3DVector& end, float radius);   // 0x008F58C0",
             "namespace NPhysicsHelpers { C2DBoxI __fastcall GetLineThingSearchArea(const C3DVector& start, const C3DVector& end, float radius); } using NPhysicsHelpers::GetLineThingSearchArea;   // 0x008F58C0 (NF311: PDB scope)")],
'0064f6dc':[("C2DVector __fastcall GFAnchorToTopRight(const C2DVector&);",
             "namespace NPlayerGui{C2DVector __fastcall GFAnchorToTopRight(const C2DVector&);}using NPlayerGui::GFAnchorToTopRight;/*NF311: PDB scope*/")],
'0064499c':[("C2DVector __fastcall GFAnchorToTopRight(const C2DVector& in);         // 0x00643CA1",
             "namespace NPlayerGui { C2DVector __fastcall GFAnchorToTopRight(const C2DVector& in); } using NPlayerGui::GFAnchorToTopRight;   // 0x00643CA1 (NF311: PDB scope)")],
'0049e8c0':[("void __fastcall StopProgress(bool stopNow, bool keepScreen);   // 0x009E9F20",
             "namespace NProgressDisplay { void __fastcall StopProgress(bool stopNow, bool keepScreen); } using NProgressDisplay::StopProgress;   // 0x009E9F20 (NF311: PDB scope)")],
'009e9f20':[("void __fastcall StopProgress(bool a, bool b)\n{\n    Prog* p = g_prog;\n    if (p)\n        p->stop(a, b);\n}",
             "namespace NProgressDisplay {   // NF311: the PDB scope (Ego_r / FableWin ?StopProgress@NProgressDisplay@@YIX_N0@Z)\nvoid __fastcall StopProgress(bool a, bool b)\n{\n    Prog* p = g_prog;\n    if (p)\n        p->stop(a, b);\n}\n}")],
}
import sys
for a,ed in E.items():
    if len(sys.argv)>1 and a not in sys.argv[1:]: continue
    t=open(A+a+'.cpp').read()
    for o,n in ed:
        assert t.count(o)==1,(a,o); t=t.replace(o,n)
    open(A+a+'.cpp','w').write(t); print('edited',a)
