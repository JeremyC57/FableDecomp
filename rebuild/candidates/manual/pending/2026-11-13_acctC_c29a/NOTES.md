# agentC29 NOTES

## 0x0074E860 CTCContainerRewardHero::GetRewardItems -- RELOCATION_MATCH, test PASS, addmatch + read-only vland done (~12 grades)
Draft from C27's decode (prev_C27/NOTES.md) + retail_dis. First grade 1119v1005 (64 hunks): every CDefPointer handle stayed in
memory with its `Object = 0` dead store. LEVEL that closed it: callee body visibility -- FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
+ FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (noinline template bodies; retail calls 0x0074F760 / 0x0042AF3C / 0x0074F6A0 / 0x006AE320)
-> 1005v1005 7 hunks (thing-def / gold-def share [esp+18h], defs in esi/ebx). Last hunk (final erase read through esi): ContainerItems.clear()
instead of erase(begin(), end()); items.clear() for the first one too (both forms match for items). Call targets checked: all 25 relocs
are the retail callees (pair ctor out of line only at the def-path site, inline in the loop -- natural). vland: only RELOC_NAME warn
(0x0074FB20 labelled std::vector_CopyAssignSegment in the catalog; it is vector<pair<long,CCharString> >::erase).
Headers: CTCCONTAINERREWARDHERO_GETREWARDITEMS, STLP_VECTOR_AT_OUTOFLINE (hdr/README.md). Local models: CContainerRewardHeroDef
(ObjectFamilies CArray<CDefIndex> +0x28, Ego_r) and CObjectFamilyDef (GetRandomObject) -- their engine headers are quarantined.

## 0x00945060 CAIStateGroup_WorkHammerAnvil::Update -- RELOCATION_MATCH, test PASS, addmatch + vland (11 grades)
## 0x00944940 CAIStateGroup_WorkSawingWood::Update -- RELOCATION_MATCH, test PASS, addmatch + vland (3 grades)
Names: Ego_r ?Update@..@@MAEXXZ (donors 0x00968DE0 / 0x00968560, C27 LABELS). Model: local class deriving the shared
CAIStateGroup_WorkBase<CAIStateGroupBase> (RTTI base) with the Ego_r members at the Ego_r offsets (TookActionPoint +0x14, pointer +0x15,
ActionPointPos +0x1d, RHSet +0x29, index +0x44, NextFrameHammer +0x48) under pack(1) + explicit 3-byte gap. DECISION for coordinator:
the generated flat engine/CAIStateGroup_Work*.h cannot carry the protected virtual Update; alternative is an overlay class opt-in.
(Landed GoToPub::ThankBarmaid uses a local state-group class; landed OnActivate siblings use a `_Methods` stand-in with a wrong leaf.)
Opt-ins: CAISTATEGROUP_WORKBASE_ISINWORKHOURS (inline body), CTCAISCRATCHPAD_SETISPUSHABLEBYHERO, CAISTATEGROUPBASE_PEEKPLAYERMANAGER.
What closed them: CVECTORMAP_INLINE_LOWERBOUND (first lookup expanded, later ones call 0x0040F020 by budget), uninitialised
`bool arrived`, `end_frame > GetFrame()`, and ONE reused `EAIActionReturn result` for the loop / perform results (SawingWood 889->883
in one step; HammerAnvil's last residue -- the X add order of the LookAtPos vector -- also closed by reusing `result` in the hammer loop).
SawingWood: the TCI_CARRYING lookup, PeekPlayerManager().GetMainPlayerNumber(), GetPos() and GetDefGlobalIndexFromName("OBJECT_SAW_01")
have unused results in retail (calls kept, lookup end test gone) -- written as bare statements; perhaps a release-stubbed inline helper.

## 0x00537E70 (catalog ContainerUI_HandleMouseInput) -- NOT A FUNCTION START: tail of NUISystem::CList::ComputeChildrenVisibility
0x00537E70 begins mid-body (`lea edx,[ebp-20h]` on a frame set up elsewhere; `jb 0x00537A1E` loops back). The enclosing function starts
at 0x005379F5 (`push ebp ; lea ebp,[esp-78h] ; sub esp,0ECh`, right after ChangePosition's `ret 0Ch` at 0x005379F2) and ends where
0x00537E70+955 = 0x0053822B (landed CList::GetDataFromDef) begins: 2102 bytes. Name: CList vtable 0x012462E4 slot 185 (+0x2E4) holds
0x005379F5 (xref 0x012465C8; second vtable 0x01249508 = a derived list), the landed ScrollToFirstChild row names slot 185
ComputeChildrenVisibility, and Ego_r 0x005B0B5A / FableWin 0x021A0834 ?ComputeChildrenVisibility@CList@NUISystem@@MAEXXZ.
DECISION: the row must be re-cut to 0x005379F5 / 2102 bytes (oracle_from_exe.py) -- not authored here (out of this batch's size).

## 0x00735FD0 CTCCombat::AttemptToStartParticle(CThing*) -- decoded (name proven), draft below if any
Name: Ego_r 0x0074D250 + FableWin 0x02504840 ?AttemptToStartParticle@CTCCombat@@AAEXPAVCThing@@@Z; reloc_bindings 0x00735FD0. `ret 4`.
Decode: CDefPointer<const CWeaponDef> def ([esp+14h], zeroed) ; weapon->GetPBaseDef() by-value temporary (inc / dec around the call)
->GetSubDef<CWeaponDef>(&def) 0x0042B1A6, fail -> release def, return. this+0x84 = (long)(ConstantFPS * def+0x50) (fild/fmul/ftol).
count = def vector<long> +0x64..+0x68 size ([esp+2Ch]); this+0x88 (vector of CIntelligentPointer<CThing>, 8-byte elems) .resize(count)
0x00739B40. for i < count (edi, [esp+20h]): a = def+0x58 ([esp+24h]), b = def+0x54 (esi); s1 = CDefStringTable::GetString(table
0x013CA828, def+0x64[i]) 0x009D49B0 ([esp+6Ch]); s2 = GetString(def+0x70[i]) ([esp+10h]); C3DVector p1 [esp+34h..], p2 [esp+44h..];
weapon+0x64 (CTCGraphicAppearance*) ->GetHelperPoint(s1, 0, 0, p1, 1.0f) 0x004C0560 || ->GetDummyObject(s1, 0, 0, p1, rhs [esp+50h],
1.0f) 0x004C08C0 else {~s2, ~s1, release def, return}; same for s2/p2 (rhs [esp+50h]). if (b > 0): e1 = CTCDParticleEmitter::Create(b,
weapon->GetPos(), true) 0x006E0880 (fastcall + push 1); a CIntelligentPointer<CThing> temporary ([esp+30h]: ctor 0x00A01B10, vptr
0x01238C6C, SetPItem 0x00A01B90) then vec[i].SetPItem(tmp.GetPItem()) and ~tmp 0x00A01C10 (= `vec[i] = e1` through a temp);
e2 = Create(a, GetPos(), true) ([esp+1Ch]); if (vec[i] alive (GetPItem, dead bit +0x91) && e2 && e2 alive): em1 = 0 (ebx), em2 = 0
([esp+24h]); TCI 0x4B (+0x28 bit 0x800) lookups on vec[i] and e2 via out-of-line LowerBound 0x0040F020; pe1 = em1+0x14 GetPItem,
pe2 = em2+0x14 GetPItem (edi); pe2 byte +0x41 |= 2; dist = (p1 - p2).GetMagnitude() (fsqrt) [esp+24h]; len2 = pe2->GetApproximate
TotalSplineLength 0x00AED050 [esp+28h], len1 = pe1->...; diff = len2 - len1 -> clamp ref: `&diff` unless diff <= 0.0001f (0x0129BA3C)
then `&0.0001f` (pointer select = an inline max/limit helper taking const float&); pe1->InterpolateTo(*pe2, (dist - len1) / *clamp * 2)
0x00AED600 (fadd st0,st0); pe1 byte +0x44 |= 0x10; em1->AttachToThing(weapon, 10, s1?, 0, 0) 0x006E0BE0 (push 0,0,&[esp+74h],0xA,ebp);
e2->Kill(true) 0x004C9B80. ~s2, ~s1 each iteration; release def at the end (two duplicated epilogues for the early returns).

## 0x007E15C0 = NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw -- name proven, partly decoded
Name: Ego_r order OnAppearanceClear 0x007DCF20 -> OnAppearanceDraw 0x007DCF60 -> GetMeshesUsed 0x007DD360 = retail OnAppearanceClear
0x007E1590 (landed) -> 0x007E15C0 -> GetMeshesUsed 0x007E1990 (landed); sizes 0x400 v 0x3D0; `ret 8` = (CTCGraphicAppearance&, const
CGraphicAppearanceDrawInfo&) returning bool (`mov al,1`); referenced from vtable data 0x0126FFB4. Leaf
?OnAppearanceDraw@CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z.
`this` is a SECONDARY base (CAGraphicAppearanceComponent at full+0x0C?): [this-8] = ParentThing (IsAffectedByBulletTime 0x004C9920),
Primitives vector<CPrimitive*> at this+4, def pointer at this+0x10 (def+0x28 vector of 0x38-byte mesh entries: +0x28 mesh id,
+0x2C bool, +0x2D bool, +0x30 float, +0x34 bool). Start: x = info+0x4C * info+0x34; fade = appearance.GetFadeDistance() 0x004BC180;
if Primitives empty: per mesh push_back(appearance.AddPersistentPrimitive(CPrimitive(type 1, mesh id))) 0x004C0050 (inline push_back,
_M_insert_overflow 0x006422B0, ~CPrimitive 0x004C4160); else if info+0x1C == 1: RefreshPrimitivePos(prim[i], info+4, info+8)
0x004BC340 for all, return true. Then the per-mesh draw loop (CRightHandedSet::Interpolate 0x00A23190, CEnginePrimitiveStaticMesh ctor
0x0077BD70, SetPrimitiveGraphics 0x004BD620, GetTintColour 0x004BD040, GetPMeshEffect 0x004BCF50; global 0x013756F0 tested >= 0).
Landed template: CTCLight::OnAppearanceDraw 0x00641C30 (shared NGraphicAppearance headers).

## 0x006B2CA0 = CEnvironment::UpdateSunAndMoonPositions -- name proven, not authored
Ego_r 0x006EE710 directly after GetGlobalEnvironmentThemeIndexFromLocal 0x006EE6D0 (retail 0x006B2C60 landed; both gaps 0x40);
reloc_bindings ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ; FableWin 0x0244D960; one caller 0x006B42A4 (CEnvironment::Update).
Body: early out on byte +0x3D; angles +0xC0C / +0x1A8 (negated) / +0x1AC times a double deg->rad constant 0x0124F2B8, fsin/fcos
intrinsics, inline rotation-matrix products (0.5f-ish float 0x0122DEDC), one call C3DVector::AccurateNormalise 0x00A14440.
Row 0x00735FD0 draft (differ/00735fd0.cpp, 8 grades): 1141 -> 1092 (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) -> 1061
(STLP_VECTOR_RESIZE_ONE_DECL: retail calls the one-argument resize 0x00739B40; CINTELLIGENTPOINTER_ASSIGN) -> 1026 (explicit
`set.StartPoint[i].GetString()`: the implicit conversion leaves a CCharString copy) -> 993v973 (the whole body inside
`if (GetPBaseDef()->GetSubDef(&def)) { }`: retail has no separate early return, `je` to the shared release). Open: the
CIntelligentPointer temporary for `FlourishParticles[i] = ...` (retail: CBaseIntelligentPointer() 0x00A01B10, vptr 0x01238C6C,
SetPItem -- try a default-constructed named temp then `= e1`), weapon in ebx v ebp, loop counter mirror [esp+20h], fsub order of
(p1 - p2), AttachToThing argument pushes. Opt-in added: CTCCOMBAT_ATTEMPTTOSTARTPARTICLE (decl); CWEAPONDEF_FLOURISHPARTICLES (typed
CObjectAugmentationParticleSet at +0x50). CParticleEmitter is a local model (no engine header): Flags41 / Flags44 descriptive.
