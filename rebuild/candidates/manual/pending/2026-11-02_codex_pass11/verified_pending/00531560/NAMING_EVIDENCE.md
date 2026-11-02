# Identity and complete class model

Exact Leaf: `?GetDynamicSize@CComponent@NUISystem@@UBE?AVC2DVector@@XZ`.

Original Ego_r name-set and raw CComponent TPI prove the public virtual const member returning class C2DVector by value. Independent native CComponent/CSprite/CYesNo RTTI vtables bind slot111 to00531560. Original module records locate ui_component.cpp2212–2283 and recover max,min,first_child,shape_children,count. The manifest description is not used as proof.

The complete1503byte function ends with ret4 at00531b3c, then alignment before00531b40. Both real base layout and all134 primary virtual slots are reconstructed in engine/NUISystem_CComponent.h. CUIInterface,CObserver,CActionParam are actual public bases at0,4,0x18. Native constructors005334a0/0052d9e0 independently confirm full0x134 size and every used member offset. Unaccessed composite members retain exact opaque storage through DefIndex+0x130; no tail is dropped. Native STLport vectors are12bytes. CCountedPointer is the supplied genuine8byte Data/Info class. Protected access and original namespace/class keys are preserved.

C2DVector's original empty default constructor, user copy constructor and float-pair constructor are proved by raw TPI plus original debug line records lib_coord.hpp100–103. Its + and - operators are the original const member operators, lines314–322. New optional FABLE_RETAIL_C2DVECTOR_UI methods preserve the preexisting POD declaration in the default branch. C2DVector native getter hidden-result copies independently prove the user-constructor ABI.

# All calls and operands

The four direct-call relocations are the vector<unsigned long> copy constructor00535470, two original SGI std::__find<unsigned long*,unsigned long> calls005348e0 and CRT free00bfea14. Exact emitted symbols agree with original STLport4.6 source, PDB element/template types and native caller/callee shapes. The existing bindings include correct copies and several wrong old placeholders; corrected names are in NAME_FIXES.tsv. The real vector copy body is visible, reads the entire source range and only writes allocated/output storage. Its helper context is not a separately submitted out-of-scope row.

Eight float0.5 operands bind actual compiler literals at0122f59c. The independent main review checks their exact constant bytes. Virtual calls use GetPosition slot16, GetType65, IsPositionIsCenter106 and GetDynamicSize111, each verified against raw TPI and native tables. No named global is invented.

# Behavior and limits

The standalone test pastes the final production target body and complete headers. Eleven real headers are expanded; no source file is included by path. The native and GNU64 builds run850 scenarios:0–9 children,5filter modes,17position/type/centering arrangements, duplicate and out-of-range filter entries, no matching index, centered/noncentered sprites and nonsprite children. Exactly representable finite float inputs avoid rounding-dependent expectations. Independent bounds calculations check both coordinates; call counters check excluded children, repeated type/centering queries and the native special case that initializes first-child bounds without a centering adjustment.

Unrelated virtual methods deliberately throw in the fixture so an unexpected call fails. Fixture constructors establish real C++ vtables; no handwritten dispatch table is used. The unrelated CRGBColour by-value ABI declaration uses a named channel struct only in the GNU fixture to express the original overlapping bytes in strict C++03; the target never accesses that type. No OS/rendering behavior is claimed tested. Header static size assertions are checked by the real target build, while host tests access real fields without assuming32bit long/pointer widths.

Full raw research is in evidence/math_identity/UI*.json and UI.md. Names recovered are actual PDB spellings; original source formulas are reconstructed from native code, not claimed recovered source text.
