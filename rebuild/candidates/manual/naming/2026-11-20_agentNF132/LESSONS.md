# NF132 lessons
- A jmp forwarder row named `_Forward` is a derived class's member that tail-calls the base: find the class from the PDB donor order
  around the forwarder (Ego_r CGame::Uninitialise right before SetGameComponent) and the neighbours' bodies, then rename it with its referencers.
- Typing a member with an 8-aligned class (double inside) in a local model whose first member is its own vptr pushes every field +4:
  derive the model from the real base header instead (CGameComponent gave the retail layout back in one grade).
- A pure-ICF-free STL pair (two byte-identical unreferenced COMDATs) cannot take real names on two sources: check callers before
  spending grades on the binder side of such an entry.
- STLport list pop_back in fable_containers.h needs FABLE_RETAIL_STLP_LIST as well as FABLE_RETAIL_STLP_LIST_POPBACK (nested #ifdef).
- A header's default `class X;` forward declaration fixes the mangling key of every std:: instantiation over X: check the PDB key
  (U vs V) before trusting a container spelling built on it.
- The q6233-style CDefinitionManager_Retail stand-ins all fall to one include block (DEF_CLASS_BASE + CDEFINITIONMANAGER +
  CRESOURCELIST + CGAMEDEFINITIONMANAGER) in one grade each, even in rows with a local CCharString model.
- pdiff.sh's binds step needs a leaf that matches the object; a `?Name@` prefix leaf in an anonymous-namespace row printed no pairs:
  rerun binds.py with the plain name.
