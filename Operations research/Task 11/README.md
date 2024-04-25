There are 12 tasks (numbered 1..12) and 6 workers (numbered 1..6). Next variable array 0..1
array [1..6,1..12] of var 0..1:Assign;
Represents decisions about assigning workers to tasks. Assign[w, t] = 1 if worker w is assigned to task t, otherwise
Assign[w, t] = 0.

<b>First subtask</b>

Write a model that guarantees that at least 2 workers are assigned to each task.

<b>Second subtask</b>

Extend your model to ensure that no one worker is assigned more than 5 tasks.

<b>Third subtask</b>

Certain pairs of workers cannot be assigned to the same task. In the following matrix

array [1..6,1..6] of 0..1 : bad_pair =

[|0,1,0,1,0,0

|1,0,0,0,1,0

|0,0,0,1,0,0

|1,0,1,0,1,0

|0,1,0,1,0,0

|0,0,0,0,0,0|] ;

bad_pair[w1,w2] = 1 means that workers w1 and w2 cannot be assigned to the same task.

Extend your model of the second problem to satisfy the bad pair constraint.