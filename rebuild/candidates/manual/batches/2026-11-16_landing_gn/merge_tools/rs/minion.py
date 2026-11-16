def R(k,o,b,t):
    if k==0:
        assert o[-1]=='#else' and t[0]=='#ifdef FABLE_RETAIL_CAISTATEGROUP_MINIONGOSSIP' and t[-1]=='#else'
        return o[:-1]+['#elif defined(FABLE_RETAIL_CAISTATEGROUP_MINIONGOSSIP)']+t[1:]
    return ['#endif // FABLE_RETAIL_CAISTATEGROUP_GOSSIP / FABLE_RETAIL_CAISTATEGROUP_MINIONGOSSIP']
