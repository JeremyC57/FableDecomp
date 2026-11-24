# 0x00CF4EC0 AUTH10 log
- c0 (AUTH1 c0) default: DIFFER(6244v6164), masked 142. inl table inl/c0.json.
- forced movie dtor expand k8..k14: 6220v6164, masked 131 (18 hunks). + ~CScriptThing k75 -> call: unchanged. 3 grades + 3 hook runs.
- dump: python3 -c "import json,collections;E=json.load(open('inl/c0.json'));c=collections.Counter()
  [print(i,c.update([d['n']]) or c[d['n']]-1,d['n'][:60],d['flo']/1e8,d.get('sc'),d['r']) for i,d in enumerate(e[1] for e in E if e[0]=='d')]"
