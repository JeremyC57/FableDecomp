exec(open(sys.argv[1] + "/wk/5fd/gen.py").read())
def ind(s, n=4): return "".join((" " * n + l if l else l) + "\n" for l in s.rstrip("\n").split("\n"))
nc = ("if (card->IsVig()) {\n" + leaf("VIG", 4) + "} else if (card->IsOpt()) {\n" + leaf("OPT", 4) + "} else {\n" + leaf("FB", 4) + "}\n")
core = leaf("CORE", 0)
# A: do-while(0) with noncore then-block ending in break
a = "            do {\n                if (!card->IsCore()) {\n" + ind(nc, 20) + "                    break;\n                }\n" + ind(core, 16) + "            } while (0);\n"
emit("dwbreak", a)
# B: do-while(0) with core then-block ending in break, noncore after
b = "            do {\n                if (card->IsCore()) {\n" + ind(core, 20) + "                    break;\n                }\n" + ind(nc, 16) + "            } while (0);\n"
emit("dwbreak2", b)
# C: switch on IsCore
c = "            switch (card->IsCore()) {\n            case true:\n" + ind(core, 16) + "                break;\n            default:\n" + ind(nc, 16) + "                break;\n            }\n"
emit("swbool", c)
# D: goto join explicit from noncore then-block
d = "            if (!card->IsCore()) {\n" + ind(nc, 16) + "                goto join;\n            }\n" + ind(core, 12) + "        join:\n"
emit("gotojoin", d)
