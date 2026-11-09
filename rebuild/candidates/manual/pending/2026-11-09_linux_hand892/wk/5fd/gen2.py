exec(open(sys.argv[1] + "/wk/5fd/gen.py").read())
def body(k, ind):
    s = " " * ind
    return leaf(k, ind)
sw = ("            switch (GetType(card)) {\n            case 0:\n" + leaf("CORE", 16) + "                break;\n            case 1:\n" + leaf("VIG", 16)
      + "                break;\n            case 2:\n" + leaf("OPT", 16) + "                break;\n            default:\n" + leaf("FB", 16) + "                break;\n            }\n")
helper = "__forceinline int GetType(Card* c) { if (c->IsCore()) return 0; if (c->IsVig()) return 1; if (c->IsOpt()) return 2; return 3; }\n"
s = src[:i0] + sw + src[i1:]; s = s.replace("void F(", helper + "void F(", 1); open(A + "/wk/5fd/s_sw.cpp", "w").write(s)
helper2 = "inline int GetType(Card* c) { if (c->IsCore()) return 0; if (c->IsVig()) return 1; if (c->IsOpt()) return 2; return 3; }\n"
s = src[:i0] + sw + src[i1:]; s = s.replace("void F(", helper2 + "void F(", 1); open(A + "/wk/5fd/s_sw2.cpp", "w").write(s)
# ternary type then if chain on the value
t = ("            int type = card->IsCore() ? 0 : card->IsVig() ? 1 : card->IsOpt() ? 2 : 3;\n            if (type == 0) {\n" + leaf("CORE", 16)
     + "            } else if (type == 1) {\n" + leaf("VIG", 16) + "            } else if (type == 2) {\n" + leaf("OPT", 16) + "            } else {\n" + leaf("FB", 16) + "            }\n")
emit("tern", t)
