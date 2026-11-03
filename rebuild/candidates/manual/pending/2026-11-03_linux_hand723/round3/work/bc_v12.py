O="if (IsInitialised) Uninitialise();"
VARIANTS = [
 ("q1", [(O, "if (IsInitialised != 0) Uninitialise();")], None, "!= 0"),
 ("q2", [(O, "if (IsInitialised == 1) Uninitialise();")], None, "== 1"),
 ("q3", [(O, "if (IsInitialised == true) Uninitialise();")], None, "== true"),
 ("q4", [(O, "if ((bool)IsInitialised) Uninitialise();")], None, "(bool)"),
 ("q5", [(O, "if (IsInitialised) this->Uninitialise();"), ("SetAlphaRef(0x10);", "SetAlphaRef(16);")], None, "decimal 16"),
]
