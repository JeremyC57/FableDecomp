def R(k,o,b,t):
    i=t.index('#ifdef FABLE_RETAIL_CMATRIX3X4_SETYZROTATE')
    assert all(l in o for l in t[:i])
    return o+t[i:]
