// p11: score of the by-value CScriptThing parameter dtor at single-exit shapes
#include "st.h"
void e00_entry(CScriptThing t) { G(0); }
void e01_retloop_only(CScriptThing t) { for (;;) { if (Term()) return; G(0); } }
void e02_whileloop(CScriptThing t) { while (!Term()) G(0); }
void e03_local_entry() { CScriptThing t(Get()); G(0); }
void e04_local_in_loop() { for (;;) { if (Term()) return; CScriptThing t(Get()); G(0); } }
void e05_local_in_loop_ret() { for (;;) { CScriptThing t(Get()); if (Term()) return; G(0); } }
