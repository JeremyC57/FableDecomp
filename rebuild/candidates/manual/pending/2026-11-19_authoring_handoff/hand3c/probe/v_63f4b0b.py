import re
def parts(s):
    a=s.index('class CThingFilter_HasNavigationInformation {'); b=s.index('class CThingFilter_OverlappingBoundingBox {'); c=s.index('class CThingFindNavigationLayerAt {'); d=s.index('template bool CThingSearchTools')
    return s[:a], s[a:b], s[b:c], s[c:d], s[d:]
def order(o, early=False):
    def f(s):
        pre,h,ob,fi,tail=parts(s); m={'h':h,'o':ob,'f':fi}; cls=''.join(m[x] for x in o)
        if early:
            k=pre.index('class CTCMapwho;'); pre=pre[:k]+cls+pre[k:]; return pre+tail
        return pre+cls+tail
    return f
VARIANTS=[]
for o in ('hof','hfo','ohf','ofh','fho','foh'):
    VARIANTS.append(('cls_'+o, order(o)))
    VARIANTS.append(('early_'+o, order(o,True)))
VARIANTS=[v for v in VARIANTS if v[0]!='cls_hof']
