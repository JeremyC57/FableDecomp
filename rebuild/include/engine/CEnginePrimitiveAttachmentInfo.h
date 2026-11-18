// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEPRIMITIVEATTACHMENTINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEATTACHMENTINFO_H
#include "fable_string.h"
class CEnginePrimitiveAttachmentInfo {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTINFO_EYEDRAW_PASS24
 // Native 0x00754736/0x0075473F and 0x00754AA4/0x00754AAD construct both string members in place.
 CEnginePrimitiveAttachmentInfo() {}
#else
 CEnginePrimitiveAttachmentInfo();
#endif
 unsigned long Flags;
 CCharString ParentDummyName;
 long ParentDummyIndex;
 CCharString ChildDummyName;
 long ChildDummyIndex;
};
#endif
