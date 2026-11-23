# hb302c.py: NF308 batch 3 in-place edits (zero line delta), orig/<addr>.cpp -> rows/<addr>.cpp; each old text must match exactly once.
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF308'
R={
'005bd4ea':[("    long GetHighlightedItem();                   // 0x005BCAE3",
 "protected: long GetHighlightedItem() const; public:   // 0x005BCAE3 (both PDBs ?GetHighlightedItem@CTCInventoryBase@NInventory@@IBEJXZ: protected, const; NF308)")],
'005bda1d':[("    CWideString GetInventoryText(long textId);          // 0x005BD273",
 "    CWideString GetInventoryText(long textId) const;    // 0x005BD273 (both PDBs QBE: const; NF308)")],
}
for a,p in R.items():
    t=open(A+'/orig/%s.cpp'%a).read(); n0=t.count('\n')
    for o,nw in p:
        c=t.count(o)
        if c!=1: raise SystemExit('%s: matched %d'%(a,c))
        t=t.replace(o,nw)
    open(A+'/rows/%s.cpp'%a,'w').write(t); print(a,n0,'->',t.count('\n'))
