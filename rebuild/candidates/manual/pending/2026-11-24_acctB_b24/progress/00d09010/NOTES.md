# 0x00D09010 CCampHostageGuard::Main -- agent B24A notes (2026-11-24)

## Lessons that apply
1. Wine aborts (free(): invalid pointer / rc -11) while TMPDIR is set: `unset TMPDIR TMP` in every call (see ../00e20600/NOTES.md).
2. c2 inline trace works on the lane: $W/inl/run.sh <src> <tag> <overlay> [flags] (agent3009's tool; ~3 min here), then
   `python3 $W/inl/an.py $W/inl/<tag>.json Main@CCampHostageGuard` lists every plain-inline decision (sz, savings, f, score; r=1 expanded iff sc < T 74).
3. Row 1 (0x00E20600) showed Main inherits each inline callee's own-pipe body (bottom-up); a device that changes a callee's own pipe
   also changes the TU's pick-any COMDAT of it (check against the owner address before relying on it).

## State
- Baseline (Codex draft: ~CTimer defined NON-inline in the row under auto_inline(off), /d2inlS15): DIFFER(5672v5864), 17 hunks.
  The 192 missing bytes are retail's two hot exits that EXPAND both ~CTimer (pDSTGame->DeregisterTimer, slot 0x160):
  retail epilogues: (1) movie exit [both chat arms tail-merge into it]: ~CCountedPointer<Movie> call, ~CBaseObject call, Deregister x2
  inline, ~CScriptThing call, ~ScriptedThingBase call; (2) the hit-by-hero `while (true) { NSF; if (terminating) return; }` exit: Deregister
  x2 inline; (3) common exit: ~CTimer 0x00CD4470 called twice.
  Note: the draft's out-of-line non-inline `CTimer::~CTimer()` body in the row TU is a strong definition of the landed/owned 0xCD4470 symbol
  (pick-any / LNK2005 risk) -- prefer plain inline.
- Plain `inline CTimer::~CTimer()` (v1): the trace shows ~CTimer expanded at exactly retail's exit groups (sites 88-89 / 110-111 sc 40 = the
  two movie exits, 127-128 sc 20 = while(true) exit; all others sc >= 76 called) and our three epilogues have retail's shape. But the whole
  function re-allocates: stock 5944v5864 (231 hunks), with /d2inlS15 6016. Main visible difference: the GoadLevel switch -- retail stores each
  constant to [esp+0x40] per case (`mov dword ptr [esp+0x40], 6; jmp`), ours keeps GoadLevel in ebx and stores once; case-block order differs.
- v2 (natural + no Movie/WideScreen forceinline ctors) 5800/239; v3 (natural + plain CTimer accessors) 5928/232; v4 both 5768/239.
- Best next idea: keep natural inline ~CTimer (structurally right) and work the GoadLevel switch / register pressure (the switch arms'
  `GoadLevel++` vs constant stores; GoadLevel's type/scope), then re-add devices one at a time with the trace.

## Callee counts (relocations, $W/rc.sh <src> [flags]; retail from dis_all.txt in retcalls.txt)
retail: ~CScriptThing 16, ~CCountedPointer<Movie> 9, CBaseObject() 6, ~CBaseObject 6, ~ScriptedThingBase 3, ~CTimer 2, ~MovieBase 0.
- v0 baseline (/d2inlS15, out-of-line ~CTimer): 14 / 7 / 6 / 5 / 1 / 2 / 0  (exits with expanded timers missing).
- v1 natural inline ~CTimer, stock: 16 / 4 / 6 / 2 / 3 / 2 / ~MovieBase CALLED 8  -> MovieBase (implicit dtor, sz 25) split by frequency.
- v1 + /d2inlS15: 18 / 10 / 6 / 7 / 4 / ? / 0 -> MovieBase now always expanded (right) but ONE extra exit copy (+2 ~CScriptThing, +1 CP<Movie>,
  +1 ~CBaseObject, +1 ~Scripted): retail tail-merges the two chat-arm movie exits (first arm: `mov [esp+0x5c],0x126008c; lea ecx,[esp+0x5c];
  jmp` into the second arm's `call 0x99a430`), so the two arms' cleanups must become identical up to that call. Best next idea: v1 + /d2inlS15
  and find why our two movie-arm exits are not cross-jumped (movie slot / timer expansion order in each arm), then the GoadLevel switch.
- Why S15 overshoots: /d2inlS15 makes ~CTimer (c1 14 <= 15) always-inline -> v1+S15 expands the timers at FOUR exits (retail two + one common
  called exit). Why stock undershoots: implicit ~MovieBase (sz 25: declared-only CCountedPointer<Movie> dtor call + ~CScriptGameResourceObjectBase
  sz 11) is frequency-split (sc 54 at hot sites, called at cold ones) where retail expands it at every site. Retail therefore has a CHEAPER
  MovieBase destructor (c1 <= 9 always-inline at stock S9, i.e. sz <= ~15) with ~CTimer staying frequency-split at stock flags. Next: find the
  MovieBase / GROBase / CBaseObject destructor declaration form (implicit vs user-declared, virtual or not, which bodies visible) whose trace
  sz is <= 15 while the expansion stays `call 0x6E7AB0; store 0x0126008C; call 0x0099A430`; then stock flags, no S15.
