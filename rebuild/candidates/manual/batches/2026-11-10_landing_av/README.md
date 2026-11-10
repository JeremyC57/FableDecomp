# 2026-11-10 landing av -- chunk 1018 (5 rows, the persistence family)

Source: `pending/2026-11-10_linux_hand1018/`. vland --land 5 RELOCATION_MATCH, tests PASS:
- 0x00476E3E / 0x00477CD2 CPersistContext::TransferVectorOfSubComponents<CEnvironmentThemeDef / CEngineLocalDetailObjectDef> (strides
  0xC4 / 0x88, element vtables, callers; size-optimised def TUs `#pragma optimize("s", on)` as GOTCHAS lines 114/121). Inline budgets
  /d2inlS55 and /d2inlS98 from compile-only sweeps (the threshold depends on the element type; row notes give the windows).
- 0x00F334E0 the named TransferVectorOfSubComponents<CConversation>(const char*, vector&) (catalog `Transfer<signed_char>` wrong); real
  extent 380 bytes incl. its jump table; its four wrapper virtuals 0x00F33660 / 0x00F33680 / 0x00F336A0 / 0x00F336D0 named from vtable
  0x01301C2C + Ego_r (SPLITS.tsv).
- 0x00475DFA GFSerialiseVectorBinaryIn<C2DVector> (/d2inlS18), 0x00632B90 GFSerialiseMapBinaryIn<CCharString,unsigned long,less>.
Headers (3-way merged onto HEAD, base 1be3e32e6; hdrcheck2 default=same, 13 files): CPersistContext (NAMED, ISPARSINGTEXT,
TRANSFER_BINARY_BODY), CPersistTraits (seven opt-ins), CDataInputStream, CRGBColour, CCombatAbilityBlockDefBase, stlport algorithm /
containers / map; new CEnvironmentThemeDef, CEngineLocalDetailObjectDef, CPersistContext_TransferableVectorOfSubComponents,
CDegradableInfo, CDegradableDef (member GraphicType is descriptive: NOTES).
Parked (differ/): 0x0067E290 CDegradableDef::Transfer (register rotation), 0x00465C54, 0x00405220 (CRGBColour return packing),
0x006C9350 (really CPersistTraits<pair<long,COpinionDeed*>>::TransferIn), and five rows whose prior drafts still model classes locally
(0x005D8978, 0x0047042A, 0x005C0A98, 0x006DC6A0 really <CAttackHistoryCombo>, 0x009E81E0 really <C2DVector>). Queued land1011au: 7 name fixes.
