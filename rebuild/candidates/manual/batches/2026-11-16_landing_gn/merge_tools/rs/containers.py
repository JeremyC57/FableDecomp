def R(k,o,b,t):
    i=[n for n,l in enumerate(o) if l.startswith('#if defined(FABLE_RETAIL_PASS22_SCOREBOARD_COPY_BACKWARD_ORIGINAL)')]; assert len(i)==1; i=i[0]
    assert t[0]=='#ifdef FABLE_RETAIL_STLP_COPY' and t[-1]=='#endif' and o[-1]=='#else'
    return o[:i]+t+o[i:]
