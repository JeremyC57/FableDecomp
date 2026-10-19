# Retry201C native and declaration audit

All four original bodies end at their own RET; no overcapture was found. Original201 selected files are comment-only placeholders, as recorded by HISTORY_AUDIT.tsv and SELECTION_AUDIT.json. No additional authored history was found. This section retains four UNCERTAIN historical reservations per row; the new grades are 3/4/5/2 for 009F50A0/009F5170/009F5BD0/00691C50.

## Shared manager/event representation

CInputManager and CInputEvent headers do not exist under rebuild/include, including quarantine. The actual available donor headers are retained whenever their types matter: CDiskFileWin32 for manager storage, CMouse/CJoystick for checksum device projections, fable_filesystem.h/fable_string.h for the constructor, and input/camera/world/processed headers for movement.

Donor CInputManager is0x90: vectors16 bytes. Native ctor proves vectors12, resulting manager0x80: ProcessedEvents4, EventStore10, NoStoredEvents1C, NoEventScansRunning20, PMouse24, PKeyboard28, PJoysticks2C, Loading38, Saving39, Checksum3C, LoadedEventPackages4C, CurrentFilePackageIndex58, TimeStamp5C, PackageFile60, NoUpdates7C. Fixed-width int fields project native32-bit long storage on the full-width host. Native/donor event size0x34: Data32 opaque UInputEventData bytes, DeviceType20, DeviceNumber24, Type28, CurrentTime2C, StartTime30. No byte injection is used.

Native RTTI129C79C independently proves CInputManager->CBaseClass. Authentic destructor slot0, SetExclusiveMode(bool)slot1=9F4E70, Update()slot2=9F57A0. Both non-constructor TUs now declare the real CInputManager(const CInputManagerInit&,bool) dependency, stubbed with that same signature in fixtures. Their tests do not add a default engine constructor. Init has three bools UseMouse/UseJoystick/UseKeyboard.

## 009F50A0

PDBegorB48B60 proves bool IsKeyboardEventInQueue(EInputEventType,EInputKey,CInputEvent&) const; B48940 proves bool IsEventInQueue(const CInputEvent&)const. Native direct call9F4D40 searches using device/type/key and overwrites the passed event on success.

All direct relocations audited: event constructorA04410; keyPressedA03BF0(EInputKey,double); keyHeldA03C20/keyReleasedA03C50(EInputKey,double,double); search9F4D40; event destructorA03B30 twice. All five double-pool relocations are literal0, native0122ED70 raw0000000000000000. No allocator, global, VT or RTTI relocation occurs inside the query body.

Fixture executes all three event cases plus null/unsupported types, both search results, actual passed zero times, full event copy, output unchanged on failure, and temporary lifetime counts. Queue matching itself is a stub dependency; this is not an integration test of9F4D40. Native PMF4 and manager/event offsets retained. The declared real manager ctor is stubbed.

## 009F5170

PDBegorB48C20 proves unsigned long CalculateInternalChecksum()const; event B6EC80 proves unsigned long GetChecksum()const. Current final source retains both unsigned-long signatures while using unsigned32 accumulator storage on full-width host.

Actual donor CMouse/CJoystick headers are included under aliases. RTTI129DBC4 proves mouse->CBaseClassNonCopyable->CBaseClass; RTTI129C0EC proves joystick has no base. Authentic virtual prefixes have no invented slots: mouse destructor0/GetPos(C2DVector*)const1; joystick destructor0/SetExclusiveMode(bool)1/Update()2/GetXAxis()3/GetYAxis()4. Native DX table129EA14 slot1=AB46E0, donor egorC54F80 signature proves void GetPos(C2DVector*)const. Native joystickDX129EA7C slots3/4=AB6AD0/AB6AE0; donorC570C0/C570D0 proves non-const float GetXAxis/GetYAxis.

Direct body relocations only GetChecksumA04160, compiler helper_ftol2BFEA70 twice, and float2^32 pool122DCB4 raw0000804F twice. BFEA70 is the actual local compiler conversion helper, not an import allocator. Virtual calls use the authentic declarations above. No allocations, frees, globals, VT/RTTI initialization or strings appear in this method.

C2DVector default is empty; explicit C2DVector(float,float) zero initialization reproduces the four local stores. Both authentic constructors are evidenced by fablewin1898E30/18B5310. Real iterator objects in *(begin()+n) are needed for native movEAX/leaECX element addressing. Raw Start[n] differs by one instruction byte.

Fixture covers absent devices, two event checksums, two sequential unsigned-to-float/truncation additions, signed axes, unsigned high bit, event-sum wraparound, NoUpdates and re-reading joystick Data after the X-axis callback changes it. Native/full-host virtual dispatch and PMF4 are checked. It does not execute actual input hardware or A04160.

## 009F5BD0

Native RTTI122D06C proves CDiskFileWin32->CAFile->CBaseClassNonCopyable->CBaseClass; file layout0x1C unchanged. Actual flat donor header remains an alias; native view retains genuine hierarchy and members, plus actual CWideString as Pathname. Actual fable_filesystem.h statics-only CAFile is retained under donor alias; it is not used as an invented polymorphic base. Native CAFile is abstract/novtable; its implicit initialization invokes the existing CBaseClassNonCopyable ctor99A2F0. Disk ctor is actual inline initialization, not a test-only constructor.

Native body direct relocations: CBaseClass ctor99A2D0, NonCopyable ctor99A2F0, CWideString ctor99AED0, vector<CInputEvent>::erase9F67E0 twice. VTs manager129C79C, file122D06C. Both empty erases receive begin/end pointer values, and counts/flags/time/update stores follow them. CurrentFilePackageIndex and file AccessType/Position/Length/Handle are not initialized by this body; fixtures do not read their indeterminate values.

All16 disk VT slots independently match native addresses and real PDB signatures:
0 destructor4016B0;1 Open99A6A0(const CWideString&,FileAccessType,unsigned long);2 Close99A920();3 Read99A960(void*,long,CASuspendableProcess*);4 Write99AA90(const void*,long,CASuspendableProcess*);5 SetPosition99ABF0(long);6 MovePosition99AC90(long);7 GetPosition99ABD0()const;8 SetLength99A560(long);9 GetLength401670()const;10 IsOpen401680()const;11 GetPathname401690()const;12 inherited IsIOFinished401650()const;13 inherited GetAlignment401660()const;14 Clone99AE00()const;15 GetFileInformation99A5A0(). Last returns the genuine CFileInformation type, left incomplete in production since body does not use it. Fixture completes its PDB fields only to define the stub.

Winning vector end-of-storage representation is an allocator proxy: NativeStorageEnd<T>:NativeAllocator<T>, holding the genuine pointer Value, initialized from Start. It introduces no padding or invented alignment. This retains the native12-byte vector, and changes scheduling so LEA file comes between final Finish/End pointer stores. Flat pointer triples (initializer list or ctor body) emitted identical161-byte bodies with this one scheduling difference; /Oa did not fix it.

No target-body allocations occur. Compiler-generated scalar deleting thunks separately reference real scalar operator delete; they are not confused with constructor logic or native malloc/new[] imports. Generated disk/manager VT records are preserved in logs/009f5bd0.relocations.txt. Fixture checks all8 init flag combinations, empty vectors/pointers/checksum/counters, existing dependency constructor counts and two empty erases, pathname/Opened, real manager/file virtual dispatch, hierarchy offsets and PMF4. Dependency stubs are not full filesystem implementations.

The earlier ctor metadata-only edit changed comments only; logs/009f5bd0.metadata-verification.pregrade.json proves identical non-comment lines to matched snapshot4, records full command/hash/flags, and its subsequent grade remains RELOCATION_MATCH. It is a metadata verification, not a new source/flag attempt.

## 00691C50

PDBegor6BDB00 proves virtual void ProcessInput(const CInputEvent&,CProcessedInput&); egor6B2E10 proves GetControlMovementFromGameActionEvent(EGameAction,const CInputEvent&,C2DVector&)const. EGameAction GAME_ACTION_MOVEMENT=60 is recorded in controller_enums_pdb.txt. Direct native callee687D00 forwards to real game interface445C30.

RTTI1237F74/125BE98/129CA10 proves watcher->CInputProcessGameBase->CAInputProcess->CBaseClassNonCopyable->CBaseClass. Actual donor headers retained under aliases; genuine base pointer fields and CLinkedListPosition modeled, with authentic virtuals destructor/AddToInterface/Init/Cleanup/ProcessInput/PreparePrimitivesForEngine/Update/ProcessWhilePaused/IsInputEventToBeSentToThisInputProcess. Sizes0x14/0x28/0x30 and LastJoyDirection28 asserted natively.

CWorld actual PCurrentGameCamera34 is accessed by name. Authentic CAIGameCameraBase virtual prefix0..9: destructor, GetRenderCamera(float)const, PreparePrimitivesForRendering, IsPosInViewingFrustrum, Is2DAreaInViewingFrustrum, IsVolumeInViewingFrustrum, IsLineOfSightToPos, GetControlAngleXY, SetChangeInControlDirection, non-const GetCameraManager(CGameCameraManager**). Native manager slots8/9=697750/69E1E0; donor fablewin21FC860/21FC890 confirms signatures. Native target69E1E0 writes this to the output and returns true; native base6970C0 returns false.

Native camera manager prefix differs from donor because CamTools.ScanThings vector16->12 and AvailableCamPoints list12->4: CamTools78, CurrentMode98, PendingModeA0, flagsA8-AA, Available listheadAC, SingleCutB0, ChangeInControlDirectionB1, InsideHeroCountB4, ControlAngleXYB8, AngleChangeBC, ControlNoControlMovementBD. Native ctor69AE80 independently proves these; SetChangeInControlDirection697750 writesB1, GetControlAngle697700 readsB8. Therefore the zero/tiny movement storeBD is ControlNoControlMovement, not the donor-offset ChangeInControlDirection. Actual full donor manager header is retained; native prefix is a genuine typed field projection without arbitrary pad bytes.

All constant relocations independently read from PE:129BA3C raw17B7D138=0.0001f,122DED8 raw0000803F=1.0f,125C0B8 raw3333733F=0.95f. Length must be ordered greater than epsilon; dot change requires ordered less than0.95; equality and NaN follow native x87 masks. Only direct body callee687D00; no imports/allocators/strings/globals/VT initializers. sqrt is intrinsic fsqrt.

Fixture executes no-action, normalized(3,4), repeated direction, dot threshold equality/below, zero/tiny/exact length threshold, missing camera-manager interface, NaN and unchanged processed output. It dispatches through real inherited camera/input virtual types and checks base offsets plus native PMF4. It stubs game-action decoding and camera-manager lookup. One fixture compile failed because variable gets conflicted with CRT gets; both failure logs and original fixture are preserved. Correcting that fixture-only identifier did not change candidate source or consume another source/flag grade.


## Root159 declaration consistency corrections

Query009F50A0 grade3 and ctor009F5BD0 grade5 consistently declare unused event GetChecksum/manager CalculateInternalChecksum as unsigned long, matching the actual PDB signatures already used by checksum TU. Both remain RELOCATION_MATCH; all snapshots and pregrade records preserved. Refreshed exact-source host98/O2 and VC/O2 fixtures pass.

Ctor FileAccessType labels FILE_READ=1 and FILE_WRITE=2 are descriptive labels, not claimed recovered PDB enumerator spellings. Numerical evidence is native: Open99A6A0 switches on access-1 at99A744, table99A910 entries map1->99A75B and2->99A861. Access1 pushes GENERIC_READ0x80000000 and OPEN_EXISTING3 before CreateFileW IAT143FE2C at99A773. Write99AA90 explicitly requires AccessType2 at99AAF2. Copy constructor99ADC0 pushes flags2, access1 then other.Pathname into Open at99ADE8. These independent native checks substantiate values1/2; no exact donor PDB numeric enum excerpt is available. Read99A960 itself calls ReadFile IAT143FED0 without checking AccessType1; do not misread its cmpEAX1 return-value check as an enum check. Native excerpts retained in logs/FileAccessType_*.
