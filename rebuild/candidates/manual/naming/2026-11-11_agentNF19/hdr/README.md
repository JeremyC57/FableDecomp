# agentNF19 header opt-ins (full copies of the CURRENT repo headers; only #ifdef blocks added, no default-path change)
- engine/CString.h: FABLE_RETAIL_CSTRING_MAKEUPPER declares CopyBeforeWrite (0x00C0BE20) and MakeUpper (0x00C0C910). Used by row 00c0c910.
- engine/CTCPhysicsBase.h (layout/default branch):
  - FABLE_RETAIL_CTCPHYSICSBASE_GETMASS declares `float GetMass() const` (0x006AFE00). Used by 007232e0.
  - FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET declares `class CRightHandedSet;` and `protected: static CRightHandedSet DummyRHSet` (0x0137F9B4). Used by 006b1450.
- engine/NUISystem_CMouseCursor.h: FABLE_RETAIL_CMOUSECURSOR_GETPOSITION declares `virtual C2DVector GetPosition() const`. Used by 0055c5d0.
- engine/CCreatureActionBase.h: FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED declares `public: void SetPItemRemoved(CIntelligentPointer<CThing>)` (0x0072D360; needs FULL_MEMBERS). Used by 0072d360.
- stlport/fable_containers.h: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL turns std::__destroy_aux(I,I,const __false_type&) into a declaration-only FABLE_FASTCALL template (it includes rebuild_abi.h for the macro). Used by 006eda80.
