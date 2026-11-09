core_hdr = "            if (card->IsCore()) {\n"
V = [
 ("inv", [(core_hdr, "            if (!card->IsCore()) {\n                text = 0;\n            } else\n            if (card->IsCore()) {\n")]),
 ("vigdtor", [("struct Card { bool IsCore() const; bool IsVig() const;", "struct D { D(const D&); ~D(); bool v; };\nstruct Card { D d; bool IsCore() const; bool IsVig() const { return D(d).v; }")]),
 ("txt0", [("            C* text;\n", "            C* text = 0;\n")]),
 ("cont", [("        if (Match(card)) {\n", "        if (!Match(card))\n            continue;\n        {\n")]),
]
