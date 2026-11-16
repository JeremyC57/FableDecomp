def R(k,o,b,t):
    assert t[0]=='#ifdef FABLE_RETAIL_CMATRIX3X4_POSTSCALE_XYZ'
    i=t.index('    FABLE_FORCEINLINE void PostScale(float x,float y,float z) {')
    assert t[i+4]=='    }'
    t=t[:i]+['#if !(defined(FABLE_RETAIL_CMATRIX3X4) && defined(FABLE_RETAIL_CMATRIX3X4_POSTSCALE3))   // (that opt-in declares the same PostScale)']+t[i:i+5]+['#endif']+t[i+5:]
    return o+t
