# NF134 lessons (one line each)
- A namespaced N_<addr> action-base stand-in (N_0062e900) is not renameable in place, but the row moves onto CAIStateGroupBase.h PERFORMACTION +
  CCombatActionBase.h (+ the action's CLASS opt-in) in one grade: copy the define block of landed 0x009208E0; 0x0062E900 is ~CCombatActionBase.
- Check the callee's catalog name before trusting a stand-in's class: 0x00920730's "GoUnderground" ctor 0x008EC6A0 is UndeadReturnToEarth.
- A row-local pointer-form PerformActionAndWait on a derived stand-in respells in place: move the declaration to the local base under protected:,
  const&, EAIActionReturn (fable_ai_action_enums.h), and drop the `&` at the call; the bytes do not change.
- Re-authoring a self row on CShaderRenderManager.h: STLport resize inlines unless FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE is defined (684v633);
  an inline Get<Range>() + FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY reproduces the vftable store + ~CBaseClass temporary.
- The CScopedShadersEnable / CScopedVertexShaderConstantLayout shared helpers match where a row hand-rolled the same RAII savers.
- For "no new symbol" acceptance, compile one landed row on the same shared headers and compare its defined set: the CAFile ??0/??1/??_7/??_G
  COMDATs come from the shared header in every includer.
- PDB access of virtual overrides is often protected (MAE) while landed rows define UAE: when a row is ported anyway, the leaf-only fix is free if
  the exe scan shows no referrer.
