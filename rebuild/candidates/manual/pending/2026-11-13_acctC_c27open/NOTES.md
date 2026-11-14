# agentC27 NOTES

## 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading(unsigned long, unsigned long) -- IN PROGRESS
Name: Ego_r ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z + FableWin 0x031277F0 (2614 debug bytes).
Work dir w/9fde80 (base.cpp = best). Callees all identified: TouchFrame inlined (FABLE_FORCEINLINE, with a Data null test that
GetFrame's header copy lacks), CManagedTexture ctor (CBaseClass 0x0099A2D0 + vptr 0x0129C880) / dtor 0x00A69E20, Lock/Unlock
(+0x49 Locked), CPixelFormat::Initialise(D3DFMT 0x15) 0x009E3830, CTextureManager::AllocateVolumeTexture 0x00A69C20 /
AllocateTexture 0x00A69B70, CBankFileAsyncData::Abort 0x009D6350 / WaitUntilFinished 0x009D5B60, ??4CManagedTexture 0x00A69B20,
Release 0x00A69AD0, CBankFileAsync::CreateEntry 0x009D58D0, GetEntryDataSize 0x009CC240, GetEntryDataType 0x009CC210,
CBankFileAsyncEntry::BeginReadDataAutoAlloc 0x009D6100 with TextureLoadingPriority 0x01396C64 (Anniversary PDB global name; only
reference), Sizeof inline, SetResourceSize 0x009FBF20, CResource::RemoveFromResourceList 0x009FC440, CResourceBank::AddToLoadingQueue
0x009FC3D0. Two CCountedPointer<CTexture> copies (texture.GetTexture() before LoadingManagedTexture = texture, and
LoadingManagedTexture.GetTexture() after) whose object word is dead-stored away: retail keeps only the count blocks
([esp+3Ch] / [esp+18h]) and releases them at the end -> unused locals required by the bytes.
Header opt-ins added (hdr/README.md): CBANKFILEASYNC_CREATEENTRY, new CBankFileAsyncEntry.h, CTEXTUREMANAGER_ALLOCATEVOLUMETEXTURE,
CMANAGEDTEXTURE_LOCK (+SetLocked, dtor decl), CGRAPHICDATABANK_GETPIXELFORMAT (+GetTextureManager; typed PixelFormats),
CGRAPHICINFO_PEEKGRAPHICHEADER, CGRAPHICINFOFRAME_GETINDEX, CBANKFILEASYNCDATA_WAITUNTILFINISHED.
Grades: a.cpp 1077 (TouchFrame not inlined) -> FORCEINLINE 1271 -> bank local + manager first 1269v1265 (67 hunks).
Residue: header reference kept in ebx by retail all function long while mipmap_level lives in its param home; ours spills
header ([esp+10h]) and gives the allocation loop's mipmap_level ebx -> frame offsets +4 everywhere, offset/size regs swapped.
Tried (no gain): std::min clamps (pointer-select, worse), header as pointer, offset declared first, info_frame local, IsLoading
v FileData.IsNull spellings, while loop (1233), if/else arms in loop, depth-first decl (1255), depth inline (1238).
More tried: GFLimitUpper (FableWin has GFLimitUpper<long,long>) on the long clamps / unsigned clamps (no change / worse), separate
`long level` local (1279), offset-loop spellings: `offset += skip` before `size -= skip` and a long counter cut hunks 67->58
(differ/009fde80.cpp = this best), header declared late (no change), decl forms / scopes of width/height/depth (no change).
Diagnostic: removing the offset loop's use of mipmap_level makes c2 keep header in ebx exactly as retail (52 hunks, but frame 0x30
v 0x34) -> the residue is c2's priority of the post-loop mipmap_level web vs the header web. PARKED after ~45 grades (no
measured lead left at spelling level; a forced-priority c2 what-if would be the next level).

## 0x006823D0 CTCCreatureOpinionOfHero::FrameUpdate -- IN PROGRESS (taken before row 2 because it had a measured lead; rows 2-8 untouched so far)
Work dir w/6823d0. C24 draft regraded today: DIFFER(960v962) 34 hunks (frame 0x18 v 0x14).
LEAD (measured, but FITTED): GFFloatAbs text `unsigned long& bits = *(unsigned long*)&f; bits &= 0x7fffffff; return *(float*)&bits;`
gives 962v962 with retail's whole frame (20 hunks, all temp-register rotation: push 1 / mov ecx at 0x006824A4, clamp base
ecx v edx 0x0068258D.., run-out block 0x006825DE, UniqueID hi-word order 0x0068264E, deed loop regs). But fwsize: that text is
15i/37b v FableWin ?GFFloatAbs@@YIMM@Z 8i/21b (C24's `*(unsigned long*)&f &= 0x7fffffff; return f;` is SAME) -> not acceptable as is;
it shows what retail does: the peak value is read back from the abs helper's own parameter memory (store of the and-result into f).
Tried (no gain): Kill via Get()/GetPItem/GET_ORIGINAL_CHAIN; UniqueID expression orders / named local / GetParentThing(); scope
changes (offset ref removed 43 hunks, end in for, decl order); `peak = value; peak = GFFloatAbs(peak)` forms (34/31).
`return *(float*)&f;` (fwsize SAME) does not change the grade; a named pointer local in the helper (v5, 14i/35b DIFF) gives the same
962v962/20 as v3. Mechanism: with a pointer-store the and-result stays in f's slot and peak is coalesced with it; with the FableWin
text c2 forwards the and-result straight into a separate peak slot. Retail's source must create that coalescing some other way
(peak passed by value into an inline helper whose parameter is the slot? no PDB helper name found). PARKED ~25 grades;
differ/006823d0.cpp = C24 text (34 hunks today).
Row 1 later diagnostics: offset-loop bound read from the LoadingMipmapLevel member instead (x3, not retail: retail reads the
param home [esp+48h]) gives 1265v1265 / 38 hunks with header in ebx exactly as retail; the remaining frame gap is depth: retail
gives depth its own top slot (ebp-4, packer order depth, new_texture -12, entry -20, texture -28, T/manager -36, format -40,
loading_texture/max -48, success -49), ours lets depth share entry's slot (fs.sh table). Other spellings: `!= 0` (58), if/else
skip_size (56, 1256 bytes). Next level for row 1: find why c2 values the post-loop mipmap_level web less than retail does.

## 0x0074E860 CTCContainerRewardHero::GetRewardItems(std::vector<std::pair<long,CCharString> >&, long&) -- decoded, not authored
Name: reloc_bindings ?GetRewardItems@CTCContainerRewardHero@@IAEXAAV?$vector@U?$pair@JVCCharString@@@std@@...@@std@@AAJ@Z + Ego_r donor
0x00760960. Frame: `and esp,-8`, no EH. Outline: gold = 0; items.erase(begin,end) (0x0074FB20); if the parent thing has TC 0xCC (CTCGold,
inline GetTC: bit test +0x38 & 0x1000 then LowerBound 0x004365B0) gold = GetGold() 0x007CBDC0; potion_index /
resurrection_index = CGameDefinitionManager::Get() 0x0044C6B0 ->GetDefGlobalIndexFromName(CCharString("OBJECT_DYNAMIC_POTION_DUMMY" /
"OBJECT_DYNAMIC_RESURRECTION_DUMMY")) 0x009AD410. If ContainerItems (this+0xC) not empty: for i < size: item == potion ->
GetDynamicPotionDefIndex 0x0074E280 (>0: items.push_back(pair(idx, ContainerItems[i].second)) inline pair, copy ctor 0x0099EC30,
push_back 0x0074FE50); == resurrection -> GetDynamicResurrectionDefIndex 0x0074E460 (>0 push, else gold += 20); else
push_back(ContainerItems[i]); then ContainerItems.erase(begin,end); return. Else (def path): CDefPointer def (GetSubDef
<CContainerRewardHeroDef> 0x0074F6A0 on PBaseDef +0x70); def+0x28 vector<long> non-empty -> index = GFRandom(size, PeekWorld()
0x00686A80 ->GetRandom? 0x0049D810) 0x004CDAE0; element via 0x0074CBC0; GetDefinitionManager() 0x006869D0 ->GetDef 0x0074F760
-> reward def; 0x008ED590 on it -> thing def index (>0): GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CGoldDef> 0x006AE320 ->
CreateNewObject(index, GetPos() 0x004C73D0, C3DVector(0, 0.25f, 0.25f), CCharString("")) 0x0074E560 and return; else potion /
resurrection / plain index -> items.push_back(pair<long,CCharString>(idx, CCharString("")) via the out-of-line pair ctor 0x00695250).
Landed siblings in rebuild/src/compiled/00/74 (RewardHeroWithItems, GetDynamicPotionDefIndex, ReleaseItems) use local stand-ins.
