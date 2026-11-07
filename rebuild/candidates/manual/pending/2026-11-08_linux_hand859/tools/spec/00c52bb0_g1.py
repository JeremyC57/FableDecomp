F5_OLD = """        if (Model < 4)
            return "K5";
        if (Model == 6 || Model == 7)
            return "K6";
        if (Model == 8)
            return "K6-2";
        if (Model == 9)
            return "K6-III";
        break;"""
F5_LE = F5_OLD.replace("        if (Model < 4)\n            return \"K5\";", "        if (Model <= 3)\n            return \"K5\";")
F5_SW = """        switch (Model) {
        case 0: case 1: case 2: case 3:
            return "K5";
        case 6: case 7:
            return "K6";
        case 8:
            return "K6-2";
        case 9:
            return "K6-III";
        }
        break;"""
F5_GE = """        if (Model >= 4) {
            if (Model == 6 || Model == 7)
                return "K6";
            if (Model == 8)
                return "K6-2";
            if (Model == 9)
                return "K6-III";
            break;
        }
        return "K5";"""
F5_NOT = F5_OLD.replace("        if (Model < 4)\n            return \"K5\";", "        if (!(Model > 3))\n            return \"K5\";")
F6_OLD = """        if (Model == 1)
            return "K7";
        if (Model == 2 || Model == 4)
            return "Athlon";
        if (Model == 3 || Model == 7)
            return "Duron";
        if (Model == 6 || Model == 8)
            return "Athlon XP";
        break;"""
F6_SW = """        switch (Model) {
        case 1:
            return "K7";
        case 2: case 4:
            return "Athlon";
        case 3: case 7:
            return "Duron";
        case 6: case 8:
            return "Athlon XP";
        }
        break;"""
OUTER_OLD = """    switch (Family) {
    case 4:
        return "486 or 5x86";
    case 5:
"""
AXES = [
 ("f5", [("", "", []), ("le", "Model <= 3", [(F5_OLD, F5_LE)]), ("sw", "family-5 arm as a switch", [(F5_OLD, F5_SW)]),
         ("ge", "Model >= 4 guard", [(F5_OLD, F5_GE)]), ("nt", "!(Model > 3)", [(F5_OLD, F5_NOT)])]),
 ("f6", [("", "", []), ("s6", "family-6 arm as a switch", [(F6_OLD, F6_SW)])]),
 ("outer", [("", "", []),
            ("ic", "if chain on Family", [(OUTER_OLD, "    if (Family == 4)\n        return \"486 or 5x86\";\n    if (Family == 5) {\n"),
                                           ("        break;\n    case 6:\n", "    }\n    if (Family == 6) {\n"),
                                           ("        break;\n    }\n    return \"<unknown>\";", "    }\n    return \"<unknown>\";")]),
            ("sw4", "switch on Family - 4", [(OUTER_OLD, "    switch (Family - 4) {\n    case 0:\n        return \"486 or 5x86\";\n    case 1:\n"), ("    case 6:\n", "    case 2:\n")])]),
 ("type", [("", "", []), ("ui", "unsigned int Model", [("    unsigned long Model;                                 // +0xc", "    unsigned int Model;                                  // +0xc")]),
           ("sl", "long Model with unsigned compares", [("    unsigned long Model;                                 // +0xc", "    long Model;                                          // +0xc"),
                                                        (">Model < 4", "(unsigned long)Model < 4"), (">Model <= 3", "(unsigned long)Model <= 3"), (">Model > 3", "(unsigned long)Model > 3"), (">Model >= 4", "(unsigned long)Model >= 4")])]),
 ("ret", [("", "", []), ("rv", "result variable", [(">return \"486 or 5x86\";", "{ result = \"486 or 5x86\"; goto done; }"), (">return \"K5\";", "{ result = \"K5\"; goto done; }"), (">return \"K6\";", "{ result = \"K6\"; goto done; }"),
                                                    (">return \"K6-2\";", "{ result = \"K6-2\"; goto done; }"), (">return \"K6-III\";", "{ result = \"K6-III\"; goto done; }"), (">return \"K7\";", "{ result = \"K7\"; goto done; }"),
                                                    (">return \"Athlon\";", "{ result = \"Athlon\"; goto done; }"), (">return \"Duron\";", "{ result = \"Duron\"; goto done; }"), (">return \"Athlon XP\";", "{ result = \"Athlon XP\"; goto done; }"),
                                                    ("    return \"<unknown>\";\n}", "    result = \"<unknown>\";\ndone:\n    return result;\n}"),
                                                    ("const char* CCpuInfo::GetAMDModelName() const\n{\n", "const char* CCpuInfo::GetAMDModelName() const\n{\n    const char* result;\n")])]),
]
