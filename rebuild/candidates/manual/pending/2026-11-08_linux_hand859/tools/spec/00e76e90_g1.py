ARM1_OLD = """            while (timer.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (!Game->IsHeroHandLampLit()) {
                    if (IsActiveThreadTerminating()) return;
                    HeroFlashesLamp(flashes - 1);
                }
            }
            if (IsActiveThreadTerminating()) return;
        } else {"""
ARM2_OLD = """            while (timer.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (Game->IsHeroHandLampLit()) {
                    if (IsActiveThreadTerminating()) return;
                    HeroFlashesLamp(flashes - 1);
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        ResetLamp = true;"""
AXES = [
 ("arm1", [("", "", []),
           ("a1c", "arm 1 inner test as continue", [(ARM1_OLD, ARM1_OLD.replace("                if (!Game->IsHeroHandLampLit()) {\n                    if (IsActiveThreadTerminating()) return;\n                    HeroFlashesLamp(flashes - 1);\n                }", "                if (Game->IsHeroHandLampLit())\n                    continue;\n                if (IsActiveThreadTerminating()) return;\n                HeroFlashesLamp(flashes - 1);"))]),
           ("a1e", "arm 1 inner test if/else", [(ARM1_OLD, ARM1_OLD.replace("                    if (IsActiveThreadTerminating()) return;\n                    HeroFlashesLamp(flashes - 1);\n                }\n            }", "                    if (!IsActiveThreadTerminating())\n                        HeroFlashesLamp(flashes - 1);\n                    else\n                        return;\n                }\n            }"))])]),
 ("arm2", [("", "", []),
           ("a2c", "arm 2 inner test as continue", [(ARM2_OLD, ARM2_OLD.replace("                if (Game->IsHeroHandLampLit()) {\n                    if (IsActiveThreadTerminating()) return;\n                    HeroFlashesLamp(flashes - 1);\n                }", "                if (!Game->IsHeroHandLampLit())\n                    continue;\n                if (IsActiveThreadTerminating()) return;\n                HeroFlashesLamp(flashes - 1);"))]),
           ("a2e", "arm 2 inner test if/else", [(ARM2_OLD, ARM2_OLD.replace("                    if (IsActiveThreadTerminating()) return;\n                    HeroFlashesLamp(flashes - 1);\n                }\n            }", "                    if (!IsActiveThreadTerminating())\n                        HeroFlashesLamp(flashes - 1);\n                    else\n                        return;\n                }\n            }"))])]),
 ("post", [("", "", []),
           ("ph", "post-loop terminate check hoisted after the if/else", [(">            if (IsActiveThreadTerminating()) return;\n        } else {", "        } else {"), (">            if (IsActiveThreadTerminating()) return;\n        }\n        ResetLamp = true;", "        }\n        if (IsActiveThreadTerminating()) return;\n        ResetLamp = true;")])]),
 ("wh", [("", "", []),
         ("wf", "for(;cond;) loops", [(">            while (timer.GetTime() > 0) {", "            for (; timer.GetTime() > 0;) {")])]),
 ("next", [("", "", []),
           ("nx", "next count hoisted", [("        CTimer timer;\n        timer = 3;", "        long next = flashes - 1;\n        CTimer timer;\n        timer = 3;"), (">HeroFlashesLamp(flashes - 1);", "HeroFlashesLamp(next);")])]),
 ("zero", [("", "", []),
           ("zr", "zero-flash arm: distance result in a bool", [("        if (IsDistanceBetweenThingsUnder(Game->GetHero(), point, MaxFlashDistFromStables)) {", "        bool near = IsDistanceBetweenThingsUnder(Game->GetHero(), point, MaxFlashDistFromStables);\n        if (near) {")])]),
]
