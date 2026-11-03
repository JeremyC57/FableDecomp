# 008edff0 identity and operand evidence

The selected symbol is `??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z`: original public class constructor, enum ability by value, mutable CThing reference. Raw Ego_r public record0x78279c names the constructor at donor section1+0x512620; the procedure from release/will_button_helper_timer.obj is1181bytes and supplies the exact prototype. FableWin debug/will_button_helper_timer.obj independently names the same signature and its unoptimized scopes. Native callers at78ed16,794216,7a09fe,7c677f,7c880f construct this object while passing an ability and creature reference. The COFF table of attempt09 contains exactly this complete symbol, and the strict grader selected it exactly.

## Original owner and members

Ego_r TPI2ed53 is the complete class, size56; fields are private ButtonHeldDownTimer, WillAccumulatedLevel, PCreature, MyAbility, CurrentState, WillPowerUpAccumulatedTimings, MaxNumberLevels, CurMaxLevels, WillIncrementedFlag, UpdateAccLevel, IsAICasting, IsAIHoldingDown, LevelToCast. Native initializes all of these at offsets0,4,8,10,14,18,24,28,2c,2d,2e,2f,30 (hex). The only size adjustment is the original std::vector<float> from donor16 to retail12bytes; resulting retail sizeof52. Raw EHeroAbility enum0x289a and nested EButtonHeldState enum0x2ed43 establish the exact enumerator names and values; the native13-byte dispatch translation and all six switch targets agree.

The full four definition classes derive CParentDefClassBase, as their original TPI records show. Every original data member is retained. Retail LevelUpTimings offsets are50 ForcePush,5c BulletTime,150 Berserk and7c Enflame; each matches the actual memory operand. Source definitions retain their exact native data type (three inline floats in three classes; std::vector<float> in Berserk). No fake prefix class was introduced.

## Callees and globals

Raw exact publics and types are saved in workers/engine/evidence/008edff0/EXACT_SYMBOLS.json and WILL_TYPES.json, with original procedure locals in evidence/function_locals_CWillButtonHelperTimer_CWillButtonHelperTimer.json. Each address below also has direct native call/data evidence in this row; historic binding labels were checked before declaration. NAME_FIXES.tsv enumerates conflicting landed spellings instead of adopting them.

| Retail target | Correct identity and second evidence |
|---|---|
|00a01b10|CBaseIntelligentPointer public default constructor; native operates on embedded PCreature+8.|
|01238c6c|CIntelligentPointer<CThing> vtable; native constructor stores it after the base constructor; original instantiated class key isV.|
|00a01b90|CBaseIntelligentPointer::SetPItem(CBaseObject*); native passes the creature to the embedded pointer.|
|00a01b50|CBaseIntelligentPointer::GetPItem() const returning CBaseObject*; native loads the creature before its interface test.|
|004365b0|Const CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&); native uses const PeekTC and compares key111, retaining const pair-pointer return. Distinct from nonconst40f020.|
|005edea3|CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility) const, long return; native passes MyAbility and converts long result to float.|
|005ede93|CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) const, long return; same native argument and conversion. These calls do not belong to CTCHeroStats.|
|0099ebf0|CCharString(const char*,long); native uses-1 length and one of the four exact definition literals.|
|0099eae0|CCharString public destructor; native destroys the class-name temporary after lookup.|
|0044c6b0|Static fastcall CGameDefinitionManager::Get(), mutable reference return; native calls before inherited manager members.|
|009ad390|CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&,long) const; second argument is long1, not a bool. Raw TPI9fae confirms the entire signature.|
|00798c10|CDefinitionManager::GetDef<CSpecialAbilitiesForcePushDef>(long,CDefPointer<const CSpecialAbilitiesForcePushDef>*) const; native literal, resulting50timing load and original scoped def local agree.|
|0079dbb0|GetDef<CSpecialAbilitiesBulletTimeDef>; native literal,5ctiming load and original scoped def local agree.|
|0078da50|GetDef<CSpecialAbilitiesBerserkDef>; native literal,150vector and original scoped def local agree.|
|007a5080|GetDef<CSpecialAbilitiesEnflameDef>; native literal,7ctiming load and original scoped def local agree.|
|00bfea70|Compiler float-to-long runtime helper __ftol2; native computes the signed level-count limit.|
|01375550|Protected static long CMainGameComponent::ConstantFPS; accessed through original static fastcall GetConstantFPS. Raw PDB exact spelling and both native integer-multiply/default and fild/float timing uses agree.|
|004408a0|Protected std::vector<float,std::allocator<float> >::_M_insert_overflow(float*,const float&,const std::__true_type&,unsigned int,bool); actual STLport4.6 true-tag signature and native five outgoing arguments/capacity path agree.|

The indirect release call uses original const CIVCountedPointeeBase::Delete at vtable slot1; native dispatches through+4 after decrementing count+4. Existing genuine headers are reused. Original header bodies for LowerBound, GetDef and vector overflow are visible in the TU, allowing the compiler to keep escaped locals and empty tags in the correct slots. These are real algorithms over original types, with read-only inputs, not arbitrary data-touching bodies.

## Extent and complete verification

Assigned span8edff0..8ee4a1 holds the constructor plus its generated switch data. Its1201bytes exactly match after relocation masking. All58 target operands were then checked from raw COFF: exact external target and addend, all four terminated literal strings, the switch byte table reference, six dispatch targets and dispatch-table reference. RELOCATIONS.json records each operand. This is a complete operand audit, not just a masked-call comparison.

The standalone test copies the selected constructor body exactly. It exercises all21 enum values, absent inventory/default3levels, inventory maximum-1..3, current0..3, every special definition, ordinary timers, both zero-level abilities, exact initial flags, ability calls, definition lookup counts, cumulative thresholds and balanced temporary references:441cases. Tests pass GNU64 C++03/noRTTI and actual VC7.1/GR-. Test doubles isolate dependencies; the semantic fixture does not claim to verify native field offsets. The production header assertions and native byte match establish those separately.

A compliance audit found the inherited CThing/CDefPointer forced-inline spelling was supported only by native expansion, which does not prove the original annotation. Row-specific ordinary-inline branches preserve the exact1201-byte result at grade14; defaults were preserved.

Final main compliance check disables unsupported inherited forced-inline annotations only for this row feature. Genuine method bodies and layouts are unchanged; the final shared-header compile remains exact and all previously reviewed operands are unchanged. Default branches remain untouched.

The final compliance source activates FABLE_RETAIL_VENDOR_VECTOR_ORDINARY. Raw Ego_r/FableWin TPI independently confirms the unchanged C3DVector member signatures, but no original proof establishes stronger inline attributes. The private header therefore uses ordinary inline and preserves the old default branch verbatim. Real VC7.1 preprocessing has zero active forced attributes; fresh target bytes and every relocation offset/kind/symbol agree exactly with the prior independently reviewed object. Both current-source-bound fixtures pass. Receipt workers/vendor/vector_compliance_receipts/008edff0.json.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

Final header correction enables FABLE_RETAIL_DEFPTR_ORDINARY_INLINE, using the common TPI-reviewed model. CDefPointer::Object is private CDefPointeeBase* (original Ego_r/FableWin type records); Assign is absent from the original method set and removed. Original operator bodies and SetDynamicCast retain genuine behavior directly. Receipt workers/vendor/WILL_DEFPTR_READY.json; underlying TPI review workers/vendor/ACTIVE_HEADER_AUDIT/DEFPTR_COMMON_MODEL_REVIEW.json.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

## CResource original field access correction

Both original TPI records prove CResource::ResourceList,PrevResource,NextResource,ResourceSize,LastUsedFrame are private at offsets8,12,16,20,24. The opt-in shared header restores that access, keeping every original field and the entire default branch unchanged. Source includes FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS. Actual VC7.1 preprocessing confirms those fields and the earlier private IVRefCount and private CDefPointer::Object declarations. See RESOURCE_ACTIVE_DECLARATIONS.json and the vendor original signature matrix at workers/vendor/BASE_DECLARATION_AUDIT/REPORT.json. Fresh byte and dual-compiler fixture checks pass; independent main acceptance is pending.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.
