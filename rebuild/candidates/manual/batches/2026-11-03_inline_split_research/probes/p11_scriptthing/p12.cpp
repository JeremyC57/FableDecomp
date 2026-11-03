// p12: CScriptThing param dtor with K returns in a for(;;) loop vs straight line
#include "st.h"
void s2(CScriptThing t) { if (Test(1)) return; G(1); }
void s3(CScriptThing t) { if (Test(1)) return; G(1); if (Test(2)) return; G(2); }
void s4(CScriptThing t) { if (Test(1)) return; G(1); if (Test(2)) return; G(2); if (Test(3)) return; G(3); }
void s6(CScriptThing t) { if (Test(1)) return; G(1); if (Test(2)) return; G(2); if (Test(3)) return; G(3); if (Test(4)) return; G(4); if (Test(5)) return; G(5); }
void l1(CScriptThing t) { for (;;) { if (Term()) return; G(0); } }
void l2(CScriptThing t) { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; G(1); } G(0); } }
void l3(CScriptThing t) { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; G(2); } } G(0); } }
void l4(CScriptThing t) { for (;;) { if (Term()) return; if (Test(1)) { if (Term()) return; if (Test(2)) { if (Term()) return; if (Test(3)) { if (Term()) return; G(3); } } } G(0); } }
