set Workers;
set Tasks;

param bad_pair {w in Workers, t in Tasks} binary;
param minWorkers;
param maxTasks;

var Assign{w in Workers, t in Tasks} binary;

/*Не входит в модель*/
/**/
maximize result: sum{w in Workers, t in Tasks} Assign[w,t];
/**/

subject to minimalWorkers{t in Tasks}: sum{w in Workers} Assign[w,t] >= minWorkers;
subject to maximumTasks{w in Workers}: sum{t in Tasks} Assign[w,t] <= maxTasks;
subject to NoBadPairs{t in Tasks, w1 in Workers, w2 in Workers : bad_pair[w1,w2] = 1}:
Assign[w1,t] + Assign[w2,t] <= 1;

