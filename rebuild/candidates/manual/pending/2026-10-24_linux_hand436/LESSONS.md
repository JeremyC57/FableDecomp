# Chunk 436 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A VC7.1 `static` (internal-linkage) helper can get a custom calling convention. A `static bool __fastcall
  IsSubEventName(const CString&)` received its argument in eax (`lea eax,[esp+18h]`) where retail passes ecx. Give
  same-TU stand-ins external linkage (0x00C2E8D0, helper 0x00C2ECD0).
- A missing EH state store around a call into a same-TU helper that calls only C runtime functions comes from /EHsc's
  extern "C" nothrow assumption. Give the helper a same-TU noinline body and declare the CRT functions `extern "C"`
  (0x00C2E8D0). The caller 0x00C2E960 keeps its state-0 store because a library C++ call follows.
- An inline constructor that stores members out of declaration order (codec word +0xE before channel word +0xC)
  assigns them in its body in that order. An init list follows declaration order (0x00C2E190).
- A sub-word field read after a call that feeds a later argument means the call result is a named local
  (`void* sample = Create(...); map.Add(log, sample, header->m_index);`). Written inline, the field loads before the
  call (0x00C2E190).
- Under the lane's RTM compiler, an 8-byte loop-local object whose address is taken (a trigger reference with an
  out-of-line ctor) gives retail's `push ebp ; mov ebp,esp ; and esp,-8` frame, with ebp reused as the loop index, on
  the first grade (0x00C2E4D0). This is not a lane gap here.
- `if (flag) Defragment(); else return 0;` moves the `return 0` block to the function end, where it merges with an
  earlier `return 0`. `if (!flag) return 0; Defragment();` keeps it inline (0x00C28C20).
- If/else arms that fall through to one shared `return 1` keep retail's branch; an early `return 1` collapses into
  `setne` (0x00C2D420).
- An `unsigned char` parameter fed from a byte member pushes `xor eax,eax ; mov al,[m] ; push eax`; an int parameter
  gives `movzx` and reschedules (0x00C2A7A0). `unsigned short` pushes `xor eax,eax ; mov ax,[m]` (0x00C2C670).
- Same-TU `auto_inline(off)` / `noinline` callee bodies let VC7.1 keep registers that the callee provably leaves
  alone:
  - `this` in edx across a message ctor (0x00C2A7A0);
  - ecx across GetCurrentInstance (0x00C2D420);
  - ecx/edx across GetBlockAlign / GetSamplesPerBlock (0x00C2D890);
  - ecx and an x87 value across them (0x00C2D970).
- A libpk keyed-list Find taking `const unsigned long&`, called with `(unsigned long)ptr`, homes the conversion
  temporary in the dead argument slot and re-stores it before every call in a loop (0x00C2A640, 0x00C2BC10). A
  by-value key passes straight through (0x00C2D9C0).
- libpk error macro (PKCSemaphore, 0x00C29FC0 / 0x00C2A090 / 0x00C2A150, /EHsc):
  - the steps run in this order:
    - a CDebugStream over the nil buffer 0x0139C900;
    - `__fastcall` stream operators, 0x00C0DEC0 for text and 0x00C0DF00 for long, giving
      `".\\File.cpp" << "(" << (long)line << ") : " << text << "\n"`;
    - the hook pointer 0x0143BB2C called with the buffer;
    - the stream destructor 0x00C0BA40;
    - a `CDebugBreak()` temporary (0x00C16750) homed in a dead argument slot.
  - The bank code's log messages are different: `log->AddMessage()` (0x00C307E0) returns a stream reference that
    nobody destroys, and 0x00C0DF10 is the unsigned `<<`.
- VC7.1 Dinkumware `_Uninit_fill_n` / `copy_backward` rows are `__fastcall` templates with their iterator tag.
  Explicit instantiation emits the row (0x00C294B0, 0x00C29250, 0x00C29290).

Review at landing:
- 0x00C28B80 used 4 grades and is parked as the RTM vector-slot `lea` gap (GOTCHAS wave 24l). If the
  `*(begin() + pos)` operator[] form does not close it, keep it parked.
- 0x00C28C20 (and the parked 0x00C28B80) use a local retail-layout `CHandleBasedMemoryPool` beside
  `rebuild/include/engine/CHandleBasedMemoryPool.h`. The header has the Ego_r layout (FreeLists +0x38, Handles
  +0x438), which this second build does not have (FailureHandler +0x38, the embedded default handler +0x34, Handles
  +0x43c). Keep the local view, or add a header for the 0x00C27xxx build.
- Errors in already-landed sources:
  - Pool and steering:
    - 0x00C21620 (CSteeringAgent OnCreate) calls 0x00C29EE0 "Register"; that body removes an agent from its group
      (CSteeringGroup::RemoveAgent).
    - Landed 0x00C29EC0 `CEngineInternalPrimitive_Update` is CSteeringGroup::AddAgent.
    - Landed 0x00C29EB0 `CtorSetVtable012b40a4` is the CSteeringGroup destructor.
    - Landed CVectorulong 0x00C29C30 / 0x00C29B30 model `vector<unsigned long>`. They are
      `vector<CHandleBasedMemoryPool::CHandleInfo>` and `vector<CHeader*>`. 0x00C294B0 is the non-scalar-tag fill,
      not the scalar one.
  - Registries and navigator:
    - Landed 0x00C2A620 `CTCPhysicsNavigator::EnableNavigator` is the CNavigatorStateMessage constructor.
    - Landed 0x00C2A5C0 `CTCParticleAttacher::HandleMessage` is CResourceRegistry::ReleaseEntry.
    - Landed 0x00C2BAF0 `CTCMapwho::OnDie` is CInstanceRegistry::DestroyInstance. It is a thiscall, but the landed
      source models it as `__stdcall`.
  - Script / flags:
    - Landed 0x00C2C6B0 was retyped onto the CQ_GuildTrainingPreMeleeScript engine header by label alone. It is the
      libpk CFlagOverride constructor (vtable 0x012B4630, base 0x012B2828).
    - Landed 0x00C2CC60 `CCombatScript` is the CEventScript destructor.
  - Instance manager and audio format:
    - Landed 0x00C2D5E0 (CInstanceManager Assign) calls 0x00C2D210 "Register", which is FadeOutAll, and 0x00C2D520
      "Create", which is FindQuietestInstance.
    - Landed 0x00C2D2B0 `CNavSwitchableLeafNode::GetRegion` is CInstanceManager::GetCurrentInstance.
    - Landed 0x00C3AA50 `CWorld::SetDrawEnvironmentBounds` is the segment file's SetRange.
  - Bank processor:
    - Landed 0x00C2E0C0 `CSubPrimitiveList::CSubPrimitiveList` is the libpk bank processor's constructor. Its fields
      are data, manager, log, the driver map +0xC, the sample map +0x3C and the trigger registries +0x6C/+0x70.
    - Landed 0x00C2E140 `SPair_00c2e140::~SPair` is that processor's destructor. 0x00C16D6D calls it on the local
      that 0x00C2E0C0 built, right after 0x00C2EB00 ran.
    - Landed 0x00C2E270 `CChildListLoader::AddChildren` is the processor's AddSamples step; its "AddChild" 0x00C2E190
      is AddSample.
    - Landed 0x00C2E2B0 `ResourceProcessor_00c2e2b0::Process` is CBankProcessor::AddDriver. Its "map" +0xC is the
      driver map, and its "lookup" +0x3C is the sample map.
    - Landed 0x00C307E0 `CScriptHandleRegistry::CreateNewInstance` is the libpk log's AddMessage. It builds a 12-byte
      node: a bool, then a stream over the nil buffer 0x0139C900 with a serial from 0x0143BB44. The landed
      "g_pCurrentOwner" is the nil string buffer.
    - Landed 0x00C307C0 `CTCInventoryBase::GetCanItemBeQuickAssigned` is the log's IsOk.
    - Landed 0x00C10140 `CDataOutputStream::WriteCBYTE` is a keyed registry Find(number).
    - Landed 0x00C0E700 / 0x00C0E8E0 `CFactory_*::Create` are the sound manager's CreateSample / CreateGroup.
    - Landed 0x00C0E950 `CIsThingAlive::operator()` is the manager's FindGroup.
- Catalog rows holding several functions: none in this chunk.
