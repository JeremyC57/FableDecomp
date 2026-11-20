# Re-port b1: nine landed rows moved onto engine/CGenericException.h (Account B, 2026-11-20)

These rows were newly flagged by the widened HEADER_POLICY when hand3c2 b14 added CGenericException.h. Owner decision 2026-11-20: land first,
re-port afterwards. Each row's row-local CGenericException / CExceptionBase model is replaced by the shared header. Nothing else in the row
changes, and each row keeps its line count.

| address | row |
|---|---|
| 0x00501D30 | CWorldMap::GetLevelFilesFromWorldFile (also uses the existing FABLE_RETAIL_CWAVELOADER_PASS22_EXCEPTIONS, so the text goes to the base, as the row modelled it) |
| 0x00507C30 | CWorldMap::LoadFromFile |
| 0x009A7CA0 | CBankFileManager::GetDevBankFilePath |
| 0x009A7D40 | CBankFileManager::GetRetailBankFilePath |
| 0x00A0D0E0 / 0x00A0D140 / 0x00A0D1A0 / 0x00A0D200 | CWStringParser::ReadAsInteger / ReadAsFloat / ReadAsString / ReadAsSymbol |
| 0x00A37060 | CClassFactory::GetClassNumberFromName |

- **Grades:** all nine are RELOCATION_MATCH with their tests passing, the same as before. They were landed with vland `--land` NON-STRICT.
  Six of them still stop at HEADER_POLICY under strict, for OTHER local models that are unchanged:
  - 0x00501D30: CBaseClass, CBaseClassNonCopyable, CDiskFileWin32, CParsedItem, CStringParser, CWorldMap;
  - the four CWStringParser rows: CWParsedItem, CWStringParser;
  - 0x00A37060: CClassFactory.

  No CGenericException warning is left. 0x00507C30, 0x009A7CA0 and 0x009A7D40 pass strict.
- **Header:** engine/CGenericException.h, add-only opt-in FABLE_RETAIL_CGENERICEXCEPTION_TEXT_CTOR: CGenericException(const char* text),
  Ego_r / FableWin ??0CGenericException@@QAE@PBD@Z, inline and empty in retail.
  - hdrcheck2 default=same.
  - The header's existing includers 0x009BE340 and 0x009BF7E0 regrade unchanged.
- **Review:** checked by Account B's coordinator by diffing each row against its landed text (only the model lines change), not by the rolling
  auditor.
- **Skipped:** 0x009BE870 and 0x009BFB30, which Account A had claimed (NF136 / NF140) at the time.
- keypayload changed `struct` to `class` in 0x00507C30's test.
