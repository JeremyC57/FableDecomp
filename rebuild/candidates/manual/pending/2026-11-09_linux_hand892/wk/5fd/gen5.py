exec(open(sys.argv[1] + "/wk/5fd/gen.py").read())
def ind(s, n=4): return "".join((" " * n + l if l else l) + "\n" for l in s.rstrip("\n").split("\n"))
nc = ("if (card->IsVig()) {\n" + leaf("VIG", 4) + "} else if (card->IsOpt()) {\n" + leaf("OPT", 4) + "} else {\n" + leaf("FB", 4) + "}\n")
core = leaf("CORE", 0)
sig = " C* MakeIcon(M* manager, Card* card, bool pc, C*& container)\n{\n    C* text;\n"
call = "            text = MakeIcon(manager, card, pc, container);\n"
for kw in ("__forceinline", "inline"):
    k = kw.strip("_")[:5]
    h1 = kw + sig + "    if (card->IsCore()) {\n" + ind(core, 8) + "        return text;\n    }\n" + ind(nc, 4) + "    return text;\n}\n"
    h2 = kw + sig + "    if (!card->IsCore()) {\n" + ind(nc, 8) + "        return text;\n    }\n" + ind(core, 4) + "    return text;\n}\n"
    h3 = kw + sig + "    if (card->IsCore()) {\n" + ind(core, 8) + "    } else {\n" + ind(nc, 8) + "    }\n    return text;\n}\n"
    for n, h in (("h1", h1), ("h2", h2), ("h3", h3)):
        s = src[:i0] + call + src[i1:]; s = s.replace("void F(", h + "void F(", 1); open(A + "/wk/5fd/s_%s%s.cpp" % (n, k), "w").write(s)
