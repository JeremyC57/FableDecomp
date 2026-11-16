def R(k,o,b,t):
    i=t.index('#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS'); blk=t[i:]
    j=[n for n,l in enumerate(blk) if l.startswith('    void SetupFresnelPixelShaderConstants();')]; assert len(j)==1; j=j[0]
    blk=blk[:j]+['#ifndef FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUND_BODIES_PASS22   // (that opt-in declares it)',blk[j],'#endif']+blk[j+1:]
    assert o[:i]==t[:i]
    return o+blk
