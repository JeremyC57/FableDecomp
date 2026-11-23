# hb302.py: NF308 header batch. orig/engine/<H>.h -> hdr/engine/<H>.h (add-only opt-ins before the class's closing `};`, unpinned
# asserts pinned to their HEAD line) and orig/<addr>.cpp -> rows/<addr>.cpp (zero line delta). Each old text must match exactly once.
import re,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF308'
H={
'CPixelFormat':[("""class CPixelFormat {
public:
    long FormatIndex;                                        // +0x00
};""","""class CPixelFormat {
public:
    long FormatIndex;                                        // +0x00
#ifdef FABLE_RETAIL_CPIXELFORMAT_FLAT_GETINIT
    // Add-only opt-in (naming agent NF308, 2026-11-23, binder 0x00993F60 CDataOutputStream::WritePixelFormat): Ego_r.pdb / FableWin.pdb
    // ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ (const, CPixelFormatInit by value; retail 0x009E3870, landed). Declaration only.
    class CPixelFormatInit GetInit() const;                  // 0x009E3870
#endif
};""")],
'CPlayer':[("""    long                                                         JoystickDeviceNumber; // +0x228
};""","""    long                                                         JoystickDeviceNumber; // +0x228
#ifdef FABLE_RETAIL_CPLAYER_PREPAREFORLOAD
    // Add-only opt-in (naming agent NF308, 2026-11-23, binder 0x00449C30 CPlayerManager::PrepareForLoad): Ego_r.pdb / FableWin.pdb
    // ?PrepareForLoad@CPlayer@@QAEXXZ (public; retail 0x00487550, landed; also called by 0x004A0220). Declaration only: no layout change.
public:
    void PrepareForLoad();                                                // 0x00487550
#endif
};""")],
'CResource':[("""    unsigned long  LastUsedFrame;                            // +0x18
};""","""    unsigned long  LastUsedFrame;                            // +0x18
#ifdef FABLE_RETAIL_CRESOURCE_FLAT_SETRESOURCESIZE
    // Add-only opt-in (naming agent NF308, 2026-11-23, binder 0x009C8470 NGameText::CGroupEntry::ReadFromDataStream): Ego_r.pdb /
    // FableWin.pdb ?SetResourceSize@CResource@@QAEXK@Z (retail 0x009FBF20, landed), as the DEF_CLASS_BASE branch declares it. Declaration only.
    void SetResourceSize(unsigned long);                     // 0x009FBF20
#endif
};""")],
'CResourceList':[("""    unsigned long UnloadedThisFrame;                         // +0x38
};""","""    unsigned long UnloadedThisFrame;                         // +0x38
#ifdef FABLE_RETAIL_CRESOURCELIST_FLAT_UNLOADNEXTRESOURCE
    // Add-only opt-in (naming agent NF308, 2026-11-23, binder 0x009FC210 CResourceList::ShrinkResources): Ego_r.pdb / FableWin.pdb
    // ?UnloadNextResource@CResourceList@@QAE_NK@Z (retail 0x009FC170, landed). Declaration only: no layout change.
    bool UnloadNextResource(unsigned long frames);           // 0x009FC170
#endif
};""")],
'CTCBuyableHouse':[("""    void SetOwnedByPlayer(bool owned);                                   // 0x006C17B0
#endif
};""","""    void SetOwnedByPlayer(bool owned);                                   // 0x006C17B0
#endif
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_GETPRICE
    // Add-only opt-in (naming agent NF308, 2026-11-23, binder 0x0072E940 CTCActionUseBuyHouse::OnActionUseMain): Ego_r.pdb / FableWin.pdb
    // ?GetPrice@CTCBuyableHouse@@QBEJXZ (retail 0x006C0120), as the retail branch declares it. Declaration only: no layout change.
    long GetPrice() const;                                               // 0x006C0120
#endif
};""")],
}
R={
'00993f60':[("""// through the stream's virtual slot 4 (WriteBytes(const void*, long)).
#include "engine/CPixelFormat.h"
""","""// through the stream's virtual slot 4 (WriteBytes(const void*, long)).
#define FABLE_RETAIL_CPIXELFORMAT_FLAT_GETINIT   // NF308: ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ (both PDBs) from the header
#include "engine/CPixelFormat.h"
"""),("""struct CPixelFormat_Calls : CPixelFormat {
    CPixelFormatInit GetInit();     // 0x009E3870
};""","""struct CPixelFormat_Calls : CPixelFormat {   // GetInit is CPixelFormat::GetInit() const 0x009E3870 (header opt-in above; NF308)
};""")],
'00449c30':[("""// (`sub ; sar 2`) after every call, unsigned `jb`.
#include "engine/CPlayer.h"

struct CPlayer_Calls : CPlayer {
    void PrepareForLoad();                                  // 0x00487550
};""","""// (`sub ; sar 2`) after every call, unsigned `jb`.
#define FABLE_RETAIL_CPLAYER_PREPAREFORLOAD   // NF308: ?PrepareForLoad@CPlayer@@QAEXXZ (both PDBs) from the header
#include "engine/CPlayer.h"

struct CPlayer_Calls : CPlayer {   // PrepareForLoad is CPlayer::PrepareForLoad 0x00487550 (header opt-in above; NF308)
};""")],
'009c8470':[("""//   resize is kept out of line (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE) and binds ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at
//   0x004428F0 (was LongArray::Resize).
#include "fable_compiler.h"
""","""//   resize is kept out of line (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE) and binds ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at 0x004428F0 (was LongArray::Resize).
#define FABLE_RETAIL_CRESOURCE_FLAT_SETRESOURCESIZE   // NF308: ?SetResourceSize@CResource@@QAEXK@Z (both PDBs) from the header
#include "fable_compiler.h"
"""),("""struct ResourceMethods:CResource {ResourceMethods();void SetResourceSize(unsigned long);}; //009FBF20/PDB00B6BDD0, true nonconst void(ulong)""",
"""struct ResourceMethods:CResource {ResourceMethods();}; //SetResourceSize is CResource::SetResourceSize 009FBF20/PDB00B6BDD0, true nonconst void(ulong) (header opt-in above; NF308)""")],
'009fc210':[("""#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    bool UnloadNextResource(unsigned long frames);  // 0x009FC170
    void ShrinkResources(unsigned long size);""","""#define FABLE_RETAIL_CRESOURCELIST_FLAT_UNLOADNEXTRESOURCE   // NF308: ?UnloadNextResource@CResourceList@@QAE_NK@Z (both PDBs) from the header
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {   // UnloadNextResource is CResourceList::UnloadNextResource 0x009FC170 (header opt-in above; NF308)
    void ShrinkResources(unsigned long size);""")],
'0072e940':[("""#include <stddef.h>
#include "engine/CTCBuyableHouse.h"
""","""#include <stddef.h>
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_GETPRICE   // NF308: ?GetPrice@CTCBuyableHouse@@QBEJXZ (both PDBs) from the header
#include "engine/CTCBuyableHouse.h"
"""),("""struct CTCBuyableHouseCalls : CTCBuyableHouse {
    long GetPrice();                                         // 0x006C0120
};""","""// GetPrice is CTCBuyableHouse::GetPrice() const 0x006C0120 (header opt-in above; NF308: was the stand-in
// struct CTCBuyableHouseCalls, reached through casts of `house`)."""),
("long price = ((CTCBuyableHouseCalls*)house)->GetPrice();","long price = house->GetPrice();"),
("NGameText::ConstructString(text, ((CTCBuyableHouseCalls*)house)->GetPrice());","NGameText::ConstructString(text, house->GetPrice());")],
}
def apply(src,pairs,name):
    t=open(src).read(); n0=t.count('\n')
    for o,nw in pairs:
        c=t.count(o)
        if c!=1: raise SystemExit('%s: old text matched %d times: %r'%(name,c,o[:60]))
        t=t.replace(o,nw)
    return t,n0
os.makedirs(A+'/hdr/engine',exist_ok=True); os.makedirs(A+'/rows',exist_ok=True)
for h,p in H.items():
    # pin every unpinned FABLE_STATIC_ASSERT( to its HEAD line number first (by line index: identical texts occur twice), then insert
    old=open(A+'/orig/engine/%s.h'%h).read().split('\n')
    pinned=[re.sub(r'^(\s*)FABLE_STATIC_ASSERT\(',lambda m:'%sFABLE_STATIC_ASSERT_AT(%d, '%(m.group(1),i),l,count=1) for i,l in enumerate(old,1)]
    tmp=A+'/work/pin_%s.h'%h; open(tmp,'w').write('\n'.join(pinned))
    t,n0=apply(tmp,p,h)
    open(A+'/hdr/engine/%s.h'%h,'w').write(t); print(h,'lines',n0,'->',t.count('\n'),'pinned',sum(a!=b for a,b in zip(old,pinned)))
for a,p in R.items():
    t,n0=apply(A+'/orig/%s.cpp'%a,p,a); n1=t.count('\n')
    open(A+'/rows/%s.cpp'%a,'w').write(t); print(a,'lines',n0,'->',n1,'OK' if n0==n1 else 'DELTA')
