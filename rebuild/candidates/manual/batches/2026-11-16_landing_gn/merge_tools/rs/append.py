# ours, then theirs' lines after the longest common prefix of theirs and ours (theirs added blocks after a shared start)
def R(k,o,b,t):
    i=0
    while i<min(len(o),len(t)) and o[i]==t[i]: i+=1
    assert i>0, 'no common prefix'
    print('      append after %d common lines:'%i, [l for l in t[i:] if l.startswith('#')][:6])
    return o+t[i:]
