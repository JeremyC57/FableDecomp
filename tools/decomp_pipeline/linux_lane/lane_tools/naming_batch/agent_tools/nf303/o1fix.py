# o1fix.py: NF303 opt-in unit (ENTRIES lines 1-6). Add-only declaration opt-ins in four flat (GENERATED) header branches, plus the
# five binders moved onto them (stand-in members dropped, #define merged onto a comment line ahead of every include: line counts kept).
# NOTE: for 0x006E11F0 the #define was then moved from line 1 onto comment line 7 by a one-off edit (rows/006e11f0.cpp is the final form).
# Mode `pin` writes only the CMemoryDataOutputStream.h assert pin (for its own equiv_check); mode `all` does everything.
import sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF303/'
def edit(p,pairs,keep_lines=True):
    t=open(p,encoding='utf-8').read(); n0=t.count('\n')
    for o,n in pairs:
        assert t.count(o)==1,(p,o[:80],t.count(o)); t=t.replace(o,n)
    if keep_lines: assert t.count('\n')==n0,(p,t.count('\n'),n0)
    open(p,'w',encoding='utf-8').write(t)
mode=sys.argv[1]
H=A+'hdr/engine/'
PIN=('\n\nFABLE_STATIC_ASSERT(sizeof(CMemoryDataOutputStream) == 0x18);\n\n#endif // FABLE_ENGINE_CMEMORYDATAOUTPUTSTREAM_H\n#endif // FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM',
     '\n\nFABLE_STATIC_ASSERT_AT(58, sizeof(CMemoryDataOutputStream) == 0x18);\n\n#endif // FABLE_ENGINE_CMEMORYDATAOUTPUTSTREAM_H\n#endif // FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM')
if mode=='pin':
    edit(H+'CMemoryDataOutputStream.h',[PIN]); print('pin ok'); sys.exit()
edit(H+'CMemoryDataOutputStream.h',[PIN,
 ('    unsigned long CurrentPosition;                           // +0x14\n};\n#pragma pack(pop)',
  '    unsigned long CurrentPosition;                           // +0x14\n'
  '#ifdef FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_FLAT_GETLENGTH_PEEKDATA\n'
  '    // Add-only opt-in (naming agent NF303, 2026-11-23, binders 0x00A99840 / 0x00A9BA70, the C3DAnimFileSaver / C3DMeshFileSaver\n'
  '    // SaveFileCompressed pair): Ego_r.pdb / FableWin.pdb ?GetLength@CMemoryDataOutputStream@@QBEKXZ (retail 0x00994B40) and\n'
  '    // ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ (0x00995E70), as the retail branch above declares them. Declarations only: no layout change.\n'
  '    unsigned long GetLength() const;                         // 0x00994B40\n'
  '    const void* PeekData() const;                            // 0x00995E70\n'
  '#endif\n};\n#pragma pack(pop)')],keep_lines=False)
edit(H+'CWorldMap.h',[(
 ' float GetHeightZBelowPos(const C3DVector& pos) const;\n#endif\n    void*                   __vftable;',
 ' float GetHeightZBelowPos(const C3DVector& pos) const;\n#endif\n'
 '#ifdef FABLE_RETAIL_CWORLDMAP_FLAT_ISLOADINGANYMAP\n'
 ' // Add-only opt-in (naming agent NF303, 2026-11-23, binder 0x006E11F0 CTCDParticleEmitter::Create): Ego_r.pdb / FableWin.pdb\n'
 ' // ?IsLoadingAnyMap@CWorldMap@@QBE_NXZ (retail 0x004FB4A0), as the retail branch declares it. Declaration only: no layout change.\n'
 ' bool IsLoadingAnyMap() const;                                      // 0x004FB4A0\n'
 '#endif\n    void*                   __vftable;')],keep_lines=False)
edit(H+'CTCBuyableHouse.h',[(
 '    unsigned char    _pad_0x4a[0x2];                         // +0x4a\n};\n#pragma pack(pop)',
 '    unsigned char    _pad_0x4a[0x2];                         // +0x4a\n'
 '#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_SETRENTED_SETOWNED\n'
 '    // Add-only opt-in (naming agent NF303, 2026-11-23, binder 0x00895ED0 CGameScriptInterface::SetHouseOwnedByPlayer): Ego_r.pdb /\n'
 '    // FableWin.pdb ?SetRented@CTCBuyableHouse@@QAEX_N@Z (retail 0x006C1150) and ?SetOwnedByPlayer@CTCBuyableHouse@@QAEX_N@Z\n'
 '    // (0x006C17B0), as the retail branch declares them. Declarations only: no layout change.\n'
 '    void SetRented(bool rented);                                         // 0x006C1150\n'
 '    void SetOwnedByPlayer(bool owned);                                   // 0x006C17B0\n'
 '#endif\n};\n#pragma pack(pop)')],keep_lines=False)
edit(H+'CTCDoor.h',[(
 '    CDefPointeeBase* PDef_Object;                            // +0x20 CDefPointer<CDoorDef_const_>::Object\n};\n#pragma pack(pop)',
 '    CDefPointeeBase* PDef_Object;                            // +0x20 CDefPointer<CDoorDef_const_>::Object\n'
 '#ifdef FABLE_RETAIL_CTCDOOR_FLAT_UPDATEOPENSTATUSTRIGGERTYPESCRIPT\n'
 '    // Add-only opt-in (naming agent NF303, 2026-11-23, binder 0x007311F0 CTCDoor::FrameUpdate): Ego_r.pdb / FableWin.pdb\n'
 '    // ?UpdateOpenStatusTriggerTypeScript@CTCDoor@@IAEXXZ (retail 0x007310C0, protected), as FABLE_RETAIL_CTCDOOR_OPENCLOSE declares it\n'
 '    // in the retail branch. Declaration only: no layout change.\n'
 'protected:\n'
 '    void UpdateOpenStatusTriggerTypeScript();                            // 0x007310C0\n'
 'public:\n'
 '#endif\n};\n#pragma pack(pop)')],keep_lines=False)
# rows
for a in ('00a99840','00a9ba70'):
    edit(A+'rows/%s.cpp'%a,[
     ('// NF42 (round 5): WriteULONG binds',
      '#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_FLAT_GETLENGTH_PEEKDATA // NF303: GetLength 0x00994B40 / PeekData 0x00995E70 from CMemoryDataOutputStream.h (Ego_r + FableWin QBE); NF42 (round 5): WriteULONG binds'),
     ('CMemoryDataOutputStreamMethods();~CMemoryDataOutputStreamMethods();unsigned long GetLength()const;const void*PeekData()const;};',
      'CMemoryDataOutputStreamMethods();~CMemoryDataOutputStreamMethods();}; // GetLength / PeekData: the CMemoryDataOutputStream.h members (NF303; were declared on this stand-in)')])
# (as run, then the define moved onto line 7 by a one-off fix: see NOTES)
edit(A+'rows/006e11f0.cpp',[
 ('// 006e11f0 CTCDParticleEmitter::Create\n',
  '#define FABLE_RETAIL_CWORLDMAP_FLAT_ISLOADINGANYMAP // NF303: IsLoadingAnyMap 0x004FB4A0 from CWorldMap.h (Ego_r + FableWin QBE). 006e11f0 CTCDParticleEmitter::Create\n'),
 ('struct CWorldMapAdapter: ::CWorldMap {bool IsLoadingAnyMap()const;};',
  'struct CWorldMapAdapter: ::CWorldMap {}; /* IsLoadingAnyMap 0x004FB4A0: the CWorldMap.h member (NF303) */')])
edit(A+'rows/00895ed0.cpp',[
 ('// Retail00895ED0,289 bytes;',
  '#define FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_SETRENTED_SETOWNED // NF303: SetRented 0x006C1150 / SetOwnedByPlayer 0x006C17B0 from CTCBuyableHouse.h (Ego_r + FableWin QAE). Retail00895ED0,289 bytes;'),
 ('struct BuyableHouseMethods:CTCBuyableHouse{void SetOwnedByPlayer(bool);void SetRented(bool);}; //006C17B0,006C1150',
  'struct BuyableHouseMethods:CTCBuyableHouse{}; //006C17B0 SetOwnedByPlayer,006C1150 SetRented: the CTCBuyableHouse.h members (NF303; were declared on this stand-in)')])
edit(A+'rows/007311f0.cpp',[
 ("// callee names are the manifest's (CTCDoor::UpdateAnimation / UpdateOpenStatusTriggerType*).\n",
  "#define FABLE_RETAIL_CTCDOOR_FLAT_UPDATEOPENSTATUSTRIGGERTYPESCRIPT // NF303: 0x007310C0 from CTCDoor.h (Ego_r + FableWin IAE); the other two callee names are the manifest's (CTCDoor::UpdateAnimation / UpdateOpenStatusTriggerTypePerson).\n"),
 ('    void UpdateOpenStatusTriggerTypeScript();     // 0x007310C0\n',
  '    // UpdateOpenStatusTriggerTypeScript 0x007310C0: the CTCDoor.h member (protected; NF303, was declared on this stand-in)\n')])
print('all ok')
