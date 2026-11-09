"""pack.py: simulate c2.dll 13.10.3077's regime-1 frame packer (0x10790ad6, new-slot 0x10791426, sort 0x107ae0c4)
from a phook.py trace: list order + interference sets -> slots -> sorted layout."""
import json, sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr')
import qs
D = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr/'
def load(tag):
    ev = json.load(open(D + tag + '.json'))['events']
    pk = [e for e in ev if e[0] == 'pack'][0]
    ps = [e for e in ev if e[0] == 'presort']
    pe = [e for e in ev if e[0] == 'packend'][0]
    nodes = {}
    for x in pk[3]:
        nb = bytes.fromhex(x[12]); sb = bytes.fromhex(x[13]) if x[13] else b''
        import struct as _st
        gs = False
        if sb:
            s39 = sb[0x39]; s5f = _st.unpack_from('<i', sb, 0x5f)[0]; s20 = _st.unpack_from('<I', sb, 0x20)[0]
            ssz = _st.unpack_from('<i', sb, 0x1c)[0]
            gs = (((s39 & 1) and s5f <= 2) or (s20 & 0x4000000)) and ssz > 4
            gs2 = (((s39 & 1) and s5f > 2) and False)
        nodes[x[0]] = dict(nid=x[0], name=x[1], kind=x[2], f5=x[3], f6=x[4], size=x[5], w=x[6], idx=x[7],
                           symsize=x[9], off0=x[10], b7=nb[7], w10=_st.unpack_from('<H', nb, 0x10)[0], gs=bool(gs))
    inter = {}
    for nid, bits in pk[4]:
        inter[nid] = set(bits or [])
    order = [x[0] for x in pk[3]]
    idx2nid = {n['idx']: n['nid'] for n in nodes.values()}
    # flagged creators from the real presort
    flagged = set()
    if ps:
        for s in ps[0][4]:
            if s[6] & 1:
                for k in s[1] or []: flagged.add(idx2nid[k])
    final_off = {x[0]: x[10] for x in pe[3]}
    wev = []
    on = False
    for e in ev:
        if e[0] == 'pipe': on = 'Action@CManager' in (e[1] or '')
        if e[0] == 'w' and on: wev.append((e[1], e[2]))
    edi = ps[0][3] if ps else 1
    return dict(nodes=nodes, inter=inter, order=order, idx2nid=idx2nid, flagged=flagged, final_off=final_off,
                wev=wev, edi=edi, ev=ev)
def pack(M, order=None, weights=None, sort=True):
    nodes, inter, flagged = M['nodes'], M['inter'], M['flagged']
    order = order or M['order']
    W = lambda nid: (weights or {}).get(nid, nodes[nid]['w'])
    slots = []
    for nid in order:
        n = nodes[nid]
        if n['kind'] == 5:
            slots.append(dict(mem={n['idx']}, inter=set(inter[nid]), size=(n['symsize'] + 3) & ~3, w=W(nid), flag=0,
                              fixed=n['off0'], creator=nid))
    B = len(slots); total = 0
    for nid in order:
        n = nodes[nid]
        if n['kind'] == 5: continue
        i = n['idx']; lv = inter[nid]
        done = False
        for j in range(B):
            s = slots[j]
            if n['size'] > s['size']: continue
            if M.get('gsfunc', True) and ((n['b7'] & 0x20) or (n['size'] > 4 and n['w10'] != 0x4008)): continue
            if i in s['inter'] or (s['mem'] & lv): continue
            s['mem'].add(i); s['inter'] |= lv; s['w'] += W(nid); done = True; break
        if done: continue
        if len(slots) - 1 >= B:
            for j in range(len(slots) - 1, B - 1, -1):
                s = slots[j]
                if n['size'] > 2 * s['size'] or i in s['inter'] or (s['mem'] & lv): continue
                s['mem'].add(i); s['inter'] |= lv; s['w'] += W(nid)
                if n['size'] > s['size']:
                    total += ((n['size'] + 3) & ~3) - s['size']; s['size'] = n['size']
                done = True; break
        if done: continue
        slots.append(dict(mem={i}, inter=set(lv), size=n['size'], w=W(nid), flag=1 if n['gs'] else 0, creator=nid))
        total += (n['size'] + 3) & ~3
    pre = [dict(s) for s in slots]
    if sort and total > 0x80:
        v = slots[:]
        qs.qsort(v, B, len(v) - 1, M['edi'])
        slots = v
    return pre, slots, B, total
def layout(M, slots, B, base=-4, bias=0x74):
    """offsets as the listing shows them: allocation from the frame top (cookie at -4) downward, + 0x74."""
    cur = base; offs = {}
    for k, s in enumerate(slots):
        if k < B:
            off = s['fixed']
        else:
            al = 4
            sz = s['size']
            al = 1 if sz == 1 else (2 if sz == 2 else 4)
            cur = -(((-cur) + sz + al - 1) // al * al)
            off = cur + bias
        for i in s['mem']:
            offs[M['idx2nid'][i]] = off
    return offs
if __name__ == '__main__':
    M = load(sys.argv[1])
    pre, slots, B, total = pack(M)
    ev = M['ev']
    real_pre = [e for e in ev if e[0] == 'presort'][0][4]
    print('B', B, 'slots', len(pre), 'real', len(real_pre), 'total', total)
    same_pre = [set(s['mem']) for s in pre] == [set(s[1] or []) for s in real_pre]
    print('presort membership/order identical:', same_pre)
    offs = layout(M, slots, B)
    bad = [(nid, offs.get(nid), M['final_off'][nid]) for nid in M['nodes'] if offs.get(nid) != M['final_off'][nid]]
    print('offset mismatches', len(bad), bad[:10])
