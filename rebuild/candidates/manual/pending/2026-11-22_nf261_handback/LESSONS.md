# NF261 lessons (one line each)
- A vtmap NOLEAF on a slot row means its HEAD row had no Leaf line, not that the emitters disagree: report NOLEAF -> SAME, and use the HEAD link's unresolved lines to show the binding gain.
- A base-class slot can be overridden in the class whose table you map (CombatIdle slot 74, CCombatActionBase slot 66): read the slot of the class table itself and pick an emitter that inherits it.
- VC7.1 folds `x * 0.5f * 1024.0f` into `x * 512.0f` (one literal) but keeps two fmuls when the scale is applied in a second statement (`s = x * 0.5f; s = s * 1024.0f;`).
- A made-up data name can be crossed with a literal by c2's operand order: xref every relocation site (retail target v symbol), not only the grade, before renaming a constant.
- An emitter compiled from a local model can bind a base member's spelling at a derived override's address (0x006F5C10 slot 66); a self rename that defines that symbol makes the wrong slot resolve in a full link.
- Source the lane env before calling verify_and_land helpers directly (VC path comes from FABLE_VC71).
