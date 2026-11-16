def R(k,o,b,t):
    assert o[0]==t[0]=='#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX' and o[-2]==t[4] and t[5]=='#endif'
    return o[:-2]+t[2:4]+o[-2:]+t[6:]
