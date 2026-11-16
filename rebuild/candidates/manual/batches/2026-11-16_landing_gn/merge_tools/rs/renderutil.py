def R(k,o,b,t):
    i=t.index('#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICIB'); blk=t[i:]
    assert blk[-1]=='#endif'
    j=max(n for n,l in enumerate(o) if l=='#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB')
    e=o.index('#endif',j)
    assert o[e+1]=='#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER'
    return o[:e+1]+blk+o[e+1:]
