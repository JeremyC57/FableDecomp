import json, sys
t = json.load(open(sys.argv[1])); full = "-v" in sys.argv
for e in t['events']:
    if e[0] == 'region': print('REGION', e[1], ' '.join(str(x) for x in e[2]))
    elif e[0] == 'dag':
        for n in e[1]: print('   %3d %-8s fl%-3d u%d succ %s' % (n['idx'], n['op'], n['fl'], n['unit'], ' '.join('%d/%d' % (s[0], s[1]) + ('' if not (s[2] or s[3]) else '(%d,%d)' % (s[2], s[3])) for s in n['succ'])))
    elif e[0] == 'cyc' and full: print('   cyc', e[1], [(n['op'], n['idx'], hex(n['prio']), n['early']) for n in e[3]])
    elif e[0] == 'pick': print('     pick c? slot%d %s#%d' % (e[1], e[2]['op'], e[2]['idx']), hex(e[2]['prio']))
