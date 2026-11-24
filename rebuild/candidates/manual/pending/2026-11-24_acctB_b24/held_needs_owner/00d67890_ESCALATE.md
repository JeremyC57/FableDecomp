# 0x00D67890 CQ_GuildTrainingWoodsWillScript::Main -- pick-any hold (B24A11, 2026-11-24)

Main grades RELOCATION_MATCH at stock flags (strict vland PASS, vt PASS); payload kept as payload_HELD_pickany.json until Account A rules.

1. Leaf-referenced NEQ copy: ??1?$vector@VCScriptGameResourceObjectScriptedThingBase...@QAE@XZ. Ours (50 bytes) expands the destroy loop and
   calls free; retail 0x00CD319B (35 bytes) calls __destroy_aux 0x00CD2725 (/Gr, tag in a dead slot) + 0x00BFEA14. Evidence that our TU
   configuration is retail's: in the same build ~vector<CScriptThing> is EQ to retail 0x008AC970 (inline-loop form), and declaring
   __destroy_aux (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL) breaks Main (3521v3426). The landed binder of 0x00CD319B, RunCutsceneMacro
   0x00CBFB7D, compiles to different copies of all eight NEQ symbols (a11/pk: its ~vector 30 bytes, map::operator[] 119 v our 216, ...):
   a different object's configuration. So 0x00CD319B (like 0x00CD3D2E map::operator[], 0x00CD344A _M_erase, 0x00CD3699 clear,
   0x00CD27C7 pair ctor, 0x0042AAB3 less, 0x00CDD4C0 ~CSpawnedFuncBase) is most likely another object's COMDAT that the retail link
   picked -- the same question as 0x00ED8680's ??1MovieBase 0x006E7B80. Question: may the row land with this leaf-referenced NEQ copy
   (family debt, object mode to settle), or must it wait?
2. Copy this row introduces itself: the explicit specialization vector<ScriptedThingBase>::resize(size_type) (the Inline-budget device)
   emits its own 44-byte COMDAT ?resize@...@QAEXI@Z, NEQ to retail's out-of-line 0x00CD3DA4 (from another TU; not leaf-referenced, Main
   expands it). No landed row is shown to emit this copy. Treat as blocking until ruled; a fix would be a spelling of the device that
   emits no copy (none found yet).
