import sys
A = sys.argv[1]
src = open(A + "/wk/5fd/m0.cpp").read()
src = src.replace("struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const;",
                  "struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const; bool IsA() const; bool IsB() const; bool IsC() const;")
def leaf(k, ind):
    s = " " * ind
    return (s + "if (pc) {\n" + s + '    container = manager->Create("PC_%s", false);\n' % k + s + '    text = container->Find("PC_QN");\n' + s + "} else {\n"
            + s + '    container = manager->Create("UI_%s", false);\n' % k + s + '    text = container->Find("UI_QN");\n' + s + "}\n")
i0 = src.index("            if (card->IsCore()) {\n"); i1 = src.index("            if (text)\n")
def chain(arms, fallback=True):
    s = "            if (card->IsCore()) {\n" + leaf("CORE", 16)
    for a in arms: s += "            } else if (card->Is%s()) {\n" % a + leaf(a.upper(), 16)
    if fallback: s += "            } else {\n" + leaf("FB", 16)
    return s + "            }\n"
def emit(name, body): open(A + "/wk/5fd/s_%s.cpp" % name, "w").write(src[:i0] + body + src[i1:])
