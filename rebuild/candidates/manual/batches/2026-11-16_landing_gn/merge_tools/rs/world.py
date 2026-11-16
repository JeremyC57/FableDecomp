def R(k,o,b,t):
    i=t.index('#ifdef FABLE_RETAIL_CWORLD_POPULATE'); j=t.index('#endif',i); blk=t[i:j+1]
    e=o.index('#endif'); assert o[0]=='#ifdef FABLE_RETAIL_CWORLD_ISPERFORMINGREGIONLOADREALLY'
    return o[:e+1]+blk+o[e+1:]
