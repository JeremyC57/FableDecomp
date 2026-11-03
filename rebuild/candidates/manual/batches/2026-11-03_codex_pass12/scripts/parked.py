#!/usr/bin/env python3
# parked_summary.tsv: one line per Codex PARKED row with our PLAN §3 residue bucket (assigned by hand from the residue text).
# usage: python3 -I parked.py <handoff> <out.tsv>
import sys, csv
from pathlib import Path
B = {
 'inline-vs-call': '00490be0 00d321c0 00417a58 00df5000 00d3a4a0 00ea2720 00ee3140 00de4710 00da8bf0 00ec4a00 00e71f10 00e482f0 00eeaef0 00ee4dd0 00d0ee70 00ddd430 00d07e50 00677970 00d067a0 00daadd0 00e5e5a0 007c0690 007bfe30 00de6730 00e78100 00e7ab50 009fde80 005253f0 0058a763 00590f0b 00a68de0 00d23dc0 00ef4b40 00efa4f0 00ed3b30',
 'x87-order': '00a1c010 00a86110 00ddae40',
 'stack-slot/frame': '0092f310 00507070 00e76e90 0058f9bc 00642aa0',
 'block/arm-layout': '00775350',
 'incomplete-model': '0043a570 00427216',
 'schedule': '00b2fd60 007602f0 007677e0 0058e208',
}
PLAN = {'inline-vs-call': 'other (inline-vs-call)', 'schedule': 'other (schedule)'}
m = {a: k for k, v in B.items() for a in v.split()}
st = [r for r in csv.DictReader(open(Path(sys.argv[1])/'STATUS.tsv'), delimiter='\t') if r['status'] == 'PARKED']
assert len(st) == 50 and set(m) == {r['address'] for r in st}, (len(st), set(m) ^ {r['address'] for r in st})
o = open(sys.argv[2], 'w'); o.write('address\tname\tretail_size\tbest_object_size\tflags\tcodex_residue\tour_bucket\n')
for r in sorted(st, key=lambda r: r['address']):
    o.write('\t'.join([r['address'], r['assigned_name'], r['retail_size'], r['object_size'], r['flags'], r['residue'].replace('\t', ' '), PLAN.get(m[r['address']], m[r['address']])]) + '\n')
from collections import Counter; print(Counter(PLAN.get(v, v) for v in m.values()))
