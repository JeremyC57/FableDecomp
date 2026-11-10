# Chunk 1018 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every grade in this chunk ran with `FABLE_HDR_OVERLAY` pointing here. Modified repo headers are full copies of
`rebuild/include/<path>` with additions only under new `FABLE_RETAIL_*` opt-ins (an `#elif` arm in CRGBColour.h): with none of
the new macros defined, each file preprocesses to the repo text, so the default path is byte-identical. New headers are
retail models (each refuses to compile without `FABLE_RETAIL_DEF_CLASS_BASE` where it derives the def bases).

Include-order gotcha: a quoted include inside a repo header resolves next to that repo header first. A row that needs the
overlay `stlport/fable_containers.h` (VECTOR_RANGE_CTOR) must include it itself before `fable_string.h`.

## Modified (opt-in only)
| header | opt-in | what | rows |
|---|---|---|---|
| engine/CPersistContext.h | FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED | in-class declaration of the named overload `TransferVectorOfSubComponents(const char*, vector<T>&)` and its body: CheckCRC(name); clear / binary save / binary load (ReadSLONG, resize, element Transfer) / text load through the CPersistContext_TransferableVectorOfSubComponents<T> wrapper and TransferVectorOfSubComponentsLoadText; `default: __assume(0)` under _MSC_VER (retail's 5-entry jump table with entry 1 unused) | 0x00F334E0 (match) |
| engine/CPersistContext.h | FABLE_RETAIL_CPERSISTCONTEXT_ISPARSINGTEXT | `bool IsParsingText() const { return Mode==MODE_LOAD_TEXT; }` | 0x0067E290 (differ) |
| engine/CPersistContext.h | FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY | bodies of `Transfer(T&)` / `TransferVector(vector<T>&)` (binary arms) + `GFSerialiseVectorBinaryIn/Out` declarations (FABLE_FASTCALL) | 0x0067E290, 0x00465C54 (differ) |
| engine/CPersistTraits.h | FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY | generic `TransferBinaryIn`: `T tval; is.Read(&tval,sizeof(T)); val=tval;` (visible body lets VC7.1 share the empty traits frame word) | 0x00632B90 (match) |
| engine/CPersistTraits.h | FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN | `val = is.ReadNullTerminatedString();` | 0x00632B90 (match) |
| engine/CPersistTraits.h | FABLE_RETAIL_CPERSISTTRAITS_C2DVECTOR_BINARYIN | `val = is.Read2DVector();` | 0x00475DFA (match) |
| engine/CPersistTraits.h | FABLE_RETAIL_GFSERIALISEMAPBINARYIN | template `GFSerialiseMapBinaryIn<V,K,C>(CMemoryDataInputStream&, map<K,V,C>&)` FABLE_FASTCALL body | 0x00632B90 (match) |
| engine/CPersistTraits.h | FABLE_RETAIL_GFSERIALISEVECTORBINARYIN | template `GFSerialiseVectorBinaryIn<T>` FABLE_FASTCALL body | 0x00475DFA (match) |
| engine/CPersistTraits.h | FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT | inline `CPersistTraits<bool>::TransferBinaryOut` -> WriteEBOOL. Header debt: PDB form takes `const bool&` | 0x0067E290 (differ) |
| engine/CPersistTraits.h | FABLE_RETAIL_CPERSISTTRAITS_PAIR | `CPersistTraits<COpinionDeed*>() : DefaultVal(0)` + partial specialisation `CPersistTraits<std::pair<A,B> >` with the inline text TransferIn "(" a "," b ")" | 0x006C9350 (differ) |
| engine/CDataInputStream.h | FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR | `class C2DVector; C2DVector Read2DVector();` (0x00995030, Ego_r ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ) | 0x00475DFA (match) |
| engine/CRGBColour.h | FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY (#elif arm) | `CRGBColour(const CRGBColour& c) : IntValue(c.IntValue) {}` | 0x00405220 (differ) |
| engine/CCombatAbilityBlockDefBase.h | FABLE_RETAIL_CCOMBATABILITYBLOCKDEFBASE_TRANSFER | `virtual void Transfer(CPersistContext&)` declaration (slot 18) | 0x00465C54 (differ) |
| stlport/fable_algorithm.h | FABLE_RETAIL_STLP_FOR_EACH_BODY | inline STLport `for_each` body | 0x0067E290 (differ) |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_VECTOR_RANGE_CTOR | template range ctor `vector(first,last)` -> `_M_range_initialize(first,last,forward_iterator_tag())` (declaration of the helper only; out of line 0x00465E44) | 0x00465C54 (differ) |
| stlport/fable_map.h | FABLE_RETAIL_STLP_SET_RANGE | `_Rb_tree::insert_unique(first,last)`, set range ctor, `set::operator=` (`_M_t = x._M_t`), `set::insert(first,last)` | 0x00465C54 (differ) |

## New
| header | what | rows |
|---|---|---|
| engine/CEnvironmentThemeDef.h | `class CEnvironmentThemeDef : public CParentDefClassBase` (0xC4): Transfer / Copy / GetSizeofClass (slots 18-20), members TimeOfDay +0x3C .. WaterReflectionOffset +0xC0 (Ego_r layout less 4: TLC STLport vectors are 12 bytes) | 0x00476E3E |
| engine/CEngineLocalDetailObjectDef.h | 0x88 def: Mesh +0x3C .. WindSkewSpeedFactor +0x80, eight `bool X:1` flags at +0x84 | 0x00477CD2 |
| engine/CPersistContext_TransferableVectorOfSubComponents.h | the text-load wrapper template: ctor stores TransVector; private virtuals Size, Resize, AreDefaultValsApplied, SetDefaultValsApplied, Transfer (vtable 0x01301C2C order for CConversation) | 0x00F334E0 |
| engine/CDegradableInfo.h | 0x1C: HealthPercentage, CEngineGraphic EngineGraphic +4, SmashParticleEmitter +0x14, BlocksNavigation +0x18; ctor declared | 0x0067E290 |
| engine/CDegradableDef.h | `: CSubDefClassBase` (0x3C): Degradable +0x25, GraphicType +0x28, InitiallyBlocksNavigation +0x2C, Degradations +0x30; nested CIsHealth1GTHealth2 (operator() declared) and CSetEngineGraphicType (inline `info.EngineGraphic.Type = GraphicType`; the member name GraphicType is descriptive, not found in a PDB) | 0x0067E290 |
