# Chunk 772 -- NOTES

## Results
| address | name | result | grades | note |
|---|---|---|---|---|
| 0x00E11670 | NScript::CQ_WaspBossScript::CWaspVictim::Main | RELOCATION_MATCH + PASS (vland) | 13 | shared headers + FABLE_RETAIL_CQ_WASPBOSSSCRIPT_ENTITIES; same-TU noinline CTimer ctor/dtor + __forceinline operator= |
| 0x00E137B0 | NScript::CQ_WaspBossScript::Outro | RELOCATION_MATCH + PASS (vland) | 7 | EndMission TU prelude + STLP_MAP + __forceinline STB operator= + `&&` IsAlive |
| 0x00E62D60 | NScript::CV_ChickenKickingScript::CreateSpectators | RELOCATION_MATCH + PASS (vland) | 3 | shared header; `!= false` second wait |
| 0x00EE8390 | NScript::CCS_OakValeRevisitedScript::OakVale (PROPOSAL-plus) | PARKED DIFFER(1224v1240) dl=12 | 9 | resize(n) nested out-of-line ~CScriptThing |
| 0x00E1F7E0 | NScript::CQR_EscortTraderScript::CTraderToEscort::Init | PARKED DIFFER(1357v1286) dl=61 | 6 | CSpawnedFunc1 Par1 copy inline (retail 0x004ABE90 call) |
| 0x00D29880 | NScript::CQ_EndGameScript::CHookCoastTeleporter::Main | PARKED DIFFER(1274v1319) dl=74 | 10 | c2 static-frequency split (see below) |
| 0x0078C900 | CTCBerserk::InitAbility | NOT ATTEMPTED | 0 | budget; callee map below |
| 0x00828710 | CRegionConnectionGraph::InitialiseGraph | NOT ATTEMPTED | 0 | budget; decoded flow below |

vland (read-only, `vland_result.txt`): 3 WINS, all RELOCATION_MATCH + PASS; one WARN RELOC_NAME on 0x00CDBFB0 (stale catalog
label of the cast-map dtor, NAME_FIXES row).

## Header overlay (`hdr/`, mirror rebuilt with mkmirror.sh; hdrcheck2: both default=same)
- `engine/NScript_CQ_WaspBossScript.h`: `FABLE_RETAIL_CQ_WASPBOSSSCRIPT_ENTITIES` -- forward-declares the nested entity class
  CQ_WaspBossScript::CWaspVictim (Ego_r 0x1c: ParentClass +0x14, PMasterData +0x18; vtable 0x012E0490, Alloc 0x00E11580).
- `fable_string.h`: `FABLE_RETAIL_CCHARSTRING_CONCAT_STRING` -- declares the free `CCharString operator+(const CCharString&, const
  CCharString&)` (`??H@YI?AVCCharString@@ABV0@0@Z`, __fastcall, 0x0099F570); used by the parked 0x00E1F7E0.
- Siblings: the only landed includer of NScript_CQ_WaspBossScript.h (EndMission 0x00E13310) regraded on the chunk mirror:
  RELOCATION_MATCH. fable_string.h: default branch unchanged (1940 includers; hdrcheck2 default=same).

## Header debt
- The CWaspVictim class body is defined in the row (header only forward-declares it); other WaspBoss nested entities could move
  into the header behind the same switch.
- No shared models for CRegionConnectionGraph / CRegionConnectionNode (Ego_r: GraphNodes CVectorMap 0x14 with Dirty +0x11 [retail
  +0xd]; node 0x1c RegionID, Connections vector<long>, MostRecentSearchID, FirstNodeInSearch) and no retail model of CTCBerserk
  (generated header is opaque; landed siblings use local views).
- `AddLogbookStoryEntry` (0x00CBE87F, __fastcall long) has no shared header declaration (the parked 0x00D29880 declares it).

## Callee names used (all bound by relocation in the graded objects)
IsActiveThreadTerminating 0x00F35B30 / 0x00CB7940; CTimer 0x00CD4450 / ~ 0x00CD4470; CCharString 0x0099EBF0 / ~ 0x0099EAE0;
operator+ 0x0099F570; CBaseObject ctor/op=/dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430; scripted-thing handle ctor/dtor/IsValid/
Release 0x007E72A0 / 0x007E74D0 / 0x00CD23B9 / 0x00CD2770; CCountedPointer<STB>::operator= 0x008AB1E0; movie PImp dtor 0x006E7AB0;
cast map ctor/op[]/dtor 0x00CDBF70 / 0x00CD3D2E / 0x00CDBFB0; RunCutsceneMacro 0x00CBFB7D; AddLogbookStoryEntry 0x00CBE87F;
~CScriptThing 0x004AA840, CScriptThing copy 0x004ABE90; CSpawnedFuncBase 0x00CDD450; AddSpawnedFunction 0x00CB7E50.

## Parked residues and next ideas
- 0x00EE8390 OakVale (dl=12): retail expands `vector<CScriptThing>::resize(n)` but calls ~CScriptThing 0x004AA840 inside it while
  the GetThingWithScriptName temporary's dtor is expanded. Next: per-site frequency meter (see 0x00D29880 tools) on the resize
  site; the INLINE_DEPTH_REVISIT "out-of-line callee inside an expanded inline" family.
- 0x00E1F7E0 CTraderToEscort::Init (dl=61): each CSpawnedFunc1 ctor's Par1 copy is expanded here; retail calls the CScriptThing
  copy 0x004ABE90 inside the expanded ctor and expands the by-value argument copies. Same family; `/d2inlT55-65` reproduces such
  rows (diagnostic only).
- 0x00D29880 CHookCoastTeleporter::Main (dl=74): retail expands the cut-scene's movie ctor/dtor, pause ctor/dtor and STB operator=,
  calls ~CTimer out of line at the wait-loop exits and at the final idle-loop exit, and expands ~CTimer at the four drain-loop
  returns. Measured with /d2inlT (diagnostic only; scripts `meter/seq.sh`, `meter/fin.sh`, `meter/tstar.sh`): our movie ctor
  needs T* ~620 (retail < 74) and our final exit has T* ~55-60 (retail > 74). Wrapping the cut-scene block in `do { } while (0)`
  makes the movie expand (T* ~50; grade f 1322v1319 dl=96) but cannot make the final exit colder; one extra drain-loop exit share
  moves the final exit to T* ~69, three to ~87. Next: find a source shape that gives the drain loop (or the code after it) about
  two more 50/50 shares without new code -- or an alternative cut-scene construct that is hot without leaking.
- No helpers, no asm, no pragmas in any source.

## 0x00828710 InitialiseGraph (decoded, not authored)
`GraphNodes.clear()` (copy 0x00828CD0 + destroy 0x00519050), `reserve(0x50)` 0x008292D0; `CStringParser parser(text)` 0x00404720;
loop `while (!parser.AtEnd() && parser.ReadAsSymbol() == "<0x0122DF24>")` (ReadAsSymbol 0x009BA790, CCharString == 0x004115A0,
conditional-temporary flag bit 1): NProgressDisplay::DisplayProgress(CCharString(0x01272568), false, -1.0f, 0) 0x009E9F40;
name = parser 0x009BA330(...); `region = world_map 0x004FC210(name)`; if region > 0: `new CRegionConnectionNode(region)` (0x18,
inline) wrapped in a CCountedPointer (info 0xc: {1, deleter 0x00829160, data}), push_back into GraphNodes (inline, overflow
0x008294F0), Dirty=1; `vector<long> connections`; FindKeyword(0x01272564) 0x009B9BA0, loop over 0x009B9E00/0x009BA330/0x004FC210
pushing region ids (inline vector<long> growth: malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14) until FindKeyword(<0x0122DF24>)
fails, then 0x009B9E00(0x0122E024); node->Connections = connections (vector<long>::operator= 0x00454886); SkipPastNewline
0x009B9D70; after the loop `if (Dirty) { Sort 0x008298F0; Dirty = 0; }`; ~list 0x004046B0, ~CCharString x2, ~CBaseClass 0x0099A2E0.

## 0x0078C900 CTCBerserk::InitAbility (callee map, not authored)
CTCBase::GetWorld 0x00686A70, 0x006B8730 (radial-blur fade key from PDef +0x50..+0x64 -> RadialBlurFadeKey +0x6c), CThing
interface lookup (0x8c, CVectorMap LowerBound 0x0040F020) else CThing::AddTC 0x004C9D60, CTCHaste 0x00580D21 / 0x00800020,
GetSubDef<CSpecialEffectsDef> 0x00669580 + GetEffectFromString 0x00669D30 (conditional-temporary flag words), CThing::GetPosition
0x004C73D0, CreateParticleEffect 0x0072AB70, CTCSoundPlayer::PlaySound 0x0066BD00, CBaseIntelligentPointer 0x00A01B10/50/90 /
0x00A01C10, CCharString operator= 0x0099EFB0, a 4-way switch (jump table 0x0078CD70).

## GOTCHAS one-liners
- Same-TU `__declspec(noinline)` CTimer ctor/dtor + `__forceinline` CTimer::operator= keep TimerIndex and a cached 0 in registers as retail does (0x00E11670).
- `CScriptThing::IsAlive` written `!PImp.IsNull() && PImp->IsAlive()` gives retail's `test al,al`; the if-form compares with a cached zero (`cmp al,bl`) (0x00E137B0).
- `CCharString("A.") + CCharString("B")` needs the FABLE_RETAIL_CCHARSTRING_CONCAT_STRING declaration of 0x0099F570, else C2666 (0x00E1F7E0).
- Retail expands `vector<CScriptThing>::resize(n)` with ~CScriptThing out of line inside; VC7.1 keeps resize(n) a call (0x00EE8390, unsolved).
- Per-site inline score in minutes: compile with `/d2inlT<N>` (diagnostic) and bisect N for one call site (`meter/tstar.sh`, `meter/fin.sh`); a `do { } while (0)` around a cut-scene block drops the movie ctor's T* from ~620 to ~50 (0x00D29880).
- The seed-corpus Lua ports (`refs/script_recovery/seed_corpus/sources/`) are a second identity source for quest-script methods (WaspBoss Outro / WaspVictim, OakValeRevisited OakVale).
