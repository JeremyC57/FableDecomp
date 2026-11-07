F5_OLD = """        if (Model < 4)
            return "K5";
        if (Model == 6 || Model == 7)
            return "K6";
        if (Model == 8)
            return "K6-2";
        if (Model == 9)
            return "K6-III";
        break;"""
F6_OLD = """        if (Model == 1)
            return "K7";
        if (Model == 2 || Model == 4)
            return "Athlon";
        if (Model == 3 || Model == 7)
            return "Duron";
        if (Model == 6 || Model == 8)
            return "Athlon XP";
        break;"""
def f5(test, local):
    body = F5_OLD.replace("        if (Model < 4)", "        if (" + test + ")")
    if local:
        body = "        {\n        unsigned long model = Model;\n" + body.replace("Model", "model").replace("        break;", "        }\n        break;")
    return body
def f6(local):
    if not local: return F6_OLD
    return "        {\n        unsigned long model = Model;\n" + F6_OLD.replace("Model", "model").replace("        break;", "        }\n        break;")
AXES = [
 ("f5", [("", "", []),
         ("t3", "Model <= 3", [(F5_OLD, f5("Model <= 3", False))]),
         ("m3", "arm-local model copy, <= 3", [(F5_OLD, f5("Model <= 3", True))]),
         ("m4", "arm-local model copy, < 4", [(F5_OLD, f5("Model < 4", True))]),
         ("g3", "!(Model > 3)", [(F5_OLD, f5("!(Model > 3)", False))]),
         ("u3", "Model <= 3u", [(F5_OLD, f5("Model <= 3u", False))])]),
 ("f6", [("", "", []),
         ("n6", "arm-local model copy in the family-6 arm", [(F6_OLD, f6(True))])]),
 ("fam", [("", "", []),
          ("fu", "unsigned long Family", [("    long Family;                                         // +4", "    unsigned long Family;                                // +4")]),
          ("fi", "int Family", [("    long Family;                                         // +4", "    int Family;                                          // +4")])]),
 ("type", [("", "", []),
           ("ti", "unsigned int Model", [("    unsigned long Model;                                 // +0xc", "    unsigned int Model;                                  // +0xc"), ("?unsigned long model = Model;", "unsigned int model = Model;")])]),
 ("step", [("", "", []),
           ("st", "Stepping declared unsigned int", [("    unsigned long Stepping;                              // +8", "    unsigned int Stepping;                               // +8")])]),
]
