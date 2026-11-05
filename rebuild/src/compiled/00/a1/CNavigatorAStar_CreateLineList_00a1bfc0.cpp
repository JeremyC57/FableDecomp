// Forwarder to a member of the sub-object pointer at this+0x20. VC7.1 tail-calls
// Leaf: ?CreateLineList@CNavigatorAStar@@QAEXXZ
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#pragma pack(push,1)
struct Sub { void Run_00a1b470(); };
struct CNavigatorAStar {
    char pad[0x20];
    Sub* sub;
    void CreateLineList();
};
#pragma pack(pop)
void CNavigatorAStar::CreateLineList() { this->sub->Run_00a1b470(); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CreateLineList@CNavigatorAStar@@UAEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z); evidence: RTTI vtable slot + PDB name set.
