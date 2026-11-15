# RenderBumpMap

The old opaque local model nearly matched but lost original semantics: two C2DVector settings per water kind and CRGBFloatColour temporaries converted to C4DVector. The original debug donor proves both. Rebase to the existing shared classes and restore those original operations before trying low-level state-mask variations. First new strategy matched the final paired operands without any new inline controls, constants or flags. The shared headers required no edits; a new original enum definition sufficed.
