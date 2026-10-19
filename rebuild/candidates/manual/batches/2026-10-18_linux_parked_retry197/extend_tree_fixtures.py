from pathlib import Path
import json,hashlib,subprocess,shutil
p=Path('work/codex-lane/w22/coordinator_retry197');data=json.loads((p/'payload.json').read_text());amend=[]
for a,volume in [('00b92f80','camera'),('00b92e50','sphere')]:
 f=p/(a+'_test.cpp');before=f.read_text();original=p/'original_fixtures'/(a+'_test.cpp');original.parent.mkdir(exist_ok=True);assert not original.exists();shutil.copy2(f,original)
 addition='''
 // Root-only fixture extension: balanced three-node outer tree, two-node inner trees;
 // exercise right-child traversal and ascent to both header sentinels (six root grids).
 TreeNodeBase multiHead={0},innerHeads[3]={0};
 TreeNode<RetailTree<CCountedSceneMap> > outerNodes[3];
 TreeNode<CCountedSceneMap> innerNodes[3][2];
 for(int q=0;q<3;++q){
  outerNodes[q].Colour=1;outerNodes[q].Left=outerNodes[q].Right=0;
  outerNodes[q].Value.Header=&innerHeads[q];outerNodes[q].Value.Count=2;
  innerHeads[q].Parent=innerHeads[q].Left=&innerNodes[q][0];innerHeads[q].Right=&innerNodes[q][1];
  innerNodes[q][0].Colour=1;innerNodes[q][0].Parent=&innerHeads[q];innerNodes[q][0].Left=0;innerNodes[q][0].Right=&innerNodes[q][1];
  innerNodes[q][1].Colour=1;innerNodes[q][1].Parent=&innerNodes[q][0];innerNodes[q][1].Left=innerNodes[q][1].Right=0;
  for(int t=0;t<2;++t){innerNodes[q][t].Value.Data=&map;innerNodes[q][t].Value.Info=0;}
 }
 outerNodes[1].Parent=&multiHead;outerNodes[1].Left=&outerNodes[0];outerNodes[1].Right=&outerNodes[2];
 outerNodes[0].Parent=outerNodes[2].Parent=&outerNodes[1];
 multiHead.Parent=&outerNodes[1];multiHead.Left=&outerNodes[0];multiHead.Right=&outerNodes[2];
 scene.MapList.Header=&multiHead;scene.MapList.Count=3;scene.FirstMovingPrimitive=0;
 ResetCounts();CHECK(!scene.PreloadPrimitives(VOLUME,77));CHECK(gridCalls==6&&!loadCalls&&!failed);
'''.replace('VOLUME',volume)
 needle='std::printf("'+a+'_TEST PASS';assert before.count(needle)==1;after=before.replace(needle,addition+'\n'+needle);f.write_text(after)
 for row in data['result']['authored']:
  if row['address']==a:row['test_cpp']=after
 amend.append(dict(address=a,kind='FIXTURE_ONLY_EXTENSION',source_grades=0,original_fixture=str(original),original_sha256=hashlib.sha256(before.encode()).hexdigest(),fixture=str(f),sha256=hashlib.sha256(after.encode()).hexdigest(),scenario='3-node outer map x2-node inner maps: six root-grid callbacks, successor right-child descent and sentinel ascent; no moving primitives.'))
 for flags in [[],['-fno-elide-constructors']]:
  suffix='_noelide'if flags else'';cmd=['g++','-std=c++98','-O2','-fcheck-new']+flags+[str(f),'-o',str(p/(a+'_host_tree'+suffix))];r=subprocess.run(cmd,capture_output=True,text=True);(p/(a+'_tree'+suffix+'_build.log')).write_text(json.dumps(cmd)+'\n'+r.stdout+r.stderr);r.check_returncode();r=subprocess.run([str(p/(a+'_host_tree'+suffix))],capture_output=True,text=True);(p/(a+'_tree'+suffix+'_host.log')).write_text(r.stdout+r.stderr);print(r.stdout);r.check_returncode()
(p/'fixture_extensions.json').write_text(json.dumps(amend,indent=2)+'\n');(p/'payload.json').write_text(json.dumps(data,indent=2)+'\n')
