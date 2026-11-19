import itertools
R={"00":("v0_x","v0_y"),"10":("v1_x","v0_y"),"01":("v0_x","v1_y"),"11":("v1_x","v1_y")}
FORMS={"XY":"gradient{k}.X * {a} + gradient{k}.Y * {b}","YX":"gradient{k}.Y * {b} + gradient{k}.X * {a}",
       "D":"gradient{k}.Dot(C2DVector({a}, {b}))","aX":"{a} * gradient{k}.X + {b} * gradient{k}.Y"}
def mk(form):
    def fn(s):
        for k,(a,b) in R.items():
            s=s.replace(FORMS["XY"].format(k=k,a=a,b=b), FORMS[form].format(k=k,a=a,b=b))
        return s
    return fn
VARIANTS=[(f,mk(f)) for f in FORMS]
