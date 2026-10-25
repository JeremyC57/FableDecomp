# Hand chunk 452 (_global .. NScript::CQ_OakValeBanditRaidScript): 40 of 40 byte-exact

The catalog files every row under `_global`. In fact the chunk covers three families:
- the quest-helper globals TU (0xCBEB1A..0xCBFACA);
- the cutscene-director STLport instantiations and data-marker readers (0xCD2919..0xCD5090);
- quest-script code (0xCDBD20..0xDC24E0): script factories, constructors, members, and their STLport helpers.

Line 1 of each source carries the real (PDB) name. Every row has a test that passes under g++ and VC7.1.

## What matched (40)

### Quest-helper globals (size TU: `optimize("y",off)` + `("s",on)`, frame pointer)
- 00cbeb1a InitializeMusicTypes:
  - What it does: creates the global atmos CSymbolMap once and loads `GetDefsDirectory() + L"atmos_types.h"`.
  - Shape: the size form of the zero store is `and [g],0`. Arguments are evaluated right to left, so the literal is pushed before the directory temporary.
- 00cbeb7e IsCutsceneSkipped: an if/else with one `if (call()) return true;` per arm and a shared `return false`. A ternary of the two calls gives `setne` instead (37 v 41).
- 00cbed00 AreAllThingsInVectorDead / 00cbed48 AreAllThingsInVectorAlive:
  - Counted loops: begin is read once and end is re-read every pass, because the virtual calls may change it.
  - `count == size()` goes through `sete`; the size TU emits the 12-byte element divide as `push 0Ch ; pop ecx ; cdq ; idiv`.
- 00cbed82 KillAllThingsInVector: the interface is held in ebx, and the bool is re-read from its stack slot for every RemoveThing call.
- 00cbeeb0 ResetGlobalScriptDef: the CDefPointer-family counted release (the landed 0x0042AEDA model), with two inline accessors on the +4 count base.
- 00cbeed2 IsPlayerThreateningEntity: an `&&` returned as int. The right operand's CScriptThing temporary exists only on that path, so VC7.1 keeps a "constructed" flag word at [ebp-4].
- 00cbef30 CreateAndFadeInCreature / 00cbefc1 CreateAndFadeInObject:
  - The fade loop's float locals are homed in dead argument slots.
  - Starting from 0 folds the `while (alpha < 1.0f)` entry test away.
  - VC7.1 has no NRVO here: the local is copied into the hidden result.
- 00cbf050 CutsceneMacro_ReadLightScene:
  - GetLength() is the zero-reusing inline `data == 0 ? 0 : data->length`.
  - The scan needs its own `for` index starting at the colon.
  - The colour state is an unsigned char homed in the frame.
- 00cbf679 MoveAllThingsInVectorToPosition: one resource-object local for the whole loop, built before begin is read and destroyed after the loop.
- 00cbf9de CSFindThing:
  - The inline CCharString::operator==(const char*) of the landed 0x00E6A000, with both levels `__forceinline`.
  - An `actor && actor->IsScripting()` IsScriptingActor helper gives the `test esi,esi` on the value address.
- 00cbfaca CSExtractParameterList:
  - The `""` literal is hoisted into esi.
  - `current = current + c` builds a frame temporary, which is assigned and then destroyed.
  - size() of a 4-byte vector is `sar esi,2`.

### Cutscene-director STLport instantiations and data-marker readers
- 00cd2919 / 00cd29f0 / 00cd344a `_Rb_tree::_M_erase`, for set<CCharString>, map<long,CCharString> and map<CCharString,CScriptGameResourceObjectScriptedThingBase>:
  - The source is the STLport 4.6 `_tree.c` body under `#pragma inline_recursion(on)`, with the real allocator chain: `_STLP_alloc_proxy` -> `allocator` -> `__malloc_alloc` -> free. That chain is what caps the expansion at two inline levels, matching retail's three nested loops.
  - The value is destroyed with `p->~T()`. For the long map this goes through the pair's `??_G` (0xCD22C0).
- 00cd378d `vector<CScriptGameResourceObjectScriptedThingBase>::_M_fill_insert`:
  - The callee helpers that take a tag get same-TU noinline bodies, so the tag temporaries share the dead argument slot [ebp+0Bh].
  - `__copy_backward_ptrs` is inline over an out-of-line `__copy_backward`.
- 00cd3a16 `map<long,CCharString>::operator[]`:
  - The `__default_constructed` string is homed in the dead key slot.
  - The hint iterator is passed by value through a user copy constructor.
- 00cd3e7b `vector<CScreenFilterSThingByPass>::push_back`: `_Construct` is a placement new with the null test, and a `__false_type` tag is homed in the dead slot.
- 00cd4ff0 GetStringFromDataMarker / 00cd5090 GetNumberFromDataMarker:
  - An index loop where size() is recomputed at the bottom and every match assigns, so the last match wins.
  - The element-reference local `const CElement& element = data.Elements[i]` gives the strength-reduced ebx + esi address.

### Quest-script factories and constructors (NScript)
- 00cdbd20 pSunnyvaleDataAlloc_func:
  - `new NScript::CQ_SunnyvaleMasterData` (0x144 bytes), with the CScriptDataBase base ctor (0x99A2F0) out of line.
  - The CCharString TeddySolution member (+0x54) and the BoastRegions vector (+0x134) are constructed inline.
- 00cdeb40 CreateArcheryCompetitionSignpost:
  - An entity-script ClassAllocFunc returning `CCountedPointer<CEntityScriptBase>` by value (hidden result in ecx).
  - The CEntityScriptBase / CArcheryCompetitionSignpost ctors are inline (the root-class trick for Game). The CCountedPointer ctor 0xCDEDB0 is out of line.
- 00cef950 NScript::CGameflowScript::Alloc / 00dc24e0 NScript::CQ_OakValeBanditRaidScript::Alloc:
  - Static `__fastcall Alloc(data /*ecx*/, game /*edx*/)` doing `new` and an inline ctor over the out-of-line CScriptBase ctor 0xCB8110.
  - Game (+0x40) and MasterScriptData (+0x44) live in a `__declspec(novtable)` CLevelScriptBase<T> intermediate.
  - 00dc24e0 also inlines a CScriptThing member: CBaseClass ctor 0x99A2D0, then the vtable store, then PImp {0,0}.
- 00cfad30 / 00d7acb0 / 00d84b50: the CQ_AwakeningTheOracleScript, CQ_HeroSoulsThunderScript and CQ_HeroSoulsGuildmasterScript ctors.
  - Each inlines STLport vector and `_Rb_tree` constructors: the header node comes from malloc 0xBFEA0E, then colour, left and right are stored at the header.
  - Retail layout = Ego_r.pdb offset − 0x14 (Dinkumware vs STLport container sizes).

### Quest-script members and their STLport helpers
- 00d18c00 `vector<NScript::CQ_BountyHuntScript::TeleportData>::push_back`: the TeleportData copy ctor must be declared out of line (it is 0xD18C50). If the implicit one is used, VC inlines it into `_Construct`, which then stays out of line (70 v 67).
- 00d258c0 CQ_DragonBossFightScript::SetDragonState:
  - An if/else-if chain over the four EDragonState values.
  - Each arm opens with the translator's `if (IsActiveThreadTerminating()) return;` and then loads two scriptdef longs into TargetNumMinions (+0x68) and TargetNumSummoners (+0x6C).
- 00d34f00 `_STL::vector<bool>::vector(const vector<bool>&)`:
  - `_Vector_base(n, a)` (0x450CCC) is called out of line, followed by `__uninitialized_copy`.
  - The helper needs an explicit `__fastcall`, because the lane has no /Gr.
  - Fable's vector<bool> is not specialised; it is a vector of bytes.
- 00d34f40 `__uninitialized_copy<bool>`: the STLport `__copy_trivial` `(last == first) ? result : memmove(...) + n` form gives `cmp ; je`. The `n ? ... : result` form gives `sub ; je`.
- 00d34fd0 `__copy_ptrs<CFireHeartPatternDef>`:
  - The source is a counted assignment loop, with `__copy` marked `__forceinline`.
  - The element's implicit operator= calls CDefClassBase::operator= (0x431F10) and vector<bool>::operator= (0x462E78).
- 00d38400 CQ_FireHeartScript::TriggerProphetComment: `ProphetComment = "TEXT_QST_B01_PROPHET" + GFIntToCharString(Freed + Killed + 1) + "_" + comment`, then `ShouldProphetSpeak[Freed + Killed] = true`.
- 00d38f30 `_STL::vector<long>::vector(size_type n)`: an out-of-line `_Vector_base` (0x50FA40) and an inline `uninitialized_fill_n` of 0. VC7.1 turns the fill into `rep stosd`.
- 00d9f070 CQ_MinionCampScript::ShouldSpawnPointSpawn:
  - Nested ifs ending in `if (IsActiveThreadTerminating()) return false; return true;`, which gives the byte-wide `neg al ; sbb al,al ; inc al`.
  - `return !IsActiveThreadTerminating()` gives a full-width `sbb eax,eax` instead.
- 00da01e0 `_STL::list<CScriptThing>::push_back` / 00da0200 `_M_create_node`:
  - push_back is `insert(end(), x)` with insert inline and `_M_create_node` out of line.
  - The node allocator is malloc (0xBFEA0E).
  - The placement new tests the element address (`lea ecx,[esi+8] ; test ecx,ecx`).
- 00da0d90 CQ_MinionCampScript::FindFurthestSpawnPoint:
  - A const_iterator loop with terminate guards.
  - `distance > furthestDistance || furthestDistance == -1.0f`: the `== -1.0f` becomes an integer compare against 0xBF800000.
  - GetDistanceBetweenThings is 0xCBE583.
- 00dc0830 CQ_OakValeBanditRaidScript::DepopulateLevel:
  - A local `vector<CScriptThing>`, then `Game->GetAllThingsWithScriptName("OakValeBanditRaidBandit", v)` (slot 75) and `KillAllThingsInVector(*Game, v, false)`.
  - The vector destructor is inline (destroy loop, then free).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
Catalog names that are wrong, mostly propagated across byte-identical bodies. The PDB name or the STLport spelling is now on line 1.
- Quest-helper globals:
  - 0xCBEFC1 "CreateAndFadeInCreature" -> CreateAndFadeInObject
  - 0xCBF679 "KillAllThingsInVector" -> MoveAllThingsInVectorToPosition
  - 0xCBF9DE "GetCutsceneActorScriptThing" -> CSFindThing
- Cutscene-director STLport rows:
  - 0xCD2919 "Particle_DestroyRecursiveTree" -> `_Rb_tree<CCharString,...>::_M_erase`
  - 0xCD29F0 "CLandscapeBackgroundPatch_DestroyRecursiveTree" -> `_Rb_tree<long,pair<const long,CCharString>,...>::_M_erase`
  - 0xCD344A "StdMap_DestroyNode" -> `_Rb_tree<CCharString,pair<const CCharString,CScriptGameResourceObjectScriptedThingBase>,...>::_M_erase`
  - 0xCD378D "CActiveFile_LoadAndInsertMeshArray" -> `vector<CScriptGameResourceObjectScriptedThingBase>::_M_fill_insert`
  - 0xCD3A16 "CPersistTraits_TransferInHelper" -> `map<long,CCharString>::operator[]`
  - 0xCD3E7B "Vector_EmplaceBackWithCapacityCheck" -> `vector<CScreenFilterSThingByPass>::push_back`
- Quest-script rows:
  - 0xCDEB40 "CFileInstaller_CFileInfo_CreateInstance" -> CreateArcheryCompetitionSignpost (the entity-script creator)
  - 0xCEF950 "Alloc" -> NScript::CGameflowScript::Alloc
  - 0xDC24E0 "Alloc" -> NScript::CQ_OakValeBanditRaidScript::Alloc
  - 0xCFAD30 / 0xD7ACB0 / 0xD84B50 "CHeroMorphDef_Create[_2/_3]" -> the CQ_AwakeningTheOracleScript / CQ_HeroSoulsThunderScript / CQ_HeroSoulsGuildmasterScript ctors
    - The label is propagated from the CScriptBase ctor 0xCB8110, which is itself mislabelled "CHeroMorphDef".
  - 0xD18C00 "CQ_BountyHuntScript_EnqueueTeleport" -> `vector<TeleportData>::push_back`
  - 0xD258C0 "CScriptBase_InitCheckThread" -> NScript::CQ_DragonBossFightScript::SetDragonState
  - 0xD34F00 "Map_InsertOrUpdatePair" -> `vector<bool>::vector(const vector<bool>&)`
  - 0xD34F40 "Map_MoveOrCopyValue" -> `__uninitialized_copy<bool>` (the trivial form)
  - 0xD34FD0 "std_vector_CConversation_CopyElements" -> `__copy_ptrs<CFireHeartPatternDef>`
  - 0xD38400 "CTCGameModeController_UpdateSunCount" -> NScript::CQ_FireHeartScript::TriggerProphetComment
  - 0xD38F30 "std_vector_AllocatePointers" -> `vector<long>::vector(size_type)`
  - 0xD9F070 "CScriptBase_CheckThreadTerminated" -> NScript::CQ_MinionCampScript::ShouldSpawnPointSpawn
  - 0xDA01E0 "NScript_CDanyalTestScriptsData_InsertHead" -> `list<CScriptThing>::push_back` (it appends at end())
  - 0xDA0200 "Alloc" -> `list<CScriptThing>::_M_create_node`
  - 0xDA0D90 "CScriptBase_RunActiveSpawnPoints" -> NScript::CQ_MinionCampScript::FindFurthestSpawnPoint
  - 0xDC0830 "CQ_OakValeBanditRaid_CleanupBandits" -> NScript::CQ_OakValeBanditRaidScript::DepopulateLevel
- Callees:
  - 0xD18C50 "TeleportData::operator=" -> TeleportData copy ctor
  - 0xD18CA0 "TeleportData_OnReadFinished" -> `vector<TeleportData>::_M_insert_overflow`
  - 0x450CCC "Buffer_InitializeWithSize" -> `_Vector_base<1-byte>(n, a)`
  - 0x50FA40 "std_vector_allocate_ptrs" -> `_Vector_base<4-byte>(n, a)`
  - 0x431F10 "CThingBuildingDef::operator=" -> CDefClassBase::operator=
  - 0x462E78 "CSimpleMemoryPool_AssignFrom" -> `vector<bool>::operator=`
  - 0xD25880 "CBS_Teacher::Init" -> CDragon::Init
  - 0xCBE583 "CScriptBase::Activate" -> GetDistanceBetweenThings
  - 0x99A2F0 -> the CScriptDataBase / CBaseClassNonCopyable ctor
  - 0x99F3B0 "ResizeGameMap" -> CCharString::Append
  - 0x41A080 "GetAutoSaveFileName" -> GetDefsDirectory
  - 0xCD2AE2 "operator=" -> CScreenFilterSThingByPass copy ctor
  - 0xCD2AFD -> `fill`
  - 0xCD2C7C -> `__copy_backward`
  - 0xCD22C0 -> the pair `??_G`
  - 0xCD2B86 -> `map::_M_find`
- Already-landed sources whose names look wrong:
  - CTCCarryable_OnKill_00cd2d8b / _00cd2ddc are `_Rb_tree::clear` bodies.
  - CActiveFile_OnReadFinished_00cd31d2 / _00cd3211 are tree destructors.
