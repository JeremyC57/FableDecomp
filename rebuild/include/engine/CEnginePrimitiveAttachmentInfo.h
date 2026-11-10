// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEPRIMITIVEATTACHMENTINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEATTACHMENTINFO_H
#include "fable_string.h"
class CEnginePrimitiveAttachmentInfo {
public:
 CEnginePrimitiveAttachmentInfo();
 unsigned long Flags;
 CCharString ParentDummyName;
 long ParentDummyIndex;
 CCharString ChildDummyName;
 long ChildDummyIndex;
};
#endif
