import itertools
F = {"F1":"(3.0f - 2.0f * {v}) * {v} * {v}", "F2":"((3.0f - 2.0f * {v}) * {v}) * {v}", "F3":"(3.0f - (2.0f * {v})) * {v} * {v}",
     "F4":"{v} * {v} * (3.0f - 2.0f * {v})", "F5":"(3.0f - ({v} + {v})) * {v} * {v}"}
def mk(f, named, split, dead):
    def fn(s):
        s=s.replace("(3.0f - 2.0f * v0_x) * v0_x * v0_x", F[f].format(v="v0_x")).replace("(3.0f - 2.0f * v0_y) * v0_y * v0_y", F[f].format(v="v0_y"))
        if named: s=s.replace('#include "engine/GFFloatToLongFloor.h"', '#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED\n#include "engine/GFFloatToLongFloor.h"')
        if split: s=s.replace("    float result = GFGetInterpolatedValue(GFGetInterpolatedValue(value00, value10, tx), GFGetInterpolatedValue(value01, value11, tx), ty);\n",
            "    float v0 = GFGetInterpolatedValue(value00, value10, tx);\n    float v1 = GFGetInterpolatedValue(value01, value11, tx);\n    float result = GFGetInterpolatedValue(v0, v1, ty);\n")
        if dead: s=s.replace("    float v0_y = point.Y - y_int;\n","    float v0_y = point.Y - y_int;\n    float v1_x = v0_x - 1.0f;\n    float v1_y = v0_y - 1.0f;\n")
        return s
    return fn
VARIANTS=[(f"{f}_n{int(n)}_s{int(sp)}_d{int(d)}", mk(f,n,sp,d)) for f in F for n in (0,1) for sp in (0,1) for d in (1,)]
