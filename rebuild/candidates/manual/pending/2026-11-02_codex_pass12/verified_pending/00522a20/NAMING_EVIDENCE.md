# CThingManager::InitThingTypes identity and operand evidence

The original Ego_r procedure is `CThingManager::InitThingTypes`, module release\thing_manager.obj, stream446 record0x5028, void thiscall with no parameters. Original public class member spelling is `?InitThingTypes@CThingManager@@QAEXXZ`. FableWin independently contains the same procedure, module debug\thing_manager.obj stream443. Native CThingManager constructor00523540 calls the assigned leaf at005236d4. Native2589-byte span ends at0052343c ret followed by alignment and the next function00523440; it contains one complete function. objdump -t confirms exactly one defined leaf with that spelling.

## Registration types and members

Original TPI records prove CClassTypeInit (class key V) fields TypeID+0, AllocFunc+4, Local+8, Name+c, full16; CThingTypeInit derives publicly from it, adds ThingTypeFlags+10 and DefClassType+14, full24. Original constructors accept respectively `(long,CClass*(__fastcall*)(),bool,const CCharString&)` and `(long,const CCharString&,unsigned long,CClass*(__fastcall*)(),bool,const CCharString&)`. Native stores and strings at each registration independently confirm these fields, flags and constructor order. CThingTypeInfo is classV with long DefType, ulong ThingTypeFlags, CCharString DefClassType and native12-byte stride. CClassTypeInfo is classV, with original TypeList/NoAllocated/Used/Local/AllocFunc/Name; real STLport list4 makes its native stride20. CThingManager's full0xe8 model retains all fields; only ThingTypeInfo+3c and UpdateLists+8c become their original typed vectors for this opt-in. CClassFactory remains full1c with real ClassTypeInfo vector at+4.

## Callbacks

All14 emitted callback symbols are exact original PDB name-set entries `?Alloc@<Class>@@SIPAVCClass@@XZ`: public static fastcall returning CClass*. Raw TPI proves the same access, method kind, convention and return class key. Each native allocation size and following original constructor/RTTI confirms the class independently; the literal registration name and type id are a third check. No callback implementation is authored by this row.

|Native|Class|Native sizeof|Native constructor evidence|Registration literal|
|---|---|---|---|---|
|0052b880|CThingPlayerCreature|0x208|0052ab20|PlayerCreature|
|005272e0|CThingAICreature|0x1d8|00831f80|AICreature|
|0052ac70|CThingVillage|0xe0|005299c0|Village|
|0052ac10|CThingBuilding|0xd8|005296b0(type3),vtable0124509c|Building|
|00527320|CThingObject|0xe0|007031c0|Object|
|00527340|CThingSoundEmitter|0xb8|008367b0|Sound Emitter|
|0052ac90|CThingHolySite|0xd8|005296b0(type7),vtable012452dc|Holy Site|
|0052ace0|CThingSwitch|0xe8|00529b00|Switch|
|00527360|CThingTrackNode|0xf0|00837c70|TrackNode|
|0052ad00|CThingMarker|0xd0|005296b0(type11),vtable01245524|Marker|
|0052ad50|CThingPhysicalSwitch|0xd8|005296b0(type12)|Physical Switch|
|00527300|CThingDeadCreature|0xf0|00835c20|Dead Creature|
|00527380|CThingNoise|0x98|00838530|Noise|
|004cd4d0|CThing|0x98|004c9030(type15)|Thing|

## Remaining callee and data identities

- 0099e4b0/0099ebf0/0099ec30/0099eae0 are the original CCharString default/text/copy constructors and destructor; 0099efb0 is `??4CCharString@@QAEABV0@ABV0@@Z`. Every symbol is in the original PDB sets. The native calls have the appropriate this address and argument count; text construction takes -1 length, copy and assignment take an original CCharString address.
- 00a36f90 is protected virtual CClassFactory::AddNewClassType(const CClassTypeInit&), `?AddNewClassType@CClassFactory@@MAEXABVCClassTypeInit@@@Z`. PDB exact symbol and original TPI access/type agree with native base-this and24-byte derived init record passed as the original16-byte base. The native direct call implements the explicitly qualified base call, not a free callback.
- 0052bea0 is original STLport `vector<CThingTypeInfo>::resize(unsigned int,const CThingTypeInfo&)`. Existing binding is exact; native receiver+3c and twelve-byte temporary with CCharString at+8 confirm element and overload. `_vector.h`4.6.2 proves the one-argument inline call constructs `_Tp()` then invokes the const-reference overload.
- 00529f50 is `std::vector<ThingLList>::erase(ThingLList*,ThingLList*)`;0052b150 is its protected `_M_fill_insert(ThingLList*,unsigned int,const ThingLList&)`. Real STLport4.6.2 declarations provide access and signatures; native receiver+8c, element stride8 and two/three pushed args confirm their roles. This row does not implement either non-inline callee.
- 01245bb0 is function-local static `ThingLList dummyList`. This is not an invented global: Ego_r thing_manager.obj local-data record0x50a0 immediately within the original InitThingTypes procedure names `dummyList`, type0x19709 ThingLList, section2 offset0x33bd0. Raw TPI proves original struct keyU and Head/Tail pointer members. Native referenced data are exactly eight zeros, matching `{0,0}`. objdump confirms the emitted static symbol `?dummyList@?1??InitThingTypes@CThingManager@@QAEXXZ@4UThingLList@@A`.

All188 operands checked:145 calls,28 literal operands compared byte-for-byte,14 callback addresses,one static object. Every nonliteral symbol was checked against reloc_bindings.tsv for both target spelling and uniqueness; five existing wrong target bindings are reported in NAME_FIXES.tsv. No correct symbol binds another retail address.

## Headers and tests

The real existing inheritance headers are reused. Newly needed callback-only class headers retain their full native allocation sizes and original base classes. Every named derived member is individually represented, using its original name/type and proven native offset; unobserved composites are opaque per member. The native-only SoundEmitter pointer at+98 remains unnamed storage because all three donor PDBs omit it. See HEADER_LAYOUT_EVIDENCE.md. AICreature's supplied default branch is byte-for-byte preserved in the else arm. Existing class changes are isolated under FABLE_RETAIL_INIT_THING_TYPES; no old branches are changed.

Self-contained C++03 fixture pastes the actual registration body and constructor/AddNewThingType logic. Sixty combinations cover initial class counts0/1/8/16/20, update-list counts0/2/16/24 and type-list counts0/16/20. Each repeats registration, for120 full invocations. It checks every id, flag, name, definition category, callback identity, Local=false, resize growth/shrink, zeroed new update-list elements, preserved existing list pointers, and untouched DefType/unregistered categories. Both GNU64 and VC7.1 execute PASS.

Main compliance correction: current selected source enables FABLE_RETAIL_UI_ORDINARY_INLINE. Any inherited forced annotations in the relevant helper closure use ordinary inline for this feature; their default branches are retained. Main recompiled the target exactly, checked every operand unchanged, and reran both current source-bound fixtures.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

## CResource original field access correction

Both original TPI records prove CResource::ResourceList,PrevResource,NextResource,ResourceSize,LastUsedFrame are private at offsets8,12,16,20,24. The opt-in shared header restores that access, keeping every original field and the entire default branch unchanged. Source includes FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS. Actual VC7.1 preprocessing confirms those fields and the earlier private IVRefCount and private CDefPointer::Object declarations. See RESOURCE_ACTIVE_DECLARATIONS.json and the vendor original signature matrix at workers/vendor/BASE_DECLARATION_AUDIT/REPORT.json. Fresh byte and dual-compiler fixture checks pass; independent main acceptance is pending.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.
