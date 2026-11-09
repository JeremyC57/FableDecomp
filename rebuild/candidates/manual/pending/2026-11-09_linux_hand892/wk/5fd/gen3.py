exec(open(sys.argv[1] + "/wk/5fd/gen.py").read())
def rleaf(k, ind):
    s = " " * ind
    return (s + "if (pc) {\n" + s + '    container = manager->Create("PC_%s", false);\n' % k + s + '    return container->Find("PC_QN");\n' + s + "} else {\n"
            + s + '    container = manager->Create("UI_%s", false);\n' % k + s + '    return container->Find("UI_QN");\n' + s + "}\n")
for kw in ("__forceinline", "inline"):
    helper = (kw + " C* MakeNonCore(M* manager, Card* card, bool pc, C*& container)\n{\n    if (card->IsVig()) {\n" + rleaf("VIG", 8) + "    } else if (card->IsOpt()) {\n"
              + rleaf("OPT", 8) + "    } else {\n" + rleaf("FB", 8) + "    }\n}\n")
    body = "            if (card->IsCore()) {\n" + leaf("CORE", 16) + "            } else\n                text = MakeNonCore(manager, card, pc, container);\n"
    s = src[:i0] + body + src[i1:]; s = s.replace("void F(", helper + "void F(", 1); open(A + "/wk/5fd/s_nc_%s.cpp" % kw.strip("_"), "w").write(s)
# also: non-core helper assigning through reference, no returns
helper = ("__forceinline void MakeNonCore2(M* manager, Card* card, bool pc, C*& container, C*& text)\n{\n    if (card->IsVig()) {\n" + leaf("VIG", 8) + "    } else if (card->IsOpt()) {\n"
          + leaf("OPT", 8) + "    } else {\n" + leaf("FB", 8) + "    }\n}\n")
body = "            if (card->IsCore()) {\n" + leaf("CORE", 16) + "            } else\n                MakeNonCore2(manager, card, pc, container, text);\n"
s = src[:i0] + body + src[i1:]; s = s.replace("void F(", helper + "void F(", 1); open(A + "/wk/5fd/s_nc_ref.cpp", "w").write(s)
