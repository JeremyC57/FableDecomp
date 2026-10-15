# Hand chunk 71 (CRenderManagerCore .. CRightHandedSet): 37 of 40 byte-exact

All 37 `src/` files grade MATCH or RELOCATION_MATCH in `grades.txt` (`gall.sh`). Their tests print PASS under g++
(`tall.sh`) and under VC7.1/Wine (`tall.py`). `payload.json` and `oracle.tsv` were written by `finish.py`.
Many manifest labels in this chunk were propagated to the wrong function. Each source is named for what its bytes
do, and the corrections are listed at the end.

## What matched (37)

- **CRenderManagerCore draw forwarders** (0x00A0AC60, 0x00A0AD40, 0x00A0AE10, 0x00A0AE40, 0x00A0AE70, 0x00A0AF40):
  `RealiseRenderState()` (0x00A058C0), then an IDirect3DDevice9 draw call on `PD3DDevice` (+0x3CF0). The calls are
  COM `__stdcall` virtuals at slots 81 to 84 (DrawPrimitive, DrawIndexedPrimitive, DrawIndexedPrimitiveUP). They are
  modelled on `engine/CRenderManagerCore.h` with a methods struct, the same way as the landed SetVertexFormat.
  0x00A0AD40 reads `PSelectedIndexBuffer->PrimitiveType` (engine/CIndexBuffer.h) into a local before the realise
  call, which is why the load comes first.
- **CRenderManagerCore::EndRender** (0x00A0B520): `if (Rendering == true) { double elapsed = GFGetTime() -
  LastRenderEndTime; ...; NoTrisPerSecond = (long)(NoTrisDrawn / elapsed); Rendering = false; }`. Because `elapsed`
  is a named local, it stays on the x87 stack (`fdiv st,st(1)`) and is popped after `__ftol2`.
- **CRenderManagerCore `??_G`** (0x00A0B590): `__forceinline virtual ~CRenderManagerCore() { Uninit(); }`, with
  Uninit holding the landed IsInitialised/Uninitialise test. SystemFont (CSystemFont : CInitBaseClass) has an
  implicit destructor, so it stores no vtable before `~CInitBaseClass`. The base destructor is out of line, and a
  `new T` factory pulls in the vtable.
- **CRenderStateManager::~CRenderStateManager** (0x00A044B0): a virtual destructor body `g = 0; PD3DDevice = 0;` over
  CBaseClassNonCopyable. `// Leaf: ??1CRenderStateManager` selects the plain destructor.
- **CRenderTarget** surfaces (0x00A10E30 Uninitialise, 0x00A10E60 and 0x00A10E90 AttachTarget): each CSurface
  member is tested through an inline `IsInitialised()` before `Uninitialise()` / `operator=`. Each AttachTarget
  overload works on one surface (+4 colour, +0x14 depth).
- **CCachedFile copy constructor** (0x00A5B760, labelled CRenderTarget): this is the out-of-line copy constructor
  that the landed `Clone` inlines. To get the `lea ecx,[esi+18h]` between the stores, the array's end word must be
  its own inline-constructed part (`CByteArray : CByteArrayRange { CByteArrayEnd m_end; }`). The landed
  {first,last}+capacity form puts `xor edi,edi` before the vptr store, and a plain three-word array hoists the `lea`.
- **C3DMeshPartitionElement::~C3DMeshPartitionElement** (0x00AC2100, labelled CRenderTarget::Uninitialise): the landed
  `??_G` 0x00AC37B0 body with the destructor out of line. Each `delete SubElements[i]` becomes a self call plus
  `operator delete` under the null test.
- **0x00C35D50** (free `__stdcall`, labelled CRenderTarget::Uninitialise): `if (o->m_buffer.m_pBuffer) o->Shutdown();
  else if (o->m_pTarget) o->SetTarget(0);`. The tail call reuses the function's own argument slot
  (`mov [esp+4],0 ; jmp`).
- **0x00CA5170** (labelled CRenderTarget::DetachTarget, media module): `long hr = m_pGraph->Detach(); if (hr < 0)
  return hr; return ReleaseInterface();`. Here `m_pGraph` is a C++ `__thiscall` virtual, not COM.
- **Copy constructors** (0x0046649A CResource, 0x00737370 CCombatAbilityMeleeAttackData): the implicit copy
  constructor. VC7.1 emits it out of line, so a placement-new helper is enough to put the symbol in the TU. The base
  vptr store survives in a copy constructor (written-out or implicit) because the source reads that follow it may
  alias `this`. A default constructor drops that store (0x009FBEC0).
- **Mass::Mass** (0x008EE9B0, labelled CResource): `Pos = C2DVector(0,0); Vel = C2DVector(0,0);` are body
  ASSIGNMENTS, which build an 8-byte frame temporary that is reused. The init-list form `Pos(C2DVector(0,0))` folds
  to plain zero stores. The two float zeros share the zero register.
- **CResource::CResource** (0x009FBEC0): `IVRefCount(1)` from the inline counted-pointee base comes before the
  derived vptr, and the five zeros share one register.
- **CResource::SetAsLRU** (0x009FC450, labelled SetAsMRU): the landed ListRemove and ListInsert are inlined, with
  `ListInsert(head, head->NextResource)` relinking right after the head sentinel.
- **CVtable_012b3a94/9c/a4 destructors** (0x00C23BA0, 0x00C23FE0, 0x00C24240, labelled CResource::SetAsLRU): `delete[]
  m_pBuffer; delete[] m_pEntries;`. The 0x00C48DB0 model applies: a virtual function plus a non-virtual destructor,
  under `/EHsc`, gives the `eh vector destructor iterator` form.
- **CEntryTable_00c35d10::GetCurrentEntry** (0x00C35D10, labelled CResource::GetUnusedFrames): `if (m_pLive) return
  m_pEntries[m_current]; return 0;`.
- **CResourceList** (0x009FBFE0 AddResource, 0x009FC020 RemoveResource, which is labelled CResourceBank,
  0x009FC070 GetUnloadableMemory, 0x009FC100 ResetDebugStats, 0x009FC570 constructor):
  - AddResource needs `#pragma optimize("s", on)` to get `inc`/`add` on memory. The head sentinel must also be
    reached through an inline `GetHead()` accessor at EACH use. A cached `CResource* head` local makes the lane form
    `lea edx,[ecx+4]` first and keep the link in esi.
  - The two loops are rotated top-tested `for` loops over `Head.NextResource .. &Head`, calling virtual slot 3 and
    slot 9.
  - In the constructor, the vptr is stored before the inlined CResource head constructor. The rest are body
    assignments in source order.
- **CResourceBank** (0x009FC380 AddResource, 0x009FC3D0 AddResourceToLoadingQueue): the list's AddResource,
  `__forceinline` under "s", on ResourceList (+4) and on LoadingQueue (+0x40).
- **CVtable_012bed90 constructor** (0x00C6D9D0, labelled CResourceBank): the base is the landed 0x00C6E6A0. The first
  two 0x18-byte records clear themselves with an intrinsic `memset(m_words, 0, sizeof m_words)`. Each expansion
  gets its own `xor` and a copied address register. The third record is an init-list zeroing with stores straight
  off esi. The two pointer members are initialised in between.
- **CDispatcher_00c1ac40 constructor** (0x00C1AE20, labelled CResourceList): the dispatcher at +0x20 of the landed
  file manager 0x00C1AC40. The sink comes from the init list; the cursors, sentinel and zeros are body
  assignments in source order.
- **CRightHandedSet** (0x00487050 constructor, 0x0049CE00 GetRight, 0x004BBE10 Set):
  - The constructor copy-constructs in the init list through the engine's memberwise C3DVector copy constructor.
    The implicit copy and assignment give `push esi` and a `this` copy register instead.
  - 0x004BBE10 does not return `this`, so it is not a constructor. It is `Set(up, forward)` with implicit C3DVector
    assignment, which shows the `mov edx,ecx` / `add ecx,0Ch` shape.
  - GetRight is `CrossProduct(Forward, Up)` through an inline helper taking references. The same products written
    directly on the members change the x87 load order of the first product. VC7.1 does not keep the source operand
    order of `*`, so swapping the operands had no effect.

## Parked for Fable 5.1 (3)

- 00def6a0 CVtable_012ddf3c::CVtable_012ddf3c DIFFER(91v91): `mov ecx,edi` for the +0x28 CBase constructor sits between the +0x20 and +0x24 zero stores in retail, before all three in the lane | tried: three plain init-list zeros, a 3-word member object with inline ctor + CScriptThing's two words as a member object (fixed the two vtable-store positions), the zero words and the default CScriptThing as one member object | next: lane gap: member-constructor `this` scheduled after a register-zero store run (the 24i gap)
- 009fbd00 CRenderTarget::`scalar deleting destructor' DIFFER(66v60): retail stores no CRenderTarget vptr (`mov dword ptr [esi],offset vftable` extra in ours) | tried: implicit destructor (the lane emits ??1 out of line and calls it: 30v60, also with /Ob2 and a __forceinline CSurface destructor), `__forceinline virtual ~CRenderTarget() {}` (inlined but stores the vptr), `__declspec(novtable)` (no ??_G emitted) | next: lane gap: the lane keeps the implicit destructor out of line instead of inlining it into `??_G`, as retail did. In this chunk the lane also emitted the implicit copy constructors (0x0046649A, 0x00737370) out of line.
- 009fc4a0 CResource::SetAsMRU DIFFER(78v80): retail copies the previous node through eax before linking (`mov eax,edx ; mov [eax+10h],ecx ; mov edx,[ecx+10h] ; mov [edx+0Ch],ecx`), the lane stores through edx and reloads into eax | tried: landed ListInsert(prev, next), four links written out, a next-first insert helper with `prev->` and with `PrevResource->` | next: the form that fixed 0x009FBFE0 (head reached through an inline `GetHead()` accessor at each use, maybe under optimize("s"))

## Skipped

None. No row spans two functions.

## Label corrections

- 0x00A0AE40 "CRenderManagerCore::DrawCurrentBuffersAsPointList" -> DrawCurrentBuffersAsTriFan (draws D3DPT_TRIANGLEFAN; the label was propagated from 0x00A0AE70).
- 0x00DEF6A0 "CRenderManagerCore::CRenderManagerCore" -> constructor of the no-RTTI script class CVtable_012ddf3c (base CVtable_012c3224).
- 0x00A5B760 "CRenderTarget::CRenderTarget" -> CCachedFile::CCachedFile(const CCachedFile&) (vtable 0x012724D4).
- 0x00AC2100 "CRenderTarget::Uninitialise" -> C3DMeshPartitionElement::~C3DMeshPartitionElement.
- 0x00C35D50 "CRenderTarget::Uninitialise" -> a free `__stdcall` over CMarkedBufferOwner_00c43810 (Shutdown / SetTarget(0)).
- 0x00CA5170 "CRenderTarget::DetachTarget" -> a media-module class (named CMediaGraphOwner_00ca5170 by address); callee 0x00CA8B50 is not CSurface::GetRefCount but releases the interface at +0x9C.
- 0x00737370 "CResource::CResource" -> CCombatAbilityMeleeAttackData copy constructor (vtables 0x01232484/0x01232498).
- 0x008EE9B0 "CResource::CResource" -> Mass::Mass (vtable 0x01288230).
- 0x009FC450 "CResource::SetAsMRU" -> SetAsLRU (relinks after the head, the oldest end that GetUnloadableMemory walks first). 0x009FC4A0 is the real SetAsMRU.
- 0x00C23BA0 / 0x00C23FE0 / 0x00C24240 "CResource::SetAsLRU" -> plain destructors of CVtable_012b3a94 / _012b3a9c / _012b3aa4.
- 0x00C35D10 "CResource::GetUnusedFrames" -> an indexed-table getter (CEntryTable_00c35d10::GetCurrentEntry); the real GetUnusedFrames is the landed 0x009FBF70.
- 0x009FC020 "CResourceBank::RemoveResource" -> CResourceList::RemoveResource.
- 0x009FC3D0 "CResourceBank::AddResource" -> AddResourceToLoadingQueue (the twin of 0x009FC380 on the +0x40 LoadingQueue; the name is chosen from what the body does).
- 0x00C6D9D0 "CResourceBank::CResourceBank" -> constructor of CVtable_012bed90 (base CVtable_012beda4 0x00C6E6A0).
- 0x00C1AE20 "CResourceList::CResourceList" -> CDispatcher_00c1ac40 constructor (the file manager's dispatcher).
- 0x004BBE10 "CRightHandedSet::CRightHandedSet" -> CRightHandedSet::Set (does not return `this`).
- 0x009FBD00 "CRenderTarget::`vector deleting destructor'" -> the scalar deleting destructor (only flag bit 0 is tested).
- Callee labels: 0x0099A330 is CInitBaseClass IsInitialised (not CTCInventoryBase::GetCanItemBeQuickAssigned). 0x0099A410 is ~CInitBaseClass. 0x00C43E20 is a `__thiscall` SetTarget returning bool, not "CExclusiveAssert_UpdateSelectedWeapon".
