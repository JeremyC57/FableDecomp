# usage (repo root, S exported): python3 mkprompt25.py 61 62 ...  -- wave 25: build $S/w22/agentNN (scripts, dis_all.txt, PROMPT.md) for the 16-agent fleet
import os,sys,csv,collections,subprocess,re
S=os.environ['S']; TR=open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'brief_tools_rules.md')).read().split('\n')
sec=lambda a,b: '\n'.join(TR[a-21:b-21])
tools=sec(21,49).replace('agent58','agentNN')
rules=sec(49,83)
for n in map(int,sys.argv[1:]):
    N='%02d'%n; A=f'{S}/w22/agent{N}'
    for d in ('src','test','differ','probe','bin'): os.makedirs(f'{A}/{d}',exist_ok=True)
    for f in ('g.sh','gall.sh','tall.sh','tall.py','finish.py'):
        t=open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'agent_scripts',f)).read()
        t=t.replace('__S__',S).replace('agentNN',f'agent{N}').replace('chunkNN',f'chunk{N}').replace('"tNN"',f'"t{N}"')
        open(f'{A}/{f}','w').write(t)
    rows=list(csv.DictReader(open(f'{S}/w22/chunk{N}.tsv'),delimiter='\t'))
    if not os.path.exists(f'{A}/dis_all.txt'):
        with open(f'{A}/dis_all.txt','w') as o:
            for r in rows:
                o.write('#### %s %s %s len=%s\n'%(r['address'],r['module'],r['name'],r['length']))
                o.write(subprocess.run(['python3',S+'/w17/xdis.py',r['address'],hex(int(r['length']))],capture_output=True,text=True).stdout+'\n')
    c=collections.Counter(r['module'] for r in rows)
    cls=[]
    for m,k in c.items():
        h=os.path.exists(f'rebuild/include/engine/{m}.h')
        cls.append(f"{m} ({k} row{'s' if k>1 else ''}{'; header rebuild/include/engine/'+m+'.h' if h else ''})")
    tsv=open(f'{S}/w22/chunk{N}.tsv').read().strip()
    P=f'''# Hand-authoring brief (FableDecomp, Linux Wine lane) -- wave 25, chunk {N}

Goal: byte-exact ("matching") C++ reconstructions of small native-PC Fable.exe functions, compiled
with the VC7.1-era compiler and compared against the retail bytes. You author one source + one
behaviour test per function, iterate with the compiler until the bytes match, and leave the results
in your directory. You do NOT land and you do NOT do the final re-grade: the coordinator stages your
work for a later session (Fable 5.1) that re-grades every row, runs the tests and lands it.

## Environment (every Bash call must start with these two lines; shell state does not persist)
    export S={S}
    source $S/wine/env.sh; export S={S}; cd /home/user/FableDecomp
Never `cd` into the scratchpad; keep the repo as cwd. Never commit. Never modify anything under the repo
(rebuild/, docs/, tools/ are read-only for you).

## Wave 25 rules (16 agents share this lane at the same time -- read first)
- You are one of 16 agents, each on its own 40-row chunk. Work ONLY inside $S/w22/agent{N}. Never write to another
  agentNN directory, $S/tools, $S/batch, $S/wine/prefix or the repo, and never run git.
- NEVER kill processes (no pkill, killall, kill, `wineserver -k`): the other agents' compiles would die. Wrap every
  grade in `timeout 200` (the scripts below already do); a hung compile ends there. A source with a missing brace can
  make cl.exe spin until that timeout -- check braces before grading.
- At most 4 grading attempts per body (an attempt = a source change + a grade). Then move the best source to
  differ/<addr>.cpp (header line `// Grade: DIFFER(ours v retail) -- <what still differs>`) and write its hand-over
  line (format below). Scheduling-only diffs (same instructions in another order, register swaps, `pop ecx` vs
  `add esp,4`) are usually compiler-build artefacts: park them after one or two tries instead of burning four.
- A row whose bytes contain int3/nop padding followed by ANOTHER function body within the row length covers two
  functions: skip it (STATUS `SKIP`, one line in NOTES.md).
- A row counts as matched only if q.py printed MATCH or RELOCATION_MATCH for the exact file that sits in src/.
- Work through the rows in order, cheapest first is fine; write the test as soon as a body matches.

## File layout and scripts (already in your directory)
- `dis_all.txt` -- the retail disassembly of all 40 rows (read it first).
- `src/<addr>.cpp` -- ONLY sources that graded MATCH/RELOCATION_MATCH. `test/<addr>.cpp` -- the behaviour test of each
  src/ file. `differ/<addr>.cpp` -- the best source of each parked row. `probe/` -- scratch variants.
- `bash $S/w22/agent{N}/g.sh <addr> [leaf] [--flags ...]` grades src/<addr>.cpp and prints the side-by-side
  disassembly; `python3 $S/w18/udiff.py <(bash $S/w22/agent{N}/g.sh <addr>) | grep -v "^ "` shows only the differing lines.
  To grade a probe: `timeout 200 python3 $S/tools/q.py <addr> $S/w22/agent{N}/probe/<file>.cpp [leaf] [--flags ...]`.
- `bash $S/w22/agent{N}/gall.sh > $S/w22/agent{N}/grades.txt` grades every src/*.cpp (it honours `// Leaf:` and `// Flags:` lines).
- `bash $S/w22/agent{N}/tall.sh` compiles and runs every test with g++ (64-bit host: in tests use member access, never
  raw offsets, and never memset a polymorphic object). `python3 $S/w22/agent{N}/tall.py` compiles and runs every test
  with the lane's VC7.1 cl.exe under Wine (what the lander runs; keep tests plain C++98, no <string>/<vector> tricks).
  Every test must print `<addr>_TEST PASS` under both.
- `python3 $S/w22/agent{N}/finish.py` writes payload.json + oracle.tsv from src/ + test/ (run it LAST).
- Line 1 of every source: `// <Module>::<Name>(<params>) -- <what it does>`; finish.py takes the module and the leaf
  name from it. Add `// Leaf: <symbol substring>` (e.g. `??1CFoo` for a destructor whose TU also emits `??_G`, or a
  mangled name for an overload) and/or `// Flags: /EHsc` on their own lines when needed.

{tools}
{rules}
## Output (in $S/w22/agent{N})
1. src/ + test/ for every matched row, differ/ for every parked row; then run gall.sh (all must be MATCH/RELOCATION_MATCH),
   tall.sh and tall.py (all PASS), then finish.py.
2. `STATUS.tsv`: header `address	status	bytes	test	note`, one line per chunk row; status MATCH | RELOCATION_MATCH |
   DIFFER | SRC_FAIL | SKIP; bytes `<ours>v<retail>`; test `PASS` (g++ and VC) / `FAIL` / `-`; a short note.
3. `NOTES.md`:
   - `# Hand chunk {N} (<first class> .. <last class>): <M> of 40 byte-exact`
   - `## What matched (<M>)` -- bullets by class: what each function does and the SHAPE lesson that made it match.
   - `## Parked for Fable 5.1 (<K>)` -- one bullet per parked row in exactly this form:
     `- <addr> <Class::Name> DIFFER(<ours>v<retail>): <the differing instructions, one line> | tried: <forms tried> | next: <the next idea, or "lane gap: <kind>">`
   - `## Skipped` and `## Label corrections` (propagated manifest labels you found wrong).
4. `LESSONS.md`: `# Chunk {N} -- lessons from the authoring agent (fold into GOTCHAS at landing)`, then one bullet per
   codegen lesson worth adding to docs/pipeline/GOTCHAS.md (the source form -> the retail shape, with an address), then
   `Review at landing:` items the landing session must check (rows that needed more than 4 grades, local base models
   next to an existing engine header, errors you noticed in ALREADY-LANDED sources, catalog rows that hold several functions).
5. Final message (short): matched / parked / skipped counts, the parked addresses, and the lessons.

## Chunk {N} classes
{'; '.join(cls)}.
Before authoring a class, look for landed relatives: `grep -rl "<Class>" rebuild/src/compiled | head` and reuse their
models (same members, same accessor shapes). Read `docs/pipeline/GOTCHAS.md` (whole file) before starting -- the
"Wave 24n/24o/24p" lines are the newest lessons.

## Your chunk (chunk{N}.tsv: address, module, name, length, bytes)
```
{tsv}
```
'''
    open(f'{A}/PROMPT.md','w').write(P)
    print(N, len(P), 'chars')
