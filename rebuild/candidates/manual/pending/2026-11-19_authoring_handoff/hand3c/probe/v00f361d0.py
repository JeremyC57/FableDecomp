import re
OPT=["", "DOT_ORIGINAL_SCOPE_PASS23","DOT_XY_THEN_Z","DOT_GROUPED","DOT_ORDERED_YXZ","DOT_ORDERED_ZYX","DOT_ORDERED_XYZ"]
pat=re.compile(r"gradient(\d{3})\.X \* (\w+) \+ gradient\d{3}\.Y \* (\w+) \+ gradient\d{3}\.Z \* (\w+);")
def mk(opt, rev):
    def fn(s):
        if rev: s=pat.sub(lambda m: f"C3DVector({m.group(2)}, {m.group(3)}, {m.group(4)}).Dot(gradient{m.group(1)});", s)
        else: s=pat.sub(lambda m: f"gradient{m.group(1)}.Dot(C3DVector({m.group(2)}, {m.group(3)}, {m.group(4)}));", s)
        d="#define FABLE_RETAIL_C3DVECTOR\n"+(f"#define FABLE_RETAIL_C3DVECTOR_{opt}\n" if opt else "")
        return s.replace("#define FABLE_RETAIL_CPERLINNOISE_GETNOISE\n", d+"#define FABLE_RETAIL_CPERLINNOISE_GETNOISE\n")
    return fn
VARIANTS=[(f"{o or 'default'}_r{r}", mk(o,r)) for o in OPT for r in (0,1)]
