LINES=0
BASE='base_pdb_order.cpp'
C1="float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);\n            if (score > best_score) {"
V=[
('f1','REGION-reload2 phase-1 score through a functional float cast',[(C1,"float score = float(CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map));\n            if (score > best_score) {")]),
('f6','REGION-reload2 phase-1 score assigned inside the comparison',[(C1,"float score;\n            if ((score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map)) > best_score) {")]),
('f7','REGION-reload2 phase-1 score held as double',[(C1,"double score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);\n            if (score > best_score) {"),("                best_score = score;\n                best_start_vert = start_vert;\n            }\n            if (best_score == 1e30f)\n                break;\n        }\n    skip","                best_score = (float)score;\n                best_start_vert = start_vert;\n            }\n            if (best_score == 1e30f)\n                break;\n        }\n    skip")]),
('f8','REGION-reload2 both phases score held as double',[("float score = CalcSectionScore@all","double score = CalcSectionScore"),("best_score = score;@all","best_score = (float)score;")]),
('f9','REGION-reload2 phase-1 improvement test as !(score <= best_score)',[("            if (score > best_score) {\n                best_score = score;\n                best_start_vert = start_vert;\n            }\n            if (best_score == 1e30f)\n                break;\n        }\n    skip","            if (!(score <= best_score)) {\n                best_score = score;\n                best_start_vert = start_vert;\n            }\n            if (best_score == 1e30f)\n                break;\n        }\n    skip")]),
('f10','REGION-reload2 phase-1 perfect test against a named float local constant',[("    long best_start_vert = -1;\n","    long best_start_vert = -1;\n    float perfect_score = 1e30f;\n"),("if (best_score == 1e30f)@all","if (best_score == perfect_score)")]),
]
